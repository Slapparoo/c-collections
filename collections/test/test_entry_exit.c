#include <stdio.h>
#include <stdlib.h>
#include "rawhashmap.h"

// begin
#define B \
    {     \
        incScope();
// return
#define R       \
    decScope(); \
    }
#define REASSIGN(dest, src) pointerReassign(&dest, src)
#define ASSIGN(src) pointerAssign(src)
#define FREE(x) decRef(x)

// return value
#define RV(x)   \
    decScope(); \
    return x;   \
    }
// return pointer
#define RP(x)   \
    decScope(); \
    return x;   \
    }

// Main begin
// // #define MB \
// //     {      \
// //         premain();
// #define MR(x)  \
//     postmain(); \
//     return x;  \
//     }

Pointer allocMem(u64 size, Fn_free);

typedef struct MemoryEntry
{
    Pointer ptr;
    i32 count;
    Fn_free free;
} MemoryEntry;

typedef MemoryEntry *PMemoryEntry;

PRawHashmap memoryUsed; // = RawHashmap.create();

/**
 * @brief runs before the application enters main
 * https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
 * 
 */
__attribute__((constructor)) void premain()
{
    memoryUsed = RawHashmap.create(sizeof(MemoryEntry), hash$Pointer, compare$Pointer);
    printDebug("premain %p", memoryUsed);
}

/**
 * @brief runs after the application exists main or exit() is called
 * https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
 * 
 * @param map 
 * @param other 
 * @param entry 
 */
void myTraverse(PRawHashmap map, Pointer other, MemoryEntry *entry)
{
    printDebug("free %p, %p entry:%p %i", map, other, entry->ptr, entry->count);
    entry->free(entry->ptr);
}

__attribute__((destructor)) void postmain()
{
    // traverse hahsmap and free memory
    // foreach (e -> {e.free(e.pointer)})
    RawHashmap.traverse(memoryUsed, null, &myTraverse);
    RawHashmap.clear(memoryUsed);

    printDebug("End main items %i", memoryUsed->length);

    RawHashmap.free(&memoryUsed);
}

void stdFree(Pointer pointer)
{
    printDebug("standard free %p", pointer);
    free(pointer);
}

Pointer allocMem(u64 size, Fn_free free)
{
    Pointer res = malloc(size);
    MemoryEntry memtree = {res, 1, free};
    RawHashmap.put(memoryUsed, &memtree);
    printDebug("allocMem %p", res);

    return res;
}

u32 scope = 0;
static __inline__ Pointer incScope()
{
    scope++;
}

static __inline__ Pointer decScope()
{
    scope--;
}

static __inline__ Pointer pointerAssign(Pointer src)
{
    PMemoryEntry me = RawHashmap.get(memoryUsed, &src);
    if (me)
    {
        me->count++;
        printDebug("Assing pointer. %p", src);
    }
    else
    {
        printError("Pointer reference not found use allocMem instead of malloc. %p", src);
    }
    return src;
}

static __inline__ Pointer pointerReassign(Pointer *dest, Pointer src)
{
    PMemoryEntry me = RawHashmap.get(memoryUsed, dest);
    if (me)
    {
        me->count--;
        printDebug("dec pointer. %p", *dest);
    }
    else
    {
        printError("Pointer reference not found use allocMem instead of malloc. %p", src);
    }
    me = RawHashmap.get(memoryUsed, &src);
    if (me)
    {
        me->count--;
        printDebug("inc pointer. %p", *dest);
    }
    else
    {
        printError("Pointer reference not found use allocMem instead of malloc. %p", src);
    }
    *dest = src;
}

Pointer incRef(Pointer pointer)
{
    PMemoryEntry me = RawHashmap.get(memoryUsed, &pointer);
    if (me)
    {
        me->count++;
    }
    else
    {
        printError("Pointer reference not found use allocMem instead of malloc. %p", pointer);
    }
    return pointer;
}

void decRef(Pointer pointer)
{
    PMemoryEntry me = RawHashmap.get(memoryUsed, &pointer);
    if (me)
    {
        me->count--;

        if (me->count == 0)
        {
            me->free(me->ptr);
            RawHashmap.remove(memoryUsed, &pointer);
        }
    }
    else
    {
        printError("Pointer reference not found use attempting to free??. %p", pointer);
    }
}

/**
 * @brief Mem managed starts here...
 * 
 */

void method1(int a)
    B
    printf("method1 %i, %i\n", a, scope);
R

    int
    method2(int a)
        B
    printf("method2 %i %i\n", a, scope);
RV(a)

int main(int argc, char const *argv[])
{
    Pointer ptr = allocMem(100, stdFree);
    Pointer ptr2 = allocMem(200, stdFree);
    method1(5);
    method2(6);
    printf("scope %i\n", scope);
    Pointer ptr3 = ASSIGN(ptr2);
    REASSIGN(ptr2, ptr);

    PMemoryEntry me = RawHashmap.get(memoryUsed, &ptr);
    printf("refcount %i\n", me->count);

    FREE(ptr3);
    FREE(ptr);

#define _FAST_HASH(x) (i32) x ^ (x * 131072) ^ 0x823A3481 * (x * 32)
    // #define FAST_HASH(x) (i32)x ^ (x << 17) ^ 0x823A3481 * (x << 5)

    printf("hash %i\n", _FAST_HASH(1));
    printf("hash %i\n", FAST_HASH(1));
}
