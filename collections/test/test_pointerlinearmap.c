#include <assert.h>
#include "bypointer/pointerlinearmap.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

int my_compare(u64 a, u64 b)
{
    return a - b;
}


int my_hashCode(i32 a)
{
    return (i32)a;
}

Fn_traverser(my, Pointer, u64*)
{
    printDebug("myTraverse %p, key:%lu, value:%lu", this, entry[0], entry[1]);
}

void test_create (void ** state)
{
    PPointerLinearmap hashmap = PointerLinearmap_create(my_compare);
    assert_true(hashmap);
    assert_int_equal(hashmap->length, 0);
    assert_ptr_not_equal(hashmap->$keys, 0);
    assert_ptr_not_equal(hashmap->$values, 0);

    assert_ptr_equal(hashmap->compare, my_compare);
    
    PointerLinearmap_free(&hashmap);
    assert_true(!hashmap);
}

void test_put (void ** state)
{
    PPointerLinearmap hashmap = PointerLinearmap_create(my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerLinearmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);

    u64 sameValue = (u64)PointerLinearmap_get(hashmap, (Pointer)key);

    assert_int_equal(sameValue, 41);

    PointerLinearmap_free(&hashmap);
    assert_true(!hashmap);
}

void test_spill (void ** state)
{
    PPointerLinearmap hashmap = PointerLinearmap_create(my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerLinearmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);
    u64 sameValue = (u64)PointerLinearmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 41);

    key = key + 64;
    value = 61;

    storedValue = (u64)PointerLinearmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 61);
    sameValue = (u64)PointerLinearmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 61);

    key = key + 64;
    value = 79;

    storedValue = (u64)PointerLinearmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 79);
    sameValue = (u64)PointerLinearmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 79);

    PointerLinearmap_free(&hashmap);
    assert_true(!hashmap);
}

void test_replace (void ** state)
{
    PPointerLinearmap tree = PointerLinearmap_create(&my_compare);
    PointerMapEntry entry = {(Pointer)5, (Pointer)10};
    assert_int_equal(tree->length, 0);
    PointerLinearmap_putEntry(tree, &entry);
    PointerLinearmap_putEntry(tree, &entry);
    PointerLinearmap_putEntry(tree, &entry);
    assert_int_equal(tree->length, 1);
    assert_int_equal(PointerLinearmap_get(tree, entry.key), 10);
    PointerLinearmap_free(&tree);
    assert_true(!tree);
}

void test_strings (void ** state)
{
    PPointerLinearmap tree = PointerLinearmap_create(&compare$PChar);
    PointerMapEntry entry = {"some key", "some value"};
    assert_int_equal(tree->length, 0);
    PointerLinearmap_putEntry(tree, &entry);
    assert_int_equal(tree->length, 1);
    PointerMapEntry entry2 = {"some key other key", "some value value"};
    PointerLinearmap_putEntry(tree, &entry2);
    assert_int_equal(tree->length, 2);

    assert_ptr_not_equal(PointerLinearmap_get(tree, entry.key), null);
    assert_ptr_not_equal(PointerLinearmap_get(tree, entry2.key), null);
    PointerLinearmap_free(&tree);
    assert_true(!tree);
}

void test_remove (void ** state)
{
    PPointerLinearmap hashmap = PointerLinearmap_create(my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerLinearmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);
    u64 sameValue = (u64)PointerLinearmap_get(hashmap, (Pointer)key);

    PointerLinearmap_deleteEntry(hashmap, (Pointer)key);
    assert_int_equal(hashmap->length, 0);
    sameValue = (u64)PointerLinearmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 0);

    PointerLinearmap_free(&hashmap);
    assert_true(!hashmap);
}


void test_traverse (void ** state)
{
    PPointerLinearmap hashmap = PointerLinearmap_create(my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerLinearmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);

    PointerLinearmap_traverse(hashmap, "blah", &my_traverser);

    PointerLinearmap_free(&hashmap);
    assert_true(!hashmap);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_put),
        cmocka_unit_test (test_remove),
        cmocka_unit_test (test_traverse),
        cmocka_unit_test (test_spill),
        cmocka_unit_test (test_replace),
        cmocka_unit_test (test_strings)
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}