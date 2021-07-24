#include <assert.h>
#include "rawbitslist.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_function_mapping (void ** state)
{
    assert_true(RawBitslist.create == &RawBitslist_create);
    assert_true(RawBitslist.init == &RawBitslist_init);
    assert_true(RawBitslist.free == &RawBitslist_free);
    assert_true(RawBitslist.freeChildren == &RawBitslist_freeChildren);
    assert_true(RawBitslist.clearBit == &RawBitslist_clearBit);
    assert_true(RawBitslist.findFirst == &RawBitslist_findFirst);
    assert_true(RawBitslist.getBitCount == &RawBitslist_getBitCount);
    assert_true(RawBitslist.isBit == &RawBitslist_isBit);
    assert_true(RawBitslist.set == &RawBitslist_set);
    assert_true(RawBitslist.setAll == &RawBitslist_setAll);
    assert_true(RawBitslist.setBit == &RawBitslist_setBit);
}

void test_create (void ** state)
{
    PRawBitslist bitslist = RawBitslist.create(100, false);
    assert_true(bitslist);
    assert_int_not_equal(bitslist->$capacity, 0);
    assert_int_not_equal(bitslist->content, 0);
    assert_int_equal(bitslist->length, 100);
    
    RawBitslist.free(&bitslist);
    assert_true(!bitslist);
}

void test_setbits (void ** state)
{
    PRawBitslist bitslist = RawBitslist.create(100, false);

    assert_false(RawBitslist.isBit(bitslist, 0));
    assert_false(RawBitslist.isBit(bitslist, 99));

    RawBitslist.setBit(bitslist, 50);
    assert_true(RawBitslist.isBit(bitslist, 50));
    assert_false(RawBitslist.isBit(bitslist, 49));
    assert_false(RawBitslist.isBit(bitslist, 51));

    RawBitslist.clearBit(bitslist, 50);
    assert_false(RawBitslist.isBit(bitslist, 50));

    RawBitslist.set(bitslist, 50, true);
    assert_true(RawBitslist.isBit(bitslist, 50));

    RawBitslist.set(bitslist, 50, false);
    assert_false(RawBitslist.isBit(bitslist, 50));

    RawBitslist.free(&bitslist);
    assert_true(!bitslist);
}

void test_setall (void ** state)
{
    PRawBitslist bitslist = RawBitslist.create(100, false);

    for (int i = 0; i < bitslist->length; i++) {
        assert_false(RawBitslist.isBit(bitslist, i));
    }

    RawBitslist.setAll(bitslist, true);
    for (int i = 0; i < bitslist->length; i++) {
        assert_true(RawBitslist.isBit(bitslist, i));
    }

    RawBitslist.setAll(bitslist, false);
    for (int i = 0; i < bitslist->length; i++) {
        assert_false(RawBitslist.isBit(bitslist, i));
    }

    RawBitslist.free(&bitslist);
    assert_true(!bitslist);
}

void test_findFirst (void ** state)
{
    PRawBitslist bitslist = RawBitslist.create(100, false);

    i32 bit = RawBitslist.findFirst(bitslist);
    assert_int_equal(bit, -1);
    RawBitslist.setBit(bitslist, 50);
    bit = RawBitslist.findFirst(bitslist);
    assert_int_equal(bit, 50);

    RawBitslist.free(&bitslist);
    assert_true(!bitslist);
}

void test_getBitCount (void ** state)
{
    PRawBitslist bitslist = RawBitslist.create(100, false);

    u32 bit = RawBitslist.getBitCount(bitslist);
    assert_int_equal(bit, 0);
    RawBitslist.setBit(bitslist, 50);
    bit = RawBitslist.getBitCount(bitslist);
    assert_int_equal(bit, 1);

    RawBitslist.free(&bitslist);
    assert_true(!bitslist);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_function_mapping),
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_setbits),
        cmocka_unit_test (test_setall),
        cmocka_unit_test (test_findFirst),
        cmocka_unit_test (test_getBitCount)
        
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, null, null);

    return count_fail_tests;
}