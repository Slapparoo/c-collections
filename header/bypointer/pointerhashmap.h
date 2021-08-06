#ifndef _RAWHASHMAP_HH_
#define _RAWHASHMAP_HH_

#include "collection_base.h"
#include "../byvalue/valuetree.h"
#include "bitslist.h"

// *************************************
// Interfaces
// *************************************
Fn_mapInterface(PointerHashmap);
Fn_cleanupInterface(PointerHashmap);
Fn_traversableInterface(PointerHashmap);

// *************************************
// data structure
// *************************************

typedef struct PointerHashmap_data {
    i32 length;
    i32 $capacity;
    PValueTree $tree;
    boolean $resizing;
    i32 $nextGrow;
    i32 $nextShrink;
    PFn_hashcode hashCode;
    PFn_compare compare;
    PBitslist $collisionMap;
    PPointerMapEntry * content; 
} PointerHashmap_data;

typedef PointerHashmap_data * PPointerHashmap;

// *************************************
// functions
// *************************************

PPointerHashmap PointerHashmap_create(PFn_hashcode, PFn_compare);
__attribute__ ((nonnull (1))) void PointerHashmap_init(const PPointerHashmap hashmap, PFn_hashcode, PFn_compare);

Fn_put(PointerHashmap, PPointerHashmap, Pointer, Pointer, Pointer);
Fn_putEntry(PointerHashmap, PPointerHashmap, PPointerMapEntry);
Fn_get(PointerHashmap, PPointerHashmap, Pointer, Pointer);
Fn_deleteEntry(PointerHashmap, PPointerHashmap, Pointer);
Fn_contains(PointerHashmap, PPointerHashmap, Pointer);
Fn_clear(PointerHashmap, PPointerHashmap);
Fn_traverse(PointerHashmap, PPointerHashmap);
Fn_free(PointerHashmap, PPointerHashmap);
Fn_freeChildren(PointerHashmap, PPointerHashmap);

#endif