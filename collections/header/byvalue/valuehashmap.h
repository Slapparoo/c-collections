#ifndef _RAWHASHMAP_HH_
#define _RAWHASHMAP_HH_

#include "collection_base.h"
#include "valuetree.h"
#include "bitslist.h"

// *************************************
// Interfaces
// *************************************
Fn_mapInterface(ValueHashmap);
Fn_cleanupInterface(ValueHashmap);
Fn_traversableInterface(ValueHashmap);

// *************************************
// data structure
// *************************************
#define __C_HASHMAP_DATA_ \
    i32 length; \
    i32 $capacity; \
    i32 $entrySize; \
    PValueTree $tree; \
    boolean $resizing; \
    i32 $nextGrow; \
    i32 $nextShrink; \
    PFn_hashcode hashCode; \
    PFn_compare compare; \
    PBitslist $collisionMap; \
    Pointer content; 

typedef struct ValueHashmap_data {
     __C_HASHMAP_DATA_
} ValueHashmap_data;

typedef ValueHashmap_data * PValueHashmap;

// *************************************
// functions
// *************************************
#define __C_HASHMAP_CODE_ \
    PValueHashmap (*create)(int itemSize, PFn_hashcode, PFn_compare); \
    __attribute__ ((nonnull (1))) void (*init)(const PValueHashmap hashmap, int itemSize, PFn_hashcode, PFn_compare); \
    PFn_put put; \
    PFn_putEntry putEntry; \
    PFn_get get; \
    PFn_deleteEntry deleteEntry; \
    PFn_contains contains; \
    PFn_clear clear; \
    PFn_free free; \
    PFn_freeChildren freeChildren; \
    PFn_traverse traverse;

typedef struct ValueHashmap_code {
    __C_HASHMAP_CODE_
} ValueHashmap_code;

struct ValueHashmap_code ValueHashmap;

PValueHashmap ValueHashmap_create(int itemSize, PFn_hashcode, PFn_compare);
__attribute__ ((nonnull (1))) void ValueHashmap_init(const PValueHashmap hashmap, int itemSize, PFn_hashcode, PFn_compare);

Fn_put(ValueHashmap, PValueHashmap, Pointer, Pointer, Pointer);
Fn_putEntry(ValueHashmap, PValueHashmap, Pointer);
Fn_get(ValueHashmap, PValueHashmap, Pointer, Pointer);
Fn_deleteEntry(ValueHashmap, PValueHashmap, Pointer);
Fn_contains(ValueHashmap, PValueHashmap, Pointer);
Fn_clear(ValueHashmap, PValueHashmap);
Fn_traverse(ValueHashmap, PValueHashmap);
Fn_free(ValueHashmap, PValueHashmap);
Fn_freeChildren(ValueHashmap, PValueHashmap);

#endif