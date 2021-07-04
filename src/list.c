#include "list.h"


PChar List_classname() {
    return "List";
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param list 
 * @param initialSize 
 * @param itemSize 
 */
void List_init(List_data *list, u64 initialSize, u64 itemSize) {

    list->classMethods = &List;
    list->instanceName = List_data_name;
    list->capacity = max(initialSize, List_Growby);
    list->items = Pointers.malloc(list->capacity * itemSize);
    list->length = 0;
    list->itemSize = itemSize;

    printDebug("init list length:%ld, capacity:%ld, items:%p, itemSize:%ld", list->length, list->capacity, list->items, list->itemSize);
}

List_data * List_create(u64 initialSize, u64 itemSize) {
    List_data *list = Pointers.malloc(sizeof(List_data)); 
    List_init(list, initialSize, itemSize);
    return list;
}

void List_grow(List_data *list) {
    list->capacity += List_Growby;
    list->items = realloc(list->items, list->capacity * list->itemSize);
    printDebug("grow list length:%ld, capacity:%ld, items:%p ", list->length, list->capacity, list->items);
}

void List_shrink(List_data *list) {
    list->capacity -= List_Growby;
    list->items = realloc(list->items, list->capacity * list->itemSize);
    printDebug("shrink list length:%ld, capacity:%ld, items:%p ", list->length, list->capacity, list->items);
}

/**
 * @brief Add an item to the list resizing as required, return a pointer to the tip
 * 
 * @param list 
 * @param element 
 */
Pointer List_add(List_data *list) {
    if (list->length == list->capacity) {
        List_grow(list);
    }
    Pointer res = list->items + (list->length++ * list->itemSize);
    return res;
}

Pointer List_value(const List_data *list, u64 index) {
    // @todo maybe blowup here
    if (index > list->length) {
        return null;
    }
    return list->items + (index * list->itemSize);
}

Pointer List_remove(List_data *list) {
    if (list->length + List_Growby == list->capacity) {
        List_shrink(list);
    }
    return list->items + (--list->length * list->itemSize);
}

void List_clear(List_data *list) {
    list->capacity = List_Growby;
    list->items = Pointers.malloc(list->capacity * list->itemSize);
    list->length = 0;

    Pointers.setAll(list->items, 0, list->capacity * list->itemSize);
    printDebug("clear list length:%ld, capacity:%ld", list->length, list->capacity);
}

void List_freeChildren(const List_data * list) {
    printDebug("freeChildren list length:%ld, capacity:%ld", list->length, list->capacity);
    free(list->items);
}

void List_free(List_data ** list) {
    printDebug("free list length:%ld, capacity:%ld", (*list)->length, (*list)->capacity);
    List_freeChildren(*list);
    BaseObject.free(list);
}

void List_sort(const List_data * list, int (*compar)(const Pointer, const Pointer)) {
    System.qsort(list->items, list->length, list->itemSize, compar);
}

Pointer List_search(const List_data * list, const Pointer key, int (*compar)(const Pointer, const Pointer)) {
    return System.bsearch(key, list->items, list->length, list->itemSize, compar);
}

 struct List_code List = {
    &BaseObject,
    &List_classname,
    &BaseObject_hashCode,
    &BaseObject_equals,
    &BaseObject_toString,
    &List_free,
    &List_freeChildren,

    &List_create,
    &List_init,
    &List_add,
    &List_value,
    &List_remove,
    &List_clear,
    &List_sort,
    &List_search
};

int List_PPChar_compare(const Pointer a, const Pointer b) {
    return PStrings.compare(*(PChar*)a, *(PChar*)b);
}

#ifdef TEST_LIST

#include <assert.h>

typedef struct TestStruct {
    i64 n1;
    i64 n2;
    i64 n3;
} TestStruct;


int main(int argc, char const *argv[])
{

    PChar name = "bob";
    PChar mark = "Mark";
    PChar rob = "Rob";
    PChar bob = "bob";
    PChar aname = "A long name";

    List_data * pCharList = List.create(3, sizeof(Pointer));
    assert(pCharList->capacity);
    assert(pCharList->classMethods);
    assert(pCharList->instanceName);
    assert(pCharList->items);
    assert(pCharList->itemSize);
    assert(pCharList->itemSize == sizeof(Pointer));
    assert(pCharList->length == 0);


//#define ListRemove(list, value ) *(Pointer*)List.remove(list) = value;    
    ListAdd(pCharList, "brook")
    ListAdd(pCharList, "dave")
    *(PChar*)List.add(pCharList) = name;
    *(PChar*)List.add(pCharList) = aname;
    *(PChar*)List.add(pCharList) = mark;
    *(PChar*)List.add(pCharList) = rob;
    *(PChar*)List.add(pCharList) = "brook";
    *(PChar*)List.add(pCharList) = "dave";
    *(PChar*)List.add(pCharList) = "bob";
    *(PChar*)List.add(pCharList) = "John";
    *(PChar*)List.add(pCharList) = mark;
    *(PChar*)List.add(pCharList) = rob;

    assert(pCharList->length == 12);

    Console.println("for 1");

    for (u64 i = 0; i < pCharList->length; i++) {
        Console.format("item %d, %p\n", i, *(PChar*)List.value(pCharList, i));
    }

    Console.println("for 2");

    for (u64 i = 0; i < pCharList->length; i++) {
        Console.format("item %d, %s\n", i, *(PChar*)List.value(pCharList, i));
    }
   
    Console.println("sort for 3");
    List.sort(pCharList, &List_PPChar_compare);

    for (u64 i = 0; i < pCharList->length; i++) {
        Console.format("item %d, %p, %s\n", i, *(PChar*)List.value(pCharList, i),  *(PChar*)List.value(pCharList, i));
    }

    Console.format("Looking for %s\n", *(PChar*)List.value(pCharList, 5));

    PChar found = List.search(pCharList, List.value(pCharList, 5), &List_PPChar_compare);

    assert(found);
    if (found) {
        Console.format("Found : %s\n", *(PChar*)found);    
    } else {
        Console.format("Not found\n");    
    }

    Console.format("Looking for %p\n", name);
    found = List.search(pCharList, &name, PStrings.compare);

    assert(found);
    if (found) {
        Console.format("Found : %s\n", *(PChar*)found);    
    } else {
        Console.format("Not found\n");    
    }

    for (int i = 0; i < 5; i++) {
        Console.format("remove %s\n", *(Pointer*)List.remove(pCharList));
    }
    assert(pCharList->length == 7);
    PChar res = List.remove(pCharList);
    Console.format("removed %s %s %p %p\n", *(Pointer*)res, bob, res, &bob);
    assert(*(Pointer*)res == bob);

    Console.println("ListCode.toString(nameList)");
    Console.println(List.toString(pCharList));

    Console.println("classname");
    Console.println(pCharList->instanceName);
    assert(PStrings.compare(pCharList->instanceName, "List"));
    
    Console.format("classname %p %p %p\n", pCharList->classMethods, pCharList->classMethods->className, &List);
    Console.println(pCharList->classMethods->className());
    Console.println(List.className());
    Console.println(((BaseObject_code*)List.parentClass)->className());


    List.clear(pCharList);
    assert(pCharList->capacity);
    assert(pCharList->classMethods);
    assert(pCharList->instanceName);
    assert(pCharList->items);
    assert(pCharList->itemSize);
    assert(pCharList->length == 0);


    List.free(&pCharList);
    assert(pCharList == 0);

    Console.format("namelist @%p\n", pCharList);

// *******************
    List_data * u8List = List.create(3, sizeof(u8));
    Console.format("u8List @%p\n", u8List);

    assert(u8List->capacity);
    assert(u8List->classMethods);
    assert(u8List->instanceName);
    assert(u8List->items);
    assert(u8List->itemSize);
    assert(u8List->itemSize == sizeof(u8));
    assert(u8List->length == 0);

    *(u8*)List.add(u8List) = 31;
    *(u8*)List.add(u8List) = 41;
    *(u8*)List.add(u8List) = 51;

    Console.println("u8 for 1");

    for (u64 i = 0; i < u8List->length; i++) {
        Console.format("item %i, %i\n", i, *(u8*)List.value(u8List, i));
    }

    // List.free(&u8List);

// *******************
    // array of stuff
    List_data * testStructList = List.create(3, sizeof(TestStruct));
    Console.format("TestStructList @%p\n", testStructList);

    assert(testStructList->capacity);
    assert(testStructList->classMethods);
    assert(testStructList->instanceName);
    assert(testStructList->items);
    assert(testStructList->itemSize);
    assert(testStructList->itemSize == sizeof(TestStruct));
    assert(testStructList->length == 0);

    TestStruct * testStruct = (TestStruct*)List.add(testStructList);
    testStruct->n1 = 13;
    testStruct->n2 = 17;
    testStruct->n3 = 53;

    ((TestStruct*)List.value(testStructList, 0))->n1 = 97;

    TestStruct * testStructRes = (TestStruct*)List.remove(testStructList);

    Console.format("%li, %li, %li\n", testStructRes->n1, testStructRes->n2, testStructRes->n3);

    List.free(&testStructList);



    return 0;
}
#endif
