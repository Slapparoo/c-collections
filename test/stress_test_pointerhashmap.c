#include "collections_bypointer.h"
#include <assert.h>




i32 my_compare(u64 a, u64 b) {
    return a - b;
}

i32 my_hash(u64 a) {
    return FAST_HASH(a);
}


int main(int argc, char const *argv[])
{


#define  TOTAL 20000000
#define  LOOP 2500000
#define  ITERATIONS TOTAL / LOOP

printDebug("loop %i, %i", LOOP, ITERATIONS);
    
    PPointerHashmap map = PointerHashmap_create(&my_hash, &my_compare);
    for (int j = 0; j < ITERATIONS; j++) {
        for (u64 i = 1; i < LOOP; i++) {
            PointerMapEntry entry = {(Pointer)i, (Pointer)i};
            PointerHashmap_putEntry(map, &entry);
            u64 res = (u64)PointerHashmap_get(map, (Pointer)i);
            // assert(res);
            // assert(res == i);
        }

        for (u64 i = 1; i < LOOP; i++) {
            PointerHashmap_deleteEntry(map, (Pointer)i);
            u64 res = (u64)PointerHashmap_get(map, (Pointer)i);
            assert(!res);
        }
        assert(!map->length);
    }

    PointerHashmap_free(&map);

    return 0;
}
