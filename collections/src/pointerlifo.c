#include "bypointer/pointerlifo.h"


implementCleanup(PointerLifo);
implementStackAs(PointerLifo, PointerLifo_push, PointerLifo_pop, PointerLifo_peek);

/**
 * @brief PointerLifo/Stack implementation
 * 
 * @return PPointerLifo 
 */
PPointerLifo PointerLifo_create() {
    PPointerLifo this = Pointers.malloc(sizeof(PointerLifodata));
    PointerLifo_init(this);
    return this;
}

void PointerLifo_init(const PPointerLifo this) {
    this->tip = null;
}

Fn_push(PointerLifo, PPointerLifo, Pointer) {
    if (!value) {
        return;
    }

    PPointerLifoEntry newEntry = Pointers.malloc(sizeof(PointerLifoEntry));
    newEntry->entry = value;
    newEntry->next = null;

    if (!this->tip) {
        this->tip = newEntry;
    } else {
        newEntry->next = this->tip;
        this->tip = newEntry;
    }

    return;
}

Fn_pop(PointerLifo, PPointerLifo, Pointer) {
    if (!this->tip) {
        return null;
    }
    PPointerLifoEntry entry = this->tip;
    Pointer result = entry->entry;
    this->tip = entry->next;
    Pointers.free(entry);

    return result;
}

Fn_peek(PointerLifo, PPointerLifo, Pointer) {
    if (!this->tip) {
        return null;
    }

    return this->tip->entry;
}

Fn_free(PointerLifo, PPointerLifo) {
    PointerLifo_freeChildren(*this);
    Pointers.free(*this);
    *this = null;
}

Fn_freeChildren(PointerLifo, PPointerLifo) {
    PPointerLifoEntry entry = this->tip;
    while (entry) {
        PPointerLifoEntry tmp = entry->next;
        Pointers.free(entry);
        entry = tmp;
    }
    this->tip = null;
}