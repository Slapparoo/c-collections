#include <assert.h>
#include "rawhashmap.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

int RawHashMapEntry_compare(Pointer a, Pointer b)
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

int RawHashMapEntry_hashCode(Pointer a)
{
    return *(i32*)a;
}

void myTraverse(PRawHashmap map, Pointer other, u32* value) {
    printDebug("myTraverse %p, %p key:%i, value:%i", map, other, value[0], value[1]);
}

void test_function_mapping (void ** state)
{
    assert_true(RawHashmap.create == &RawHashmap_create);
    assert_true(RawHashmap.init == &RawHashmap_init);
    assert_true(RawHashmap.clear == &RawHashmap_clear);
    assert_true(RawHashmap.free == &RawHashmap_free);
    assert_true(RawHashmap.freeChildren == &RawHashmap_freeChildren);
    assert_true(RawHashmap.get == &RawHashmap_get);
    assert_true(RawHashmap.put == &RawHashmap_put);
    assert_true(RawHashmap.remove == &RawHashmap_remove);
    assert_true(RawHashmap.traverse == &RawHashmap_traverse);
}

void test_create (void ** state)
{
    PRawHashmap hashmap = RawHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);
    assert_true(hashmap);
    assert_int_not_equal(hashmap->$nextGrow, 0);
    assert_int_not_equal(hashmap->$nextShrink, 0);
    assert_true(!hashmap->$resizing);
    assert_int_not_equal(hashmap->$capacity, 0);
    assert_int_not_equal(hashmap->$collisionMap, 0);
    assert_int_equal(hashmap->$entrySize, 8);
    assert_int_equal(hashmap->length, 0);
    assert_int_not_equal(hashmap->$tree, 0);

    assert_ptr_equal(hashmap->compare, RawHashMapEntry_compare);
    assert_ptr_equal(hashmap->hashCode, RawHashMapEntry_hashCode);
    
    RawHashmap.free(&hashmap);
    assert_true(!hashmap);
}

void test_put (void ** state)
{
    PRawHashmap hashmap = RawHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);

    u64 value = 57;

    u32* storedValue = RawHashmap.put(hashmap, &value);
    assert_int_equal(*storedValue, 57);
    storedValue[1] = 100;
    u32* sameValue = RawHashmap.get(hashmap, &value);

    assert_int_equal(sameValue[0], 57);
    assert_int_equal(sameValue[1], 100);

    RawHashmap.free(&hashmap);
    assert_true(!hashmap);
}

void test_remove (void ** state)
{
    PRawHashmap hashmap = RawHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);

    u64 value = 57;

    u32* storedValue = RawHashmap.put(hashmap, &value);
    assert_int_equal(*storedValue, 57);
    storedValue[1] = 100;
    u32* sameValue = RawHashmap.get(hashmap, &value);

    assert_int_equal(sameValue[0], 57);
    assert_int_equal(sameValue[1], 100);

    RawHashmap.remove(hashmap, &value);
    assert_int_equal(hashmap->length, 0);
    sameValue = RawHashmap.get(hashmap, &value);
    assert_int_equal(sameValue, 0);

    RawHashmap.free(&hashmap);
    assert_true(!hashmap);
}


void test_traverse (void ** state)
{
    PRawHashmap hashmap = RawHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);

    u64 value = 57;

    u32* storedValue = RawHashmap.put(hashmap, &value);
    assert_int_equal(*storedValue, 57);
    storedValue[1] = 100;

    RawHashmap_traverse(hashmap, "blah", &myTraverse);

    RawHashmap.free(&hashmap);
    assert_true(!hashmap);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_function_mapping),
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_put),
        cmocka_unit_test (test_remove),
        cmocka_unit_test (test_traverse)
        
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}