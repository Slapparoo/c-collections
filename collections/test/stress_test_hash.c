#include "rawhashmap.h"
#include <assert.h>


i32 myhash$32(u32* a) {
    return *a;
}

int main(int argc, char const *argv[])
{
#define  ITERATIONS 20000000
    PRawHashmap hashmap = RawHashmap.create(8, &hash$32, &compare$32);
    for (u64 i = 1; i < ITERATIONS; i++) {
        RawHashmap.put(hashmap, &i);
        u64* res = RawHashmap.get(hashmap, &i);
        assert(res);
        assert(*res == i);
    }

    for (u64 i = 1; i < ITERATIONS; i++) {
        RawHashmap.remove(hashmap, &i);
        u64* res = RawHashmap.get(hashmap, &i);
        assert(!res);
    }

    return 0;
}
