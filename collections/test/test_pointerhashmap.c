#include <assert.h>
#include "bypointer/pointerhashmap.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

int my_compare(Pointer a, Pointer b)
{
    if (!a) {
        return -1;
    }
    if (!b) {
        return 1;
    }
    if (a == b) {
        return 0;
    }
    return *(i32*)a - *(i32*)b;
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
    PPointerHashmap hashmap = PointerHashmap_create(my_hashCode, my_compare);
    assert_true(hashmap);
    assert_int_not_equal(hashmap->$nextGrow, 0);
    assert_int_not_equal(hashmap->$nextShrink, 0);
    assert_true(!hashmap->$resizing);
    assert_int_not_equal(hashmap->$capacity, 0);
    assert_int_not_equal(hashmap->$collisionMap, 0);
    assert_int_equal(hashmap->length, 0);
    assert_int_not_equal(hashmap->$tree, 0);

    assert_ptr_equal(hashmap->compare, my_compare);
    assert_ptr_equal(hashmap->hashCode, my_hashCode);
    
    PointerHashmap_free(&hashmap);
    assert_true(!hashmap);
}

void test_put (void ** state)
{
    PPointerHashmap hashmap = PointerHashmap_create(my_hashCode, my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerHashmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);

    u64 sameValue = (u64)PointerHashmap_get(hashmap, (Pointer)key);

    assert_int_equal(sameValue, 41);

    PointerHashmap_free(&hashmap);
    assert_true(!hashmap);
}

void test_spill (void ** state)
{
    PPointerHashmap hashmap = PointerHashmap_create(my_hashCode, my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerHashmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);
    u64 sameValue = (u64)PointerHashmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 41);

    key = key + 64;
    value = 61;

    storedValue = (u64)PointerHashmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 61);
    sameValue = (u64)PointerHashmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 61);

    key = key + 64;
    value = 79;

    storedValue = (u64)PointerHashmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 79);
    sameValue = (u64)PointerHashmap_get(hashmap, (Pointer)key);
    assert_int_equal(sameValue, 79);

    PointerHashmap_free(&hashmap);
    assert_true(!hashmap);
}


void test_remove (void ** state)
{
    PPointerHashmap hashmap = PointerHashmap_create(my_hashCode, my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerHashmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);
    u64 sameValue = (u64)PointerHashmap_get(hashmap, (Pointer)key);

    // assert_int_equal(sameValue[0], 57);
    // assert_int_equal(sameValue[1], 100);

    // PointerHashmap_deleteEntry(hashmap, &value);
    // assert_int_equal(hashmap->length, 0);
    // sameValue = PointerHashmap_get(hashmap, &value);
    // assert_int_equal(sameValue, 0);

    // PointerHashmap_free(&hashmap);
    // assert_true(!hashmap);
}


void test_traverse (void ** state)
{
    PPointerHashmap hashmap = PointerHashmap_create(my_hashCode, my_compare);

    u64 key = 57;
    u64 value = 41;

    u64 storedValue = (u64)PointerHashmap_put(hashmap, (Pointer)key, (Pointer)value);
    assert_int_equal(storedValue, 41);

    PointerHashmap_traverse(hashmap, "blah", &my_traverser);

    PointerHashmap_free(&hashmap);
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
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}