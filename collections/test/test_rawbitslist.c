#include <assert.h>
#include "bitslist.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


void test_create (void ** state)
{
    PBitslist bitslist = Bitslist_create(100, false);
    assert_true(bitslist);
    assert_int_not_equal(bitslist->$capacity, 0);
    assert_int_not_equal(bitslist->$values, 0);
    assert_int_equal(bitslist->length, 100);
    
    Bitslist_free(&bitslist);
    assert_true(!bitslist);
}

void test_setbits (void ** state)
{
    PBitslist bitslist = Bitslist_create(100, false);

    assert_false(Bitslist_isBit(bitslist, 0));
    assert_false(Bitslist_isBit(bitslist, 99));

    Bitslist_setBit(bitslist, 50);
    assert_true(Bitslist_isBit(bitslist, 50));
    assert_false(Bitslist_isBit(bitslist, 49));
    assert_false(Bitslist_isBit(bitslist, 51));

    Bitslist_clearBit(bitslist, 50);
    assert_false(Bitslist_isBit(bitslist, 50));

    Bitslist_set(bitslist, 50, true);
    assert_true(Bitslist_isBit(bitslist, 50));

    Bitslist_set(bitslist, 50, false);
    assert_false(Bitslist_isBit(bitslist, 50));

    Bitslist_free(&bitslist);
    assert_true(!bitslist);
}

void test_setall (void ** state)
{
    PBitslist bitslist = Bitslist_create(100, false);

    for (int i = 0; i < bitslist->length; i++) {
        assert_false(Bitslist_isBit(bitslist, i));
    }

    Bitslist_setAll(bitslist, true);
    for (int i = 0; i < bitslist->length; i++) {
        assert_true(Bitslist_isBit(bitslist, i));
    }

    Bitslist_setAll(bitslist, false);
    for (int i = 0; i < bitslist->length; i++) {
        assert_false(Bitslist_isBit(bitslist, i));
    }

    Bitslist_free(&bitslist);
    assert_true(!bitslist);
}

void test_findFirst (void ** state)
{
    PBitslist bitslist = Bitslist_create(100, false);

    i32 bit = Bitslist_findFirst(bitslist);
    assert_int_equal(bit, -1);
    Bitslist_setBit(bitslist, 50);
    bit = Bitslist_findFirst(bitslist);
    assert_int_equal(bit, 50);

    Bitslist_free(&bitslist);
    assert_true(!bitslist);
}

void test_getBitCount (void ** state)
{
    PBitslist bitslist = Bitslist_create(100, false);

    u32 bit = Bitslist_getBitCount(bitslist);
    assert_int_equal(bit, 0);
    Bitslist_setBit(bitslist, 50);
    bit = Bitslist_getBitCount(bitslist);
    assert_int_equal(bit, 1);

    Bitslist_free(&bitslist);
    assert_true(!bitslist);
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
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