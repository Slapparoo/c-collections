#include "bypointer/pointerfifo.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_create (void ** state)
{
    PPointerFifo list = PointerFifo_create();
    assert_true(list);

    assert_int_equal(list->tip, null);

    PointerFifo_free(&list);
    assert_true(!list);
}

void test_push (void ** state)
{
    PPointerFifo list = PointerFifo_create();

    for (u64 i = 0; i < 100; i++) {
        PointerFifo_push(list, &i);
    }

    PointerFifo_free(&list);
}

void test_pop (void ** state)
{
    PPointerFifo list = PointerFifo_create();

    for (u64 i = 1; i < 100; i++) {
        PointerFifo_push(list, (Pointer)i);
    }

    for (u64 i = 1; i < 100; i++) {
        Pointer peek = PointerFifo_peek(list);
        Pointer p = PointerFifo_pop(list);
        assert_int_equal(i, (u64)p);
        assert_ptr_equal(peek, p);
    }

    PointerFifo_free(&list);
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
