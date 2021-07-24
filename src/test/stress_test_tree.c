#include "rawtree.h"
#include <assert.h>


int main(int argc, char const *argv[])
{
    PRawTree tree = RawTree_create(8, &compare$32);

    // u64 i = 10;
    // u64 * value = RawTree_insert(tree, &i);
    // RawTree_deleteNode(tree, &i);
    // i32 ix = RawTree_find(tree, &i);

#define SIZE 200000

    for (i64 i = 1; i < SIZE; i++) {
        i64 val = hash$64(&i);
        // i64 val = i;
        u64 * value = RawTree_insert(tree, &val);
        assert(*value = val);
        u64* res = RawTree_get(tree, RawTree_find(tree, &val));
        assert(res);
        assert(*res == val);
    }
    
    
    for (i64 i = 1; i < SIZE; i++) {
        i64 val = hash$64(&i);
        // i64 val = i;
        RawTree_deleteNode(tree, &val);
        i32 ix = RawTree_find(tree, &val);

        if (ix != -1) {
            printError("RawTree_find %li, %li, %i", val, i, ix);
        }

        assert(ix == -1);
    }
    
    return 0;
}
