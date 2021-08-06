#include "byvalue/valuearray.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "quicksort.h"
void test_push_tail (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 0; i < 50; i++) {
        ValueArray_tail_push(list, &i);
    }

    ValueArray_free(&list);
}

void test_push_head (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 0; i < 50; i++) {
        ValueArray_head_push(list, &i);
    }

    ValueArray_free(&list);
}


void test_forward_queue (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 1; i < 50; i++) {
        ValueArray_tail_push(list, &i);
        assert_int_equal(i, list->length);
    }

    for (u64 i = 1; i < 50; i++) {
        Pointer peek = ValueArray_head_peek(list);
        u64* p = ValueArray_head_pop(list);
        assert_int_equal(i, *p);
        assert_ptr_equal(peek, p);
    }

    ValueArray_free(&list);
}

void test_grow (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 1; i < 100; i++) {
        ValueArray_tail_push(list, &i);
        assert_int_equal(i, list->length);
    }

    for (u64 i = 1; i < 100; i++) {
        // Pointer peek = ValueArray_peek(list);
        u64* p = ValueArray_head_pop(list);
        assert_int_equal(i, *p);
        // assert_ptr_equal(peek, p);
    }

    ValueArray_free(&list);
}


void test_remove (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 1; i < 100; i++) {
        ValueArray_tail_push(list, &i);
        assert_int_equal(i, list->length);
    }

    u64* value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 1);

    value = ValueArray_getEntry(list, 10);
    assert_int_equal(*value, 11);

    ValueArray_removeEntry(list, 10);
    value = ValueArray_getEntry(list, 10);
    assert_int_equal(*value, 12);

    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 1);
    ValueArray_head_pop(list);
    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 2);

    ValueArray_free(&list);
}

void test_sort(void **state)
{

    PValueArray list = ValueArray_create(8);

    for (u64 i = 0; i < 1000000; i++) {
        ValueArray_tail_push(list, &i);
        assert_int_equal(i, list->length -1);
    }

    list->compare = &compare$64;

    u64* value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 0);

    sort(list, 0, list->length -1, descending, &ValueArray_compareEntry, &ValueArray_swapEntry, &ValueArray_relocateEntry);
    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 1000000 -1);

    sort(list, 0, list->length -1, ascending, &ValueArray_compareEntry, &ValueArray_swapEntry, &ValueArray_relocateEntry);
    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 0);

    ValueArray_free(&list);
}

void test_shortSort(void **state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 0; i < 10; i++) {
        ValueArray_tail_push(list, &i);
        assert_int_equal(i, list->length -1);
    }

    ValueArray_head_pop(list);

    list->compare = &compare$64;

    u64* value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 1);

    sort(list, 0, list->length -1, descending, &ValueArray_compareEntry, &ValueArray_swapEntry, &ValueArray_relocateEntry);

    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 9);

    sort(list, 0, list->length -1, ascending, &ValueArray_compareEntry, &ValueArray_swapEntry, &ValueArray_relocateEntry);

    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 1);

    ValueArray_free(&list);
}

void test_wrappedshortSort(void **state)
{
    PValueArray list = ValueArray_create(8);

    // create a wrap
    for (u64 i = 0; i < 40; i++) {
        ValueArray_tail_push(list, &i);
    }

    for (u64 i = 0; i < 20; i++) {
        ValueArray_head_pop(list);
    }

    for (u64 i = 40; i < 80; i++) {
        ValueArray_tail_push(list, &i);
    }

    list->compare = &compare$64;

    u64* value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 20);

    // sort(list, 0, list->length -1, descending, &ValueArray_compareEntry, &ValueArray_swapEntry, &ValueArray_relocateEntry);
    sort(list, 0, list->length -1, descending, &ValueArray_compareEntry, &ValueArray_swapEntry, null);

    value = ValueArray_getEntry(list, 0);
    u64* values = list->$values + (list->$startIndex * list->$entrySize);
    // printDebug("%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu", 
    //     values[0], values[1], values[2], values[3], values[4], values[5], values[5], values[7], values[8], values[9]);

    assert_int_equal(*value, 79);

    // sort(list, 0, list->length -1, ascending, &ValueArray_compareEntry, &ValueArray_swapEntry, &ValueArray_relocateEntry);
    sort(list, 0, list->length -1, ascending, &ValueArray_compareEntry, &ValueArray_swapEntry, null);

    value = ValueArray_getEntry(list, 0);
    assert_int_equal(*value, 20);

    ValueArray_free(&list);
}


void test_volumne (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 0; i < 20000000; i++) {
        ValueArray_tail_push(list, &i);
        u64* p = ValueArray_head_peek(list);
        assert_int_equal(list->length, i+1);
    }
    
    ValueArray_free(&list);
}

void test_volumne_pop (void ** state)
{
    PValueArray list = ValueArray_create(8);
    PushPop queue = ValueArray_queueInterface();

    for (u64 i = 0; i < 4; i++) {
        u64* p;
        for (u64 j = 0; j < 62; j++) {
            queue->push(list, &i);
            p = queue->peek(list);
        }

        u64* p1 = queue->pop(list);
        assert_int_equal(*p1, *p);
    }
        
    ValueArray_free(&list);
}

void test_volumne_pop_after (void ** state)
{
    PValueArray list = ValueArray_create(8);

    for (u64 i = 0; i < 20000000; i++) {
        ValueArray_tail_push(list, &i);
        Pointer p = ValueArray_head_peek(list);
        assert_int_equal(list->length, i+1);
    }

    for (u64 i = 0; i < 20000000; i++) {
        u64* p = ValueArray_head_peek(list);
        u64*  p1 = ValueArray_head_pop(list);
        assert_int_equal(*p1, *p);
    }

    ValueArray_free(&list);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_push_head),
        cmocka_unit_test (test_push_tail),
        cmocka_unit_test (test_forward_queue),
        cmocka_unit_test (test_grow),
        cmocka_unit_test (test_volumne),
        cmocka_unit_test (test_volumne_pop),
        cmocka_unit_test (test_volumne_pop_after),
        cmocka_unit_test (test_remove),
        cmocka_unit_test (test_sort),
        cmocka_unit_test (test_shortSort),
        cmocka_unit_test (test_wrappedshortSort),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}
