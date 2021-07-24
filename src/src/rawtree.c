// Generated from template $TEMPLATE$.h
#include "rawtree.h"
#include "assert.h"

#define RAWTREE_CAPACITY 64

#define RAW_NODE(t, x) t->nodes + ((t->nodeSize + t->objectSize) * x)
#define RAW_OBJECT(t, x) RAW_NODE(t, x) + t->nodeSize

static __inline__ int RawTree_$min(PRawTree this, int index)
{
    PRawTreeNode current = RAW_NODE(this, index);
    /* loop down to find the leftmost leaf */
    while (current && current->left != -1) {
        index = current->left;
        current = RAW_NODE(this, current->left);
    }
 
    return index;
}


/**
 * @brief look for the parent node
 * 
 */
static __inline__ int RawTree_$findParent(PRawTree this, int index, Pointer object)
{
    if (index == -1) {
        return -1;
    }
    PRawTreeNode node = RAW_NODE(this, index);

    if (node->left != -1) {
        if (this->compare(object, RAW_OBJECT(this, node->left)) == 0) {
            return index;
        }
    }

    if (node->right != -1) {
        if (this->compare(object, RAW_OBJECT(this, node->right)) == 0) {
            return index;
        }
    }
    
    int res = this->compare(object, RAW_OBJECT(this, index));

    if (res > 0)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        return RawTree_$findParent(this, node->left, object);
    }
    else if (res < 0)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        return RawTree_$findParent(this, node->right, object);
    }
    // ==
    return -1;
}

static __inline__ void RawTree_$resize(PRawTree this, int newSize) {
    // remalloc and inc size
    this->$resizing = true;

    if (newSize > this->$capacity) {
        printDebug("grow from %i to %i", this->$capacity, newSize);
        
        RawBitslist.free(&this->nodesMap);
        this->nodesMap = RawBitslist_create(newSize, false);

        for (int i = this->$capacity; i < newSize; i++) {
            RawBitslist_setBit(this->nodesMap, i);
        }

        this->nodes = Pointers.realloc(this->nodes, newSize * (this->nodeSize + this->objectSize));
    } else {
        // printDebug("shrink from %i to %i", this->$capacity, newSize);
        // for (int i = newSize; i < this->$capacity; i++) {
        //     if (!RawBitslist_isBit(this->nodesMap, i)) {
                
        //         int parent = RawTree_$findParent(this, this->rootNode, RAW_OBJECT(this, i));
        //         if (parent == -1) {
        //             printError("noparent %i", i);
        //             assert(false);
        //         }

        //         // find new home
        //         int offset = RawBitslist_findFirst(this->nodesMap);

        //         memcpy(RAW_NODE(this, offset), RAW_NODE(this, i), this->objectSize + this->nodeSize); 

        //         RawBitslist.clearBit(this->nodesMap, offset);
        //         RawBitslist.setBit(this->nodesMap, i);
        //         PRawTreeNode parentNode = RAW_NODE(this, parent);
        //         if (parentNode->left == i) {
        //             parentNode->left = offset;
        //         } else {
        //             parentNode->right = offset;
        //         }
        //     }
        // }
        
        // for (int i = newSize; i < this->$capacity; i++) {
        //     if (!RawBitslist.isBit(this->nodesMap, i)) {
        //         printError("RawTree_resize still has object assigned in shrink area? %i", i);
        //         for (int j = 0; j < newSize; j++) {
        //             if (RawBitslist.isBit(this->nodesMap, j)) {
        //                 printError("why not use? %i", j);
        //             }
        //         }
        //     }
        // }

        // RawBitslist.free(&this->nodesMap);
        // this->nodesMap = RawBitslist.create(newSize, false);
        // this->nodes = Pointers.realloc(this->nodes, newSize * (this->nodeSize + this->objectSize));
    }

    this->$capacity = newSize;
    this->$resizing = false;
};

static __inline__ int RawTree_$find(PRawTree this, int index, Pointer object)
{
    int res = this->compare(object, RAW_OBJECT(this, index));

    if (index == -1) {
        return -1;
    }

    if (res > 0)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        return RawTree_$find(this, node->left, object);
    }
    else if (res < 0)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        return RawTree_$find(this, node->right, object);
    }
    // ==
    return index;
}

/**
 * @brief Insert a node into the tree
 * 
 * @param this 
 * @param node 
 * @param object 
 */

static __inline__ Pointer RawTree_$insert(PRawTree this, int * index, Pointer object)
{
    if (*index == -1)
    {
        int nodeIndex = 0;

        if (RawBitslist.isBit(this->nodesMap, this->size)) {
            nodeIndex = this->size;
        } else {
            nodeIndex = RawBitslist.findFirst(this->nodesMap);
        }

        if (nodeIndex == -1) {
            printError("Failed to find empty bucket nx:%i, sz:%i", nodeIndex, this->size);
            exit(1);
        }

        PRawTreeNode node = RAW_NODE(this, nodeIndex);

        node->left = -1;
        node->right = -1;

        memcpy(RAW_OBJECT(this, nodeIndex), object, this->objectSize);
        RawBitslist.clearBit(this->nodesMap, nodeIndex);

        *index = nodeIndex;
        this->size++;
        
        return RAW_OBJECT(this, nodeIndex);
    }

    PRawTreeNode node = RAW_NODE(this, *index);
    int res = this->compare(object, RAW_OBJECT(this, *index));

    if (res > 0)
    {
        RawTree_$insert(this, &node->left, object);
    }
    else if (res < 0)
    {
        RawTree_$insert(this, &node->right, object);
    }
    else
    {
        printError("HashTree insert error == %p", object);
        // over write object data
        memcpy(RAW_OBJECT(this, *index), object, this->objectSize);
    }
}

static __inline__ int RawTree_$deleteNode(PRawTree this, int index, Pointer object) {
    if (index == -1) {
        // manage size
        this->size++;
        return -1;
    }

    PRawTreeNode node = RAW_NODE(this, index);
    int res = this->compare(object, RAW_OBJECT(this, index));

    if (res > 0) {
        node->left = RawTree_$deleteNode(this, node->left, object);
    } else if (res < 0) {
        node->right = RawTree_$deleteNode(this, node->right, object);
    } else {
        // node with only one child or no child
        if (node->left == -1 && node->right == -1) {
            RawBitslist.setBit(this->nodesMap, index);
            return index;
        }
        else if (node->left == -1) {
            RawBitslist.setBit(this->nodesMap, index);
            return node->right;
        }
        else if (node->right == -1) {
            RawBitslist.setBit(this->nodesMap, index);
            return node->left;
        }

        int tmp = RawTree_$min(this, node->right);

        if (tmp > this->$capacity || tmp < 0) {
            printDebug("invalid tmp %i, %i, %i", tmp, this->nodesMap->length, this->$capacity);
            assert(false);
        }

        PRawTreeNode tmpNode = RAW_NODE(this, tmp);
        
        if (tmpNode->left == -1 && tmpNode->right == -1) {
            // last node?
            RawBitslist.setBit(this->nodesMap, tmp);
        } 
 
        // Copy the inorder
        // successor's content to this node
        memcpy(RAW_OBJECT(this, index), RAW_OBJECT(this, tmp), this->objectSize);

        // Delete the inorder successor
        node->right = RawTree_$deleteNode(this, node->right, RAW_OBJECT(this, index));
    }
    return index;
}

/**
 * @brief traverse the items "in order"
 * 
 * @param this 
 * @param traverser 
 */

static __inline__ void RawTree_$traverse_inorder(PRawTree this, Pointer other, int index, Fn_traverse traverser)
{
    if (RawBitslist.isBit(this->nodesMap, index))
    {
        return;
    }

    if (index != -1)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        RawTree_$traverse_inorder(this, other, node->left, traverser);
        traverser(this, other, RAW_OBJECT(this, index));
        RawTree_$traverse_inorder(this, other, node->right, traverser);
    }
}

static __inline__ void RawTree_$traverse_inpreorder(PRawTree this, Pointer other, int index, Fn_traverse traverser)
{

    if (RawBitslist.isBit(this->nodesMap, index))
    {
        return;
    }

    if (index != -1)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        traverser(this, other, RAW_OBJECT(this, index));
        RawTree_$traverse_inpreorder(this, other, node->left, traverser);
        RawTree_$traverse_inpreorder(this, other, node->right, traverser);
    }
}

static __inline__ void RawTree_$traverse_inpostorder(PRawTree this, Pointer other, int index, Fn_traverse traverser)
{

    if (RawBitslist.isBit(this->nodesMap, index))
    {
        return;
    }

    if (index != -1)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        RawTree_$traverse_inpostorder(this, other, node->left, traverser);
        RawTree_$traverse_inpostorder(this, other, node->right, traverser);
        traverser(this, other, RAW_OBJECT(this, index));
    }
}


/**
 * @brief traverse the items "in order"
 * 
 * @param this 
 * @param traverser 
 */

static __inline__ void RawTree_$traverse_reverseOrder(PRawTree this, Pointer other, int index, Fn_traverse traverser)
{
    if (RawBitslist.isBit(this->nodesMap, index))
    {
        return;
    }

    if (index != -1)
    {
        PRawTreeNode node = RAW_NODE(this, index);
        RawTree_$traverse_reverseOrder(this, other, node->right, traverser);
        traverser(this, other, RAW_OBJECT(this, index));
        RawTree_$traverse_reverseOrder(this, other, node->left, traverser);
    }
}


Pointer RawTree_insert(PRawTree this, Pointer object)
{
    if (this->size == this->$capacity && !this->$resizing) {
        // RawTree_$resize(this, this->$capacity + RAWTREE_CAPACITY);
        RawTree_$resize(this, this->$capacity * 2);
    }

    return RawTree_$insert(this, &this->rootNode, object);
}
   
Pointer RawTree_get(PRawTree this, int index) {
    return RAW_OBJECT(this, index);
}   

void RawTree_clear(PRawTree this) {
    RawBitslist.setAll(this->nodesMap, true);
    this->size = 0;
    Pointers.setAll(this->nodes, 0, this->$capacity * (this->nodeSize + this->objectSize));
}   

void RawTree_unorderedTraverse(PRawTree this, Pointer other, Fn_traverse traverser) {
    for (int i = 0; i < this->$capacity; i++) {
        if (!RawBitslist.isBit(this->nodesMap, i)) {
            traverser(this, other, RAW_OBJECT(this, i));
        }
    } 
}   


/**
 * @brief free memory for thei node tree
 * 
 * @param tree 
 */

void RawTree_freeChildren(PRawTree this) {
    if (this->nodes) {
        Pointers.free(this->nodes);
        RawBitslist.free(&this->nodesMap);
    }
}

/**
 * @brief Match a node based on the compare method
 * 
 * @param this 
 * @param node 
 * @param object 
 * @return index 
 */
int RawTree_find(PRawTree this, Pointer object)
{
    if (this->rootNode == -1) return -1;

    if (RawBitslist.isBit(this->nodesMap, this->rootNode))
    {
        return -1;
    }

    return RawTree_$find(this, this->rootNode, object);
}

/* Given a binary search tree
   and a key, this function
   deletes the key and
   returns the new root */
void RawTree_deleteNode(PRawTree this, Pointer object)
{
    if (RawBitslist.isBit(this->nodesMap, this->rootNode))
    {
        return;
    }

    // // shrink and compact not entirely efficient
    // if (this->size == (this->$capacity / 2) && (this->$capacity > RAWTREE_CAPACITY)  && !this->$resizing) {
    //     RawTree_$resize(this, this->$capacity / 2 );
    // }

    int ix = RawTree_$deleteNode(this, this->rootNode, object);
    if (ix != -1) {
        this->rootNode = ix;
        this->size--;
    }

    if (this->$capacity - RawBitslist_getBitCount(this->nodesMap) != this->size && !this->$resizing) {
        printError("size mismatch %i %i, %i %i ", RawBitslist.getBitCount(this->nodesMap), this->$capacity - RawBitslist.getBitCount(this->nodesMap), this->size, ix);
        exit(1);
    }
}

void RawTree_traverse_inorder(PRawTree this, Pointer other, Fn_traverse traverser) {
    RawTree_$traverse_inorder(this, other, this->rootNode, traverser);
}

void RawTree_traverse_reverseOrder(PRawTree this, Pointer other, Fn_traverse traverser) {
    RawTree_$traverse_reverseOrder(this, other, this->rootNode, traverser);
}
/**
 * @brief traverse the items "in preorder"
 * 
 * @param this 
 * @param traverser 
 */

void RawTree_traverse_inpreorder(PRawTree this, Pointer other, Fn_traverse traverser) {
    RawTree_$traverse_inpreorder(this, other, this->rootNode, traverser);
}

/**
 * @brief traverse the items "in preorder"
 * 
 * @param this 
 * @param traverser 
 */

void RawTree_traverse_inpostorder(PRawTree this, Pointer other, Fn_traverse traverser)
{
    RawTree_$traverse_inpostorder(this, other, this->rootNode, traverser);
}



/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void RawTree_init(PRawTree this, int objectSize, Fn_compare compare)
{
    // printDebug("RawTree_init %p, %i, %p ", this, objectSize, compare);
    this->compare = compare;
    this->$capacity = RAWTREE_CAPACITY;
    this->$resizing = false;
    this->nodeSize = sizeof(RawTree_node);
    this->nodes = Pointers.malloc(RAWTREE_CAPACITY * (this->nodeSize + objectSize));
    Pointers.setAll(this->nodes, 0, RAWTREE_CAPACITY * (this->nodeSize + objectSize));
    this->nodesMap = RawBitslist.create(RAWTREE_CAPACITY, true);
    this->size = 0;
    this->rootNode = -1;
    this->objectSize = objectSize;
    // printDebug("RawTree_init done %p, %p ", this->nodes, this->nodesMap);
}

PRawTree RawTree_create(int objectSize, Fn_compare compare)
{
    
    RawTree_data *this = Pointers.malloc(sizeof(RawTree_data));
    printDebug("RawTree_create %p %i, %p, %lu", this, objectSize, compare, sizeof(RawTree_data));
    RawTree_init(this, objectSize, compare);
    return this;
}

void RawTree_free(PRawTree *this)
{
    RawTree_freeChildren(*this);
    free(*this);
    *this = 0;
}

struct RawTree_code RawTree = {
    &RawTree_free,
    &RawTree_freeChildren,
    &RawTree_create,
    &RawTree_init,
    &RawTree_insert,
    &RawTree_deleteNode,
    &RawTree_find,
    &RawTree_traverse_inorder,
    &RawTree_traverse_reverseOrder,
    &RawTree_traverse_inpreorder,
    &RawTree_traverse_inpostorder,
    &RawTree_get,
    &RawTree_unorderedTraverse,
};

