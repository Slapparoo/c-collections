#include "valuetree.h"
#include <assert.h>


Fn_traversal(Stress_test, Pointer,  i64 *) {
    printDebug("travrse %li", *value);
}


int main(int argc, char const *argv[])
{
    PValueTree tree = ValueTree_create(8, &compare$64);

    // u64 i = 10;
    // u64 * value = ValueTree_insert(tree, &i);
    // ValueTree_deleteNode(tree, &i);
    // i32 ix = ValueTree_find(tree, &i);

// #define SIZE 20000
#define SIZE 17080



    for (i64 i = 1; i < SIZE; i++) {
        if (hash$64(&i) == 1493189302) {
            printInfo("match %u, %lu", hash$64(&i), i);
        }
        i64 val = hash$64(&i);
        // i64 val = i;
        u64 * value = ValueTree_addEntry(tree, &val);
        assert(*value == val);
        u64* res = ValueTree_find(tree, &val);
        assert(res);
        assert(*res == val);
    }
    
    u32 len = tree->length;
    u64 n = 17078;
    i64 v = hash$64(&n);

    u64* res1 = ValueTree_find(tree, &v);

        // if (!res1) {
            printError("v %lu, %u %lu", n, len, v);
        // }

 printInfo("hash %u, %u", hash$64(&n), len);
    for (i64 i = 1; i < SIZE; i++) {
        i64 val = hash$64(&i);
        // i64 val = i;
        u64* res1 = ValueTree_find(tree, &val);

        if (i == 17077) {
            // ValueTree_traverse(tree, null, &Stress_test_traversal);
            res1 = ValueTree_find(tree, &v);
        }

        if (!res1) {
            printError("length %lu, %u %lu", val, len, i);
            ValueTree_traverse(tree, null, &Stress_test_traversal);
            ValueTree_find(tree, &val);
        }
        assert(res1);

        ValueTree_deleteEntry(tree, &val);
        u64* res = ValueTree_find(tree, &val);
        --len;

// printInfo("length  %u, %u", tree->length, len);
        if (tree->length != len)  {
            printError("length  %u, %u", tree->length, len);
        }
        
        // assert(tree->length == len);

        if (res) {
            printError("ValueTree_find %li, %li", val, i);
        }

        assert(!res);
    }
    
    return 0;
}
