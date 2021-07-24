#include "rawlinkedlist.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "quicksort.h"


void test_function_mapping (void ** state)
{
    assert_true(RawLinkedlist.create == RawLinkedlist_create);
    assert_true(RawLinkedlist.init == RawLinkedlist_init);
    assert_true(RawLinkedlist.clear == RawLinkedlist_clear);
    assert_true(RawLinkedlist.free == RawLinkedlist_free);
    assert_true(RawLinkedlist.freeChildren == RawLinkedlist_freeChildren);
    assert_true(RawLinkedlist.get == RawLinkedlist_get);
    assert_true(RawLinkedlist.popHead == RawLinkedlist_popHead);
    assert_true(RawLinkedlist.popTail == RawLinkedlist_popTail);
    assert_true(RawLinkedlist.pushHead == RawLinkedlist_pushHead);
    assert_true(RawLinkedlist.pushTail == RawLinkedlist_pushTail);
    assert_true(RawLinkedlist.remove == RawLinkedlist_remove);

    assert_true(RawLinkedlist.traverseFromHead == RawLinkedlist_traverseForward);
    assert_true(RawLinkedlist.traverseFromTail == RawLinkedlist_traverseBackward);
    assert_true(RawLinkedlist.traverseUnordered == RawLinkedlist_traverseUnordered);
    assert_true(RawLinkedlist.swap == RawLinkedlist_swap);
}

void test_create (void ** state)
{
    PRawLinkedlist list = RawLinkedlist.create(8);
    assert_true(list);

    assert_int_not_equal(list->$capacity, 0);
    assert_int_not_equal(list->$nextGrow, 0);
    assert_int_not_equal(list->$nextShrink, 0);
    assert_true(!list->$resizing);
    assert_int_not_equal(list->content, 0);
    assert_int_not_equal(list->entrySize, 0);
    assert_int_not_equal(list->headIx, 0);
    assert_int_equal(list->length, 0);
    assert_int_not_equal(list->nodeMap, 0);
    assert_int_not_equal(list->tailIx, 0);

    RawLinkedlist.free(&list);
    assert_true(!list);
}

void test_pushHead (void ** state)
{
    u64 value = 99;
    PRawLinkedlist list = RawLinkedlist.create(8);
    RawLinkedlist.pushHead(list, &value);
    assert_true(list->length == 1);
    RawLinkedlist.pushHead(list, &value);
    assert_true(list->length == 2);

    RawLinkedlist.free(&list);
}

void test_pushGet (void ** state)
{
    u64 value = 99;
    PRawLinkedlist list = RawLinkedlist.create(8);
    RawLinkedlist.pushHead(list, &value);
    assert_true(list->length == 1);
    Pointer res = RawLinkedlist.get(list, 0);
    assert_true(res);
    assert_true(*(u64*)res == value);

    RawLinkedlist.free(&list);
}

void test_pushTail (void ** state)
{
    u64 value1 = 99;
    u64 value2 = 83;
    PRawLinkedlist list = RawLinkedlist.create(8);
    RawLinkedlist.pushTail(list, &value1);
    assert_true(list->length == 1);
    RawLinkedlist.pushTail(list, &value2);
    assert_true(list->length == 2);

    Pointer res = RawLinkedlist.get(list, 0);
    assert_true(*(u64*)res == value1);
    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value2);

    RawLinkedlist.free(&list);
}

void test_popTail (void ** state)
{
    u64 value1 = 99;
    u64 value2 = 83;
    PRawLinkedlist list = RawLinkedlist.create(8);
    RawLinkedlist.pushHead(list, &value1);
    assert_true(list->length == 1);
    RawLinkedlist.pushHead(list, &value2);
    assert_true(list->length == 2);

    Pointer res = RawLinkedlist.get(list, 0);
    assert_true(*(u64*)res == value2);
    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value1);

    res = RawLinkedlist.popTail(list);
    assert_true(*(u64*)res == value1);
    res = RawLinkedlist.popTail(list);
    assert_true(*(u64*)res == value2);

    assert_int_equal(list->headIx, -1);
    assert_int_equal(list->tailIx, -1);
    assert_int_equal(list->length, 0);

    // re-run with push tail
    RawLinkedlist.pushTail(list, &value1);
    assert_true(list->length == 1);
    RawLinkedlist.pushTail(list, &value2);
    assert_true(list->length == 2);

    res = RawLinkedlist.get(list, 0);
    assert_true(*(u64*)res == value1);
    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value2);

    res = RawLinkedlist.popTail(list);
    assert_true(*(u64*)res == value2);
    res = RawLinkedlist.popTail(list);
    assert_true(*(u64*)res == value1);

    assert_int_equal(list->headIx, -1);
    assert_int_equal(list->tailIx, -1);
    assert_int_equal(list->length, 0);

    RawLinkedlist.free(&list);    
}

void test_popHead (void ** state)
{
    u64 value1 = 99;
    u64 value2 = 83;
    PRawLinkedlist list = RawLinkedlist.create(8);
    RawLinkedlist.pushHead(list, &value1);
    assert_true(list->length == 1);
    RawLinkedlist.pushHead(list, &value2);
    assert_true(list->length == 2);

    Pointer res = RawLinkedlist.get(list, 0);
    assert_true(*(u64*)res == value2);
    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value1);

    res = RawLinkedlist.popHead(list);
    assert_true(*(u64*)res == value2);
    res = RawLinkedlist.popHead(list);
    assert_true(*(u64*)res == value1);

    assert_int_equal(list->headIx, -1);
    assert_int_equal(list->tailIx, -1);
    assert_int_equal(list->length, 0);

    // re-run
    RawLinkedlist.pushTail(list, &value1);
    assert_true(list->length == 1);
    RawLinkedlist.pushTail(list, &value2);
    assert_true(list->length == 2);

    res = RawLinkedlist.get(list, 0);
    assert_true(*(u64*)res == value1);
    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value2);

    res = RawLinkedlist.popHead(list);
    assert_true(*(u64*)res == value1);
    res = RawLinkedlist.popHead(list);
    assert_true(*(u64*)res == value2);

    assert_int_equal(list->headIx, -1);
    assert_int_equal(list->tailIx, -1);
    assert_int_equal(list->length, 0);    

    RawLinkedlist.free(&list);
}

void test_remove (void ** state)
{
    u64 value1 = 99;
    u64 value2 = 83;
    PRawLinkedlist list = RawLinkedlist.create(8);
    RawLinkedlist.pushHead(list, &value1);
    assert_true(list->length == 1);
    RawLinkedlist.pushHead(list, &value2);
    assert_true(list->length == 2);

    Pointer res = RawLinkedlist.get(list, 0);
    assert_true(*(u64*)res == value2);
    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value1);

    RawLinkedlist.remove(list, 1);
    assert_int_not_equal(list->headIx, -1);
    assert_int_not_equal(list->tailIx, -1);
    assert_int_equal(list->length, 1);

    res = RawLinkedlist.get(list, 1);
    assert_true(*(u64*)res == value2);

    RawLinkedlist.remove(list, 0);
    assert_int_equal(list->headIx, -1);
    assert_int_equal(list->tailIx, -1);
    assert_int_equal(list->length, 0);

    RawLinkedlist.free(&list);

}

void test_resize (void ** state) {
    PRawLinkedlist list = RawLinkedlist.create(4);
    for (int i = 0; i < 100; i++) {
        RawLinkedlist.pushTail(list, &i);
    }

    assert_int_equal(list->headIx, 0);
    assert_int_equal(list->$capacity, 128);
    assert_int_equal(list->length, 100);

    RawLinkedlist.free(&list);
}

void myTraverse(PRawLinkedlist list, int* other, int* value) {
    // printDebug("myTraverse %p, %i %i ", list, *other, *value);
    *other = *value; 
}

void fn_swap$(PRawLinkedlist list, int a, int b)
{
    int* entryA = RawLinkedlist_get(list, a);
    int* entryB = RawLinkedlist_get(list, b);

    // printDebug("swap %i %i ", a, b);

    int tmp = *entryA;
    *entryA = *entryB;
    *entryB = tmp;
}

int fn_compare$(PRawLinkedlist list, const int a, const int b)
{
    int* entryA = RawLinkedlist_get(list, a);
    int* entryB = RawLinkedlist_get(list, b);

    // printDebug("compare %i %i ", a, b);

    return *entryA - *entryB;
}

void test_traverse (void ** state) {
    PRawLinkedlist list = RawLinkedlist.create(4);
    for (int i = 0; i < 5; i++) {
        RawLinkedlist.pushTail(list, &i);
    }

    int res = 3;
    RawLinkedlist.traverseUnordered(list, &res, myTraverse);
    assert_int_equal(res, 4);

    RawLinkedlist.free(&list);
}

void test_fromHead (void ** state) {
    PRawLinkedlist list = RawLinkedlist.create(4);
    for (int i = 0; i < 5; i++) {
        RawLinkedlist.pushTail(list, &i);
    }

    int res = 5;
    RawLinkedlist.traverseFromHead(list, &res, myTraverse);
    assert_int_equal(res, 4);

    RawLinkedlist.free(&list);
}

void test_fromTail (void ** state) {
    PRawLinkedlist list = RawLinkedlist.create(4);
    for (int i = 0; i < 5; i++) {
        RawLinkedlist.pushTail(list, &i);
    }

    int res = 10;
    RawLinkedlist.traverseFromTail(list, &res, myTraverse);
    assert_int_equal(res, 0);

    RawLinkedlist.free(&list);
}

void test_sort (void ** state) {
    PRawLinkedlist list = RawLinkedlist.create(4);
    for (int i = 0; i < 500; i++) {
        RawLinkedlist.pushTail(list, &i);
    }

    int res = 10;
    RawLinkedlist.traverseFromTail(list, &res, myTraverse);
    assert_int_equal(res, 0);

    quicksort(list, 0, list->length - 1, descending, fn_compare$, fn_swap$);

    res = 10;

    RawLinkedlist.traverseFromTail(list, &res, myTraverse);
    assert_int_equal(res, 499);

    RawLinkedlist.free(&list);
}



/* These functions will be used to initialize
   and clean resources up after each test run */
int setup (void ** state)
{
    return 0;
}

int teardown (void ** state)
{
    return 0;
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_function_mapping),
        cmocka_unit_test (test_create),
        cmocka_unit_test (test_pushHead),
        cmocka_unit_test (test_pushGet),
        cmocka_unit_test (test_pushTail),
        cmocka_unit_test (test_popHead),
        cmocka_unit_test (test_popTail),
        cmocka_unit_test (test_remove),
        
        cmocka_unit_test (test_traverse),
        cmocka_unit_test (test_fromHead),
        cmocka_unit_test (test_fromTail),
        cmocka_unit_test (test_sort),
        cmocka_unit_test (test_resize),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests =
        cmocka_run_group_tests (tests, setup, teardown);

    return count_fail_tests;
}
