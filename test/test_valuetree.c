#include <assert.h>
#include "byvalue/valuetree.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


void test_create (void ** state)
{
    PValueTree tree = ValueTree_create(8, &compare$64);
    assert_true(tree);
    assert_int_equal(tree->length, 0);

    assert_int_equal(tree->compare, &compare$64);
    assert_int_equal(tree->$objectSize, 8);
    assert_int_equal(tree->$rootNode, null);
    
    ValueTree_free(&tree);
    assert_true(!tree);
}

void test_add (void ** state)
{
    PValueTree tree = ValueTree_create(8, &compare$64);
    u64 a = 8234;
    assert_int_equal(tree->length, 0);
    ValueTree_addEntry(tree, &a);
    assert_int_equal(tree->length, 1);
    assert_int_equal(*(u64*)ValueTree_find(tree, &a), a);
    ValueTree_free(&tree);
    assert_true(!tree);
}

void test_delete (void ** state)
{
    PValueTree tree = ValueTree_create(8, &compare$64);
    u64 a = 8234;
    assert_int_equal(tree->length, 0);
    ValueTree_addEntry(tree, &a);
    assert_int_equal(tree->length, 1);
    ValueTree_deleteEntry(tree, &a);
    assert_int_equal(tree->length, 0);
    ValueTree_free(&tree);
    assert_true(!tree);
}

void test_inorder_find (void ** state)
{
    PValueTree tree = ValueTree_create(4, &compare$32);
    i32 numbers[] = {7, 5, 13, 4, 11, 19};
    for (int i = 0; i < 6; i++) {
        ValueTree_addEntry(tree, &numbers[i]);
    } 

    for (int i = 0; i < 6; i++) {
        i32 * res = ValueTree_find(tree, &numbers[i]);
        if (!res) {
            printDebug("find %i, %i", i, numbers[i]);
        }
        assert_ptr_not_equal(res, null);
        assert_int_equal(*res, numbers[i]);
    } 

    ValueTree_free(&tree);
    assert_true(!tree);
}


void test_inorder_delete (void ** state)
{
    PValueTree tree = ValueTree_create(4, &compare$32);
    i32 numbers[] = {7, 5, 13, 4, 11, 19};
    for (int i = 0; i < 6; i++) {
        ValueTree_addEntry(tree, &numbers[i]);
    } 

    for (int i = 0; i < 6; i++) {
        i32 * res = ValueTree_find(tree, &numbers[i]);
        if (!res) {
            printDebug("find %i, %i", i, numbers[i]);
        }
        assert_ptr_not_equal(res, null);
        assert_int_equal(*res, numbers[i]);

        ValueTree_deleteEntry(tree, &numbers[i]);
        res = ValueTree_find(tree, &numbers[i]);
        if (res) {
            printDebug("no delete %i, %i", i, numbers[i]);
        }
        assert_ptr_equal(res, null);
        assert_int_equal(tree->length, 5-i);

    } 

    ValueTree_free(&tree);
    assert_true(!tree);
}

int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_add),
        cmocka_unit_test (test_delete),
        cmocka_unit_test (test_inorder_find),
        cmocka_unit_test (test_inorder_delete),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}