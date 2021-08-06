#include "byvalue/valuelifo.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_create (void ** state)
{
    PValueLifo list = ValueLifo_create(21);
    assert_true(list);

    assert_int_equal(list->tip, null);

    ValueLifo_free(&list);
    assert_true(!list);
}

void test_push (void ** state)
{
    PValueLifo list = ValueLifo_create(8);

    for (u64 i = 0; i < 100; i++) {
        ValueLifo_push(list, &i);
    }

    ValueLifo_free(&list);
}

void test_pop (void ** state)
{
    PValueLifo list = ValueLifo_create(8);

    for (u64 i = 1; i < 100; i++) {
        ValueLifo_push(list, &i);
    }

    for (u64 i = 1; i < 100; i++) {
        Pointer peek = ValueLifo_peek(list);
        u64* p = ValueLifo_pop(list);
        assert_ptr_equal(peek, p);
        assert_int_equal(100 - i, *p);
    }

    ValueLifo_free(&list);  
}

void test_recycle (void ** state)
{
    PValueLifo list = ValueLifo_create(8);

    for (u64 i = 1; i < 100; i++) {
        ValueLifo_push(list, &i);
    }

    for (u64 i = 1; i < 100; i++) {
        Pointer peek = ValueLifo_peek(list);
        u64* p = ValueLifo_pop(list);
        assert_int_equal(100 - i, *p);
        assert_ptr_equal(peek, p);
    }

    for (u64 i = 1; i < 100; i++) {
        ValueLifo_push(list, &i);
        Pointer peek = ValueLifo_peek(list);
        u64* p = ValueLifo_pop(list);
        assert_int_equal(i, *p);
        assert_ptr_equal(peek, p);
    }

    ValueLifo_free(&list);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_push),
        cmocka_unit_test (test_pop),
        cmocka_unit_test (test_recycle),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}
