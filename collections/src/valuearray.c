#include "byvalue/valuearray.h"
#include "collection_base.h"
#include "collection_interfaces.h"

/**
 * @brief Circular/wrap around buffer that stores Pointers FIFO
 * Raw refeers to value store as opposed to pointer store, to the whole content of the object will 
 * be copied into the array, using the addEntry will retunr a pointer to the space which can be used to initialise 
 * and store values in.
 * 
 */

/**
 * interface implementations
 * 
 */
implementStackAs(ValueArray, ValueArray_head_push, ValueArray_head_pop, ValueArray_head_peek);
implementQueueAs(ValueArray, ValueArray_tail_push, ValueArray_head_pop, ValueArray_head_peek);
implementList(ValueArray);
implementCleanup(ValueArray);
implementTraversable(ValueArray);
implementSortableAs(ValueArray, ValueArray_swapEntry, ValueArray_compareEntry, ValueArray_length, null);

#define INITIAL_CAPACITY 64

/**
 * Convienience variable names 
 * 
 */
#define length this->length
#define $capacity this->$capacity
#define $startIndex this->$startIndex
#define $endIndex this->$endIndex
#define $values this->$values
#define $entrySize this->$entrySize

#define getEntry ValueArray_getEntry
#define copyValues(dest, src, number) memcpy(dest, src, $entrySize * (number));    
#define moveValues(dest, src, number) memmove(dest, src, $entrySize * (number));    

#define indexOf(index) $values + ((index) * $entrySize)
#define offsetOf(index) indexOf(($startIndex + index) % $capacity)

#define tmpCopyEntrybyIndex(index) \
    Pointer tmp = Pointers.malloc($entrySize); \
    copyValues(tmp, indexOf(index), 1) 

#define tmpCopyEntrybyOffset(index) \
    Pointer tmp = Pointers.malloc($entrySize); \
    copyValues(tmp, offsetOf(index), 1) 

void ValueArray_$grow(PValueArray this, u32 newSize) {

    $values = Pointers.realloc($values, newSize * $entrySize);
    // roll around to start = 0;
    if ($startIndex == 0) {
        $endIndex = length;
    } else {
        moveValues(indexOf($capacity + $startIndex), indexOf($startIndex), length - $startIndex);
        $startIndex += length;
    }

    $capacity = newSize;
}

/**
 * @brief slide left one entry used by deletes and relocates
 * 
 */
void ValueArray_$moveLeft(PValueArray this, u32 startIndex, u32 endIndex) {
    // endIndex > startIndex

    Pointer ixStart = offsetOf(startIndex);
    Pointer ixEnd = offsetOf(endIndex);



    if ($startIndex < $endIndex || ixStart < ixEnd) {
        // no wrap required
         moveValues(ixStart, (ixStart + $entrySize), endIndex - startIndex);
    } else {
        Pointer ix$Start = offsetOf($startIndex);
        Pointer ix$End = offsetOf($endIndex);

        if (ixStart < ix$Start && ixEnd < ix$Start) {
            moveValues(ixStart, ixStart + $entrySize, endIndex - startIndex);
        } else {
            // the fancy wrap move 
            // shift top area
            // move spanning entry
            // shift bottom area

            // boundry??

            u32 destIx = $startIndex + startIndex;
            u32 number = $capacity - destIx;
            moveValues(offsetOf(destIx), offsetOf(destIx + 1), number);
            copyValues(offsetOf($capacity -1), $values, 1);
            moveValues($values, offsetOf(1), (endIndex + $startIndex) % $capacity);
        }
    }
}

Fn_removeEntry(ValueArray, PValueArray) {
    // @todo add GC?
    if (index >= length) {
        return;
    }

    if (index == length - 1) {
        $endIndex--;
        if ($endIndex < 0) {
            $endIndex = $capacity;
        }
    } else if (index == 0) {
        $startIndex++;
        if ($startIndex >= $capacity) {
            $startIndex = 0;
        }
    } else {
        ValueArray_$moveLeft(this, index, $endIndex--);
    }

    length--;
}

/**
 * @brief Construct a new Fn_relocateEntry object
 * I should always move right low to high
 * 
 */
Fn_relocateEntry(ValueArray, PValueArray) {
    if (dest > src) {
        // no dice
        return -1;
    }
    if (src - dest == 1) {
        ValueArray_swapEntry(this, src, dest);
    } else {
        if ($startIndex < $endIndex) {
            tmpCopyEntrybyOffset(src);
            moveValues(offsetOf(dest + 1), offsetOf(dest), src - dest);
            copyValues(offsetOf(dest), tmp, 1);    
            Pointers.free(tmp);
        } else {
            tmpCopyEntrybyOffset(src);
            ValueArray_$moveLeft(this, dest, src);
            copyValues(offsetOf(dest), tmp, 1);    
            Pointers.free(tmp);
        }
    }

    u64* values = $values;
    return 0;
}

Fn_swapEntry(ValueArray, PValueArray) {

    tmpCopyEntrybyOffset(a);
    copyValues(offsetOf(a), offsetOf(b), 1);    
    copyValues(offsetOf(b), tmp, 1);    
    Pointers.free(tmp);
}

Fn_traverse(ValueArray, PValueArray) {
    // @todo make faster
    for (int i = 0; i < length; i++) {
        traverser(other, getEntry(this, i));
    }
}

Fn_setEntry(ValueArray, PValueArray, Pointer) {
    if (index > length) {
        return;
    }
    copyValues(offsetOf(index), value, 1);    
}

Fn_getEntry(ValueArray, PValueArray, Pointer) {
    if (index > length) {
        return null;
    }
    return offsetOf(index);
}

Fn_insertEntry(ValueArray, PValueArray, Pointer) {
    if (index > length) {
        return;
    }
}

Fn_addEntry(ValueArray, PValueArray, Pointer) {
    ValueArray_tail_push(this, value);
    return ValueArray_tail_peek(this);
}

Fn_compareEntry(ValueArray, PValueArray) {
    // @todo improve
    if (this->compare) {
        return this->compare(getEntry(this, a), getEntry(this, b));
    }
    return getEntry(this, a) - getEntry(this, b);
}

/**
 * @brief Construct a new Fn_contains object
 * thie value of the pointer not what it points to
 */
Fn_contains(ValueArray, PValueArray, Pointer) {
    for (int i = 0; i < length; i++) {
        if (key == getEntry(this, i)) {
            return true;
        }
    }
    return false;
}

Fn_clear(ValueArray, PValueArray) {
    // @ todo GC
    Pointers.realloc($values, INITIAL_CAPACITY);
    $capacity = INITIAL_CAPACITY;
    length = 0;
    $startIndex = 0;
    $endIndex = 0;
}

Fn_push(ValueArray_tail, PValueArray, Pointer) {
    if (length == $capacity) {
        ValueArray_$grow(this, $capacity + INITIAL_CAPACITY);
    }

    copyValues(indexOf($endIndex++), value, 1);    
    if ($endIndex == $capacity) {
        $endIndex = 0;
    }
    length++;
}

Fn_peek(ValueArray_head, PValueArray, Pointer) {
    if (length == 0) {
        return null;
    }
    return indexOf($startIndex);
}

/**
 * @brief Construct a new Fn_pop object
 * its up to the consumer to make a copy
 * 
 */
Fn_pop(ValueArray_head, PValueArray, Pointer) {
    if (length == 0) {
        return null;
    }
    Pointer res = indexOf($startIndex++);

    if ($startIndex >= $capacity) {
        $startIndex = 0;
    }

    length--;
    return res;
}

Fn_push(ValueArray_head, PValueArray, Pointer) {
    if (length == $capacity) {
        ValueArray_$grow(this, $capacity + INITIAL_CAPACITY);
    }

    if ($startIndex == 0) {
        $startIndex = $capacity;
    }

    copyValues(indexOf(--$startIndex), value, 1);    
    length++;
}

Fn_peek(ValueArray_tail, PValueArray, Pointer) {
    if (length == 0) {
        return null;
    }
    return indexOf($endIndex);
}

Fn_length(ValueArray, PValueArray) {
    return length;
}

/**
 * @brief Construct a new Fn_pop object
 * its up to the comsummer to make a copy of the data
 */
Fn_pop(ValueArray_tail, PValueArray, Pointer) {
    if (length == 0) {
        return null;
    }
    Pointer res = indexOf($endIndex--);

    if ($endIndex <= 0) {
        $endIndex = $capacity;
    }

    length--;
    return res;
}

PValueArray ValueArray_create(u32 entrySize) {
    PValueArray res = Pointers.malloc(sizeof(ValueArraydata));
    ValueArray_init(res, entrySize);
    return res;
}

void ValueArray_init(PValueArray this, u32 entrySize) {
    $entrySize = entrySize;
    $capacity = INITIAL_CAPACITY;
    $endIndex = 0;
    $startIndex = 0;
    length = 0;
    $values = Pointers.malloc($entrySize * INITIAL_CAPACITY);
}

Fn_free(ValueArray, PValueArray) {
    ValueArray_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

Fn_freeChildren(ValueArray, PValueArray) {
    Pointers.free($values);
}
