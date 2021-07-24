// Generated from template $TEMPLATE$.h
#include "rawhashmap.h"
#include "rawtree.h"
#include <pthread.h>
#include <assert.h>

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

// static __inline__ void RawHashmap_$copyTo(Pointer dest, Pointer src, u32 size) {
//     switch (size) {
//         case 1: {
//                 *(u8*)dest = *(u8*)src; return;
//         }
//         case 2: {
//                 *(u16*)dest = *(u16*)src; return;
//         }
//         case 4: {
//                 *(u32*)dest = *(u32*)src; return;
//         }
//         case 8: {
//                 *(u64*)dest = *(u64*)src; return;
//         }
//         default: {
//             memcpy(dest, src, size);
//         }


//     }
// }



static __inline__ void RawHashmap_$alloc(PRawHashmap this) {
    int memSize = (sizeof(i32) + this->$entrySize) * this->$capacity;
    this->content = malloc(memSize);
    Pointers.setAll(this->content, 0, memSize);

    this->$collisionMap = RawBitslist.create(this->$capacity, false);
    // printDebug("cmp:%p, cmap:%p", this->compare, this->$collisionMap);
}

static __inline__ Pointer RawHashmap_$put(PRawHashmap this, Pointer entry, int hc, int ix) {
    Pointer entryValue = ENTRY(this, ix);
    if (RawBitslist.isBit(this->$collisionMap, ix)) {
        RawTree.insert(this->$tree, entry);
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
                RawBitslist.setBit(this->$collisionMap, ix);
                RawTree.insert(this->$tree, entry);
                this->length++;
                // printError("fix RawTree.insert tp return the pointer %p", ENTRY_VALUE(entryValue));
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

void treeTraverser(PRawHashmap this, Pointer other, Pointer entry) {
    // printInfo("Tree traveser this:%p, entry:%p", this, entry);
    RawHashmap_put(other, entry);
}

void myThreadFree(Pointer values[])
{
    // printDebug("       free     %p, %p, %p, %p", values, values[0], values[1], *values);
    Pointers.free(values[0]);
    RawBitslist.free(&values[1]);
    Pointers.free(values);
#ifdef __THREADS__
    pthread_detach(pthread_self());
#endif    
}

static __inline__ void RawHashmap_$resize(PRawHashmap this, i32 newSize) {
    if (this->$resizing) {
        return;
    }
    // @todo fix after including tree
    this->$resizing = true;

    printInfo("resize RawHashmap old:%i, new:%i, len:%i, tree:%i", this->$capacity, newSize, this->length, this->$tree->size);

    Pointer oldContent = this->content;
    i32 oldCapacity = this->$capacity;
    i32 oldLength = this->length;
    PRawTree oldTree = 0;

    if (this->$tree->size) {
        oldTree = this->$tree;
        this->$tree = RawTree.create(this->$entrySize, this->compare);
    }

    PRawBitslist oldCollisionMap = this->$collisionMap;

    this->$capacity = newSize;
    this->length = 0;

    RawHashmap_$alloc(this);

    for (i32 i = 0; i < oldCapacity; i++) {
        Pointer entry = ENTRYBYCONTENT(oldContent, this->$entrySize, i);
        if (entry && ENTRY_HASH(entry)) {
            RawHashmap_$put(this, entry, ENTRY_HASH(entry), ENTRY_HASH(entry) % this->$capacity);
        }
    }

    // 
    if (oldTree) {
        RawTree.traverseUnordered(oldTree, this, &treeTraverser);
        RawTree.free(&oldTree);
    }

    // *** @todo iterate tree items

    // printDebug("pre free %p, %p, %p", args, args[0], args[1]);
    // printDebug("pre free %p, %p, %p", aa, *(Pointer*)aa, *(Pointer*)(aa + 8));

// #ifdef __THREADS__
//     Pointer args[] = {oldContent, oldCollisionMap};
//     Pointer aa = malloc(16);
//     memcpy(aa, args, 16);
//     pthread_t thread_id;
//     pthread_create(&thread_id, NULL, &myThreadFree, aa);
// #else
    Pointers.free(oldContent);
    RawBitslist.free(&oldCollisionMap);
// #endif    
    // Pointers.free(oldContent);
    // Pointers.free(oldCollisionMap->array);
    // Pointers.free(oldCollisionMap);
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

Pointer RawHashmap_get(PRawHashmap this, Pointer key)
{
    //get the hash
    i32 hc = abs(this->hashCode(key));
    i32 ix = hc % this->$capacity;

    // entry had a collision and has been moved to the Tree
    if (RawBitslist.isBit(this->$collisionMap, ix)) {

        Pointer entry = ENTRY(this, ix);
        if (ENTRY_HASH(entry) == hc) {
            return ENTRY_VALUE(entry);
        }

        int res = RawTree_find(this->$tree, key);
        
        if (res == -1) {
            return null;
        }
        return RawTree.get(this->$tree, res);
    }

    
    Pointer entry =  ENTRY(this, ix);
    if (entry && ENTRY_HASH(entry)) {
        return ENTRY_VALUE(entry);
    } else {
        // printDebug("no entry found %i", ENTRY_HASH(key));
        return null;
    }
}


Pointer RawHashmap_put(PRawHashmap this, Pointer entry) {
    if (this->length >= this->$nextGrow) {
        RawHashmap_$resize(this, this->$capacity * 2);
        this->$nextGrow = this->$capacity * FILL_PERCENT / CAPACITY_PERCENT;
    }

    //get the hash
    int hc = abs(this->hashCode(entry));
    int ix = hc % this->$capacity;

    return RawHashmap_$put(this, entry, hc, ix);
}

void RawHashmap_remove(PRawHashmap this, Pointer key)
{

    //get the hash
    i32 hc = abs(this->hashCode(key));
    i32 ix = hc % this->$capacity;

    // entry had a collision and has been moved to the Tree
    if (RawBitslist.isBit(this->$collisionMap, ix)) {
        // this collision map doesnt keep a counter so the bit cant be cleared
        // unless the tree is empty
        Pointer entry = ENTRY(this, ix);
        if (ENTRY_HASH(entry) == hc) {
            // move the rtee entry into the table
            i32 ix = RawTree.find(this->$tree, key);
            if (ix != -1) {
                Pointer treeEntry = RawTree.get(this->$tree, ix);
                Pointers.copy(ENTRY_VALUE(entry), treeEntry, this->$entrySize);
                RawTree.deleteNode(this->$tree, treeEntry);
                ENTRY_HASH(entry) = this->hashCode(treeEntry);
                this->length--;
            }
        } else {
            i32 ix = RawTree.find(this->$tree, key);
            if (ix != -1) {
                RawTree.deleteNode(this->$tree, key);
                this->length--;
            }
        }
        if (this->$tree->size == 0) {
            RawBitslist.clearBit(this->$collisionMap, ix);
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
    // //     RawHashmap_resize(this, this->$capacity / 2);
    // // }

    // // // removed fragmentation
    // // if (this->removed * CAPACITY_PERCENT > REDUCE_PERCENT * this->$capacity && !this->$resizing) {
    // //     RawHashmap_resize(this, this->$capacity);
    // // }
}

void RawHashmap_clear(PRawHashmap this)
{
    // Pointers.free(this->content);

    // this->$capacity = INITIAL_CAPACITY;
    // this->$entrySize = **sizeof(Pointer);
    // this->content = Pointers.malloc((this->itemSize *  this->$capacity) * 2);
    // Pointers.setAll(this->content, 0, (this->itemSize *  this->$capacity) * 2);
    
    // this->conflicts = 0;
    // this->removed = 0;
    // this->length = 0;
    // this->$nextGrow = this->$capacity * FILL_PERCENT / CAPACITY_PERCENT;
    // RawTree.delnode(this->$tree->rootNode);
}

void RawHashmap_traverse(PRawHashmap this, Pointer other, Fn_traverse traverser) {

    u32 entryOffset = 0;
    u32 entrySize = sizeof(i32) + this->$entrySize; 
    u32 memSize = entrySize * this->$capacity;
    while (entryOffset < memSize) {
        Pointer entry = this->content + entryOffset;
        if (ENTRY_HASH(entry)) {
            traverser(this, other, entry + sizeof(i32));
        }
        entryOffset += entrySize;
    }
    RawTree.traverseUnordered(this->$tree, other, traverser);
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void RawHashmap_init(PRawHashmap this, int itemSize, Fn_hashcode hashcode, Fn_compare compare) {
    printDebug("PRawHashmap item size :%lu", sizeof(i32));
    
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

    RawHashmap_$alloc(this);
    this->$tree = RawTree.create(this->$entrySize, this->compare);

}

PRawHashmap RawHashmap_create(int itemSize, Fn_hashcode hashcode, Fn_compare compare) {
    PRawHashmap this = Pointers.malloc(sizeof(RawHashmap_data)); 
    printDebug("PRawHashmap size :%lu", sizeof(RawHashmap_data));
    assert(this);
    assert(hashcode);
    assert(compare);
    RawHashmap_init(this, itemSize, hashcode, compare);
    return this;
}

void RawHashmap_freeChildren(const PRawHashmap this) {
    free(this->content);
    RawTree.free(&this->$tree);
    RawBitslist.free(&this->$collisionMap);
}

void RawHashmap_free(PRawHashmap * this) {
    RawHashmap_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

struct RawHashmap_code RawHashmap = {
    &RawHashmap_create,
    &RawHashmap_init,
    &RawHashmap_put,
    &RawHashmap_get,
    &RawHashmap_remove,
    &RawHashmap_clear,
    &RawHashmap_free,
    &RawHashmap_freeChildren,
    &RawHashmap_traverse
};


