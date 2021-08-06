// Generated from template $TEMPLATE$.h
#ifndef _POINTERTREEMAP_HH_
#define _POINTERTREEMAP_HH_

#include "collection_base.h"
#include "allocators/valueallocator.h"

Fn_cleanupInterface(PointerTreemap);
Fn_traversableInterface(PointerTreemap);

typedef struct PointerTreemap_node PointerTreemap_node;

typedef PointerTreemap_node * PPointerTreemapNode;

typedef struct PointerTreemap_node {
    _POINTER_MAP_ENTRY_
    PPointerTreemapNode right;
    PPointerTreemapNode left;
} PointerTreemap_node;

typedef struct PointerTreemap_data {
    PFn_compare compare; 
    int length; 
    PPointerTreemapNode $rootNode;
    PValueAllocator $allocator;
} PointerTreemap_data;

typedef PointerTreemap_data * PPointerTreemap;


__attribute__ ((nonnull (1, 2))) void PointerTreemap_init(PPointerTreemap this, PFn_compare);
__attribute__ ((nonnull (1))) PPointerTreemap PointerTreemap_create(PFn_compare);

Fn_traverse(PointerTreemap, PPointerTreemap);
Fn_find(PointerTreemap, PPointerTreemap, Pointer);
Fn_addEntry(PointerTreemap, PPointerTreemap, PPointerMapEntry);
Fn_deleteEntry(PointerTreemap, PPointerTreemap, Pointer);
Fn_clear(PointerTreemap, PPointerTreemap);

Fn_free(PointerTreemap, PPointerTreemap);
Fn_freeChildren(PointerTreemap, PPointerTreemap);

#endif