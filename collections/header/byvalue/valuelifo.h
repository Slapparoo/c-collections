/**
 * @brief Single linked list each item contains a pointer to the next item
 * the value as opposed to the pointer is stored in the list
 * 
 */
#ifndef _VALUELIFO_HH_
#define _VALUELIFO_HH_

#include "collection_base.h"
#include "allocators/valueallocator.h"


/**
 * interfaces
 * 
 */
Fn_cleanupInterface(ValueLifo);
Fn_stackInterface(ValueLifo);

typedef struct ValueLifodata {
    Pointer tip;
    u32 $entrySize;
    PValueAllocator valueAllocator;
    Pool pool;
} ValueLifodata;

typedef ValueLifodata * PValueLifo;

PValueLifo ValueLifo_create(u32 entrySize);
__attribute__ ((nonnull (1))) void ValueLifo_init(const PValueLifo this, u32 entrySize);
Fn_push(ValueLifo, PValueLifo, Pointer);
Fn_pop(ValueLifo, PValueLifo, Pointer);
Fn_peek(ValueLifo, PValueLifo, Pointer);
Fn_free(ValueLifo, PValueLifo);
Fn_freeChildren(ValueLifo, PValueLifo);

#endif