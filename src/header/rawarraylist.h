// Generated from template $TEMPLATE$.h
#ifndef _RAWARRAYLIST_HH_
#define _RAWARRAYLIST_HH_

#include "data-types.h"
#include "object-types.h"

#define __C_RAWARRAYLIST_DATA_ \
    Pointer items; \
    u32 length; \
    u32 $capacity; \
    u32 $itemSize; 


typedef struct RawArraylistdata {
    __C_RAWARRAYLIST_DATA_
} RawArraylistdata;

typedef RawArraylistdata * PRawArraylist;

#define __C_RAWARRAYLIST_CODE_ \
    PRawArraylist (*create)(u32 initialSize, u32 itemSize); \
    __attribute__ ((nonnull (1))) void (*init)(const PRawArraylist this, u32 initialSize, u32 itemSize); \
    __attribute__ ((nonnull (1))) Pointer (*add)(const PRawArraylist this); \
    __attribute__ ((nonnull (1))) Pointer (*get)(const PRawArraylist this, i32 index); \
    __attribute__ ((nonnull (1))) Pointer (*set)(const PRawArraylist this, i32 index, Pointer value); \
    __attribute__ ((nonnull (1))) void (*clear)(const PRawArraylist this); \
    __attribute__ ((nonnull (1))) void (*swap)(const PRawArraylist this, int a, int b); \
    __attribute__ ((nonnull (1))) void (*free)(PRawArraylist* this); \
    __attribute__ ((nonnull (1))) void (*freeChildren)(const PRawArraylist this); \
    __attribute__ ((nonnull (1, 3))) void (*traverseForward)(const PRawArraylist this, Pointer other, Fn_traverse traverser); \
    __attribute__ ((nonnull (1, 3))) void (*traverseBackward)(const PRawArraylist this, Pointer other, Fn_traverse traverser); \
    __attribute__ ((nonnull (1, 3))) void (*traverseUnordered)(const PRawArraylist this, Pointer other, Fn_traverse traverser);

typedef struct RawArraylistcode {
    __C_RAWARRAYLIST_CODE_
} RawArraylistcode;

struct RawArraylistcode RawArraylist;

PRawArraylist RawArraylist_create(u32 initialSize, u32 itemSize); 
__attribute__ ((nonnull (1))) void RawArraylist_init(const PRawArraylist this, u32 initialSize, u32 itemSize); 
__attribute__ ((nonnull (1))) Pointer RawArraylist_add(const PRawArraylist this); 
__attribute__ ((nonnull (1))) Pointer RawArraylist_get(const PRawArraylist this, i32 index); 
__attribute__ ((nonnull (1))) Pointer RawArraylist_set(const PRawArraylist this, i32 index, Pointer value); 
__attribute__ ((nonnull (1))) void RawArraylist_clear(const PRawArraylist this); 
__attribute__ ((nonnull (1))) void RawArraylist_swap(const PRawArraylist this, int a, int b); 

__attribute__ ((nonnull (1))) void RawArraylist_free(PRawArraylist* this);
__attribute__ ((nonnull (1))) void RawArraylist_freeChildren(const PRawArraylist this);
__attribute__ ((nonnull (1,3))) void RawArraylist_traverseForward(const PRawArraylist this, Pointer other, Fn_traverse traverser);
__attribute__ ((nonnull (1,3))) void RawArraylist_traverseBackward(const PRawArraylist this, Pointer other, Fn_traverse traverser);
__attribute__ ((nonnull (1,3))) void RawArraylist_traverseUnordered(const PRawArraylist this, Pointer other, Fn_traverse traverser);


#endif