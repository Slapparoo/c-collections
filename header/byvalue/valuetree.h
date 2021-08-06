// Generated from template $TEMPLATE$.h
#ifndef _RAWTREE2_HH_
#define _RAWTREE2_HH_

#include "collection_base.h"
#include "allocators/valueallocator.h"

Fn_cleanupInterface(ValueTree);
Fn_traversableInterface(ValueTree);

typedef struct ValueTree_node ValueTree_node;

typedef ValueTree_node * PValueTreeNode;

typedef struct ValueTree_node {
    PValueTreeNode right;
    PValueTreeNode left;
    u8 values[];
} ValueTree_node;

typedef struct ValueTree_node ValueTreeNode;


typedef struct ValueTree_data {
    int $objectSize; 
    PFn_compare compare; 
    int length; 
    PValueTreeNode $rootNode;
    PValueAllocator $allocator;
} ValueTree_data;

typedef ValueTree_data * PValueTree;


__attribute__ ((nonnull (1, 3))) void ValueTree_init(PValueTree this, int objectSize, PFn_compare);
__attribute__ ((nonnull (2))) PValueTree ValueTree_create(int objectSize, PFn_compare);

Fn_traverse(ValueTree, PValueTree);
Fn_find(ValueTree, PValueTree, Pointer);
Fn_addEntry(ValueTree, PValueTree, Pointer);
Fn_deleteEntry(ValueTree, PValueTree, Pointer);
Fn_clear(ValueTree, PValueTree);

Fn_free(ValueTree, PValueTree);
Fn_freeChildren(ValueTree, PValueTree);

#endif