#include <assert.h>
#include "byvalue/valuehashmap.h"

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

void myTraverse(PValueHashmap map, Pointer other, u32* value) {
    printDebug("myTraverse %p, %p key:%i, value:%i", map, other, value[0], value[1]);
}

void test_function_mapping (void ** state)
{
    assert_ptr_equal(ValueHashmap.create, &ValueHashmap_create);
    assert_ptr_equal(ValueHashmap.init, &ValueHashmap_init);
    assert_ptr_equal(ValueHashmap.clear, &ValueHashmap_clear);
    assert_ptr_equal(ValueHashmap.free, &ValueHashmap_free);
    assert_ptr_equal(ValueHashmap.freeChildren, &ValueHashmap_freeChildren);
    assert_ptr_equal(ValueHashmap.get, &ValueHashmap_get);
    assert_ptr_equal(ValueHashmap.put, &ValueHashmap_put);
    assert_ptr_equal(ValueHashmap.putEntry, &ValueHashmap_putEntry);
    assert_ptr_equal(ValueHashmap.deleteEntry, &ValueHashmap_deleteEntry);
    assert_ptr_equal(ValueHashmap.traverse, &ValueHashmap_traverse);
}

void test_create (void ** state)
{
    PValueHashmap hashmap = ValueHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);
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
    
    ValueHashmap.free(&hashmap);
    assert_true(!hashmap);
}

void test_put (void ** state)
{
    PValueHashmap hashmap = ValueHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);

    u64 value = 57;

    u32* storedValue = ValueHashmap.putEntry(hashmap, &value);
    assert_int_equal(*storedValue, 57);
    storedValue[1] = 100;
    u32* sameValue = ValueHashmap.get(hashmap, &value);

    assert_int_equal(sameValue[0], 57);
    assert_int_equal(sameValue[1], 100);

    ValueHashmap.free(&hashmap);
    assert_true(!hashmap);
}

void test_remove (void ** state)
{
    PValueHashmap hashmap = ValueHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);

    u64 value = 57;

    u32* storedValue = ValueHashmap.putEntry(hashmap, &value);
    assert_int_equal(*storedValue, 57);
    storedValue[1] = 100;
    u32* sameValue = ValueHashmap.get(hashmap, &value);

    assert_int_equal(sameValue[0], 57);
    assert_int_equal(sameValue[1], 100);

    ValueHashmap.deleteEntry(hashmap, &value);
    assert_int_equal(hashmap->length, 0);
    sameValue = ValueHashmap.get(hashmap, &value);
    assert_int_equal(sameValue, 0);

    ValueHashmap.free(&hashmap);
    assert_true(!hashmap);
}


void test_traverse (void ** state)
{
    PValueHashmap hashmap = ValueHashmap.create(8, RawHashMapEntry_hashCode, RawHashMapEntry_compare);

    u64 value = 57;

    u32* storedValue = ValueHashmap.putEntry(hashmap, &value);
    assert_int_equal(*storedValue, 57);
    storedValue[1] = 100;

    ValueHashmap_traverse(hashmap, "blah", &myTraverse);

    ValueHashmap.free(&hashmap);
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