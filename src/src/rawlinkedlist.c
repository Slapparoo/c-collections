#include "rawlinkedlist.h"

#define INITIAL_CAPACITY 64

#define CONTROL_SZ (sizeof(u32) * 2)
#define ENTRYBYCONTENT(content, size, ix) content + ((CONTROL_SZ + size) * ix)
#define ENTRY(this, ix) ENTRYBYCONTENT(this->content, this->entrySize, ix)
#define ENTRY_VALUE(e) e + CONTROL_SZ
#define ENTRY_NEXT(e) *(u32*)e
#define ENTRY_PREVIOUS(e) *(u32*)(e + sizeof(u32))
#define STORE_VALUE(this, to, from) Pointers.copy(to + CONTROL_SZ, from, this->entrySize);

void RawLinkedlist_$resize(const PRawLinkedlist this, u32 newSize) {
    this->$resizing = true;

    this->content = Pointers.realloc(this->content, (CONTROL_SZ + this->entrySize) * newSize);

    RawBitslist.free(&this->nodeMap);

    this->nodeMap = RawBitslist.create(newSize, true);
    for (int i = 0; i < this->$capacity; i++) {
        RawBitslist.clearBit(this->nodeMap, i);
    }

    this->$capacity = newSize;
    this->$resizing = false;
}


/**
 * @brief 
 * 
 * manages memory for the items in the list each entry store the index of teh next entry, the index of teh previous entry, and the object data
 * 
 * | next    | previous |   object   | .....
 * |                                 |
 * @param itemSize 
 * @return PRawLinkedlist 
 */
PRawLinkedlist RawLinkedlist_create(u32 itemSize) {
    PRawLinkedlist this = Pointers.malloc(sizeof(RawLinkedlist_data));
    RawLinkedlist_init(this, itemSize);
    return this;
}

void RawLinkedlist_init(const PRawLinkedlist this, u32 itemSize) {
    this->$capacity = INITIAL_CAPACITY;
    this->entrySize = itemSize;
    this->length = 0;
    this->headIx = -1;
    this->tailIx = -1;
    this->content = Pointers.malloc((CONTROL_SZ + this->entrySize) * this->$capacity);
    this->$resizing = false;
    this->$nextShrink = INITIAL_CAPACITY;
    this->$nextGrow = INITIAL_CAPACITY;
    this->nodeMap = RawBitslist.create(this->$capacity, true);
}

void RawLinkedlist_pushHead(PRawLinkedlist this, Pointer entry) {
    // find next slot
    if (this->length == this->$capacity) {
        this->$nextShrink = this->$capacity;
        RawLinkedlist_$resize(this, this->$capacity * 2);
        this->$nextGrow *= 2;
    }

    int newIx = 0;

    if (RawBitslist.isBit(this->nodeMap, this->length)) {
        newIx = this->length;
    } else {
        newIx = RawBitslist.findFirst(this->nodeMap);
    }

    if (newIx == -1) {
        printError("Failed to find empty bucket nx:%i, sz:%i", newIx, this->length);
        exit(1);
    }

    Pointer newEntry = ENTRY(this, newIx);
    STORE_VALUE(this, newEntry, entry);
    RawBitslist.clearBit(this->nodeMap, newIx);
    ENTRY_NEXT(newEntry) = this->headIx;
    ENTRY_PREVIOUS(newEntry) = -1;
    // first entry
    if (this->length == 0) {
        this->tailIx = newIx;    
    } else {
        ENTRY_PREVIOUS(ENTRY(this, this->headIx)) = newIx;
    }

    this->headIx = newIx;
    this->length++;    
}

void RawLinkedlist_pushTail(PRawLinkedlist this, Pointer value) {
    // find next slot
    int newIx = 0;

    if (RawBitslist.isBit(this->nodeMap, this->length)) {
        newIx = this->length;
    } else {
        newIx = RawBitslist.findFirst(this->nodeMap);
    }

    if (newIx == -1) {
        printError("Failed to find empty bucket nx:%i, sz:%i", newIx, this->length);
        exit(1);
    }

    if (this->length == this->$capacity) {
        this->$nextShrink = this->$capacity;
        RawLinkedlist_$resize(this, this->$capacity * 2);
        this->$nextGrow *= 2;
    }


    Pointer newEntry = ENTRY(this, newIx);
    STORE_VALUE(this, newEntry, value);
    RawBitslist.clearBit(this->nodeMap, newIx);
    ENTRY_NEXT(newEntry) = -1;
    ENTRY_PREVIOUS(newEntry) = this->tailIx;

    if (this->tailIx != -1) {
        Pointer prevTailEntry = ENTRY(this, this->tailIx);
        ENTRY_NEXT(prevTailEntry) = newIx;    
    } else {
        // first entry
        this->headIx = newIx;
    }

    this->tailIx = newIx;
    this->length++;    
}

Pointer RawLinkedlist_popHead(PRawLinkedlist this) {

    if (this->headIx == -1) {
        return null;
    }
    // find next slot
    int returnIx = this->headIx;

    Pointer returnEntry = ENTRY(this, returnIx);
    int newHeadIx = ENTRY_NEXT(returnEntry);

    this->headIx = newHeadIx;
    if (newHeadIx != -1) {
        Pointer newHeadEntry = ENTRY(this, newHeadIx);
        ENTRY_PREVIOUS(newHeadEntry) = -1;
    }

    // last entry
    if (this->length == 1) {
        this->tailIx = -1;    
    }

    RawBitslist.setBit(this->nodeMap, returnIx);
    this->length--;

    return ENTRY_VALUE(returnEntry);        
}

Pointer RawLinkedlist_popTail(PRawLinkedlist this) {
 if (this->headIx == -1) {
        return null;
    }
    // find next slot
    int returnIx = this->tailIx;

    Pointer returnEntry = ENTRY(this, returnIx);
    int newTailIx = ENTRY_PREVIOUS(returnEntry);

    this->tailIx = newTailIx;
    if (newTailIx != -1) {
        Pointer newTailEntry = ENTRY(this, newTailIx);
        ENTRY_NEXT(newTailEntry) = -1;
    }

    // last entry
    if (this->length == 1) {
        this->headIx = -1;    
    }

    RawBitslist.setBit(this->nodeMap, returnIx);
    this->length--;

    return ENTRY_VALUE(returnEntry);        
}

Pointer RawLinkedlist_get(const PRawLinkedlist this, i32 index) {
    if (this->headIx == -1) {
        return null;
    }

    if (index > this->length || index < 0) {
        return null;
    }

    // go forward or back
    if (this->length /2 > index) {
        Pointer returnEntry = ENTRY(this, this->headIx);
        for (int i = 0; i < index; i++) {
            returnEntry = ENTRY(this, ENTRY_NEXT(returnEntry));
        }

        return ENTRY_VALUE(returnEntry);
    } else {
        Pointer returnEntry = ENTRY(this, this->tailIx);
        for (int i = this->length -1; i > index; i--) {
            returnEntry = ENTRY(this, ENTRY_PREVIOUS(returnEntry));
        }

        return ENTRY_VALUE(returnEntry);
    }
}

void RawLinkedlist_remove(PRawLinkedlist this, int index) {
  if (this->headIx == -1) {
        return;
    }

    if (index > this->length || index < 0) {
        return;
    }

    Pointer removeEntry;

    // go forward or back
    if (this->length /2 > index) {
        removeEntry = ENTRY(this, this->headIx);
        for (int i = 0; i < index; i++) {
            removeEntry = ENTRY(this, ENTRY_NEXT(removeEntry));
        }

    } else {
        removeEntry = ENTRY(this, this->tailIx);
        for (int i = this->length -1; i > index; i--) {
            removeEntry = ENTRY(this, ENTRY_PREVIOUS(removeEntry));
        }
    }

    i32 next = ENTRY_NEXT(removeEntry);
    i32 prev = ENTRY_PREVIOUS(removeEntry);
    if (next != -1) {
        ENTRY_PREVIOUS((ENTRY(this, next))) = prev;
    }
    if (prev != -1) {
        ENTRY_NEXT((ENTRY(this, prev))) = next;
    }

    this->length--;
    if (this->length == 1) {
        if (index == 0) {
            // removed head
            this->headIx = this->tailIx;
        } else {
            // removed tail
            this->tailIx = this->headIx;
        }
    } else if (this->length == 0) {
        this->tailIx = -1;
        this->headIx = -1;
    }
    RawBitslist.setBit(this->nodeMap, index);

}

void RawLinkedlist_clear(PRawLinkedlist this) {

}

void RawLinkedlist_free(PRawLinkedlist* this) {
    RawLinkedlist_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

void RawLinkedlist_freeChildren(PRawLinkedlist this) {
    Pointers.free(this->content);
    RawBitslist.free(&this->nodeMap);
}

void RawLinkedlist_traverseForward(PRawLinkedlist this, Pointer other, Fn_traverse traverser) {
    if (this->length < 1) {
        return;
    }
    Pointer returnEntry = ENTRY(this, this->headIx);
    for (int i = 0; i < this->length -1; i++) {
        traverser(this, other, ENTRY_VALUE(returnEntry));
        returnEntry = ENTRY(this, ENTRY_NEXT(returnEntry));
    }
    traverser(this, other, ENTRY_VALUE(returnEntry));
}

void RawLinkedlist_traverseBackward(PRawLinkedlist this, Pointer other, Fn_traverse traverser) {
    if (this->length < 1) {
        return;
    }
    Pointer returnEntry = ENTRY(this, this->tailIx);
    for (int i = 0; i < this->length -1; i++) {
        traverser(this, other, ENTRY_VALUE(returnEntry));
        returnEntry = ENTRY(this, ENTRY_PREVIOUS(returnEntry));
    }
    traverser(this, other, ENTRY_VALUE(returnEntry));
}

void RawLinkedlist_traverseUnordered(PRawLinkedlist this, Pointer other, Fn_traverse traverser) {
    int ix = 0, i = 0;
    while (i < this->length) {
        if (!RawBitslist.isBit(this->nodeMap, ix)) {
           traverser(this, other, ENTRY_VALUE(ENTRY(this, ix)));
           i++; 
        }
        ix++;
    }
}

void RawLinkedlist_swap(PRawLinkedlist this, i32 a, i32 b) {
    if (a == b) {
        return;
    } else if (a == -1 || a > this->length) {
        return;
    } else if (b == -1 || b > this->length) {
        return;
    }
    Pointers.swap(&a, &b, this->entrySize);
}

RawLinkedlist_code RawLinkedlist = {
    &RawLinkedlist_create,
    &RawLinkedlist_init,
    &RawLinkedlist_pushHead,
    &RawLinkedlist_pushTail,
    &RawLinkedlist_popHead,
    &RawLinkedlist_popTail,
    &RawLinkedlist_get,
    &RawLinkedlist_remove,
    &RawLinkedlist_clear,
    &RawLinkedlist_free,
    &RawLinkedlist_freeChildren,
    &RawLinkedlist_traverseForward,
    &RawLinkedlist_traverseBackward,
    &RawLinkedlist_traverseUnordered,
    &RawLinkedlist_swap
};
