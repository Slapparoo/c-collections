#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "quicksort.h"

#include <time.h>

static u64 getTimeStamp()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("clock_gettime");
        // exit(EXIT_FAILURE);
    }

    return ((ts.tv_sec % 3600) * 1000) + (ts.tv_nsec / 1000000);
}

int compare(int *list, int a, int b)
{
    return list[a] - list[b];
}

void swap(int *list, int a, int b)
{
    int tmp = list[a];
    list[a] = list[b];
    list[b] = tmp;
}

i32 relocateEntry(int * arry, const i32 a, const i32 b) {
    if (a > b) {
        // no dice
        return -1;
    }
    int tmp = arry[b];
    memmove(&arry[a+1], &arry[a], (b - a) * sizeof(int));
    arry[a] = tmp;
    return 0;
}


void test_sort(void **state)
{
    int i, count = 5, number[] = {2, 4, 7, 1, 5}; // {2, 1, 7, 4, 5}, {2, 1, 5, 4, 7}
    sort(number, 0, count - 1, ascending, &compare, &swap, null);
    int expected[] = {1, 2, 4, 5, 7};
    assert_memory_equal(number, expected, 5);
}

void test_shortSort(void **state)
{
    int i, count = 5, number[] = {2, 4, 7, 1, 5}; // {2, 1, 7, 4, 5}, {2, 1, 5, 4, 7}
    shortSort(number, 0, count - 1, ascending, &compare, &relocateEntry);
    int expected[] = {1, 2, 4, 5, 7};
    assert_memory_equal(number, expected, 5);
}

void test_sort_descending(void **state)
{
    int i, count = 5, number[] = {2, 4, 7, 1, 5}; // {7, 4, 2, 1, 5}, {7, 4, 5, 1, 2}
    quicksortPrep(number, 0, count - 1, descending, &compare, &swap, null);
    int expected[] = {7, 5, 4, 2, 1};
    assert_memory_equal(number, expected, 5);
}

void test_sort_shortdescending(void **state)
{
    int i, count = 5, number[] = {2, 4, 7, 1, 5}; // {7, 4, 2, 1, 5}, {7, 4, 5, 1, 2}
    shortSort(number, 0, count - 1, descending, &compare, &relocateEntry);
    int expected[] = {7, 5, 4, 2, 1};
    assert_memory_equal(number, expected, 5);

    count = 800;
    int number2[count];

    for (int i = 0; i < count; i++)
    {
        number2[i] = i % (count / 2);
        // number2[i] = hash$32(&i);
    }
    // printf("items %i, %i, %i\n", number2[0], number2[1], number2[count-1]); 
    u64 start = getTimeStamp();
    for (int i = 0; i < 1000; i++) {
        shortSort(number2, 0, count - 1, descending, &compare, &relocateEntry);
        shortSort(number2, 0, count - 1, ascending, &compare, &relocateEntry);
    }
    printf("ss time %lu\n", getTimeStamp() - start); 

    start = getTimeStamp();
    for (int i = 0; i < 1000; i++) {
        sort(number2, 0, count - 1, descending, &compare, &swap, &relocateEntry);
        sort(number2, 0, count - 1, ascending, &compare, &swap, &relocateEntry);
    }
    printf("s time %lu\n", getTimeStamp() - start); 

    start = getTimeStamp();
    for (int i = 0; i < 1000; i++) {
        sort(number2, 0, count - 1, descending, &compare, &swap, null);
        sort(number2, 0, count - 1, ascending, &compare, &swap, null);
    }
    printf("qs time %lu\n", getTimeStamp() - start); 
}

void test_sort_sorted(void **state)
{
    int count = 50000, number[count];

    for (int i = 0; i < count; i++)
    {
        number[i] = i % 100;
    }
    u64 start = getTimeStamp();
    sort(number, 0, count - 1, descending, &compare, &swap, null);
    printf("time %lu\n", getTimeStamp() - start); 
    start = getTimeStamp();
    sort(number, 0, count - 1, ascending, &compare, &swap, null);
    printf("time %lu\n", getTimeStamp() - start); 
    start = getTimeStamp();
    quicksortPrep(number, 0, count - 1, descending, &compare, &swap, null);
    printf("time %lu\n", getTimeStamp() - start); 
    start = getTimeStamp();
    quicksortPrep(number, 0, count - 1, ascending, &compare, &swap, null);
    printf("time %lu\n", getTimeStamp() - start); 

    // re-run first
    start = getTimeStamp();
    sort(number, 0, count - 1, descending, &compare, &swap, null);
    printf("time %lu\n", getTimeStamp() - start); 
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(test_sort),
            // cmocka_unit_test(test_sort_descending),
            cmocka_unit_test(test_sort_sorted),
            cmocka_unit_test(test_shortSort),
            cmocka_unit_test(test_sort_shortdescending),
        };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests(tests, null, null);

    return count_fail_tests;
}
