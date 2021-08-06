#include "bypointer/pointerlinearmap.h"
#include <assert.h>

/**
 * @brief Construct a new implement Map object
 * the linear map is used where the known quantity of items will be less than ~ 30
 */

implementMap(PointerLinearmap);
implementCleanup(PointerLinearmap);
implementTraversable(PointerLinearmap);

/**
 * @brief | keys    | values  |
 *        | | | | | | | | | | |
 * 
 * @param this 
 * @param key 
 * @return int 
 */
Fn_get(PointerLinearmap, PPointerLinearmap, Pointer, Pointer)
{
    PPointerArray keys = this->$keys;
    PPointerArray values = this->$values;
    for (int i = 0; i < keys->length; i++) {
        Pointer ky = PointerArray_getEntry(keys, i);
        if (ky) {
            if (!this->compare(ky, key)) {
                return PointerArray_getEntry(values, i);
            }
        }
    }
    return null;
}

Fn_put(PointerLinearmap, PPointerLinearmap, Pointer, Pointer, Pointer) {
    PointerMapEntry entry = {key, value};
    return PointerLinearmap_putEntry(this, &entry);
}

Fn_putEntry(PointerLinearmap, PPointerLinearmap, PPointerMapEntry) {
    PPointerArray keys = this->$keys;
    PPointerArray values = this->$values;
    for (int i = 0; i < keys->length; i++) {
        Pointer ky = PointerArray_getEntry(keys, i);
        if (ky && this->compare(entry->key, ky) == 0) {
            // replace
            PointerArray_setEntry(keys, i, entry->key);
            PointerArray_setEntry(values, i, entry->value);
            return entry->value;
        }
    }

    PointerArray_tail_push(keys, entry->key);
    PointerArray_tail_push(values, entry->value);
    this->length++;
    return entry->value;

}

Fn_deleteEntry(PointerLinearmap, PPointerLinearmap, Pointer)
{
    PPointerArray keys = this->$keys;
    PPointerArray values = this->$values;
    for (int i = 0; i < keys->length; i++) {
        Pointer ky = PointerArray_getEntry(keys, i);
        if (ky) {
            if (!this->compare(ky, entry)) {
                PointerArray_removeEntry(keys, i);
                PointerArray_removeEntry(values, i);
                this->length--;
                return;
            }
        }
    }
}

Fn_clear(PointerLinearmap, PPointerLinearmap)
{
    // @todo GC?
    this->length = 0;

    PointerArray_clear(this->$keys);
    PointerArray_create(this->$values);
}


Fn_traverse(PointerLinearmap, PPointerLinearmap)
{
    PPointerArray keys = this->$keys;
    PPointerArray values = this->$values;
    for (int i = 0; i < keys->$capacity; i++) {
        Pointer ky = PointerArray_getEntry(keys, i);
        if (ky) {
            PointerMapEntry entry = {ky, PointerArray_getEntry(values, i)};
            traverser(other, &entry);
        }
    }
}

void PointerLinearmap_init(PPointerLinearmap this, PFn_compare compare) 
{
    assert(this);
    assert(compare);

    this->length = 0;
    this->compare = compare;

    this->$keys = PointerArray_create();
    this->$values = PointerArray_create();
}

PPointerLinearmap PointerLinearmap_create(PFn_compare compare) {
    PPointerLinearmap this = Pointers.malloc(sizeof(PointerLinearmap_data)); 
    assert(this);
    assert(compare);
    PointerLinearmap_init(this, compare);
    return this;
}

Fn_contains(PointerLinearmap, PPointerLinearmap, Pointer) {
    return false;
}

Fn_freeChildren(PointerLinearmap, PPointerLinearmap) {
    PointerArray_free(&this->$keys);
    PointerArray_free(&this->$values);
}

Fn_free(PointerLinearmap, PPointerLinearmap) {
    PointerLinearmap_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

