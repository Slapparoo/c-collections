#include "rawtree.h"
#include <assert.h>


int main(int argc, char const *argv[])
{
    PRawTree tree = RawTree_create(8, &compare$32);

    // u64 i = 10;
    // u64 * value = RawTree_insert(tree, &i);
    // RawTree_deleteNode(tree, &i);
    // i32 ix = RawTree_find(tree, &i);

    for (i64 i = 1; i < 2000; i++) {
        i64 val = hash$64(&i);
        u64 * value = RawTree_insert(tree, &val);
        assert(*value = val);
        u64* res = RawTree_get(tree, RawTree_find(tree, &val));
        assert(res);
        assert(*res == val);
    }

    for (i64 i = 1; i < 2000; i++) {
        i64 val = hash$64(&i);
        RawTree_deleteNode(tree, &val);
        i32 ix = RawTree_find(tree, &val);
        assert(ix == -1);
    }
    
    return 0;
}
