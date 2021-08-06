#include "byvalue/valuetree.h"
#include "assert.h"

#define RAWTREE_CAPACITY 64

#define NODESIZE (sizeof(Pointer) * 2)

#define $objectSize this->$objectSize
#define length this->length
#define $rootNode this->$rootNode
#define $allocator this->$allocator

static __inline__ PValueTreeNode ValueTree_$minRight(PValueTree this, PValueTreeNode current)
{
    /* loop down to find the leftmost leaf */
    while (current && current->left)
    {
        current = current->left;
    }

    return current;
}


/**
 * @brief Insert a node into the tree
 * 
 * @param this 
 * @param node 
 * @param object 
 */

static __inline__ PValueTreeNode ValueTree_$newNode(PValueTree this, Pointer object) {
    PValueTreeNode node = ValueAllocator_borrow($allocator);

    node->left = null;
    node->right = null;

    memcpy(node->values, object, $objectSize);
    return node;
}

static __inline__ PValueTreeNode ValueTree_$insert(PValueTree this, PValueTreeNode *node, Pointer object)
{
    
    if (!*node)
    {
        (*node) = ValueTree_$newNode(this, object);
        length++;
        return (*node);
    }

    int res = this->compare(object, (*node)->values);

    if (res > 0)
    {
        ValueTree_$insert(this, &(*node)->left, object);
    }
    else if (res < 0)
    {
        ValueTree_$insert(this, &(*node)->right, object);
    } else {
        // replace
        memcpy((*node)->values, object, $objectSize);
        return (*node);
    }
}

static __inline__ PValueTreeNode ValueTree_$deleteNode(PValueTree this, PValueTreeNode node, Pointer object)
{
    if (!node)
    {
        return null;
    }

    int res = this->compare(object, node->values);

    if (res > 0)
    {
        node->left = ValueTree_$deleteNode(this, node->left, object);
    }
    else if (res < 0)
    {
        node->right = ValueTree_$deleteNode(this, node->right, object);
    }
    else
    {
        if (node->left == null && node->right == null)
        {
            ValueAllocator_returnIt($allocator, node);
            length--;
            return null;
        }
        else if (node->left == null)
        {
            ValueAllocator_returnIt($allocator, node);
            length--;
            return node->right;
        }
        else if (node->right == null)
        {
            ValueAllocator_returnIt($allocator, node);
            length--;
            return node->left;
        }
        else
        {
            PValueTreeNode tmp = ValueTree_$minRight(this, node->right);

            // Copy the inorder
            // successor's content to this node
            memcpy(node->values, tmp->values, $objectSize);

            // Delete the inorder successor
            node->right = ValueTree_$deleteNode(this, node->right, node->values);
            // printInfo("delete successor %u, %lu", length, *(u64*)node->values);
        }
    }
    return node;
}

/**
 * @brief traverse the items "in order"
 * 
 * @param this 
 * @param traverser 
 */
static __inline__ void ValueTree_$traverse_inorder(PValueTree this, Pointer other, PValueTreeNode node, PFn_traverser traverser)
{
    if (node)
    {
        ValueTree_$traverse_inorder(this, other, node->left, traverser);
        traverser(other, node->values);
        ValueTree_$traverse_inorder(this, other, node->right, traverser);
    }
}

/**
 * @brief traverse the items "in order"
 * 
 * @param this 
 * @param traverser 
 */
Fn_addEntry(ValueTree, PValueTree, Pointer)
{
    return ValueTree_$insert(this, &$rootNode, value)->values;
}

// Fn_clear(ValueTree, PValueTree) {
//     RawBitslist.setAll($nodesMap, true);
//     length = 0;
//     Pointers.setAll($nodes, 0, $capacity * ($nodeSize + $objectSize));
// }

/**
 * @brief free memory for thei node tree
 * 
 * @param tree 
 */

Fn_freeChildren(ValueTree, PValueTree)
{
    ValueAllocator_free(&$allocator);
}

/**
 * @brief Match a node based on the compare method
 * 
 * @param this 
 * @param node 
 * @param object 
 * @return index 
 */

Fn_find(ValueTree, PValueTree, Pointer)
{
    if (!$rootNode)
        return null;

    PValueTreeNode node = $rootNode;

    while (true)
    {
        if (!node)
        {
            return null;
        }

        int res = this->compare(value, node->values);

        if (res > 0)
        {
            node = node->left;
        }
        else if (res < 0)
        {
            node = node->right;
        }
        else
        {
            return node->values;
        }
    }
}

void ValueTree_$deltree(PValueTree this, PValueTreeNode node)
{
    if (node)
    {
        ValueTree_$deltree(this, node->left);
        ValueTree_$deltree(this, node->right);
        ValueAllocator_returnIt($allocator, node);
    }
}

void ValueTree_deltree(PValueTree this)
{
    ValueTree_$deltree(this, $rootNode);
    if ($rootNode) {
        ValueAllocator_returnIt($allocator, $rootNode);
        $rootNode = null;
    }
}

/* Given a binary search tree
   and a key, this function
   deletes the key and
   returns the new root */
Fn_deleteEntry(ValueTree, PValueTree, Pointer)
{
    $rootNode = ValueTree_$deleteNode(this, $rootNode, entry);
}

Fn_traverse(ValueTree, PValueTree)
{
    ValueTree_$traverse_inorder(this, other, $rootNode, traverser);
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void ValueTree_init(PValueTree this, int objectSize, PFn_compare compare)
{
    this->compare = compare;
    length = 0;
    $rootNode = null;
    $objectSize = objectSize;
    $allocator = ValueAllocator_create(objectSize + NODESIZE);
}

PValueTree ValueTree_create(int objectSize, PFn_compare compare)
{
    PValueTree this = Pointers.malloc(sizeof(ValueTree_data));
    ValueTree_init(this, objectSize, compare);
    return this;
}

Fn_free(ValueTree, PValueTree)
{
    ValueTree_freeChildren(*this);
    free(*this);
    *this = 0;
}
