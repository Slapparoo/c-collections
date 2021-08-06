#include "byvalue/valuelifo.h"


implementCleanup(ValueLifo);
implementStack(ValueLifo);

/**
 * @brief ValueLifo/Stack implementation
 * 
 * @return PValueLifo 
 */
PValueLifo ValueLifo_create(u32 entrySize) {
    PValueLifo this = Pointers.malloc(sizeof(ValueLifodata));
    ValueLifo_init(this, entrySize);
    return this;
}

void ValueLifo_init(const PValueLifo this, u32 entrySize) {
    this->tip = null;
    this->valueAllocator = ValueAllocator_create(entrySize + sizeof(Pointer));
    this->pool = ValueAllocator_poolInterface();
    this->$entrySize = entrySize;
}

Fn_push(ValueLifo, PValueLifo, Pointer) {
    if (!value) {
        return;
    }

    Pointer newEntry = this->pool->borrow(this->valueAllocator);
    *(Pointer*)newEntry = null;
    Pointers.copy(newEntry + sizeof(Pointer), value, this->$entrySize);

    if (!this->tip) {
        this->tip = newEntry;
    } else {
        *(Pointer*)newEntry = this->tip;
        this->tip = newEntry;
    }

    return;
}

Fn_pop(ValueLifo, PValueLifo, Pointer) {
    if (!this->tip) {
        return null;
    }
    Pointer entry = this->tip;
    this->tip = *(Pointer*)entry;
    this->pool->returnIt(this->valueAllocator, entry);

    return entry + sizeof(Pointer);
}

Fn_peek(ValueLifo, PValueLifo, Pointer) {
    if (!this->tip) {
        return null;
    }
    
    return this->tip + sizeof(Pointer);
}

Fn_free(ValueLifo, PValueLifo) {
    ValueLifo_freeChildren(*this);
    Pointers.free(*this);
    *this = null;
}

Fn_freeChildren(ValueLifo, PValueLifo) {
    ValueAllocator_free(&this->valueAllocator);
}