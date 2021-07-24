// Generated from template $TEMPLATE$.h
#ifndef _RAWTREE_HH_
#define _RAWTREE_HH_

#include "data-types.h"
#include "object-types.h"
#include "rawbitslist.h"

typedef struct RawTree_node {
    int right;
    int left;
} RawTree_node;

typedef struct RawTree_node RawTreeNode;
typedef RawTree_node * PRawTreeNode;

struct RawTree_code RawTree;

#define __C_TREE_DATA_ \
    int objectSize; \
    int nodeSize; \
    Pointer nodes; \
    PRawBitslist nodesMap; \
    Fn_compare compare; \
    int size;   \
    int $capacity; \
    boolean $resizing; \
    int rootNode;

typedef struct RawTree_data {
    __C_TREE_DATA_
} RawTree_data;

typedef RawTree_data * PRawTree;

#define __C_TREE_CODE_ \
    __attribute__ ((nonnull (1))) void (*free)(RawTree_data **); \
    __attribute__ ((nonnull (1))) void (*freeChildren)(RawTree_data *); \
    __attribute__ ((nonnull (2))) RawTree_data * (*create)(int objectSize, Fn_compare); \
    __attribute__ ((nonnull (1, 3))) void (*init)(RawTree_data *this, int objectSize, Fn_compare); \
    __attribute__ ((nonnull (1, 2))) Pointer (*insert)(PRawTree this, Pointer object); \
    __attribute__ ((nonnull (1, 2))) void (*deleteNode)(PRawTree this, Pointer object); \
    __attribute__ ((nonnull (1, 2))) int (*find)(PRawTree this, Pointer object); \
    __attribute__ ((nonnull (1, 3))) void (*traverseInOrder)(PRawTree this, Pointer other, Fn_traverse); \
    __attribute__ ((nonnull (1, 3))) void (*traverseReverseOrder)(PRawTree this, Pointer other, Fn_traverse); \
    __attribute__ ((nonnull (1, 3))) void (*traversePreOrder)(PRawTree this, Pointer other, Fn_traverse); \
    __attribute__ ((nonnull (1, 3))) void (*traversePostOrder)(PRawTree this, Pointer other, Fn_traverse); \
    __attribute__ ((nonnull (1))) Pointer (*get)(RawTree_data *, int index); \
    __attribute__ ((nonnull (1, 3))) void (*traverseUnordered)(PRawTree this, Pointer other, Fn_traverse);

typedef struct RawTree_code {
    __C_TREE_CODE_
} RawTree_code;

__attribute__ ((nonnull (1, 2))) Pointer RawTree_insert(PRawTree this, Pointer object);
__attribute__ ((nonnull (1))) Pointer RawTree_get(PRawTree this, int index);
__attribute__ ((nonnull (1))) void RawTree_clear(PRawTree this);
__attribute__ ((nonnull (1, 3))) void RawTree_unorderedTraverse(PRawTree this, Pointer other, Fn_traverse);
__attribute__ ((nonnull (1))) void RawTree_freeChildren(PRawTree this);
__attribute__ ((nonnull (1, 2))) int RawTree_find(PRawTree this, Pointer object);
__attribute__ ((nonnull (1, 2))) void RawTree_deleteNode(PRawTree this, Pointer object);
__attribute__ ((nonnull (1, 3))) void RawTree_traverse_inorder(PRawTree this, Pointer other, Fn_traverse);
__attribute__ ((nonnull (1, 3))) void RawTree_traverse_reverseOrder(PRawTree this, Pointer other, Fn_traverse);
__attribute__ ((nonnull (1, 3))) void RawTree_traverse_inpreorder(PRawTree this, Pointer other, Fn_traverse);
__attribute__ ((nonnull (1, 3))) void RawTree_traverse_inpostorder(PRawTree this, Pointer other, Fn_traverse);
__attribute__ ((nonnull (1, 3))) void RawTree_init(PRawTree this, int objectSize, Fn_compare);
__attribute__ ((nonnull (2))) PRawTree RawTree_create(int objectSize, Fn_compare);
__attribute__ ((nonnull (1))) void RawTree_free(PRawTree *this);

#endif