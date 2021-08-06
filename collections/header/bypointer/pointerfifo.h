/**
 * @brief Single linked list each item contains a pointer to the next item
 * 
 */
#ifndef _POINTERFIFO_HH_
#define _POINTERFIFO_HH_

#include "collection_base.h"
#include "collection_interfaces.h"


/**
 * interfaces
 * 
 */
Fn_cleanupInterface(PointerFifo);
Fn_queueInterface(PointerFifo);

/**
 * data
 * 
 */
typedef struct PointerFifoEntry PointerFifoEntry;
typedef PointerFifoEntry * PPointerFifoEntry;

typedef struct PointerFifoEntry {
    PPointerFifoEntry next;
    Pointer entry;
} PointerFifoEntry;


typedef struct PointerFifodata {
    PPointerFifoEntry tip;
    PPointerFifoEntry tail;
} PointerFifodata;

typedef PointerFifodata * PPointerFifo;

PPointerFifo PointerFifo_create();
__attribute__ ((nonnull (1))) void PointerFifo_init(const PPointerFifo this);
Fn_push(PointerFifo, PPointerFifo, Pointer);
Fn_pop(PointerFifo, PPointerFifo, Pointer);
Fn_peek(PointerFifo, PPointerFifo, Pointer);
Fn_free(PointerFifo, PPointerFifo);
Fn_freeChildren(PointerFifo, PPointerFifo);

#endif