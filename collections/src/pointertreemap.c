#include "bypointer/pointertreemap.h"
#include "assert.h"


#define NODESIZE sizeof(PointerTreemap_node)

#define length this->length
#define $rootNode this->$rootNode
#define $allocator this->$allocator

static __inline__ PPointerTreemapNode PointerTreemap_$minRight(PPointerTreemap this, PPointerTreemapNode current)
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

static __inline__ PPointerTreemapNode PointerTreemap_$newNode(PPointerTreemap this, PPointerMapEntry entry) {
    PPointerTreemapNode node = ValueAllocator_borrow($allocator);

    node->left = null;
    node->right = null;
    node->key = entry->key;
    node->value = entry->value;

    return node;
}

static __inline__ PPointerTreemapNode PointerTreemap_$insert(PPointerTreemap this, PPointerTreemapNode *node, PPointerMapEntry entry)
{
    
    if (!*node)
    {
        (*node) = PointerTreemap_$newNode(this, entry);
        length++;
        return (*node);
    }

    int res = this->compare(entry->key, (*node)->key);

    if (res > 0)
    {
        PointerTreemap_$insert(this, &(*node)->left, entry);
    }
    else if (res < 0)
    {
        PointerTreemap_$insert(this, &(*node)->right, entry);
    } else {
        // replace
        (*node)->key = entry->key;
        (*node)->value = entry->value;
        return (*node);
    }
}

static __inline__ PPointerTreemapNode PointerTreemap_$deleteNode(PPointerTreemap this, PPointerTreemapNode node, Pointer key)
{
    if (!node)
    {
        return null;
    }

    int res = this->compare(key, node->key);

    if (res > 0)
    {
        node->left = PointerTreemap_$deleteNode(this, node->left, key);
    }
    else if (res < 0)
    {
        node->right = PointerTreemap_$deleteNode(this, node->right, key);
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
            PPointerTreemapNode tmp = PointerTreemap_$minRight(this, node->right);

            // Copy the inorder
            // successor's content to this node
            node->key = tmp->key;
            node->value = tmp->value;

            // Delete the inorder successor
            node->right = PointerTreemap_$deleteNode(this, node->right, node->key);
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
static __inline__ void PointerTreemap_$traverse_inorder(PPointerTreemap this, Pointer other, PPointerTreemapNode node, PFn_traverser traverser)
{
    if (node)
    {
        PointerTreemap_$traverse_inorder(this, other, node->left, traverser);
        // PointerMapEntry entry = {node->key, node->value};
        traverser(other, node);
        PointerTreemap_$traverse_inorder(this, other, node->right, traverser);
    }
}

/**
 * @brief traverse the items "in order"
 * 
 * @param this 
 * @param traverser 
 */
Fn_addEntry(PointerTreemap, PPointerTreemap, PPointerMapEntry)
{
    return PointerTreemap_$insert(this, &$rootNode, value)->value;
}

// Fn_clear(PointerTreemap, PPointerTreemap) {
//     RawBitslist.setAll($nodesMap, true);
//     length = 0;
//     Pointers.setAll($nodes, 0, $capacity * ($nodeSize + $objectSize));
// }

/**
 * @brief free memory for thei node tree
 * 
 * @param tree 
 */

Fn_freeChildren(PointerTreemap, PPointerTreemap)
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

Fn_find(PointerTreemap, PPointerTreemap, Pointer)
{
    if (!$rootNode)
        return null;

    PPointerTreemapNode node = $rootNode;

    while (true)
    {
        if (!node)
        {
            return null;
        }

        int res = this->compare(value, node->key);

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
            return node->value;
        }
    }
}

void PointerTreemap_$deltree(PPointerTreemap this, PPointerTreemapNode node)
{
    if (node)
    {
        PointerTreemap_$deltree(this, node->left);
        PointerTreemap_$deltree(this, node->right);
        ValueAllocator_returnIt($allocator, node);
    }
}

void PointerTreemap_deltree(PPointerTreemap this)
{
    PointerTreemap_$deltree(this, $rootNode);
    if ($rootNode) {
        ValueAllocator_returnIt($allocator, $rootNode);
        $rootNode = null;
    }
}

/* Given a binary search tree
   and a key, this function
   deletes the key and
   returns the new root */
Fn_deleteEntry(PointerTreemap, PPointerTreemap, Pointer)
{
    $rootNode = PointerTreemap_$deleteNode(this, $rootNode, entry);
}

Fn_traverse(PointerTreemap, PPointerTreemap)
{
    PointerTreemap_$traverse_inorder(this, other, $rootNode, traverser);
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void PointerTreemap_init(PPointerTreemap this, PFn_compare compare)
{
    this->compare = compare;
    length = 0;
    $rootNode = null;
    $allocator = ValueAllocator_create(NODESIZE);
}

PPointerTreemap PointerTreemap_create(PFn_compare compare)
{
    PPointerTreemap this = Pointers.malloc(sizeof(PointerTreemap_data));
    PointerTreemap_init(this, compare);
    return this;
}

Fn_free(PointerTreemap, PPointerTreemap)
{
    PointerTreemap_freeChildren(*this);
    free(*this);
    *this = 0;
}
