#include "bitslist.h"
#include "collection_base.h"
#include <popcntintrin.h>
#include <x86intrin.h>

#define set_bit$32(A, k) (A[(k / 32)] |= (1 << (k % 32)))
#define clear_bit$32(A, k) (A[(k / 32)] &= ~(1 << (k % 32)))
#define is_bit$32(A, k) (A[(k / 32)] & (1 << (k % 32)))
#define array_length$32(k) (k / 32) + ((k % 32) > 0 ? 1 : 0)
#define get_bit_count$32(x) _mm_popcnt_u32(x)
#define get_bit_leadingZeros$32(x) _tzcnt_u32(x)
#define get_int_offset$32(k) (k / 32)

implementCleanup(Bitslist);

#define $values this->$values
#define length this->length
#define $capacity this->$capacity


PBitslist Bitslist_create(const u32 size, const boolean initial) {
    PBitslist res = Pointers.malloc(sizeof(Bitslist_data));
    Bitslist_init(res, size, initial);
    return res;
}

void Bitslist_init(const PBitslist this, const u32 size, const boolean initial) {
    $capacity = array_length$32(size);
    length = size;
    $values = Pointers.malloc($capacity  * 4);
    Bitslist_setAll(this, initial);
}

boolean Bitslist_isBit(const PBitslist this, const u32 index) {
    return is_bit$32($values, index);
}

void Bitslist_setBit(const PBitslist this, const u32 index) {
    set_bit$32($values, index);
}

void Bitslist_clearBit(const PBitslist this, const u32 index) {
    clear_bit$32($values, index);
}

void Bitslist_set(const PBitslist this, const u32 index, const boolean value) {
    if (value) {
        set_bit$32($values, index);
    } else {
        clear_bit$32($values, index);
    }
}

i32 Bitslist_findFirst(const PBitslist this) {
  int res = 0;
  for (int i = 0; i < $capacity -1; i++) {
    if ($values[i])  {
        int offset = get_bit_leadingZeros$32($values[i]);
        if (offset < 32) {
            return res + offset;
        }
    }
    res += 32;
  }
  for (int i = res; i < length; i++) {
      if (is_bit$32($values, i)) {
        return i;
      }
  }
  return -1;
}

void Bitslist_setAll(PBitslist this, boolean value) {
    if (value) {
        for (int i = 0; i < $capacity; i++) {
            $values[i] = 0xffffffff;
        }
    } else {
        for (int i = 0; i < $capacity; i++) {
            $values[i] = 0;
        }
    }
}

u32 Bitslist_getBitCount(const PBitslist this) {
  u32 res = 0;  
  
  for (int i = 0; i < $capacity; i++) {
      u32 val = $values[i];
    res += get_bit_count$32(val);
  }
  // @todo fixme, counts past the end of length

  return res;
}


Fn_free(Bitslist, PBitslist) {
    Bitslist_freeChildren(*this);
    Pointers.free(*this);
    *this = 0;
}

Fn_freeChildren(Bitslist, PBitslist) {
    Pointers.free($values);
}
