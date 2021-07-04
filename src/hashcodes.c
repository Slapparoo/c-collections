#include "hashcodes.h"

typedef char* PChar;

i64 hashPointer(Pointer valp) {
    return hash64((i64)valp);
}

i64 hash64(i64 val) {
    return val ^ (val << 5) * (val << 11) ^ (val << 13) ^ (val << 31) ^ (val << 43);
}

i64 hashPChar(PChar str) {
    int res = 0;
    int i = 0;
    while (str[i] != 0) {
        res += str[i]*31^ str[i];
        i++;
    }

    return hash64(res);
}

i64 hashBoolean(boolean b) {
    return b == true ? 668874480 : 892731278;
}


#ifdef TEST_HASHES

int main(int argc, char const *argv[])
{
    printf("hashBoolean true %li\n", hashBoolean(true));
    printf("hashBoolean false %li\n", hashBoolean(false));

    printf("hashInt 0 %li\n", hash64(0));
    printf("hashInt 1 %li\n", hash64(1));
    printf("hashInt 100 %li\n", hash64(100));

    printf("hashInt 100 %li\n", hash64(100l));

    printf("hashPointer str %li\n", hashPointer("100l"));


    printf("shift 5 %i\n", 100 << 5);
    printf("shift 11 %i\n", 100 << 11);
    printf("shift 13 %i\n", 100 << 13);

    printf("shift 5 %i\n", 100 >> 5);
    printf("shift 11 %i\n", 100 >> 11);
    printf("shift 13 %i\n", 100 >> 13);

    return 0;
}


#endif
