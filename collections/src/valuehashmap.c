#include "byvalue/valuehashmap.h"
#include "byvalue/valuetree.h"
#include <assert.h>

implementMap(ValueHashmap);
implementCleanup(ValueHashmap);
implementTraversable(ValueHashmap);

#define INITIAL_CAPACITY 64
// Amount to fill before resize as a fraction of CAPACITY_PERCENT so 3/4 would be 75%
#define FILL_PERCENT 3
#define REDUCE_PERCENT 2
#define CAPACITY_PERCENT 4

#define ENTRYBYCONTENT(content, size, ix) content + ((sizeof(i32) + size) * ix)
#define ENTRY(this, ix) ENTRYBYCONTENT(this->content, this->$entrySize, ix)
#define ENTRY_VALUE(e) e + sizeof(i32)
#define ENTRY_HASH(e) *(i32*)e
#define STORE_VALUE(this, to, from) memcpy(to + sizeof(i32), from, this->$entrySize);

static __inline__ void ValueHashmap_$alloc(PValueHashmap this) {
    int memSize = (sizeof(i32) + this->$entrySize) * this->$capacity;
    this->content = malloc(memSize);
    Pointers.setAll(this->content, 0, memSize);

    this->$collisionMap = Bitslist_create(this->$capacity, false);
    // printDebug("cmp:%p, cmap:%p", this->compare, this->$collisionMap);
}

static __inline__ Pointer ValueHashmap_$put(PValueHashmap this, Pointer entry, int hc, int ix) {
    Pointer entryValue = ENTRY(this, ix);
    if (Bitslist_isBit(this->$collisionMap, ix)) {
        ValueTree_addEntry(this->$tree, entry);
    } else {
        // check for collision
        // if (ENTRY_HASH(entryValue)) {
        if (ENTRY_HASH(entryValue)) {
            
            // collision
            if (hc == ENTRY_HASH(entryValue)) {
                // replace 
                STORE_VALUE(this, entryValue, entry);
            } else {
                // spill into the tree
                Bitslist_setBit(this->$collisionMap, ix);
                ValueTree_addEntry(this->$tree, entry);
                this->length++;
                // printError("fix ValueTree.insert tp return the pointer %p", ENTRY_VALUE(entryValue));
                return ENTRY_VALUE(entryValue);
            }
        } else {
            ENTRY_HASH(entryValue) = hc;
            STORE_VALUE(this, entryValue, entry);
        }
    }

    this->length++;    
    return ENTRY_VALUE(entryValue);
}

Fn_traverser(ValueHashmap, PValueHashmap, Pointer) {
// void treeTraverser(PValueHashmap this, Pointer other, Pointer entry) {
    // printInfo("Tree traveser this:%p, entry:%p", this, entry);
    ValueHashmap_putEntry(this, entry);
}

static __inline__ void ValueHashmap_$resize(PValueHashmap this, i32 newSize) {
    if (this->$resizing) {
        return;
    }
    // @todo fix after including tree
    this->$resizing = true;

    printInfo("resize ValueHashmap old:%i, new:%i, len:%i, tree:%i", this->$capacity, newSize, this->length, this->$tree->length);

    Pointer oldContent = this->content;
    i32 oldCapacity = this->$capacity;
    i32 oldLength = this->length;
    PValueTree oldTree = 0;

    if (this->$tree->length) {
        oldTree = this->$tree;
        this->$tree = ValueTree_create(this->$entrySize, this->compare);
    }

    PBitslist oldCollisionMap = this->$collisionMap;

    this->$capacity = newSize;
    this->length = 0;

    ValueHashmap_$alloc(this);

    for (i32 i = 0; i < oldCapacity; i++) {
        Pointer entry = ENTRYBYCONTENT(oldContent, this->$entrySize, i);
        if (entry && ENTRY_HASH(entry)) {
            ValueHashmap_$put(this, entry, ENTRY_HASH(entry), ENTRY_HASH(entry) % this->$capacity);
        }
    }

    // 
    if (oldTree) {
        ValueTree_traverse(oldTree, this, &ValueHashmap_traverser);
        ValueTree_free(&oldTree);
    }

    Pointers.free(oldContent);
    Bitslist_free(&oldCollisionMap);
    this->$resizing = false;
}
/**
 * @brief | keys    | values  |
 *        | | | | | | | | | | |
 * 
 * @param this 
 * @param key 
 * @return int 
 */
Fn_get(ValueHashmap, PValueHashmap, Pointer, Pointer)
{
    //get the hash
    i32 hc = abs(this->hashCode(key));
    i32 ix = hc % this->$capacity;

    // entry had a collision and has been moved to the Tree
    if (Bitslist_isBit(this->$collisionMap, ix)) {

        Pointer entry = ENTRY(this, ix);
        if (ENTRY_HASH(entry) == hc) {
            return ENTRY_VALUE(entry);
        }

        return ValueTree_find(this->$tree, key);
    }

    
    Pointer entry =  ENTRY(this, ix);
    if (entry && ENTRY_HASH(entry)) {
        return ENTRY_VALUE(entry);
    } else {
        // printDebug("no entry found %i", ENTRY_HASH(key));
        return null;
    }
}

Fn_put(ValueHashmap, PValueHashmap, Pointer, Pointer, Pointer) {
    printError("method not implemented! %s", "ValueHashmap_put");
    return null;
}


Fn_putEntry(ValueHashmap, PValueHashmap, Pointer) {
    if (this->length >= this->$nextGrow) {
        ValueHashmap_$resize(this, this->$capacity * 2);
        this->$nextGrow = this->$capacity * FILL_PERCENT / CAPACITY_PERCENT;
    }

    //get the hash
    int hc = abs(this->hashCode(entry));
    int ix = hc % this->$capacity;

    return ValueHashmap_$put(this, entry, hc, ix);
}

Fn_deleteEntry(ValueHashmap, PValueHashmap, Pointer)
{

    //get the hash
    i32 hc = abs(this->hashCode(entry));
    i32 ix = hc % this->$capacity;

    // entry had a collision and has been moved to the Tree
    if (Bitslist_isBit(this->$collisionMap, ix)) {
        // this collision map doesnt keep a counter so the bit cant be cleared
        // unless the tree is empty
        Pointer entry = ENTRY(this, ix);
        if (ENTRY_HASH(entry) == hc) {
            // move the rtee entry into the table
            Pointer treeEntry = ValueTree_find(this->$tree, entry);
            if (treeEntry) {
                // Pointer treeEntry = ValueTree_getEntry(this->$tree, ix);
                Pointers.copy(ENTRY_VALUE(entry), treeEntry, this->$entrySize);
                ValueTree_deleteEntry(this->$tree, treeEntry);
                ENTRY_HASH(entry) = this->hashCode(treeEntry);
                this->length--;
            }
        } else {
            Pointer res = ValueTree_find(this->$tree, entry);
            if (res) {
                ValueTree_deleteEntry(this->$tree, entry);
                this->length--;
            }
        }
        if (this->$tree->length == 0) {
            Bitslist_clearBit(this->$collisionMap, ix);
        }
    } else {
        Pointer entry = ENTRY(this, ix);

        if (entry && ENTRY_HASH(entry)) {
            this->length--;
            ENTRY_HASH(entry) = 0;
            Pointers.setAll(ENTRY_VALUE(entry), 0, this->$entrySize);
        }
    }
    // // if (this->length * CAPACITY_PERCENT < REDUCE_PERCENT * this->$capacity && !this->$resizing && this->$capacity > INITIAL_CAPACITY) {
    // //     ValueHashmap_resize(this, this->$capacity / 2);
    // // }

    // // // removed fragmentation
    // // if (this->removed * CAPACITY_PERCENT > REDUCE_PERCENT * this->$capacity && !this->$resizing) {
    // //     ValueHashmap_resize(this, this->$capacity);
    // // }
}

Fn_clear(ValueHashmap, PValueHashmap)
{
    // @todo GC?
    ValueHashmap_freeChildren(this);
    ValueHashmap_init(this, this->$entrySize, this->hashCode, this->compare);
}

Fn_traverse(ValueHashmap, PValueHashmap)
{
    u32 entryOffset = 0;
    u32 entrySize = sizeof(i32) + this->$entrySize; 
    u32 memSize = entrySize * this->$capacity;
    while (entryOffset < memSize) {
        Pointer entry = this->content + entryOffset;
        if (ENTRY_HASH(entry)) {
            traverser(other, entry + sizeof(i32));
        }
        entryOffset += entrySize;
    }
    ValueTree_traverse(this->$tree, other, traverser);
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void ValueHashmap_init(PValueHashmap this, int itemSize, PFn_hashcode hashcode, PFn_compare compare) 
{
    printDebug("PValueHashmap item size :%lu", sizeof(i32));
    
    assert(this);
    assert(itemSize);
    assert(hashcode);
    assert(compare);

    this->$capacity = INITIAL_CAPACITY;
    this->$entrySize = itemSize;
    this->length = 0;
    this->$nextGrow = this->$capacity * FILL_PERCENT / CAPACITY_PERCENT;
    this->$nextShrink = INITIAL_CAPACITY;
    this->hashCode = hashcode;
    this->compare = compare;
    this->$resizing = false;

    ValueHashmap_$alloc(this);
    this->$tree = ValueTree_create(this->$entrySize, this->compare);
}

PValueHashmap ValueHashmap_create(int itemSize, PFn_hashcode hashcode, PFn_compare compare) {
    PValueHashmap this = Pointers.malloc(sizeof(ValueHashmap_data)); 
    printDebug("PValueHashmap size :%lu", sizeof(ValueHashmap_data));
    assert(this);
    assert(hashcode);
    assert(compare);
    ValueHashmap_init(this, itemSize, hashcode, compare);
    return this;
}

Fn_contains(ValueHashmap, PValueHashmap, Pointer) {
    return false;
}

Fn_freeChildren(ValueHashmap, PValueHashmap) {
    free(this->content);
    ValueTree_free(&this->$tree);
    Bitslist_free(&this->$collisionMap);
}

Fn_free(ValueHashmap, PValueHashmap) {
    ValueHashmap_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

struct ValueHashmap_code ValueHashmap = {
    &ValueHashmap_create,
    &ValueHashmap_init,
    &ValueHashmap_put,
    &ValueHashmap_putEntry,
    &ValueHashmap_get,
    &ValueHashmap_deleteEntry,
    &ValueHashmap_contains,
    &ValueHashmap_clear,
    &ValueHashmap_free,
    &ValueHashmap_freeChildren,
    &ValueHashmap_traverse
};
