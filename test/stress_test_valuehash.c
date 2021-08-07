#include "collections_byvalue.h"
#include <assert.h>


i32 myhash$32(u32* a) {
    return *a;
}

int main(int argc, char const *argv[])
{

#define  ITERATIONS 20000000
    PValueHashmap hashmap = ValueHashmap_create(8, &hash$32, &compare$32);
    for (u64 i = 1; i < ITERATIONS; i++) {
        ValueHashmap_putEntry(hashmap, &i);
        u64* res = ValueHashmap_get(hashmap, &i);
        assert(res);
        assert(*res == i);
    }

    for (u64 i = 1; i < ITERATIONS; i++) {
        ValueHashmap_deleteEntry(hashmap, &i);
        u64* res = ValueHashmap_get(hashmap, &i);
        assert(!res);
    }

    ValueHashmap_free(&hashmap);

    return 0;
}
