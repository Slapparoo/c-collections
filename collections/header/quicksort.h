#ifndef QSORT_HH
#define QSORT_HH

#include "data-types.h"

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
__attribute__ ((nonnull (1, 5, 6))) void quicksort(Pointer list, int first, int last, SortOrder sortOrder, Fn_compareEntry compare, Fn_swapEntry swap);


#endif
