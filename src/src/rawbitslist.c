#include "rawbitslist.h"
#include "data-types.h"
#include <popcntintrin.h>
#include <x86intrin.h>

#define set_bit$32(A, k) (A[(k / 32)] |= (1 << (k % 32)))
#define clear_bit$32(A, k) (A[(k / 32)] &= ~(1 << (k % 32)))
#define is_bit$32(A, k) (A[(k / 32)] & (1 << (k % 32)))
#define array_length$32(k) (k / 32) + ((k % 32) > 0 ? 1 : 0)
#define get_bit_count$32(x) _mm_popcnt_u32(x)
#define get_bit_leadingZeros$32(x) _tzcnt_u32(x)
#define get_int_offset$32(k) (k / 32)


PRawBitslist RawBitslist_create(const u32 size, const boolean initial) {
    PRawBitslist res = Pointers.malloc(sizeof(RawBitslist_data));
    RawBitslist_init(res, size, initial);
    return res;
}

void RawBitslist_init(const PRawBitslist this, const u32 size, const boolean initial) {
    this->$capacity = array_length$32(size);
    this->length = size;
    this->content = Pointers.malloc(this->$capacity  * 4);
    RawBitslist_setAll(this, initial);
}

boolean RawBitslist_isBit(const PRawBitslist this, const u32 index) {
    return is_bit$32(this->content, index);
}

void RawBitslist_setBit(const PRawBitslist this, const u32 index) {
    set_bit$32(this->content, index);
}

void RawBitslist_clearBit(const PRawBitslist this, const u32 index) {
    clear_bit$32(this->content, index);
}

void RawBitslist_set(const PRawBitslist this, const u32 index, const boolean value) {
    if (value) {
        set_bit$32(this->content, index);
    } else {
        clear_bit$32(this->content, index);
    }
}

i32 RawBitslist_findFirst(const PRawBitslist this) {
  int res = 0;
  for (int i = 0; i < this->$capacity; i++) {
    if (this->content[i])  {
        int offset = get_bit_leadingZeros$32(this->content[i]);
        if (offset < 32) {
            return res + offset;
        }
    }
    res += 32;
  }
  return -1;
}

void RawBitslist_setAll(PRawBitslist this, boolean value) {
    if (value) {
        for (int i = 0; i < this->$capacity; i++) {
            this->content[i] = 0xffffffff;
        }
    } else {
        for (int i = 0; i < this->$capacity; i++) {
            this->content[i] = 0;
        }
    }
}

u32 RawBitslist_getBitCount(const PRawBitslist this) {
  u32 res = 0;  
  
  for (int i = 0; i < this->$capacity; i++) {
      u32 val = this->content[i];
    res += get_bit_count$32(val);
  }
  // @todo fixme, counts past the end of length

  return res;
}


void RawBitslist_free(PRawBitslist* this) {
    RawBitslist_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

void RawBitslist_freeChildren(const PRawBitslist this) {
    Pointers.free(this->content);
}

struct RawBitslist_code RawBitslist = {
    &RawBitslist_create,
    &RawBitslist_init,
    &RawBitslist_isBit,
    &RawBitslist_setBit,
    &RawBitslist_clearBit,
    &RawBitslist_set,
    &RawBitslist_findFirst,
    &RawBitslist_setAll,
    &RawBitslist_getBitCount,
    &RawBitslist_free,
    &RawBitslist_freeChildren
};