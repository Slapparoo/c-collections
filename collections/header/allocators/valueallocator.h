#include "collection_base.h"
#include "collection_interfaces.h"
/**
 * @brief used to pre-allocate, on free it recycles
 * 
 */

#ifndef _VALUEALLOCATOR_HH_
#define _VALUEALLOCATOR_HH_


/**
 * interfaces
 * 
 */
Fn_cleanupInterface(ValueAllocator);
Fn_poolInterface(ValueAllocator);

/**
 * data
 * 
 */
typedef struct ValueAllocatordata {
    Pointer memBlock;
    u32 $entrySize;
    u32 $capacity;
    Pointer $entry;
    Pointer recyclerData;
    Pointer allocationData;
    PushPopInterface *stackFunction;
    CleanupInterface *stackCleanup;
} ValueAllocatordata;

typedef ValueAllocatordata * PValueAllocator;

/**
 * Functionality
 * 
 */

PValueAllocator ValueAllocator_create(u32 entrySize);
__attribute__ ((nonnull (1))) void ValueAllocator_init(const PValueAllocator this, u32 entrySize);
Fn_borrow(ValueAllocator, PValueAllocator, Pointer);
Fn_returnIt(ValueAllocator, PValueAllocator, Pointer);
Fn_free(ValueAllocator, PValueAllocator);
Fn_freeChildren(ValueAllocator, PValueAllocator);

#endif