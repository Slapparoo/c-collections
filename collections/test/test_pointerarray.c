#include "bypointer/pointerarray.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "quicksort.h"
void test_push_tail (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 0; i < 50; i++) {
        PointerArray_tail_push(list, &i);
    }

    PointerArray_free(&list);
}

void test_push_head (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 0; i < 50; i++) {
        PointerArray_head_push(list, &i);
    }

    PointerArray_free(&list);
}


void test_forward_queue (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 1; i < 50; i++) {
        PointerArray_tail_push(list, (Pointer)i);
        assert_int_equal(i, list->length);
    }

    for (u64 i = 1; i < 50; i++) {
        Pointer peek = PointerArray_head_peek(list);
        Pointer p = PointerArray_head_pop(list);
        assert_int_equal(i, p);
        assert_ptr_equal(peek, p);
    }

    PointerArray_free(&list);
}

void test_grow (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 1; i < 100; i++) {
        PointerArray_tail_push(list, (Pointer)i);
        assert_int_equal(i, list->length);
    }

    for (u64 i = 1; i < 100; i++) {
        // Pointer peek = PointerArray_peek(list);
        Pointer p = PointerArray_head_pop(list);
        assert_int_equal(i, p);
        // assert_ptr_equal(peek, p);
    }

    PointerArray_free(&list);
}


void test_remove (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 1; i < 100; i++) {
        PointerArray_tail_push(list, (Pointer)i);
        assert_int_equal(i, list->length);
    }

    Pointer value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 1);

    value = PointerArray_getEntry(list, 10);
    assert_int_equal(value, 11);

    PointerArray_removeEntry(list, 10);
    value = PointerArray_getEntry(list, 10);
    assert_int_equal(value, (Pointer)12);

    value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 1);
    PointerArray_head_pop(list);
    value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 2);

    PointerArray_free(&list);
}

void test_sort(void **state)
{

    PPointerArray list = PointerArray_create(8);

    for (u64 i = 0; i < 1000000; i++) {
        PointerArray_tail_push(list, (Pointer)i);
        assert_int_equal(i, list->length -1);
    }

    // list->compare = &compare$64;

    u64* value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 0);

    sort(list, 0, list->length -1, descending, &PointerArray_compareEntry, &PointerArray_swapEntry, &PointerArray_relocateEntry);
    value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 1000000 -1);

    sort(list, 0, list->length -1, ascending, &PointerArray_compareEntry, &PointerArray_swapEntry, &PointerArray_relocateEntry);
    value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 0);

    PointerArray_free(&list);
}

void test_shortSort(void **state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 0; i < 10; i++) {
        PointerArray_tail_push(list, (Pointer)i);
        assert_int_equal(i, list->length -1);
    }

    PointerArray_head_pop(list);

    // list->compare = &compare$64;
    u64* value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 1);

    sort(list, 0, list->length -1, descending, &PointerArray_compareEntry, &PointerArray_swapEntry, &PointerArray_relocateEntry);

    assert_int_equal(PointerArray_getEntry(list, 0), 9);
    assert_int_equal(PointerArray_getEntry(list, 1), 8);
    assert_int_equal(PointerArray_getEntry(list, 2), 7);
    assert_int_equal(PointerArray_getEntry(list, 3), 6);
    assert_int_equal(PointerArray_getEntry(list, 4), 5);
    assert_int_equal(PointerArray_getEntry(list, 5), 4);
    assert_int_equal(PointerArray_getEntry(list, 6), 3);
    assert_int_equal(PointerArray_getEntry(list, 7), 2);
    assert_int_equal(PointerArray_getEntry(list, 8), 1);

    sort(list, 0, list->length -1, ascending, &PointerArray_compareEntry, &PointerArray_swapEntry, &PointerArray_relocateEntry);

    assert_int_equal(PointerArray_getEntry(list, 0), 1);
    assert_int_equal(PointerArray_getEntry(list, 1), 2);
    assert_int_equal(PointerArray_getEntry(list, 2), 3);
    assert_int_equal(PointerArray_getEntry(list, 3), 4);
    assert_int_equal(PointerArray_getEntry(list, 4), 5);
    assert_int_equal(PointerArray_getEntry(list, 5), 6);
    assert_int_equal(PointerArray_getEntry(list, 6), 7);
    assert_int_equal(PointerArray_getEntry(list, 7), 8);
    assert_int_equal(PointerArray_getEntry(list, 8), 9);

    value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 1);

    PointerArray_free(&list);
}

void test_wrappedshortSort(void **state)
{
    PPointerArray list = PointerArray_create(8);

    // create a wrap
    for (u64 i = 0; i < 40; i++) {
        PointerArray_tail_push(list, (Pointer)i);
    }

    for (u64 i = 0; i < 20; i++) {
        PointerArray_head_pop(list);
    }

    for (u64 i = 40; i < 80; i++) {
        PointerArray_tail_push(list, (Pointer)i);
    }

    // list->compare = &compare$64;

    u64* value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 20);

    // sort(list, 0, list->length -1, descending, &PointerArray_compareEntry, &PointerArray_swapEntry, &PointerArray_relocateEntry);
    sort(list, 0, list->length -1, descending, &PointerArray_compareEntry, &PointerArray_swapEntry, null);

    value = PointerArray_getEntry(list, 0);
    // u64* values = &list->$values[list->$startIndex];
    // printDebug("%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu", 
    //     values[0], values[1], values[2], values[3], values[4], values[5], values[5], values[7], values[8], values[9]);

    assert_int_equal(value, 79);

    // sort(list, 0, list->length -1, ascending, &PointerArray_compareEntry, &PointerArray_swapEntry, &PointerArray_relocateEntry);
    sort(list, 0, list->length -1, ascending, &PointerArray_compareEntry, &PointerArray_swapEntry, null);

    value = PointerArray_getEntry(list, 0);
    assert_int_equal(value, 20);

    PointerArray_free(&list);
}


void test_volumne (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 0; i < 20000000; i++) {
        PointerArray_tail_push(list, &i);
        u64* p = PointerArray_head_peek(list);
        assert_int_equal(list->length, i+1);
    }
    
    PointerArray_free(&list);
}

void test_volumne_pop (void ** state)
{
    PPointerArray list = PointerArray_create(8);
    PushPop queue = PointerArray_queueInterface();

    for (u64 i = 0; i < 4; i++) {
        u64* p;
        for (u64 j = 0; j < 62; j++) {
            queue->push(list, &i);
            p = queue->peek(list);
        }

        u64* p1 = queue->pop(list);
        assert_int_equal(*p1, *p);
    }
        
    PointerArray_free(&list);
}

void test_volumne_pop_after (void ** state)
{
    PPointerArray list = PointerArray_create(8);

    for (u64 i = 0; i < 20000000; i++) {
        PointerArray_tail_push(list, &i);
        Pointer p = PointerArray_head_peek(list);
        assert_int_equal(list->length, i+1);
    }

    for (u64 i = 0; i < 20000000; i++) {
        u64* p = PointerArray_head_peek(list);
        u64*  p1 = PointerArray_head_pop(list);
        assert_int_equal(*p1, *p);
    }

    PointerArray_free(&list);
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
