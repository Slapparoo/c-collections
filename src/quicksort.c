#include "quicksort.h"


/**
 * @brief implementation of quicksort
 * it is based on lists whihc can be indexed by int
 * example:
 
 int compare(int* list, int a, int b) {
     return list[a] - list[b];
 }
 
 void swap(int* list, int a, int b) {
    int tmp = a;
    list[a] = list[b];
    list[b] = tmp;
 }
 
 
 int main()
 {
    int i, count = 5, number[] = {2, 3, 7, 1, 5};

    for (i = 0; i < count; i++)
        printf(" %d", number[i]);

    quicksort(number, 0, count - 1, ascending, &compare, &swap);

    printf("\nOrder of Sorted elements:\n");
    for (i = 0; i < count; i++)
        printf(" %d", number[i]);
    printf("\n");

    quicksort(number, 0, count - 1, descending, &compare, &swap);

    printf("\nOrder of Sorted elements:\n");
    for (i = 0; i < count; i++)
        printf(" %d", number[i]);
    printf("\n");

    quicksort(number, 2, count - 1, ascending, &compare, &swap);

    printf("\nOrder of Sorted elements:\n");
    for (i = 0; i < count; i++)
        printf(" %d", number[i]);
    printf("\n");

    return 0;
}
*/

void quicksort$(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_swapEntry swap)
{
    i32 i, j, pivot;

    if (first < last)
    {
        // pivot = first;
        // pivot = last;
        pivot = first + ((last - first) / 2);
        i = first;
        j = last;

        while (i < j)
        {
            while (compare(list, i, pivot) * sortOrder  <= 0 && i < last)
                i++;
            while (compare(list, j, pivot) * sortOrder > 0)
                j--;
            if (i < j)
            {
                swap(list, i, j);
            }
        }

        if (compare(list, j, pivot) * sortOrder < 0) {
            swap(list, pivot, j);
        }

        quicksort$(list, first, j - 1, sortOrder, compare, swap);
        quicksort$(list, j + 1, last, sortOrder, compare, swap);
    }
}

/* 
 * 
 * 
 * @param list 
 * @param first 
 * @param last 
 * @param asc  acsending 1 or -1
 * @param compare 
 * @param swap 
 */
void quicksortPrep(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_swapEntry swap, PFn_relocateEntry relocate)
{
    if (relocate && last - first < 128) {
        shortSort(list, first, last, sortOrder, compare, relocate);
        return;
    }

    i32 mid = (last - first) / 2;
    for (int i = first; i < first +mid; i++) {
        if (compare(list, i, i+mid) * sortOrder > 0) {
            swap(list, i, i+mid);
        }
    }

    if ((last - first) & 1) {
        // odd
        if (compare(list, mid, last) * sortOrder > 0) {
            swap(list, mid, last);
        }
    } 

    sort(list, first, last, sortOrder, compare, swap, relocate);
}


/* 
 * 
 * 
 * @param list 
 * @param first 
 * @param last 
 * @param asc  acsending 1 or -1
 * @param compare 
 * @param swap 
 */
void sort(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_swapEntry swap, PFn_relocateEntry relocate)
{
    if (relocate && last - first < 256) {
        shortSort(list, first, last, sortOrder, compare, relocate);
        return;
    }

    quicksort$(list, first, last, sortOrder, compare, swap);
}


// i32 relocateEntry(int * arry, const i32 a, const i32 b) {
//     if (a > b) {
//         // no dice
//         return -1;
//     }
//     int tmp = arry[b];
//     memmove(&arry[a+1], &arry[a], (b - a) * sizeof(int));
//     arry[a] = tmp;
//     return 0;
// }

/* 
 * used for short lists do a linear compare then relocate
 *
 * peformance when sorting short lists is better that quick sort (assuming the relocate function is optimal)
 * 
 * @param list 
 * @param first 
 * @param last 
 * @param asc  acsending 1 or -1
 * @param compare 
 * @param swap 
 */
void shortSort(Pointer list, i32 first, i32 last, SortOrder sortOrder, PFn_compareEntry compare, PFn_relocateEntry relocate)
{
    for (int i = first + 1; i <= last; i++) {
        for (int j = first; j < i; j++) {
            if (compare(list, j, i) * sortOrder > 0) {
                relocate(list, j, i);
                break;
            }
        }
    }
}