#include "allocators/valueallocator.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_create (void ** state)
{
    PValueAllocator list = ValueAllocator_create(21);
    assert_true(list);

    assert_int_not_equal(list->$capacity, 0);
    assert_ptr_not_equal(list->memBlock, null);
    assert_ptr_not_equal(list->$entry, null);

    assert_ptr_not_equal(list->allocationData, null);
    assert_ptr_not_equal(list->recyclerData, null);
    assert_ptr_not_equal(list->stackCleanup, null);
    assert_ptr_not_equal(list->stackFunction, null);

    assert_int_equal(list->$entrySize, 21);

    ValueAllocator_free(&list);
    assert_true(!list);
}

void test_borrow (void ** state)
{
    PValueAllocator list = ValueAllocator_create(21);

    for (u64 i = 0; i < 100; i++) {
        Pointer p = ValueAllocator_borrow(list);
    }

    ValueAllocator_free(&list);
}

void test_return (void ** state)
{
    PValueAllocator list = ValueAllocator_create(21);

    for (u64 i = 0; i < 100; i++) {
        Pointer p = ValueAllocator_borrow(list);
        ValueAllocator_returnIt(list, p);
    }

    ValueAllocator_free(&list);
}

void test_recycle (void ** state)
{
    PValueAllocator list = ValueAllocator_create(21);

    for (u64 i = 0; i < 100; i++) {
        Pointer p = ValueAllocator_borrow(list);
        ValueAllocator_returnIt(list, p);
    }

    for (u64 i = 0; i < 100; i++) {
        Pointer p = ValueAllocator_borrow(list);
        ValueAllocator_returnIt(list, p);
    }

    ValueAllocator_free(&list);
}

int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_borrow),
        cmocka_unit_test (test_return),
        cmocka_unit_test (test_recycle),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}
