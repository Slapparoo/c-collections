#include "quicksort.h"

// typedef int (*Fn_compareEntry)(const void *object, const int a, const int b);
// typedef void (*Fn_swapEntry)(void *object, const int a, const int b);
// typedef enum SortOrder {
//     ascending  = 1,
//     descending = -1
// } SortOrder;

// void fn_swap$(int *array, int a, int b)
// {
//     int tmp = array[a];
//     array[a] = array[b];
//     array[b] = tmp;
// }

// int fn_compare$(int *array, const int a, const int b)
// {
//     return array[a] - array[b];
// }

/**
 * @brief 
 * 
 * @param list 
 * @param first 
 * @param last 
 * @param asc  acsending 1 or -1
 * @param compare 
 * @param swap 
 */
void quicksort(void *list, int first, int last, SortOrder sortOrder, Fn_compareEntry compare, Fn_swapEntry swap)
{
    int i, j, pivot;

    if (first < last)
    {
        pivot = first;
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

        swap(list, pivot, j);

        quicksort(list, first, j - 1, sortOrder, compare, swap);
        quicksort(list, j + 1, last, sortOrder, compare, swap);
    }
}

// int main()
// {
//     int i, count = 5, number[] = {2, 3, 7, 1, 5};

//     for (i = 0; i < count; i++)
//         printf(" %d", number[i]);

//     quicksort(number, 0, count - 1, ascending, fn_compare$, fn_swap$);

//     printf("\nOrder of Sorted elements:\n");
//     for (i = 0; i < count; i++)
//         printf(" %d", number[i]);
//     printf("\n");

//     quicksort(number, 0, count - 1, descending, fn_compare$, fn_swap$);

//     printf("\nOrder of Sorted elements:\n");
//     for (i = 0; i < count; i++)
//         printf(" %d", number[i]);
//     printf("\n");

//     quicksort(number, 2, count - 1, ascending, fn_compare$, fn_swap$);

//     printf("\nOrder of Sorted elements:\n");
//     for (i = 0; i < count; i++)
//         printf(" %d", number[i]);
//     printf("\n");

//     return 0;
// }