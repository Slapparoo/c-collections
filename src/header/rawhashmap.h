#ifndef _RAWHASHMAP_HH_
#define _RAWHASHMAP_HH_

#include "data-types.h"
#include "object-types.h"
#include "rawtree.h"
#include "rawbitslist.h"

#define __C_HASHMAP_DATA_ \
    i32 length; \
    i32 $capacity; \
    i32 $entrySize; \
    PRawTree $tree; \
    boolean $resizing; \
    i32 $nextGrow; \
    i32 $nextShrink; \
    Fn_hashcode hashCode; \
    Fn_compare compare; \
    PRawBitslist $collisionMap; \
    Pointer content; 


typedef struct RawHashmap_data {
     __C_HASHMAP_DATA_
} RawHashmap_data;

typedef RawHashmap_data * PRawHashmap;

#define __C_HASHMAP_CODE_ \
    PRawHashmap (*create)(int itemSize, Fn_hashcode, Fn_compare); \
    __attribute__ ((nonnull (1))) void (*init)(const PRawHashmap hashmap, int itemSize, Fn_hashcode, Fn_compare); \
    __attribute__ ((nonnull (1, 2))) Pointer (*put)(PRawHashmap hashmap, Pointer entry); \
    __attribute__ ((nonnull (1, 2))) Pointer (*get)(const PRawHashmap hashmap, Pointer key); \
    __attribute__ ((nonnull (1, 2))) void (*remove)(PRawHashmap hashmap, Pointer key); \
    __attribute__ ((nonnull (1))) void (*clear)(PRawHashmap hashmap); \
    __attribute__ ((nonnull (1))) void (*free)(PRawHashmap* hashmap); \
    __attribute__ ((nonnull (1))) void (*freeChildren)(PRawHashmap hashmap); \
    __attribute__ ((nonnull (1, 3))) void (*traverse)(PRawHashmap this, Pointer other, Fn_traverse traverser);

typedef struct RawHashmap_code {
    __C_HASHMAP_CODE_
} RawHashmap_code;

struct RawHashmap_code RawHashmap;

PRawHashmap RawHashmap_create(int itemSize, Fn_hashcode, Fn_compare);
__attribute__ ((nonnull (1))) void RawHashmap_init(const PRawHashmap hashmap, int itemSize, Fn_hashcode, Fn_compare);
__attribute__ ((nonnull (1, 2))) Pointer RawHashmap_put(PRawHashmap hashmap, Pointer entry);
__attribute__ ((nonnull (1, 2))) Pointer RawHashmap_get(const PRawHashmap hashmap, Pointer key);
__attribute__ ((nonnull (1, 2))) void RawHashmap_remove(PRawHashmap hashmap, Pointer key);
__attribute__ ((nonnull (1))) void RawHashmap_clear(PRawHashmap hashmap);
__attribute__ ((nonnull (1))) void RawHashmap_free(PRawHashmap* hashmap);
__attribute__ ((nonnull (1))) void RawHashmap_freeChildren(PRawHashmap hashmap);
__attribute__ ((nonnull (1, 3))) void RawHashmap_traverse(PRawHashmap this, Pointer other, Fn_traverse traverser);

#endif