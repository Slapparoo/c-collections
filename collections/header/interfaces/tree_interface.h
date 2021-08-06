#ifndef __TREEINTERFACE_
#define __TREEINTERFACE_

#include "collection_base.h"

typedef struct TreeInterface {
  PFn_addEntry addEntry;
  // relace
  PFn_deleteEntry delete;
  PFn_contains contains;
  PFn_find find;
  PFn_clear clear;
  PFn_traverse traverse;
  PFn_traverse reverseTraverse;
} TreeInterface;

typedef TreeInterface * Tree;

#define implementTree(class)  struct TreeInterface class ## _TreeInterface = { \
    &class ## _addEntry, \
    &class ## _delete, \
    &class ## _contains, \
    &class ## _find, \
    &class ## _clear, \
    &class ## _traverse, \
    &class ## _reverseTraverse, \
}; \
 \
Fn_treeInterface(class) { \
    return &class ## _TreeInterface; \
}

#define Fn_treeInterface(class) Pool class ## _treeInterface()
typedef Tree (*PFn_tree)(void);


#endif