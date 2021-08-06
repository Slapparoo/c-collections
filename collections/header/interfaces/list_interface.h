#ifndef __LISTINTERFACE_HH_
#define __LISTINTERFACE_HH_

#include "collection_base.h"
/**
 * @brief "ListInterface" 
 * 
 * Dynamic list
 * 
 */

typedef struct ListInterface {
  PFn_addEntry addEntry;
  PFn_insertEntry insertEntry;
  PFn_compareEntry compareEntry;
  PFn_contains contains;
  PFn_traverse traverse;
  PFn_removeEntry removeEntry;
  PFn_getEntry getEntry;
  PFn_setEntry setEntry;
  PFn_clear clear;
} ListInterface;

typedef ListInterface * List;

#define implementList(class)  struct ListInterface class ## _ListInterface = { \
    &class ## _addEntry, \
    &class ## _insertEntry, \
    &class ## _compareEntry, \
    &class ## _contains, \
    &class ## _traverse, \
    &class ## _removeEntry, \
    &class ## _getEntry, \
    &class ## _setEntry, \
    &class ## _clear, \
}; \
 \
Fn_listInterface(class) { \
    return & class ## _ListInterface; \
}

#define Fn_listInterface(class) List class ## _listInterface()
typedef List (*PFn_list)(void);

#endif