#ifndef __CIRCULARBUFFER_HH
#define __CIRCULARBUFFER_HH

#include "collection_base.h"
#include "collection_interfaces.h"

/**
 * @brief Circular/wrap around buffer that stores Pointers
 * 
 */
// *************************************
// Interfaces
// *************************************
Fn_stackInterface(PointerArray);
Fn_queueInterface(PointerArray);
Fn_listInterface(PointerArray);
Fn_cleanupInterface(PointerArray);
Fn_traversableInterface(PointerArray);
Fn_sortableInterface(PointerArray);


// *************************************
// data structure
// *************************************
typedef struct PointerArraydata
{
    u32 length;
    u32 $capacity;
    u32 $startIndex;
    u32 $endIndex;
    Pointer * $values;
    PFn_compare compare;
} PointerArraydata;

typedef PointerArraydata * PPointerArray;

// *************************************
// functions
// *************************************
PPointerArray PointerArray_create();
void PointerArray_init(PPointerArray this);

Fn_push(PointerArray_head, PPointerArray, Pointer);
Fn_pop(PointerArray_head, PPointerArray, Pointer);
Fn_peek(PointerArray_head, PPointerArray, Pointer);

Fn_push(PointerArray_tail, PPointerArray, Pointer);
Fn_pop(PointerArray_tail, PPointerArray, Pointer);
Fn_peek(PointerArray_tail, PPointerArray, Pointer);

Fn_addEntry(PointerArray, PPointerArray, Pointer);
Fn_compareEntry(PointerArray, PPointerArray);
Fn_contains(PointerArray, PPointerArray, Pointer);
Fn_clear(PointerArray, PPointerArray);

Fn_removeEntry(PointerArray, PPointerArray);
Fn_relocateEntry(PointerArray, PPointerArray);
Fn_swapEntry(PointerArray, PPointerArray);
Fn_traverse(PointerArray, PPointerArray);
Fn_setEntry(PointerArray, PPointerArray, Pointer);
Fn_getEntry(PointerArray, PPointerArray, Pointer);
Fn_insertEntry(PointerArray, PPointerArray, Pointer);
Fn_length(PointerArray, PPointerArray);

Fn_free(PointerArray, PPointerArray);
Fn_freeChildren(PointerArray, PPointerArray);

#endif