#ifndef _POINTERLINEARMAP_HH_
#define _POINTERLINEARMAP_HH_

#include "collection_base.h"
#include "bypointer/pointerarray.h"

/**
 * @brief key/value pairs allows stored in a linear map
 * 
 */

// *************************************
// Interfaces
// *************************************
Fn_mapInterface(PointerLinearmap);
Fn_cleanupInterface(PointerLinearmap);
Fn_traversableInterface(PointerLinearmap);

// *************************************
// data structure
// *************************************

typedef struct PointerLinearmap_data {
    i32 length;
    i32 $capacity;
    PFn_compare compare;
    // the key and the values are store seperately to improve cache hits while
    // doing a linear search
    PPointerArray $keys;
    PPointerArray $values;
} PointerLinearmap_data;

typedef PointerLinearmap_data * PPointerLinearmap;

// *************************************
// functions
// *************************************

PPointerLinearmap PointerLinearmap_create(PFn_compare);
__attribute__ ((nonnull (1))) void PointerLinearmap_init(const PPointerLinearmap hashmap, PFn_compare);

Fn_put(PointerLinearmap, PPointerLinearmap, Pointer, Pointer, Pointer);
Fn_putEntry(PointerLinearmap, PPointerLinearmap, PPointerMapEntry);
Fn_get(PointerLinearmap, PPointerLinearmap, Pointer, Pointer);
Fn_deleteEntry(PointerLinearmap, PPointerLinearmap, Pointer);
Fn_contains(PointerLinearmap, PPointerLinearmap, Pointer);
Fn_clear(PointerLinearmap, PPointerLinearmap);
Fn_traverse(PointerLinearmap, PPointerLinearmap);
Fn_free(PointerLinearmap, PPointerLinearmap);
Fn_freeChildren(PointerLinearmap, PPointerLinearmap);

#endif