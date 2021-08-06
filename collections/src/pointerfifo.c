#include "bypointer/pointerfifo.h"


implementCleanup(PointerFifo);
implementQueue(PointerFifo);

/**
 * @brief PointerFifo/Stack implementation
 * 
 * push to tail pop from tip
 * 
 * @return PPointerFifo 
 */
PPointerFifo PointerFifo_create() {
    PPointerFifo this = Pointers.malloc(sizeof(PointerFifodata));
    PointerFifo_init(this);
    return this;
}

void PointerFifo_init(const PPointerFifo this) {
    this->tip = null;
    this->tail = null;
}

Fn_push(PointerFifo, PPointerFifo, Pointer) {
    if (!value) {
        return;
    }

    PPointerFifoEntry newEntry = Pointers.malloc(sizeof(PointerFifoEntry));
    newEntry->entry = value;
    newEntry->next = null;

    if (!this->tail) {
        this->tail = newEntry;
        this->tip = newEntry;
    } else {
        this->tail->next = newEntry;
        this->tail = newEntry;
    }

    return;
}

Fn_pop(PointerFifo, PPointerFifo, Pointer) {
    if (!this->tip) {
        return null;
    }
    PPointerFifoEntry entry = this->tip;
    Pointer result = entry->entry;
    this->tip = entry->next;
    Pointers.free(entry);

    if (!this->tip) {
        this->tail = null;
    }

    return result;
}

Fn_peek(PointerFifo, PPointerFifo, Pointer) {
    if (!this->tip) {
        return null;
    }

    return this->tip->entry;
}

Fn_free(PointerFifo, PPointerFifo) {
    PointerFifo_freeChildren(*this);
    Pointers.free(*this);
    *this = null;
}

Fn_freeChildren(PointerFifo, PPointerFifo) {
    PPointerFifoEntry entry = this->tip;
    while (entry) {
        PPointerFifoEntry tmp = entry->next;
        Pointers.free(entry);
        entry = tmp;
    }
    this->tip = null;
}