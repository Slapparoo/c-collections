#include "bypointer/pointerhashmap.h"
#include "byvalue/valuetree.h"
#include <assert.h>

implementMap(PointerHashmap);
implementCleanup(PointerHashmap);
implementTraversable(PointerHashmap);

#define INITIAL_CAPACITY 64
// Amount to fill before resize as a fraction of CAPACITY_PERCENT so 3/4 would be 75%
#define FILL_PERCENT 3
#define REDUCE_PERCENT 2
#define CAPACITY_PERCENT 4

// #define ENTRYBYCONTENT(content, size, ix) content + ((sizeof(i32) + size) * ix)
// #define ENTRY(this, ix) ENTRYBYCONTENT(this->content, this->$entrySize, ix)
// #define ENTRY_VALUE(e) e + sizeof(i32)
// #define ENTRY_HASH(e) *(i32*)e
// #define STORE_VALUE(this, to, from) memcpy(to + sizeof(i32), from, this->$entrySize);

static __inline__ void PointerHashmap_$alloc(PPointerHashmap this) {
    int memSize = sizeof(PointerMapEntry) * this->$capacity;
    this->content = malloc(memSize);
    Pointers.setAll(this->content, 0, memSize);

    this->$collisionMap = Bitslist_create(this->$capacity, false);
}

static __inline__ Pointer PointerHashmap_$put(PPointerHashmap this, PPointerMapEntry entry, int hc, int ix) {
    PPointerMapEntry hashEntry = &this->content[ix * 2];
    
    if (Bitslist_isBit(this->$collisionMap, ix)) {
        ValueTree_addEntry(this->$tree, entry);
    } else {
        // check for collision
        if (hashEntry->key) {
            
            // collision
            if (hc == this->hashCode(hashEntry->key)) {
                hashEntry->key = entry->key;
                hashEntry->value = entry->value;
            } else {
                // spill into the tree
                Bitslist_setBit(this->$collisionMap, ix);
                ValueTree_addEntry(this->$tree, entry);
            }
        } else {
            hashEntry->key = entry->key;
            hashEntry->value = entry->value;
        }
    }

    this->length++;    
    return entry->value;
}

Fn_traverser(PointerHashmap, PPointerHashmap, Pointer) {
    PointerHashmap_putEntry(this, entry);
}


static __inline__ void PointerHashmap_$resize(PPointerHashmap this, i32 newSize) {
    if (this->$resizing) {
        return;
    }
    // @todo fix after including tree
    this->$resizing = true;

    printInfo("resize PointerHashmap old:%i, new:%i, len:%i, tree:%i", this->$capacity, newSize, this->length, this->$tree->length);

    PPointerMapEntry * oldContent = this->content;
    i32 oldCapacity = this->$capacity;
    i32 oldLength = this->length;
    PValueTree oldTree = 0;

    if (this->$tree->length) {
        oldTree = this->$tree;
        this->$tree = ValueTree_create(sizeof(PointerMapEntry), this->compare);
    }

    PBitslist oldCollisionMap = this->$collisionMap;

    this->$capacity = newSize;
    this->length = 0;

    PointerHashmap_$alloc(this);

    for (i32 i = 0; i < oldCapacity; i++) {
        PPointerMapEntry entry = &oldContent[i * 2];
        if (entry->key) {
            i32 hc = abs(this->hashCode(entry->key));
            PointerHashmap_$put(this, entry, hc, hc % this->$capacity);
        }
    }

    // 
    if (oldTree) {
        ValueTree_traverse(oldTree, this, &PointerHashmap_traverser);
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
Fn_get(PointerHashmap, PPointerHashmap, Pointer, Pointer)
{
    //get the hash
    i32 hc = abs(this->hashCode(key));
    i32 ix = hc % this->$capacity;
    PPointerMapEntry entry = &this->content[ix *2];

    // entry had a collision and may be in the tree the Tree
    // the first entry remains in the hashmap, subsequent entries go to the tree
    if (Bitslist_isBit(this->$collisionMap, ix)) {

        if (abs(this->hashCode(entry->key)) == hc) {
            return entry->value;
        }

        entry = ValueTree_find(this->$tree, &key);
        if (entry) {
            return entry->value;
        }
        return null;
    }
    
    if (entry->key) {
        return entry->value;
    } else {
        return null;
    }
}

Fn_put(PointerHashmap, PPointerHashmap, Pointer, Pointer, Pointer) {
    // printError("method not implemented! %s", "PointerHashmap_put");
    PointerMapEntry entry = {key, value};
    PointerHashmap_putEntry(this, &entry);
    return value;
}

Fn_putEntry(PointerHashmap, PPointerHashmap, PPointerMapEntry) {
    if (this->length >= this->$nextGrow) {
        PointerHashmap_$resize(this, this->$capacity * 2);
        this->$nextGrow = this->$capacity * FILL_PERCENT / CAPACITY_PERCENT;
        // next shrink??
    }

    //get the hash
    int hc = abs(this->hashCode(entry->key));
    int ix = hc % this->$capacity;

    return PointerHashmap_$put(this, entry, hc, ix);
}

Fn_deleteEntry(PointerHashmap, PPointerHashmap, Pointer)
{
    //get the hash
    i32 hc = abs(this->hashCode(entry));
    i32 ix = hc % this->$capacity;
    PPointerMapEntry hashEntry = &this->content[ix * 2];

    // entry had a collision and may be in the Tree
    if (Bitslist_isBit(this->$collisionMap, ix)) {
        // this collision map doesnt keep a counter so the bit cant be cleared
        // unless the tree is empty
        if (abs(this->hashCode(hashEntry->key)) == hc) {
            // move the tree entry into the table
            PPointerMapEntry treeEntry = ValueTree_find(this->$tree, entry);
            if (treeEntry->key) {
                hashEntry->key = treeEntry->key;
                hashEntry->value = treeEntry->value;
                ValueTree_deleteEntry(this->$tree, treeEntry);
                this->length--;
            }
        } else {
            PPointerMapEntry treeEntry = ValueTree_find(this->$tree, entry);
            if (treeEntry) {
                ValueTree_deleteEntry(this->$tree, entry);
                this->length--;
            }
        }
        if (this->$tree->length == 0) {
            Bitslist_clearBit(this->$collisionMap, ix);
        }
    } else {
        if (abs(this->hashCode(hashEntry->key)) == hc) {
            this->length--;
            hashEntry->key = 0;
            hashEntry->value = 0;
        }
    }
    // // if (this->length * CAPACITY_PERCENT < REDUCE_PERCENT * this->$capacity && !this->$resizing && this->$capacity > INITIAL_CAPACITY) {
    // //     PointerHashmap_resize(this, this->$capacity / 2);
    // // }

    // // // removed fragmentation
    // // if (this->removed * CAPACITY_PERCENT > REDUCE_PERCENT * this->$capacity && !this->$resizing) {
    // //     PointerHashmap_resize(this, this->$capacity);
    // // }
}

Fn_clear(PointerHashmap, PPointerHashmap)
{
    // @todo GC?
    PointerHashmap_freeChildren(this);
    PointerHashmap_init(this, this->hashCode, this->compare);
}

// Fn_traverse(PointerHashmap, PPointerHashmap)
// {
//     for (int i = 0; i < this->$capacity; i++) {
//         if (this->content[i]) {
//             traverser(other, this->content[i]);
//         }
//     }
//     ValueTree_traverse(this->$tree, other, traverser);
// }

Fn_traverse(PointerHashmap, PPointerHashmap)
{
    for (int i = 0; i < this->$capacity; i++) {
        if (this->content[i * 2]) {
            traverser(other, &this->content[i * 2]);
        }
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
void PointerHashmap_init(PPointerHashmap this, PFn_hashcode hashcode, PFn_compare compare) 
{
    assert(this);
    assert(hashcode);
    assert(compare);

    this->$capacity = INITIAL_CAPACITY;
    this->length = 0;
    this->$nextGrow = this->$capacity * FILL_PERCENT / CAPACITY_PERCENT;
    this->$nextShrink = INITIAL_CAPACITY;
    this->hashCode = hashcode;
    this->compare = compare;
    this->$resizing = false;

    PointerHashmap_$alloc(this);
    this->$tree = ValueTree_create(sizeof(PointerMapEntry), this->compare);
}

PPointerHashmap PointerHashmap_create(PFn_hashcode hashcode, PFn_compare compare) {
    PPointerHashmap this = Pointers.malloc(sizeof(PointerHashmap_data)); 
    assert(this);
    assert(hashcode);
    assert(compare);
    PointerHashmap_init(this, hashcode, compare);
    return this;
}

Fn_contains(PointerHashmap, PPointerHashmap, Pointer) {
    return false;
}

Fn_freeChildren(PointerHashmap, PPointerHashmap) {
    free(this->content);
    ValueTree_free(&this->$tree);
    Bitslist_free(&this->$collisionMap);
}

Fn_free(PointerHashmap, PPointerHashmap) {
    PointerHashmap_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

