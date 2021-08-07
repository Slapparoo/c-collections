#include "data-types.h"

#ifndef __FUNCTIONDEFS_HH_
#define __FUNCTIONDEFS_HH_

typedef __attribute__ ((nonnull (1, 2))) i32 (*PFn_compare)(const Pointer a, const Pointer b);

/**
 * @brief i32 based collections
 * 
 */

#define Fn_traverser(class, thisType, entryType) __attribute__ ((nonnull (1))) i32 (class ## _traverser)(const thisType this, entryType entry)
typedef __attribute__ ((nonnull (1, 2))) void (*PFn_traverser)(Pointer other, Pointer entry);

#define Fn_compareEntry(class, T) __attribute__ ((nonnull (1))) i32 (class ## _compareEntry)(const T this, const i32 a, const i32 b)
typedef __attribute__ ((nonnull (1))) i32 (*PFn_compareEntry)(const Pointer object, const i32 a, const i32 b);

#define Fn_removeEntry(class, T) __attribute__ ((nonnull (1))) void class ## _removeEntry(const T this, const i32 index)
typedef __attribute__ ((nonnull (1))) void (*PFn_removeEntry)(const Pointer object, const i32 index);

#define Fn_relocateEntry(class, T) __attribute__ ((nonnull (1))) i32 class ## _relocateEntry(T this, const i32 dest, const i32 src)
typedef __attribute__ ((nonnull (1))) i32 (*PFn_relocateEntry)(Pointer object, const i32 dest, const i32 src);

#define Fn_swapEntry(class, T) __attribute__ ((nonnull (1))) void class ## _swapEntry(const T this, const i32 a, const i32 b)
typedef __attribute__ ((nonnull (1))) i32 (*PFn_swapEntry)(Pointer object, const i32 a, const i32 b);

#define Fn_traverse(class, T) __attribute__ ((nonnull (1, 3))) void class ## _traverse(const T this, Pointer other, PFn_traverser traverser)
typedef __attribute__ ((nonnull (1, 3))) void (*PFn_traverse)(const Pointer object, Pointer other, PFn_traverser traverser);

/** client side */
#define Fn_traversal(class, T, entryType) __attribute__ ((nonnull (1, 2))) void class ## _traversal(const T this, entryType value)
typedef __attribute__ ((nonnull (1, 2))) void (*PFn_traversal)(const Pointer object, Pointer value);


#define Fn_traverseBy(class, T) __attribute__ ((nonnull (1, 3))) void class ## _traverseBy(const T this, i32 order, Pointer other, PFn_traverser traverser)
typedef __attribute__ ((nonnull (1, 3))) void (*PFn_traverseBy)(const Pointer object, i32 order, Pointer other, PFn_traverser traverser);

#define Fn_freeChildren(class, T) __attribute__ ((nonnull (1))) void class ## _freeChildren(const T this)
typedef __attribute__ ((nonnull (1))) void (*PFn_freeChildren)(const Pointer object);

#define Fn_free(class, T) __attribute__ ((nonnull (1))) void class ## _free(T* this)
typedef __attribute__ ((nonnull (1))) void (*PFn_free)(Pointer* object);

#define Fn_push(class, T, entryType) __attribute__ ((nonnull (1, 2))) void class ## _push(T this, entryType value)
typedef __attribute__ ((nonnull (1, 2)))  void (*PFn_push)(Pointer object, const Pointer value);

#define Fn_peek(class, T, entryType) __attribute__ ((nonnull (1))) entryType class ## _peek(T this)
typedef __attribute__ ((nonnull (1)))  Pointer (*PFn_peek)(Pointer object);

#define Fn_pop(class, T, entryType) __attribute__ ((nonnull (1))) entryType class ## _pop(T this)
typedef __attribute__ ((nonnull (1)))  Pointer (*PFn_pop)(Pointer object);

#define Fn_getEntry(class, T, entryType) __attribute__ ((nonnull (1))) entryType class ## _getEntry(T this, i32 index)
typedef __attribute__ ((nonnull (1)))  Pointer (*PFn_getEntry)(Pointer object, const i32 index);

#define Fn_setEntry(class, T, entryType) __attribute__ ((nonnull (1))) void class ## _setEntry(T this, i32 index, entryType value)
typedef __attribute__ ((nonnull (1)))  void (*PFn_setEntry)(Pointer object, i32 index, const Pointer value);

#define Fn_insertEntry(class, T, entryType) __attribute__ ((nonnull (1))) void class ## _insertEntry(T this, i32 index, entryType value)
typedef __attribute__ ((nonnull (1)))  void (*PFn_insertEntry)(Pointer object, i32 index, const Pointer value);

#define Fn_addEntry(class, T, entryType) __attribute__ ((nonnull (1))) entryType class ## _addEntry(T this, entryType value)
typedef __attribute__ ((nonnull (1, 2)))  void (*PFn_addEntry)(Pointer object, const Pointer value);

#define Fn_borrow(class, T, entryType) __attribute__ ((nonnull (1))) entryType class ## _borrow(T this)
typedef __attribute__ ((nonnull (1)))  Pointer (*PFn_borrow)(Pointer object);

#define Fn_returnIt(class, T, entryType) __attribute__ ((nonnull (1))) void class ## _returnIt(T this, entryType value)
typedef __attribute__ ((nonnull (1, 2)))  void (*PFn_returnIt)(Pointer object, Pointer value);

#define Fn_clear(class, T) __attribute__ ((nonnull (1))) void class ## _clear(T this)
typedef __attribute__ ((nonnull (1))) void (*PFn_clear)(Pointer object);

#define Fn_put(class, T, keyType, valueType, entryType) __attribute__ ((nonnull (1, 2, 3))) entryType class ## _put(T this, keyType key, valueType value)
typedef __attribute__ ((nonnull (1, 2, 3))) void (*PFn_put)(Pointer object, Pointer key, Pointer value);

#define Fn_putEntry(class, T, entryType) __attribute__ ((nonnull (1, 2))) entryType class ## _putEntry(T this, entryType entry)
typedef __attribute__ ((nonnull (1, 2))) Pointer (*PFn_putEntry)(Pointer object, Pointer entry);

#define Fn_get(class, T, keyType, entryType) __attribute__ ((nonnull (1, 2))) entryType class ## _get(T this, keyType key)
typedef __attribute__ ((nonnull (1, 2))) Pointer (*PFn_get)(Pointer object, Pointer key);

#define Fn_find(class, T, entryType) __attribute__ ((nonnull (1, 2))) entryType class ## _find(T this, entryType value)
typedef __attribute__ ((nonnull (1, 2)))  Pointer (*PFn_find)(Pointer object, const Pointer value);

#define Fn_contains(class, T, keyType) __attribute__ ((nonnull (1, 2))) bool class ## _contains(T this, keyType key)
typedef __attribute__ ((nonnull (1, 2)))  bool (*PFn_contains)(Pointer object, const Pointer value);

#define Fn_deleteEntry(class, T, entryType) __attribute__ ((nonnull (1, 2))) void class ## _deleteEntry(T this, entryType entry)
typedef __attribute__ ((nonnull (1, 2)))  void (*PFn_deleteEntry)(Pointer object, const Pointer entry);

#define Fn_close(class, T) __attribute__ ((nonnull (1))) void class ## _close(T this)
typedef __attribute__ ((nonnull (1)))  void (*PFn_close)(Pointer object);

#define Fn_run(class, T) __attribute__ ((nonnull (1))) u64 class ## _run(T this)
typedef __attribute__ ((nonnull (1)))  u64 (*PFn_run)(Pointer object);

#define Fn_length(class, T) __attribute__ ((nonnull (1))) u32 class ## _length(T this)
typedef __attribute__ ((nonnull (1)))  u32 (*PFn_length)(Pointer object);

#define Fn_sort(class, T) __attribute__ ((nonnull (1))) void class ## _sort(T this, PFn_compare compare)
typedef __attribute__ ((nonnull (1)))  u32 (*PFn_sort)(Pointer object, PFn_compare compare);

#define Fn_toArray(class, T, entryType) __attribute__ ((nonnull (1))) entryType* class ## _toArray(T this)
typedef __attribute__ ((nonnull (1)))  Pointer (*PFn_toArray)(Pointer object);

#define Fn_hashcode(class) __attribute__ ((nonnull (1))) i32 class ## _hashcode(Pointer value)
typedef __attribute__ ((nonnull (1))) i32 (*PFn_hashcode)(const Pointer value);

typedef __attribute__ ((nonnull (1, 2)))  void (*PFn_copy)(Pointer dest, const Pointer src);

typedef __attribute__ ((nonnull (1))) i32 (*PFn_release)(Pointer* object);
typedef __attribute__ ((nonnull (1, 2))) void (*PFn_swap)(Pointer a, Pointer b);

typedef __attribute__ ((nonnull (1, 3))) void (*PFn_message)(const Pointer object, Pointer other, Pointer value);
typedef Pointer (*PFn_allocate)(const u64 itemSize, const u64 numberOfItems, PFn_release);


/*
move b to position a, and slide a...b along
*/
// typedef __attribute__ ((nonnull (1))) void (*PFn_swapEntry)(const Pointer object, const i32 a, const i32 b);
typedef __attribute__ ((nonnull (1))) void (*PFn_hashEntry)(const Pointer object, const i32 a);
typedef __attribute__ ((nonnull (1))) void (*PFn_removed)(const Pointer object, const i32 a);
typedef __attribute__ ((nonnull (1))) boolean (*PFn_filter)(const Pointer object, const i32 a);

__attribute__ ((nonnull (1)))  i32 hash$8(u8*);
__attribute__ ((nonnull (1))) i32 hash$16(u16*);
__attribute__ ((nonnull (1))) i32 hash$32(u32*);
__attribute__ ((nonnull (1))) i32 hash$F32(i32*);
__attribute__ ((nonnull (1))) i32 hash$F64(i64*);
__attribute__ ((nonnull (1))) i32 hash$Pointer(Pointer*);
__attribute__ ((nonnull (1))) i32 hash$64(i64*);
__attribute__ ((nonnull (1))) i32 hash$PChar(PChar*);
__attribute__ ((nonnull (1))) i32 hash$Boolean(boolean*);

__attribute__ ((nonnull (1, 2))) i32 compare$8(u8*, u8*);
__attribute__ ((nonnull (1, 2))) i32 compare$16(u16*, u16*);
__attribute__ ((nonnull (1, 2))) i32 compare$32(u32*, u32*);
__attribute__ ((nonnull (1, 2))) i32 compare$F32(f32*, f32*);
__attribute__ ((nonnull (1, 2))) i32 compare$64(u64*, u64*);
__attribute__ ((nonnull (1, 2))) i32 compare$F64(f64*, f64*);
__attribute__ ((nonnull (1, 2))) i32 compare$Boolean(boolean*, boolean*);
__attribute__ ((nonnull (1, 2))) i32 compare$Pointer(Pointer*, Pointer*);

__attribute__ ((nonnull (1, 2))) i32 compare$PChar(PChar a, PChar b);

__attribute__ ((nonnull (1))) i32 pCharToI64(PChar);
__attribute__ ((nonnull (1))) i32 arrayToI64(Pointer, u64 length);

__attribute__ ((nonnull (1, 2))) void copy$Boolean(boolean*, const boolean*);
__attribute__ ((nonnull (1, 2))) void copy$8(u8*, const u8*);
__attribute__ ((nonnull (1, 2))) void copy$16(u16*, const u16*);
__attribute__ ((nonnull (1, 2))) void copy$32(u32*, const u32*);
__attribute__ ((nonnull (1, 2))) void copy$F32(f32*, const f32*);
__attribute__ ((nonnull (1, 2))) void copy$64(u64*, const u64*);
__attribute__ ((nonnull (1, 2))) void copy$F64(f64*, const f64*);


typedef struct PrimativeType {
    int length;
    boolean isSigned;
    boolean isFloat;
    PChar format;
    PChar name;
    PChar otherNames;
    PFn_hashcode hashcode;
    PFn_compare compare;
    // like a set, sets 
    PFn_copy getset;
    u64 max;
    u64 min;
} PrimativeType;

__attribute__ ((nonnull (1))) int printPrimativeType(PrimativeType* pt);
int printPrimativeTypeByNum(int type);

PrimativeType* primativeTypes;



#endif