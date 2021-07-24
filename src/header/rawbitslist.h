#ifndef _RAWBITSLIST_HH_
#define _RAWBITSLIST_HH_

#include "data-types.h"
#include "object-types.h"

#define __C_BITSLIST_DATA_ \
    u32 $capacity; \
    u32 length; \
    u32* content; 

typedef struct RawBitslist_data {
     __C_BITSLIST_DATA_
} RawBitslist_data;

typedef RawBitslist_data * PRawBitslist;

#define __C_BITSLIST_CODE_ \
    PRawBitslist (*create)(u32 length, const boolean initial); \
    __attribute__ ((nonnull (1))) void (*init)(const PRawBitslist bitslist, u32 length, const boolean initial); \
    __attribute__ ((nonnull (1))) boolean (*isBit)(PRawBitslist bitslist, u32 index); \
    __attribute__ ((nonnull (1))) void (*setBit)(const PRawBitslist bitslist, u32 index); \
    __attribute__ ((nonnull (1))) void (*clearBit)(PRawBitslist bitslist, u32 index); \
    __attribute__ ((nonnull (1))) void (*set)(PRawBitslist bitslist, u32 index, boolean value); \
    __attribute__ ((nonnull (1))) i32 (*findFirst)(PRawBitslist bitslist); \
    __attribute__ ((nonnull (1))) void (*setAll)(PRawBitslist bitslist, boolean value); \
    __attribute__ ((nonnull (1))) u32 (*getBitCount)(const PRawBitslist this); \
    __attribute__ ((nonnull (1))) void (*free)(PRawBitslist* bitslist); \
    __attribute__ ((nonnull (1))) void (*freeChildren)(PRawBitslist bitslist); 

typedef struct RawBitslist_code {
    __C_BITSLIST_CODE_
} RawBitslist_code;

struct RawBitslist_code RawBitslist;

PRawBitslist RawBitslist_create(const u32 size, const boolean initial);
__attribute__ ((nonnull (1))) void RawBitslist_init(const PRawBitslist bitslist, const u32 size, const boolean initial);
__attribute__ ((nonnull (1))) boolean RawBitslist_isBit(const PRawBitslist bitslist, const u32 index);
__attribute__ ((nonnull (1))) void RawBitslist_setBit(const PRawBitslist bitslist, const u32 index);
__attribute__ ((nonnull (1))) void RawBitslist_clearBit(const PRawBitslist bitslist, const u32 index);
__attribute__ ((nonnull (1))) void RawBitslist_set(const PRawBitslist bitslist, const u32 index, const boolean value);
__attribute__ ((nonnull (1))) i32 RawBitslist_findFirst(const PRawBitslist bitslist); 
__attribute__ ((nonnull (1))) void RawBitslist_setAll(PRawBitslist bitslist, boolean value);
__attribute__ ((nonnull (1))) u32 RawBitslist_getBitCount(const PRawBitslist this);
__attribute__ ((nonnull (1))) void RawBitslist_free(PRawBitslist* bitslist);
__attribute__ ((nonnull (1))) void RawBitslist_freeChildren(const PRawBitslist bitslist); 


#endif