#ifndef __SORTABLEINTERFACE_
#define __SORTABLEINTERFACE_

#include "collection_base.h"

/**
 * @brief Lists or collections that can be sorted
 * 
 * the relocateEntry method is optional 
 * 
 */

typedef struct SortableInterface {
  PFn_swapEntry swapEntry;
  PFn_compareEntry compareEntry;
  PFn_length length;
  PFn_relocateEntry relocateEntry; /* nullable */
} SortableInterface;

typedef SortableInterface * Sortable;

#define implementSortable(class)  struct SortableInterface class ## _SortableInterface = { \
    &class ## _swapEntry, \
    &class ## _compareEntry, \
    &class ## _length, \
    &class ## _relocateEntry, \
}; \
 \
Fn_sortableInterface(class) { \
    return &class ## _SortableInterface; \
}

#define implementSortableAs(class, swapEntry, compareEntry, length, relocateEntry)  \
        struct SortableInterface class ## _SortableInterface = { \
    swapEntry, \
    compareEntry, \
    length, \
    relocateEntry, \
}; \
 \
Fn_sortableInterface(class) { \
    return &class ## _SortableInterface; \
}

#define Fn_sortableInterface(class) Sortable class ## _sortableInterface()
typedef Sortable (*PFn_sortable)(void);

#endif