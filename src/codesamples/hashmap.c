// Copyright 2020 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file.

#include "hashmap.h"

static Pointer (*_malloc)(u64) = NULL;
static Pointer (*_realloc)(Pointer , u64) = NULL;
static void (*_free)(Pointer ) = NULL;

// hashmap_set_allocator allows for configuring a custom allocator for
// all hashmap library operations. This function, if needed, should be called
// only once at startup and a prior to calling hashmap_new().
void hashmap_set_allocator(Pointer (*malloc)(u64), void (*free)(Pointer)) 
{
    _malloc = malloc;
    _free = free;
}

#define panic(_msg_) { \
    fprintf(stderr, "panic: %s (%s:%d)\n", (_msg_), __FILE__, __LINE__); \
    exit(1); \
}

struct bucket {
    u64 hash:48;
    u64 dib:16;
};

// hashmap is an open addressed hash map using robinhood hashing.
struct hashmap {
    Pointer (*malloc)(u64);
    Pointer (*realloc)(Pointer , u64);
    void (*free)(Pointer );
    bool oom;
    u64 elsize;
    u64 cap;
    u64 seed0;
    u64 seed1;
    u64 (*hash)(const Pointer item, u64 seed0, u64 seed1);
    int (*compare)(const Pointer a, const Pointer b, Pointer udata);
    Pointer udata;
    u64 bucketsz;
    u64 nbuckets;
    u64 count;
    u64 mask;
    u64 growat;
    u64 shrinkat;
    Pointer buckets;
    Pointer spare;
    Pointer edata;
};

static struct bucket *bucket_at(struct hashmap *map, u64 index) {
    return (struct bucket*)(((PChar)map->buckets)+(map->bucketsz*index));
}

static Pointer bucket_item(struct bucket *entry) {
    return ((PChar)entry)+sizeof(struct bucket);
}

static u64 get_hash(struct hashmap *map, Pointer key) {
    return map->hash(key, map->seed0, map->seed1) << 16 >> 16;
}

// hashmap_new_with_allocator returns a new hash map using a custom allocator.
// See hashmap_new for more information information
struct hashmap *hashmap_new_with_allocator(
                            Pointer (*_malloc)(u64), 
                            Pointer (*_realloc)(void*, u64), 
                            void (*_free)(void*),
                            u64 elsize, u64 cap, 
                            u64 seed0, u64 seed1,
                            u64 (*hash)(const Pointer item, 
                                             u64 seed0, u64 seed1),
                            int (*compare)(const Pointer a, const Pointer b, 
                                           Pointer udata),
                            Pointer udata)
{
    _malloc = _malloc ? _malloc : malloc;
    _realloc = _realloc ? _realloc : realloc;
    _free = _free ? _free : free;
    int ncap = 16;
    if (cap < ncap) {
        cap = ncap;
    } else {
        while (ncap < cap) {
            ncap *= 2;
        }
        cap = ncap;
    }
    u64 bucketsz = sizeof(struct bucket) + elsize;
    while (bucketsz & (sizeof(Pointer)-1)) {
        bucketsz++;
    }
    // hashmap + spare + edata
    u64 size = sizeof(struct hashmap)+bucketsz*2;
    struct hashmap *map = _malloc(size);
    if (!map) {
        return NULL;
    }
    memset(map, 0, sizeof(struct hashmap));
    map->elsize = elsize;
    map->bucketsz = bucketsz;
    map->seed0 = seed0;
    map->seed1 = seed1;
    map->hash = hash;
    map->compare = compare;
    map->udata = udata;
    map->spare = ((PChar)map)+sizeof(struct hashmap);
    map->edata = (PChar)map->spare+bucketsz;
    map->cap = cap;
    map->nbuckets = cap;
    map->mask = map->nbuckets-1;
    map->buckets = _malloc(map->bucketsz*map->nbuckets);
    if (!map->buckets) {
        _free(map);
        return NULL;
    }
    memset(map->buckets, 0, map->bucketsz*map->nbuckets);
    map->growat = map->nbuckets*0.75;
    map->shrinkat = map->nbuckets*0.10;
    map->malloc = _malloc;
    map->realloc = _realloc;
    map->free = _free;
    return map;  
}


// hashmap_new returns a new hash map. 
// Param `elsize` is the size of each element in the tree. Every element that
// is inserted, deleted, or retrieved will be this size.
// Param `cap` is the default lower capacity of the hashmap. Setting this to
// zero will default to 16.
// Params `seed0` and `seed1` are optional seed values that are passed to the 
// following `hash` function. These can be any value you wish but it's often 
// best to use randomly generated values.
// Param `hash` is a function that generates a hash value for an item. It's
// important that you provide a good hash function, otherwise it will perform
// poorly or be vulnerable to Denial-of-service attacks. This implementation
// comes with two helper functions `hashmap_sip()` and `hashmap_murmur()`.
// Param `compare` is a function that compares items in the tree. See the 
// qsort stdlib function for an example of how this function works.
// The hashmap must be freed with hashmap_free(). 
struct hashmap *hashmap_new(u64 elsize, u64 cap, 
                            u64 seed0, u64 seed1,
                            u64 (*hash)(const Pointer item, 
                                             u64 seed0, u64 seed1),
                            int (*compare)(const Pointer a, const Pointer b, 
                                           Pointer udata),
                            Pointer udata)
{
    return hashmap_new_with_allocator(
        (_malloc?_malloc:malloc),
        (_realloc?_realloc:realloc),
        (_free?_free:free),
        elsize, cap, seed0, seed1, hash, compare, udata
    );
}

// hashmap_clear quickly clears the map. 
// When the update_cap is provided, the map's capacity will be updated to match
// the currently number of allocated buckets. This is an optimization to ensure
// that this operation does not perform any allocations.
void hashmap_clear(struct hashmap *map, bool update_cap) {
    map->count = 0;
    if (update_cap) {
        map->cap = map->nbuckets;
    } else if (map->nbuckets != map->cap) {
        Pointer new_buckets = map->malloc(map->bucketsz*map->cap);
        if (new_buckets) {
            map->free(map->buckets);
            map->buckets = new_buckets;
        }
        map->nbuckets = map->cap;
    }
    memset(map->buckets, 0, map->bucketsz*map->nbuckets);
    map->mask = map->nbuckets-1;
    map->growat = map->nbuckets*0.75;
    map->shrinkat = map->nbuckets*0.10;
}


static bool resize(struct hashmap *map, u64 new_cap) {
    struct hashmap *map2 = hashmap_new(map->elsize, new_cap, map->seed1, 
                                       map->seed1, map->hash, map->compare,
                                       map->udata);
    if (!map2) {
        return false;
    }
    for (u64 i = 0; i < map->nbuckets; i++) {
        struct bucket *entry = bucket_at(map, i);
        if (!entry->dib) {
            continue;
        }
        entry->dib = 1;
        u64 j = entry->hash & map2->mask;
        for (;;) {
            struct bucket *bucket = bucket_at(map2, j);
            if (bucket->dib == 0) {
                memcpy(bucket, entry, map->bucketsz);
                break;
            }
            if (bucket->dib < entry->dib) {
                memcpy(map2->spare, bucket, map->bucketsz);
                memcpy(bucket, entry, map->bucketsz);
                memcpy(entry, map2->spare, map->bucketsz);
            }
            j = (j + 1) & map2->mask;
            entry->dib += 1;
        }
	}
    map->free(map->buckets);
    map->buckets = map2->buckets;
    map->nbuckets = map2->nbuckets;
    map->mask = map2->mask;
    map->growat = map2->growat;
    map->shrinkat = map2->shrinkat;
    map->free(map2);
    return true;
}

// hashmap_set inserts or replaces an item in the hash map. If an item is
// replaced then it is returned otherwise NULL is returned. This operation
// may allocate memory. If the system is unable to allocate additional
// memory then NULL is returned and hashmap_oom() returns true.
Pointer hashmap_set(struct hashmap *map, Pointer item) {
    if (!item) {
        panic("item is null");
    }
    map->oom = false;
    if (map->count == map->growat) {
        if (!resize(map, map->nbuckets*2)) {
            map->oom = true;
            return NULL;
        }
    }

    
    struct bucket *entry = map->edata;
    entry->hash = get_hash(map, item);
    entry->dib = 1;
    memcpy(bucket_item(entry), item, map->elsize);
    
    u64 i = entry->hash & map->mask;
	for (;;) {
        struct bucket *bucket = bucket_at(map, i);
        if (bucket->dib == 0) {
            memcpy(bucket, entry, map->bucketsz);
            map->count++;
			return NULL;
		}
        if (entry->hash == bucket->hash && 
            map->compare(bucket_item(entry), bucket_item(bucket), 
                         map->udata) == 0)
        {
            memcpy(map->spare, bucket_item(bucket), map->elsize);
            memcpy(bucket_item(bucket), bucket_item(entry), map->elsize);
            return map->spare;
		}
        if (bucket->dib < entry->dib) {
            memcpy(map->spare, bucket, map->bucketsz);
            memcpy(bucket, entry, map->bucketsz);
            memcpy(entry, map->spare, map->bucketsz);
		}
		i = (i + 1) & map->mask;
        entry->dib += 1;
	}
}

// hashmap_get returns the item based on the provided key. If the item is not
// found then NULL is returned.
Pointer hashmap_get(struct hashmap *map, Pointer key) {
    if (!key) {
        panic("key is null");
    }
    u64 hash = get_hash(map, key);
	u64 i = hash & map->mask;
	for (;;) {
        struct bucket *bucket = bucket_at(map, i);
		if (!bucket->dib) {
			return NULL;
		}
		if (bucket->hash == hash && 
            map->compare(key, bucket_item(bucket), map->udata) == 0)
        {
            return bucket_item(bucket);
		}
		i = (i + 1) & map->mask;
	}
}

// hashmap_probe returns the item in the bucket at position or NULL if an item
// is not set for that bucket. The position is 'moduloed' by the number of 
// buckets in the hashmap.
Pointer hashmap_probe(struct hashmap *map, u64 position) {
    u64 i = position & map->mask;
    struct bucket *bucket = bucket_at(map, i);
    if (!bucket->dib) {
		return NULL;
	}
    return bucket_item(bucket);
}


// hashmap_delete removes an item from the hash map and returns it. If the
// item is not found then NULL is returned.
Pointer hashmap_delete(struct hashmap *map, Pointer key) {
    if (!key) {
        panic("key is null");
    }
    map->oom = false;
    u64 hash = get_hash(map, key);
	u64 i = hash & map->mask;
	for (;;) {
        struct bucket *bucket = bucket_at(map, i);
		if (!bucket->dib) {
			return NULL;
		}
		if (bucket->hash == hash && 
            map->compare(key, bucket_item(bucket), map->udata) == 0)
        {
            memcpy(map->spare, bucket_item(bucket), map->elsize);
            bucket->dib = 0;
            for (;;) {
                struct bucket *prev = bucket;
                i = (i + 1) & map->mask;
                bucket = bucket_at(map, i);
                if (bucket->dib <= 1) {
                    prev->dib = 0;
                    break;
                }
                memcpy(prev, bucket, map->bucketsz);
                prev->dib--;
            }
            map->count--;
            if (map->nbuckets > map->cap && map->count <= map->shrinkat) {
                // Ignore the return value. It's ok for the resize operation to
                // fail to allocate enough memory because a shrink operation
                // does not change the integrity of the data.
                resize(map, map->nbuckets/2);
            }
			return map->spare;
		}
		i = (i + 1) & map->mask;
	}
}

// hashmap_count returns the number of items in the hash map.
u64 hashmap_count(struct hashmap *map) {
    return map->count;
}

// hashmap_free frees the hash map
void hashmap_free(struct hashmap *map) {
    if (!map) return;
    map->free(map->buckets);
    map->free(map);
}

// hashmap_oom returns true if the last hashmap_set() call failed due to the 
// system being out of memory.
bool hashmap_oom(struct hashmap *map) {
    return map->oom;
}

// hashmap_scan iterates over all items in the hash map
// Param `iter` can return false to stop iteration early.
// Returns false if the iteration has been stopped early.
bool hashmap_scan(struct hashmap *map, 
                  bool (*iter)(const Pointer item, Pointer udata), Pointer udata)
{
    for (u64 i = 0; i < map->nbuckets; i++) {
        struct bucket *bucket = bucket_at(map, i);
        if (bucket->dib) {
            if (!iter(bucket_item(bucket), udata)) {
                return false;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// SipHash reference C implementation
//
// Copyright (c) 2012-2016 Jean-Philippe Aumasson
// <jeanphilippe.aumasson@gmail.com>
// Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//
// default: SipHash-2-4
//-----------------------------------------------------------------------------
static u64 SIP64(const u8 *in, const u64 inlen, 
                      u64 seed0, u64 seed1) 
{
#define U8TO64_LE(p) \
    {  (((u64)((p)[0])) | ((u64)((p)[1]) << 8) | \
        ((u64)((p)[2]) << 16) | ((u64)((p)[3]) << 24) | \
        ((u64)((p)[4]) << 32) | ((u64)((p)[5]) << 40) | \
        ((u64)((p)[6]) << 48) | ((u64)((p)[7]) << 56)) }
#define U64TO8_LE(p, v) \
    { U32TO8_LE((p), (u32)((v))); \
      U32TO8_LE((p) + 4, (u32)((v) >> 32)); }
#define U32TO8_LE(p, v) \
    { (p)[0] = (u8)((v)); \
      (p)[1] = (u8)((v) >> 8); \
      (p)[2] = (u8)((v) >> 16); \
      (p)[3] = (u8)((v) >> 24); }
#define ROTL(x, b) (u64)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND \
    { v0 += v1; v1 = ROTL(v1, 13); \
      v1 ^= v0; v0 = ROTL(v0, 32); \
      v2 += v3; v3 = ROTL(v3, 16); \
      v3 ^= v2; \
      v0 += v3; v3 = ROTL(v3, 21); \
      v3 ^= v0; \
      v2 += v1; v1 = ROTL(v1, 17); \
      v1 ^= v2; v2 = ROTL(v2, 32); }
    u64 k0 = U8TO64_LE((u8*)&seed0);
    u64 k1 = U8TO64_LE((u8*)&seed1);
    u64 v3 = UINT64_C(0x7465646279746573) ^ k1;
    u64 v2 = UINT64_C(0x6c7967656e657261) ^ k0;
    u64 v1 = UINT64_C(0x646f72616e646f6d) ^ k1;
    u64 v0 = UINT64_C(0x736f6d6570736575) ^ k0;
    const u8 *end = in + inlen - (inlen % sizeof(u64));
    for (; in != end; in += 8) {
        u64 m = U8TO64_LE(in);
        v3 ^= m;
        SIPROUND; SIPROUND;
        v0 ^= m;
    }
    const int left = inlen & 7;
    u64 b = ((u64)inlen) << 56;
    switch (left) {
    case 7: b |= ((u64)in[6]) << 48;
    case 6: b |= ((u64)in[5]) << 40;
    case 5: b |= ((u64)in[4]) << 32;
    case 4: b |= ((u64)in[3]) << 24;
    case 3: b |= ((u64)in[2]) << 16;
    case 2: b |= ((u64)in[1]) << 8;
    case 1: b |= ((u64)in[0]); break;
    case 0: break;
    }
    v3 ^= b;
    SIPROUND; SIPROUND;
    v0 ^= b;
    v2 ^= 0xff;
    SIPROUND; SIPROUND; SIPROUND; SIPROUND;
    b = v0 ^ v1 ^ v2 ^ v3;
    u64 out = 0;
    U64TO8_LE((u8*)&out, b);
    return out;
}

static u64 fastHash(const Pointer key, const int len, u32 seed, Pointer out) {
    // i64 t = (i64)key | (i64)key >> 32;
    i64 t = (i64)key;
    i64 res = t ^= (t << 5) * (t << 31) ^ (t << 19) ^ (t << 41);
    // for (int i = 0; i < len; i++) {
    //     res = t ^= (t << len*5) ^ (t << 13) | (t << 19);
    // }
    // printDebug("hash %p, %ld, %ld, %ld", key, len, seed, res);
    return res;
}
//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
//
// Murmur3_86_128
//-----------------------------------------------------------------------------
static void MM86128(const Pointer key, const int len, u32 seed, Pointer out) {
#define	ROTL32(x, r) ((x << r) | (x >> (32 - r)))
#define FMIX32(h) h^=h>>16; h*=0x85ebca6b; h^=h>>13; h*=0xc2b2ae35; h^=h>>16;
    const u8 * data = (const u8*)key;
    const int nblocks = len / 16;
    u32 h1 = seed;
    u32 h2 = seed;
    u32 h3 = seed;
    u32 h4 = seed;
    u32 c1 = 0x239b961b; 
    u32 c2 = 0xab0e9789;
    u32 c3 = 0x38b34ae5; 
    u32 c4 = 0xa1e38b93;
    const u32 * blocks = (const u32 *)(data + nblocks*16);
    for (int i = -nblocks; i; i++) {
        u32 k1 = blocks[i*4+0];
        u32 k2 = blocks[i*4+1];
        u32 k3 = blocks[i*4+2];
        u32 k4 = blocks[i*4+3];
        k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
        h1 = ROTL32(h1,19); h1 += h2; h1 = h1*5+0x561ccd1b;
        k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;
        h2 = ROTL32(h2,17); h2 += h3; h2 = h2*5+0x0bcaa747;
        k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;
        h3 = ROTL32(h3,15); h3 += h4; h3 = h3*5+0x96cd1c35;
        k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;
        h4 = ROTL32(h4,13); h4 += h1; h4 = h4*5+0x32ac3b17;
    }
    const u8 * tail = (const u8*)(data + nblocks*16);
    u32 k1 = 0;
    u32 k2 = 0;
    u32 k3 = 0;
    u32 k4 = 0;
    switch(len & 15) {
    case 15: k4 ^= tail[14] << 16;
    case 14: k4 ^= tail[13] << 8;
    case 13: k4 ^= tail[12] << 0;
             k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;
    case 12: k3 ^= tail[11] << 24;
    case 11: k3 ^= tail[10] << 16;
    case 10: k3 ^= tail[ 9] << 8;
    case  9: k3 ^= tail[ 8] << 0;
             k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;
    case  8: k2 ^= tail[ 7] << 24;
    case  7: k2 ^= tail[ 6] << 16;
    case  6: k2 ^= tail[ 5] << 8;
    case  5: k2 ^= tail[ 4] << 0;
             k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;
    case  4: k1 ^= tail[ 3] << 24;
    case  3: k1 ^= tail[ 2] << 16;
    case  2: k1 ^= tail[ 1] << 8;
    case  1: k1 ^= tail[ 0] << 0;
             k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };
    h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;
    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;
    FMIX32(h1); FMIX32(h2); FMIX32(h3); FMIX32(h4);
    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;
    ((u32*)out)[0] = h1;
    ((u32*)out)[1] = h2;
    ((u32*)out)[2] = h3;
    ((u32*)out)[3] = h4;
}

// hashmap_sip returns a hash value for `data` using SipHash-2-4.
u64 hashmap_sip(const Pointer data, u64 len, 
                     u64 seed0, u64 seed1)
{
    return SIP64((u8*)data, len, seed0, seed1);
}

// hashmap_murmur returns a hash value for `data` using Murmur3_86_128.
u64 hashmap_murmur(const Pointer data, u64 len, 
                        u64 seed0, u64 seed1)
{
    char out[16];
    MM86128(data, len, seed0, &out);

    // printDebug("hash %p, %ld, %ld, %ld, %ld", data, len, seed0, seed1, *(u64*)out);

    return *(u64*)out;
}

//==============================================================================
// TESTS AND BENCHMARKS
// $ cc -DHASHMAP_TEST hashmap.c && ./a.out              # run tests
// $ cc -DHASHMAP_TEST -O3 hashmap.c && BENCH=1 ./a.out  # run benchmarks
//==============================================================================
#ifdef HASHMAP_TEST

static u64 deepcount(struct hashmap *map) {
    u64 count = 0;
    for (u64 i = 0; i < map->nbuckets; i++) {
        if (bucket_at(map, i)->dib) {
            count++;
        }
    }
    return count;
}


#pragma GCC diagnostic ignored "-Wextra"


// #include <stdlib.h>
// #include <string.h>
#include <time.h>
#include <assert.h>
// #include <stdio.h>
#include "hashmap.h"

static bool rand_alloc_fail = false;
static int rand_alloc_fail_odds = 3; // 1 in 3 chance malloc will fail.
static u64 total_allocs = 0;
static u64 total_mem = 0;

static Pointer xmalloc(u64 size) {
    if (rand_alloc_fail && rand()%rand_alloc_fail_odds == 0) {
        return NULL;
    }
    Pointer mem = malloc(sizeof(Pointer)+size);
    assert(mem);
    *(u64*)mem = size;
    total_allocs++;
    total_mem += size;
    return (PChar)mem+sizeof(Pointer);
}

static void xfree(Pointer ptr) {
    if (ptr) {
        total_mem -= (u64)*(Pointer*)(ptr-sizeof(Pointer));
        free(ptr-sizeof(Pointer));
        total_allocs--;
    }
}

static void shuffle(Pointer array, u64 numels, u64 elsize) {
    char tmp[elsize];
    PChar arr = array;
    for (u64 i = 0; i < numels - 1; i++) {
        int j = i + rand() / (RAND_MAX / (numels - i) + 1);
        memcpy(tmp, arr + j * elsize, elsize);
        memcpy(arr + j * elsize, arr + i * elsize, elsize);
        memcpy(arr + i * elsize, tmp, elsize);
    }
}

static bool iter_ints(const Pointer item, Pointer udata) {
    int *vals = *(int**)udata;
    vals[*(int*)item] = 1;
    return true;
}

static int compare_ints(const Pointer a, const Pointer b) {
    return *(int*)a - *(int*)b;
}

static int compare_ints_udata(const Pointer a, const Pointer b, Pointer udata) {
    return *(int*)a - *(int*)b;
}

static u64 hash_int(const Pointer item, u64 seed0, u64 seed1) {
    // return hashmap_sip(item, sizeof(int), seed0, seed1);
    return hashmap_murmur(item, sizeof(int), seed0, seed1);
    // return fastHash(item, sizeof(int), seed0, seed1);
    
}

static void all() {
    int seed = getenv("SEED")?atoi(getenv("SEED")):time(NULL);
    int N = getenv("N")?atoi(getenv("N")):2000;
    printf("seed=%d, count=%d, item_size=%zu\n", seed, N, sizeof(int));
    srand(seed);

    rand_alloc_fail = true;

    // test sip and murmur hashes
    assert(hashmap_sip("hello", 5, 1, 2) == 2957200328589801622);
    assert(hashmap_murmur("hello", 5, 1, 2) == 1682575153221130884);

    int *vals;
    while (!(vals = xmalloc(N * sizeof(int)))) {}
    for (int i = 0; i < N; i++) {
        vals[i] = i;
    }

    struct hashmap *map;

    while (!(map = hashmap_new(sizeof(int), 0, seed, seed, 
                               hash_int, compare_ints_udata, NULL))) {}
    shuffle(vals, N, sizeof(int));
    for (int i = 0; i < N; i++) {
        // // printf("== %d ==\n", vals[i]);
        assert(map->count == i);
        assert(map->count == hashmap_count(map));
        assert(map->count == deepcount(map));
        int *v;
        assert(!hashmap_get(map, &vals[i]));
        assert(!hashmap_delete(map, &vals[i]));
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }
        
        for (int j = 0; j < i; j++) {
            v = hashmap_get(map, &vals[j]);
            assert(v && *v == vals[j]);
        }
        while (true) {
            v = hashmap_set(map, &vals[i]);
            if (!v) {
                assert(hashmap_oom(map));
                continue;
            } else {
                assert(!hashmap_oom(map));
                assert(v && *v == vals[i]);
                break;
            }
        }
        v = hashmap_get(map, &vals[i]);
        assert(v && *v == vals[i]);
        v = hashmap_delete(map, &vals[i]);
        assert(v && *v == vals[i]);
        assert(!hashmap_get(map, &vals[i]));
        assert(!hashmap_delete(map, &vals[i]));
        assert(!hashmap_set(map, &vals[i]));
        assert(map->count == i+1);
        assert(map->count == hashmap_count(map));
        assert(map->count == deepcount(map));
    }

    int *vals2;
    while (!(vals2 = xmalloc(N * sizeof(int)))) {}
    memset(vals2, 0, N * sizeof(int));
    assert(hashmap_scan(map, iter_ints, &vals2));
    for (int i = 0; i < N; i++) {
        assert(vals2[i] == 1);
    }
    xfree(vals2);

    shuffle(vals, N, sizeof(int));
    for (int i = 0; i < N; i++) {
        int *v;
        v = hashmap_delete(map, &vals[i]);

        if (!(v && *v == vals[i])) {
            printDebug("v && *v == vals[i] %p, %d, %p, %p", v, i, &vals[i], &map);
        }

        assert(v && *v == vals[i]);
        assert(!hashmap_get(map, &vals[i]));
        assert(map->count == N-i-1);
        assert(map->count == hashmap_count(map));
        assert(map->count == deepcount(map));
        for (int j = N-1; j > i; j--) {
            v = hashmap_get(map, &vals[j]);
            assert(v && *v == vals[j]);
        }
    }

    for (int i = 0; i < N; i++) {
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }
    }

    assert(map->count != 0);
    u64 prev_cap = map->cap;
    hashmap_clear(map, true);
    assert(prev_cap < map->cap);
    assert(map->count == 0);


    for (int i = 0; i < N; i++) {
        while (true) {
            assert(!hashmap_set(map, &vals[i]));
            if (!hashmap_oom(map)) {
                break;
            }
        }
    }

    prev_cap = map->cap;
    hashmap_clear(map, false);
    assert(prev_cap == map->cap);

    hashmap_free(map);

    xfree(vals);

    if (total_allocs != 0) {
        fprintf(stderr, "total_allocs: expected 0, got %ld\n", total_allocs);
        exit(1);
    }
}

#define bench(name, N, code) {{ \
    if (strlen(name) > 0) { \
        printf("%-14s ", name); \
    } \
    u64 tmem = total_mem; \
    u64 tallocs = total_allocs; \
    u64 bytes = 0; \
    clock_t begin = clock(); \
    for (int i = 0; i < N; i++) { \
        (code); \
    } \
    clock_t end = clock(); \
    double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC; \
    double bytes_sec = (double)bytes/elapsed_secs; \
    printf("%d ops in %.3f secs, %.0f ns/op, %.0f op/sec", \
        N, elapsed_secs, \
        elapsed_secs/(double)N*1e9, \
        (double)N/elapsed_secs \
    ); \
    if (bytes > 0) { \
        printf(", %.1f GB/sec", bytes_sec/1024/1024/1024); \
    } \
    if (total_mem > tmem) { \
        u64 used_mem = total_mem-tmem; \
        printf(", %.2f bytes/op", (double)used_mem/N); \
    } \
    if (total_allocs > tallocs) { \
        u64 used_allocs = total_allocs-tallocs; \
        printf(", %.2f allocs/op", (double)used_allocs/N); \
    } \
    printf("\n"); \
}}

static void benchmarks() {
    int seed = getenv("SEED")?atoi(getenv("SEED")):time(NULL);
    int N = getenv("N")?atoi(getenv("N")):5000000;
    printf("seed=%d, count=%d, item_size=%zu\n", seed, N, sizeof(int));
    srand(seed);


    int *vals = xmalloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        vals[i] = i;
    }

    shuffle(vals, N, sizeof(int));

    struct hashmap *map;
    shuffle(vals, N, sizeof(int));

    map = hashmap_new(sizeof(int), 0, seed, seed, hash_int, compare_ints_udata, 
                      NULL);
    bench("set", N, {
        int *v = hashmap_set(map, &vals[i]);
        assert(!v);
    })
    shuffle(vals, N, sizeof(int));
    bench("get", N, {
        int *v = hashmap_get(map, &vals[i]);
        assert(v && *v == vals[i]);
    })
    shuffle(vals, N, sizeof(int));
    bench("delete", N, {
        int *v = hashmap_delete(map, &vals[i]);
        assert(v && *v == vals[i]);
    })
    hashmap_free(map);

    map = hashmap_new(sizeof(int), N, seed, seed, hash_int, compare_ints_udata, 
                      NULL);
    bench("set (cap)", N, {
        int *v = hashmap_set(map, &vals[i]);
        assert(!v);
    })
    shuffle(vals, N, sizeof(int));
    bench("get (cap)", N, {
        int *v = hashmap_get(map, &vals[i]);
        assert(v && *v == vals[i]);
    })
    shuffle(vals, N, sizeof(int));
    bench("delete (cap)" , N, {
        int *v = hashmap_delete(map, &vals[i]);
        assert(v && *v == vals[i]);
    })

    hashmap_free(map);

    
    xfree(vals);

    if (total_allocs != 0) {
        fprintf(stderr, "total_allocs: expected 0, got %lu\n", total_allocs);
        exit(1);
    }
}

int main() {
    hashmap_set_allocator(xmalloc, xfree);

    if (getenv("BENCH")) {
        printf("Running hashmap.c benchmarks...\n");
        benchmarks();
    } else {
        printf("Running hashmap.c tests...\n");
        all();
        printf("PASSED\n");
    }
}


#endif


