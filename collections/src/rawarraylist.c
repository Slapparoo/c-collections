// Generated from template $TEMPLATE$.h
#include "rawarraylist.h"

#define INITAIL_CAPACITY 64

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void RawArraylist_init(const PRawArraylist this, u32 initialSize, u32 itemSize) {
    this->$capacity = max(initialSize, INITAIL_CAPACITY);
    this->items = Pointers.malloc(this->$capacity * itemSize);
    this->length = 0;
    this->$itemSize = itemSize;
    // printDebug("init list length:%u, capacity:%u, items:%p ", this->length, this->$capacity, this->items);
}

PRawArraylist RawArraylist_create(u32 initialSize, u32 itemSize) {
    PRawArraylist this = Pointers.malloc(sizeof(RawArraylistdata)); 
    RawArraylist_init(this, initialSize, itemSize);
    return this;
}

void RawArraylist_$grow(PRawArraylist this) {
    this->$capacity += INITAIL_CAPACITY;
    this->items = Pointers.realloc(this->items, this->$capacity * this->$itemSize);
    printDebug("grow list length:%u, capacity:%u, items:%p ", this->length, this->$capacity, this->items);
}

void RawArraylist_$shrink(PRawArraylist this) {
    this->$capacity -= INITAIL_CAPACITY;
    this->items = Pointers.realloc(this->items, this->$capacity * this->$itemSize);
    printDebug("shrink list length:%u, capacity:%u, items:%p ", this->length, this->$capacity, this->items);
}

/**
 * @brief Add an item to the list resizing as required, return a pointer to the tip
 * 
 * @param list 
 * @param element 
 */
Pointer RawArraylist_add(const PRawArraylist this) {
    if (this->length == this->$capacity) {
        RawArraylist_$grow(this);
    }
    Pointer res = this->items + (this->length++ * this->$itemSize);
    return res;
}

Pointer RawArraylist_get(const PRawArraylist this, i32 index) {
    // @todo maybe blowup here
    if (index < 0) {
        return null;
    }
    if (index > this->length) {
        return null;
    }
    return this->items + (index * this->$itemSize);
}

Pointer RawArraylist_set(const PRawArraylist this, i32 index, Pointer value) {
    // @todo maybe blowup here
    if (index < 0) {
        return null;
    }
    if (index > this->length) {
        return null;
    }
    Pointer entry = this->items + (index * this->$itemSize);
    Pointers.copy(entry, value, this->$itemSize);
    return entry;
}

void RawArraylist_swap(const PRawArraylist this, int a, int b) {
    if (a == b) {
        return;
    } else if (a == -1 || a > this->length) {
        return;
    } else if (b == -1 || b > this->length) {
        return;
    }

    Pointers.swap(this->items + (a * this->$itemSize), this->items + (b * this->$itemSize), this->$itemSize);
}

void RawArraylist_clear(const PRawArraylist this) {
    this->$capacity = INITAIL_CAPACITY;
    this->items = Pointers.realloc(this->items, this->$capacity * this->$itemSize);
    this->length = 0;

    Pointers.setAll(this->items, 0, this->$capacity * this->$itemSize);
    printDebug("clear list length:%u, capacity:%u", this->length, this->$capacity);
}

void RawArraylist_freeChildren(const PRawArraylist this) {
    printDebug("freeChildren list length:%u, capacity:%u", this->length, this->$capacity);
    Pointers.free(this->items);
}

void RawArraylist_free(PRawArraylist* this) {
    RawArraylist_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

void RawArraylist_traverseForward(const PRawArraylist this, Pointer other, Fn_traverse traverser) {

}
void RawArraylist_traverseBackward(const PRawArraylist this, Pointer other, Fn_traverse traverser) {

}


struct RawArraylistcode RawArraylist = {
    &RawArraylist_create,
    &RawArraylist_init,
    &RawArraylist_add,
    &RawArraylist_get,
    &RawArraylist_set,
    &RawArraylist_clear,
    &RawArraylist_swap,
    &RawArraylist_free,
    &RawArraylist_freeChildren,
    &RawArraylist_traverseForward,
    &RawArraylist_traverseBackward
};

