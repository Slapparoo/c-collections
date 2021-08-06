#include "allocators/valueallocator.h"
#include "bypointer/pointerlifo.h"

#define INITIALSIZE 64

/**
 * @brief memory allocator for value stores where the entry size is the same
 * preallocates a block of memory then uses a recycler for re-allocations
 * does not free main memory until cleanup.
 * 
 * The Lifo recycler should theoritical give good "memory cache" performance
 * by resuing "hot" memory.
 * 
 * @param entrySize 
 * @return PValueAllocator 
 */

implementCleanup(ValueAllocator);
implementPool(ValueAllocator);


PValueAllocator ValueAllocator_create(u32 entrySize) {
    PValueAllocator this = Pointers.malloc(sizeof(ValueAllocatordata));
    ValueAllocator_init(this, entrySize);
    return this;
}

void ValueAllocator_init(const PValueAllocator this, u32 entrySize) {
    this->$capacity = INITIALSIZE;
    this->$entrySize = entrySize;
    this->memBlock = Pointers.malloc(this->$capacity * this->$entrySize);
    this->$entry = this->memBlock; 
    this->stackFunction = PointerLifo_stackInterface();
    this->stackCleanup = PointerLifo_cleanupInterface();
    this->recyclerData = PointerLifo_create();
    this->allocationData = PointerLifo_create();
}

Fn_borrow(ValueAllocator, PValueAllocator, Pointer) {
    Pointer res = this->stackFunction->pop(this->recyclerData);
    if (res) {
        return res;
    }

    if (this->$entry == this->memBlock + (this->$capacity * this->$entrySize)) {
        // no more here it will come back to the recycler
        // put in the allocationData for cleanup and allocate a new block
        this->stackFunction->push(this->allocationData, this->memBlock);
        this->memBlock = Pointers.malloc(this->$capacity * this->$entrySize);
        this->$entry = this->memBlock; 
    }

    res = this->$entry;
    this->$entry += this->$entrySize;
    return res;
}

Fn_returnIt(ValueAllocator, PValueAllocator, Pointer) {
    if (value == null) {
        printError("error returning memory %p", this);
        return;
    }
    this->stackFunction->push(this->recyclerData, value);
}

Fn_free(ValueAllocator, PValueAllocator) {
    ValueAllocator_freeChildren(*this);
    Pointers.free(*this);
    *this = null;
}

Fn_freeChildren(ValueAllocator, PValueAllocator) {
    while (this->stackFunction->peek(this->allocationData)) {
        Pointers.free(this->stackFunction->pop(this->allocationData));    
    }
    this->stackCleanup->free(&this->recyclerData);
    this->stackCleanup->free(&this->allocationData);
    Pointers.free(this->memBlock);
}

/**
 * @brief reset back to the initial state by freeing
 * all but the most recent memory allocations
 * 
 */
Fn_clear(ValueAllocator, PValueAllocator) {
    while (this->stackFunction->peek(this->allocationData)) {
        Pointers.free(this->stackFunction->pop(this->allocationData));    
    }
    this->$entry = this->memBlock;
}
