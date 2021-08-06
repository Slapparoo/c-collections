#include "bypointer/pointerlifo.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_create (void ** state)
{
    PPointerLifo list = PointerLifo_create();
    assert_true(list);

    assert_int_equal(list->tip, null);

    PointerLifo_free(&list);
    assert_true(!list);
}

void test_push (void ** state)
{
    PPointerLifo list = PointerLifo_create();

    for (u64 i = 0; i < 100; i++) {
        PointerLifo_push(list, &i);
    }

    PointerLifo_free(&list);
}

void test_pop (void ** state)
{
    PPointerLifo list = PointerLifo_create();

    for (u64 i = 1; i < 100; i++) {
        PointerLifo_push(list, (Pointer)i);
    }

    for (u64 i = 1; i < 100; i++) {
        Pointer peek = PointerLifo_peek(list);
        Pointer p = PointerLifo_pop(list);
        assert_int_equal(100 - i, (u64)p);
        assert_ptr_equal(peek, p);
    }

    PointerLifo_free(&list);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_push),
        cmocka_unit_test (test_pop),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}
