// Copyright 2020 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file.

#ifndef HASHMAP_H
#define HASHMAP_H

#include "objects.h"

struct hashmap;

struct hashmap *hashmap_new(u64 elsize, u64 cap, 
                            u64 seed0, u64 seed1,
                            u64 (*hash)(const Pointer item, 
                                             u64 seed0, u64 seed1),
                            int (*compare)(const Pointer a, const Pointer b, 
                                           Pointer udata),
                            Pointer udata);
struct hashmap *hashmap_new_with_allocator(
                            Pointer (*malloc)(u64), 
                            Pointer (*realloc)(Pointer , u64), 
                            void (*free)(Pointer),
                            u64 elsize, u64 cap, 
                            u64 seed0, u64 seed1,
                            u64 (*hash)(const Pointer item, 
                                             u64 seed0, u64 seed1),
                            int (*compare)(const Pointer a, const Pointer b, 
                                           Pointer udata),
                            Pointer udata);
void hashmap_free(struct hashmap *map);
void hashmap_clear(struct hashmap *map, bool update_cap);
u64 hashmap_count(struct hashmap *map);
bool hashmap_oom(struct hashmap *map);
Pointer hashmap_get(struct hashmap *map, Pointer item);
Pointer hashmap_set(struct hashmap *map, Pointer item);
Pointer hashmap_delete(struct hashmap *map, Pointer item);
Pointer hashmap_probe(struct hashmap *map, u64 position);
bool hashmap_scan(struct hashmap *map,
                  bool (*iter)(const Pointer item, Pointer udata), Pointer udata);

u64 hashmap_sip(const Pointer data, u64 len, 
                     u64 seed0, u64 seed1);
u64 hashmap_murmur(const Pointer data, u64 len, 
                        u64 seed0, u64 seed1);


// DEPRECATED: use `hashmap_new_with_allocator`
void hashmap_set_allocator(Pointer (*malloc)(u64), void (*free)(Pointer));

#endif