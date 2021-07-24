
#include <assert.h>
#include "rawarraylist.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "quicksort.h"

void test_function_mapping (void ** state)
{
    assert_true(RawArraylist.create == RawArraylist_create);
    assert_true(RawArraylist.init == RawArraylist_init);
    assert_true(RawArraylist.clear == RawArraylist_clear);
    assert_true(RawArraylist.free == RawArraylist_free);
    assert_true(RawArraylist.freeChildren == RawArraylist_freeChildren);
    assert_true(RawArraylist.get == RawArraylist_get);
    assert_true(RawArraylist.add == RawArraylist_add);
    assert_true(RawArraylist.set == RawArraylist_set);
    assert_true(RawArraylist.swap == RawArraylist_swap);
    assert_true(RawArraylist.traverseForward == RawArraylist_traverseForward);
    assert_true(RawArraylist.traverseBackward == RawArraylist_traverseBackward);
}

void test_create (void ** state)
{
    PRawArraylist list = RawArraylist.create(100, 8);
    assert_true(list);
    assert_int_equal(list->$capacity, 100);
    assert_true(list->items);
    assert_int_equal(list->$itemSize, 8);

    RawArraylist.free(&list);
    assert_true(!list);
}



int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_function_mapping),
        cmocka_unit_test (test_create),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}
