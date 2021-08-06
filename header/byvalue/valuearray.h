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
Fn_stackInterface(ValueArray);
Fn_queueInterface(ValueArray);
Fn_listInterface(ValueArray);
Fn_cleanupInterface(ValueArray);
Fn_traversableInterface(ValueArray);
Fn_sortableInterface(ValueArray);


// *************************************
// data structure
// *************************************
typedef struct ValueArraydata
{
    u32 length;
    u32 $capacity;
    u32 $startIndex;
    u32 $endIndex;
    Pointer $values;
    u32 $entrySize;
    PFn_compare compare;
} ValueArraydata;

typedef ValueArraydata *PValueArray;

// *************************************
// functions
// *************************************
PValueArray ValueArray_create(u32 entrySize);
void ValueArray_init(PValueArray this, u32 entrySize);

Fn_push(ValueArray_head, PValueArray, Pointer);
Fn_pop(ValueArray_head, PValueArray, Pointer);
Fn_peek(ValueArray_head, PValueArray, Pointer);

Fn_push(ValueArray_tail, PValueArray, Pointer);
Fn_pop(ValueArray_tail, PValueArray, Pointer);
Fn_peek(ValueArray_tail, PValueArray, Pointer);

Fn_addEntry(ValueArray, PValueArray, Pointer);
Fn_compareEntry(ValueArray, PValueArray);
Fn_contains(ValueArray, PValueArray, Pointer);
Fn_clear(ValueArray, PValueArray);

Fn_removeEntry(ValueArray, PValueArray);
Fn_relocateEntry(ValueArray, PValueArray);
Fn_swapEntry(ValueArray, PValueArray);
Fn_traverse(ValueArray, PValueArray);
Fn_setEntry(ValueArray, PValueArray, Pointer);
Fn_getEntry(ValueArray, PValueArray, Pointer);
Fn_insertEntry(ValueArray, PValueArray, Pointer);
Fn_length(ValueArray, PValueArray);

Fn_free(ValueArray, PValueArray);
Fn_freeChildren(ValueArray, PValueArray);

#endif