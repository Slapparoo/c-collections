#ifndef _RAWBITSLIST_HH_
#define _RAWBITSLIST_HH_

#include "collection_base.h"
#include "collection_interfaces.h"

// *************************************
// Interfaces
// *************************************
Fn_cleanupInterface(Buffer);

// *************************************
// data structure
// *************************************

typedef struct Bitslist_data {
    u32 $capacity;
    u32 length;
    u32* $values; 
} Bitslist_data;

typedef Bitslist_data * PBitslist;

PBitslist Bitslist_create(const u32 size, const boolean initial);
__attribute__ ((nonnull (1))) void Bitslist_init(const PBitslist bitslist, const u32 size, const boolean initial);
__attribute__ ((nonnull (1))) boolean Bitslist_isBit(const PBitslist bitslist, const u32 index);
__attribute__ ((nonnull (1))) void Bitslist_setBit(const PBitslist bitslist, const u32 index);
__attribute__ ((nonnull (1))) void Bitslist_clearBit(const PBitslist bitslist, const u32 index);
__attribute__ ((nonnull (1))) void Bitslist_set(const PBitslist bitslist, const u32 index, const boolean value);
__attribute__ ((nonnull (1))) i32 Bitslist_findFirst(const PBitslist bitslist); 
__attribute__ ((nonnull (1))) void Bitslist_setAll(PBitslist bitslist, boolean value);
__attribute__ ((nonnull (1))) u32 Bitslist_getBitCount(const PBitslist this);

Fn_free(Bitslist, PBitslist);
Fn_freeChildren(Bitslist, PBitslist);

#endif