#include "bypointer/pointerarray.h"
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
implementStackAs(PointerArray, PointerArray_head_push, PointerArray_head_pop, PointerArray_head_peek);
implementQueueAs(PointerArray, PointerArray_tail_push, PointerArray_head_pop, PointerArray_head_peek);
implementList(PointerArray);
implementCleanup(PointerArray);
implementTraversable(PointerArray);
implementSortableAs(PointerArray, PointerArray_swapEntry, PointerArray_compareEntry, PointerArray_length, null);

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

#define getEntry PointerArray_getEntry
#define copyValues(dest, src, number) memcpy(dest, src, sizeof(Pointer) * (number));    
#define moveValues(dest, src, number) memmove(dest, src, sizeof(Pointer) * (number));    

#define pointerOf(index) &$values[index]
#define indexOffset(index) ($startIndex + index) % $capacity
#define offsetOf(index) pointerOf(indexOffset(index))
#define valueOf(index) $values[indexOffset(index)]


void PointerArray_$grow(PPointerArray this, u32 newSize) {

    $values = Pointers.realloc($values, newSize * sizeof(Pointer));
    // roll around to start = 0;
    if ($startIndex == 0) {
        $endIndex = length;
    } else {
        moveValues(pointerOf($capacity + $startIndex), pointerOf($startIndex), length - $startIndex);
        $startIndex += length;
    }

    $capacity = newSize;
}

/**
 * @brief slide left one entry used by deletes and relocates
 * 
 */
void PointerArray_$moveLeft(PPointerArray this, u32 startIndex, u32 endIndex) {
    if ($startIndex < $endIndex) {
        // no wrap required
         moveValues(&$values[startIndex], &$values[startIndex + 1], endIndex - startIndex);
    } else {
        int ixStart = indexOffset(startIndex);
        int ixEnd = indexOffset(endIndex);

        if (ixStart < $startIndex && ixEnd < $startIndex) {
            moveValues(&$values[ixStart], &$values[ixStart + 1], endIndex - startIndex);
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

Fn_removeEntry(PointerArray, PPointerArray) {
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
        PointerArray_$moveLeft(this, index, $endIndex--);
    }

    length--;
}

/**
 * @brief Construct a new Fn_relocateEntry object
 * I should always move right low to high
 * 
 */
Fn_relocateEntry(PointerArray, PPointerArray) {
    if (dest > src) {
        // no dice
        return -1;
    }
    if (src - dest == 1) {
        PointerArray_swapEntry(this, src, dest);
    } else {
        if ($startIndex < $endIndex) {
            Pointer tmp = valueOf(src);
            moveValues(offsetOf(dest + 1), offsetOf(dest), src - dest);
            valueOf(dest) = tmp;    
        } else {
            Pointer tmp = valueOf(src);
            PointerArray_$moveLeft(this, dest, src);
            valueOf(dest) = tmp;    
        }
    }

    u64* values = $values;
    return 0;
}

Fn_swapEntry(PointerArray, PPointerArray) {
    Pointer tmp = valueOf(a);
    valueOf(a) = valueOf(b);
    valueOf(b) = tmp;    
}

Fn_traverse(PointerArray, PPointerArray) {
    // @todo make faster
    for (int i = 0; i < length; i++) {
        traverser(other, valueOf(i));
    }
}

Fn_setEntry(PointerArray, PPointerArray, Pointer) {
    if (index > length) {
        return;
    }
    valueOf(index) = value;
}

Fn_getEntry(PointerArray, PPointerArray, Pointer) {
    if (index > length) {
        return null;
    }
    return valueOf(index);
}

Fn_insertEntry(PointerArray, PPointerArray, Pointer) {
    if (index > length) {
        return;
    }
}

Fn_addEntry(PointerArray, PPointerArray, Pointer) {
    PointerArray_tail_push(this, value);
    return PointerArray_tail_peek(this);
}

Fn_compareEntry(PointerArray, PPointerArray) {
    // @todo improve
    if (this->compare) {
        return this->compare(valueOf(a), valueOf(b));
    }
    return valueOf(a) - valueOf(b);
}

/**
 * @brief Construct a new Fn_contains object
 * thie value of the pointer not what it points to
 */
Fn_contains(PointerArray, PPointerArray, Pointer) {
    for (int i = 0; i < length; i++) {
        if (key == valueOf(i)) {
            return true;
        }
    }
    return false;
}

Fn_clear(PointerArray, PPointerArray) {
    // @ todo GC
    Pointers.realloc($values, INITIAL_CAPACITY);
    $capacity = INITIAL_CAPACITY;
    length = 0;
    $startIndex = 0;
    $endIndex = 0;
}

Fn_push(PointerArray_tail, PPointerArray, Pointer) {
    if (length == $capacity) {
        PointerArray_$grow(this, $capacity + INITIAL_CAPACITY);
    }

    $values[$endIndex++] = value;
    if ($endIndex == $capacity) {
        $endIndex = 0;
    }
    length++;
}

Fn_peek(PointerArray_head, PPointerArray, Pointer) {
    if (length == 0) {
        return null;
    }
    return $values[$startIndex];
}

/**
 * @brief Construct a new Fn_pop object
 * its up to the consumer to make a copy
 * 
 */
Fn_pop(PointerArray_head, PPointerArray, Pointer) {
    if (length == 0) {
        return null;
    }
    Pointer res = $values[$startIndex++];

    if ($startIndex >= $capacity) {
        $startIndex = 0;
    }

    length--;
    return res;
}

Fn_push(PointerArray_head, PPointerArray, Pointer) {
    if (length == $capacity) {
        PointerArray_$grow(this, $capacity + INITIAL_CAPACITY);
    }

    if ($startIndex == 0) {
        $startIndex = $capacity;
    }

    $values[--$startIndex] = value;
    length++;
}

Fn_peek(PointerArray_tail, PPointerArray, Pointer) {
    if (length == 0) {
        return null;
    }
    return $values[$endIndex];
}

Fn_length(PointerArray, PPointerArray) {
    return length;
}

/**
 * @brief Construct a new Fn_pop object
 * its up to the comsummer to make a copy of the data
 */
Fn_pop(PointerArray_tail, PPointerArray, Pointer) {
    if (length == 0) {
        return null;
    }
    Pointer res = $values[$endIndex--];

    if ($endIndex <= 0) {
        $endIndex = $capacity;
    }

    length--;
    return res;
}

PPointerArray PointerArray_create(u32 entrySize) {
    PPointerArray res = Pointers.malloc(sizeof(PointerArraydata));
    PointerArray_init(res);
    return res;
}

void PointerArray_init(PPointerArray this) {
    $capacity = INITIAL_CAPACITY;
    $endIndex = 0;
    $startIndex = 0;
    length = 0;
    this->compare = null;
    $values = Pointers.malloc(sizeof(Pointer) * INITIAL_CAPACITY);
}

Fn_free(PointerArray, PPointerArray) {
    PointerArray_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

Fn_freeChildren(PointerArray, PPointerArray) {
    Pointers.free($values);
}
