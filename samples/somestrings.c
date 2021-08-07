#include "collections.h"


Fn_hashcode(my) {
    return hash$PChar(value);
}

int main(int argc, char const *argv[])
{
    PChar key1 = "key1";
    PChar value1 = "value1";

    PPointerHashmap hashmap = PointerHashmap_create(&my_hashcode, &compare$PChar);
    PointerHashmap_put(hashmap, key1, value1);

    printDebug("the value for key1 is %s", PointerHashmap_get(hashmap, "key1"));


    return 0;
}
