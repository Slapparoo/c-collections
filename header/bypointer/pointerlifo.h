/**
 * @brief Single linked list each item contains a pointer to the next item
 * 
 */
#ifndef _POINTERLIFO_HH_
#define _POINTERLIFO_HH_

#include "collection_base.h"
#include "collection_interfaces.h"


/**
 * interfaces
 * 
 */
Fn_cleanupInterface(PointerLifo);
Fn_stackInterface(PointerLifo);

/**
 * data
 * 
 */
typedef struct PointerLifoEntry PointerLifoEntry;
typedef PointerLifoEntry * PPointerLifoEntry;

typedef struct PointerLifoEntry {
    PPointerLifoEntry next;
    Pointer entry;
} PointerLifoEntry;


typedef struct PointerLifodata {
    PPointerLifoEntry tip;
} PointerLifodata;

typedef PointerLifodata * PPointerLifo;

PPointerLifo PointerLifo_create();
__attribute__ ((nonnull (1))) void PointerLifo_init(const PPointerLifo this);
Fn_push(PointerLifo, PPointerLifo, Pointer);
Fn_pop(PointerLifo, PPointerLifo, Pointer);
Fn_peek(PointerLifo, PPointerLifo, Pointer);
Fn_free(PointerLifo, PPointerLifo);
Fn_freeChildren(PointerLifo, PPointerLifo);

#endif