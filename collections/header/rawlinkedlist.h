#ifndef _RAWLINKEDLIST_HH_
#define _RAWLINKEDLIST_HH_

#include "data-types.h"
#include "object-types.h"
#include "rawbitslist.h"

struct RawLinkedlist_code RawLinkedlist;

#define __C_LINKEDLIST_DATA_ \
    u32 length; \
    u32 entrySize; \
    boolean $resizing; \
    u32 $capacity; \
    u32 $nextGrow; \
    u32 $nextShrink; \
    i32 headIx; \
    i32 tailIx; \
    PRawBitslist nodeMap; \
    Pointer content; 


typedef struct RawLinkedlist_data {
     __C_LINKEDLIST_DATA_
} RawLinkedlist_data;

typedef RawLinkedlist_data * PRawLinkedlist;

#define __C_LINKEDLIST_CODE_ \
    PRawLinkedlist (*create)(u32 itemSize); \
    __attribute__ ((nonnull (1))) void (*init)(const PRawLinkedlist linkedlist, u32 itemSize); \
    __attribute__ ((nonnull (1))) void (*pushHead)(PRawLinkedlist linkedlist, Pointer entry); \
    __attribute__ ((nonnull (1))) void (*pushTail)(PRawLinkedlist linkedlist, Pointer entry); \
    __attribute__ ((nonnull (1))) Pointer (*popHead)(PRawLinkedlist linkedlist); \
    __attribute__ ((nonnull (1))) Pointer (*popTail)(PRawLinkedlist linkedlist); \
    __attribute__ ((nonnull (1))) Pointer (*get)(const PRawLinkedlist linkedlist, i32 index); \
    __attribute__ ((nonnull (1))) void (*remove)(PRawLinkedlist linkedlist, int index); \
    __attribute__ ((nonnull (1))) void (*clear)(PRawLinkedlist linkedlist); \
    __attribute__ ((nonnull (1))) void (*free)(PRawLinkedlist* linkedlist); \
    __attribute__ ((nonnull (1))) void (*freeChildren)(PRawLinkedlist linkedlist); \
    __attribute__ ((nonnull (1, 3))) void (*traverseFromHead)(PRawLinkedlist this, Pointer other, Fn_traverse traverser); \
    __attribute__ ((nonnull (1, 3))) void (*traverseFromTail)(PRawLinkedlist this, Pointer other, Fn_traverse traverser); \
    __attribute__ ((nonnull (1, 3))) void (*traverseUnordered)(PRawLinkedlist this, Pointer other, Fn_traverse traverser); \
    __attribute__ ((nonnull (1))) void (*swap)(const PRawLinkedlist linkedlist, i32 a, i32 b); 

typedef struct RawLinkedlist_code {
    __C_LINKEDLIST_CODE_
} RawLinkedlist_code;

PRawLinkedlist RawLinkedlist_create(u32 itemSize);
__attribute__ ((nonnull (1))) void RawLinkedlist_init(const PRawLinkedlist this, u32 itemSize);
__attribute__ ((nonnull (1))) void RawLinkedlist_pushHead(PRawLinkedlist this, Pointer entry);
__attribute__ ((nonnull (1))) void RawLinkedlist_pushTail(PRawLinkedlist this, Pointer entry);
__attribute__ ((nonnull (1))) Pointer RawLinkedlist_popHead(PRawLinkedlist this);
__attribute__ ((nonnull (1))) Pointer RawLinkedlist_popTail(PRawLinkedlist this);
__attribute__ ((nonnull (1))) Pointer RawLinkedlist_popTail(PRawLinkedlist this);
__attribute__ ((nonnull (1))) Pointer RawLinkedlist_get(const PRawLinkedlist this, i32 index);
__attribute__ ((nonnull (1))) void RawLinkedlist_remove(PRawLinkedlist this, int index);
__attribute__ ((nonnull (1))) void RawLinkedlist_clear(PRawLinkedlist this);
__attribute__ ((nonnull (1))) void RawLinkedlist_free(PRawLinkedlist* this);
__attribute__ ((nonnull (1))) void RawLinkedlist_freeChildren(PRawLinkedlist this);
__attribute__ ((nonnull (1, 2))) void RawLinkedlist_traverseForward(PRawLinkedlist this, Pointer other, Fn_traverse traverser);
__attribute__ ((nonnull (1, 2))) void RawLinkedlist_traverseBackward(PRawLinkedlist this, Pointer other, Fn_traverse traverser);
__attribute__ ((nonnull (1, 2))) void RawLinkedlist_traverseUnordered(PRawLinkedlist this, Pointer other, Fn_traverse traverser);
__attribute__ ((nonnull (1))) void RawLinkedlist_swap(PRawLinkedlist this, i32 a, i32 b);

#endif