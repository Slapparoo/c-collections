#include <assert.h>
#include "bypointer/pointertreemap.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


void test_create (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&compare$64);
    assert_true(tree);
    assert_int_equal(tree->length, 0);

    assert_int_equal(tree->compare, &compare$64);
    assert_int_equal(tree->$rootNode, null);
    
    PointerTreemap_free(&tree);
    assert_true(!tree);
}

i32 my_compare(u64 a, u64 b) {
    return a - b;
}

void test_add (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&my_compare);
    PointerMapEntry entry = {(Pointer)5, (Pointer)10};
    assert_int_equal(tree->length, 0);
    PointerTreemap_addEntry(tree, &entry);
    assert_int_equal(tree->length, 1);
    assert_int_equal(PointerTreemap_find(tree, entry.key), 10);
    PointerTreemap_free(&tree);
    assert_true(!tree);
}

void test_strings (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&compare$PChar);
    PointerMapEntry entry = {"some key", "some value"};
    assert_int_equal(tree->length, 0);
    PointerTreemap_addEntry(tree, &entry);
    assert_int_equal(tree->length, 1);
    PointerMapEntry entry2 = {"some key other key", "some value value"};
    PointerTreemap_addEntry(tree, &entry2);
    assert_int_equal(tree->length, 2);

    assert_ptr_not_equal(PointerTreemap_find(tree, entry.key), null);
    assert_ptr_not_equal(PointerTreemap_find(tree, entry2.key), null);
    PointerTreemap_free(&tree);
    assert_true(!tree);
}


void test_replace (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&my_compare);
    PointerMapEntry entry = {(Pointer)5, (Pointer)10};
    assert_int_equal(tree->length, 0);
    PointerTreemap_addEntry(tree, &entry);
    PointerTreemap_addEntry(tree, &entry);
    PointerTreemap_addEntry(tree, &entry);
    assert_int_equal(tree->length, 1);
    assert_int_equal(PointerTreemap_find(tree, entry.key), 10);
    PointerTreemap_free(&tree);
    assert_true(!tree);
}


void test_delete (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&my_compare);
    PointerMapEntry entry = {(Pointer)5, (Pointer)10};
    assert_int_equal(tree->length, 0);
    PointerTreemap_addEntry(tree, &entry);
    assert_int_equal(tree->length, 1);
    PointerTreemap_deleteEntry(tree, entry.key);
    assert_int_equal(tree->length, 0);
    PointerTreemap_free(&tree);
    assert_true(!tree);
}

void test_inorder_find (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&my_compare);
    u64 numbers[] = {7, 5, 13, 4, 11, 19};
    for (int i = 0; i < 6; i++) {
        PointerMapEntry entry = {(Pointer)numbers[i], (Pointer)numbers[i]};
        PointerTreemap_addEntry(tree, &entry);
    } 

    for (int i = 0; i < 6; i++) {
        u64 res = (u64)PointerTreemap_find(tree, (Pointer)numbers[i]);
        if (!res) {
            printDebug("find %i, %lu", i, numbers[i]);
        }
        assert_ptr_not_equal(res, null);
        assert_int_equal(res, numbers[i]);
    } 

    PointerTreemap_free(&tree);
    assert_true(!tree);
}


void test_inorder_delete (void ** state)
{
    PPointerTreemap tree = PointerTreemap_create(&my_compare);
    u64 numbers[] = {7, 5, 13, 4, 11, 19};
    for (int i = 0; i < 6; i++) {
        PointerMapEntry entry = {(Pointer)numbers[i], (Pointer)numbers[i]};
        PointerTreemap_addEntry(tree, &entry);
    } 

    for (int i = 0; i < 6; i++) {
        u64 res = (u64)PointerTreemap_find(tree, (Pointer)numbers[i]);
        if (!res) {
            printDebug("find %i, %lu", i, numbers[i]);
        }
        assert_ptr_not_equal(res, null);
        assert_int_equal(res, numbers[i]);

        PointerTreemap_deleteEntry(tree, (Pointer)numbers[i]);
        res = (u64)PointerTreemap_find(tree, (Pointer)numbers[i]);
        if (res) {
            printDebug("no delete %i, %lu", i, numbers[i]);
        }
        assert_ptr_equal(res, null);
        assert_int_equal(tree->length, 5-i);

    } 

    PointerTreemap_free(&tree);
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
        cmocka_unit_test (test_replace),
        cmocka_unit_test (test_strings),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}