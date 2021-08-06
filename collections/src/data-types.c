#include "collection_base.h"


PrimativeType primativeTypess[] = {
    // missing wchar and quad float
    {sizeof(boolean), false, false, "%i", "boolean", "bool, b8, _Bool", &hash$Boolean, &compare$8, &copy$Boolean, 1, 0},
    {sizeof(u8), false, false, "%hhu", "u8", "unsigned char", &hash$8, &compare$8, &copy$8, UCHAR_MAX, 0},
    {sizeof(i8), true, false, "%hhd", "i8", "signed char", &hash$8, &compare$8, &copy$8, CHAR_MAX, CHAR_MIN},
    {sizeof(char), false, false, "%c", "char", "char", &hash$8, &compare$8, &copy$8, CHAR_MAX, CHAR_MIN},
    {sizeof(u16), false, false, "%u", "u16", "unsigned short", &hash$16, &compare$16, &copy$16, USHRT_MAX, 0},
    {sizeof(i16), true, false, "%d", "i16", "short, short int, signed short, signed short int", &hash$16, &compare$16, &copy$16, SHRT_MAX, SHRT_MIN},
    {sizeof(u32), false, false, "%u", "u32", "unsigned, unsigned int", &hash$32, &compare$32, &copy$32, UINT_MAX, 0},
    {sizeof(i32), true, false, "%d", "i32", "int, signed, signed int", &hash$32, &compare$32, &copy$32, INT_MAX, INT_MIN},
    {sizeof(f32), true, true, "%f", "f32", "float", &hash$F32, &compare$F32, &copy$F32, (u64)FLT_MAX, (u64)FLT_MIN}, 
    {sizeof(u64), false, false, "%lu", "u64", "unsigned long, unsigned long int", &hash$64, &compare$64, &copy$64, ULONG_MAX, 0},
    {sizeof(i64), true, false, "%ld", "i64", "long, long int, signed long, signed long int", &hash$64, &compare$64, &copy$64, LONG_MAX, LONG_MIN},
    {sizeof(f64), true, true, "%f", "f64", "double", &hash$F64, &compare$F64, &copy$F64, (u64)DBL_MAX, (u64)DBL_MIN},
    {sizeof(Pointer), false, false, "%p", "Pointer", "(void*)", &hash$64, &compare$64, &copy$64, ULONG_MAX, 0},
#ifdef __SUPPORT_QUADMATH_    
// @todo fix
    {sizeof(f128), true, true, "%s", "f128", "__float128"},
    {sizeof(f80), true, true, "%s", "f80", "long double"},
#endif    
    // todo the set and get for PChar may not be great
    {sizeof(PChar), false, false, "%s", "PChar", "char*", &hash$PChar, &strcmp, &copy$64 , ULONG_MAX, 0}
};

PrimativeType* primativeTypes = &primativeTypess;

int printPrimativeType(PrimativeType* pt) {
    if (pt->isFloat) {
        return printf("name:%s, bytes:%i, format:%s, float, isSigned:%i otherNames:%s max:%f min:%f\n", 
        pt->name, pt->length, pt->format, pt->isSigned, pt->otherNames, (f64)pt->max, (f64)pt->min);
    } else if (pt->isSigned) {
        return printf("name:%s, bytes:%i, format:%s, signed otherNames:%s max:%li min:%li\n", 
        pt->name, pt->length, pt->format, pt->otherNames, pt->max, pt->min);
    } else {
        return printf("name:%s, bytes:%i, format:%s, unsigned otherNames:%s max:%lu min:%lu\n", 
        pt->name, pt->length, pt->format, pt->otherNames, pt->max, pt->min);
    }
}

int printPrimativeTypeByNum(int type) {
    printPrimativeType(&primativeTypess[type]);
}

i32 hash$Pointer(Pointer* val) {
    return FAST_HASH(*(u64*)val);
}

i32 hash$64(i64* val) {
    return FAST_HASH(*val);
}

i32 hash$$PChar(PChar str) {
    int res = 0;
    int i = 0;
    // printf("%s\n", str);
    while (str[i]) {
        res ^= (str[i] * 31) * (str[i] * (i + 1));
        // printf("%c", str[i]);
        i++;
    }

    return res;
}


i32 hash$PChar(PChar* str) {
    int res = 0;
    int i = 0;
    PChar st = *str;
    // printf("%s\n", st);
    while (st[i]) {
        res ^= (st[i] * 31) * (st[i] * (i + 31));
        i++;
    }

    return hash$32(&res);
}

inline i32 compare$Boolean(boolean* a, boolean* b) {
    if (*a == *b) {
        return 0;
    } else if (*a) {
        return 1;
    }   
    return -1;
}


inline i32 compare$8(u8* a, u8* b) {
    return *a - *b;
}

inline i32 compare$16(u16* a, u16* b) {
    return *a - *b;
}

inline i32 compare$32(u32* a, u32* b) {
    return *a - *b;
}

inline i32 compare$F32(f32* a, f32* b) {
    return *a - *b;
}

inline i32 compare$F64(f64* a, f64* b) {
    return *a - *b;
}

inline i32 compare$64(u64* a, u64* b) {
    return *a - *b;
}

inline i32 hash$Boolean(boolean* b) {
    return *b == true ? 668874480 : 892731278;
}

inline i32 compare$Pointer(Pointer* a, Pointer* b) {
    if (*a == *b) {
        return 0;
    }
    if (*a == null) {
        return -1;
    }
    if (*b == null) {
        return 1;
    }
    return *(Pointer*)a - *(Pointer*)b;

}

inline i32 compare$PChar(PChar a, PChar b) {
    if (a == b) {
        return 0;
    }
    if (a == null) {
        return -1;
    }
    if (b == null) {
        return 1;
    }
    return strcmp(a, b);

}


inline i32 hash$8(u8* value) {
    return FAST_HASH(*value * 256);
}

inline i32 hash$16(u16* value) {
    return FAST_HASH(*value * 256);    
}

inline i32 hash$32(u32* value) {
    return FAST_HASH(*value);
}

inline i32 hash$F32(i32* value) {
    return FAST_HASH(*value);
}

inline i32 hash$F64(i64* value) {
    return FAST_HASH(*value);
}



/**
 * @brief return a fast checksum, which should provide uniqueness, but won't have the distribution 
 * of a hashcode
 * 
 * @param string 
 * @return i64 
 */
i32 pCharToI64(PChar string) {
    return arrayToI64(string, strlen(string));
}

/**
 * @brief return a fast checksum, which should provide uniqueness, but won't have the distribution 
 * of a hashcode
 * 
 * @param array 
 * @param length 
 * @return i64 
 */
i32 arrayToI64(Pointer array, u64 length) {
    Pointer p = array;
    // u64 index = length;
    i32 result = 0;

    while (length > 0) {
        switch (length) {
            case 1:
                result ^= *(i8*)p;
                length -= 1;
                break;
            case 2:
                result ^= *(i16*)p;
                length -= 2;
                break;
            case 3:
                p += 2;
                result ^= *(i16*)p;
                length -= 2;
                break;
            default: {
                result ^= *(i32*)p;
                length -= 4;
                break;
            }
        }
    }
    return result;
}

void copy$Boolean(boolean* p, const boolean* val) {
    *p = *val;
}
void copy$8(u8* p, const u8* val) {
    *p = *val;
}
void copy$16(u16* p, const u16* val) {
    *p = *val;
}
void copy$32(u32* p, const u32* val) {
    *p = *val;
}
void copy$F32(f32* p, const f32* val) {
    *p = *val;
}
void copy$64(u64* p, const u64* val) {
    *p = *val;
}
void copy$F64(f64* p, const f64* val) {
    *p = *val;
}

// #define TEST_DATATYPES 
#ifdef TEST_DATATYPES


#endif

