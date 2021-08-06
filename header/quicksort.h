#ifndef QSORT_HH
#define QSORT_HH

#include "collection_base.h"

typedef enum SortOrder {
    ascending  = 1,
    descending = -1
} SortOrder;

/**
 * @brief 
 * 
 * @param list - passed back to the copare and swap functions
 * @param first 
 * @param last 
 * @param asc  acsending 1 or -1
 * @param compare 
 * @param swap 
 */
__attribute__ ((nonnull (1, 5, 6))) void sort(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_swapEntry swap, PFn_relocateEntry relocate);
__attribute__ ((nonnull (1, 5, 6))) void quicksortPrep(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_swapEntry swap, PFn_relocateEntry relocate);
__attribute__ ((nonnull (1, 5, 6))) void shortSort(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_relocateEntry relocate);


#endif
