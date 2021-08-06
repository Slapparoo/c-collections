# Collections for C

Some Collection/List/Tree/Set/Map implementions in C

They Come in 2 flavours

## Value based
The entire value is copyed in to the "collection", this means memory can be "bulk" allocated and released.
These types can also be used for "Primative" data types that may be samller that a `pointer`

## Pointer based
Only the pointer the the data is stored in the "collection" allowing storeage of types with different sizes like string, the list will manage its memory but not the memory of what is pointed to

## dependancies
cmocka
`version 1.15` but others hould also be fine

## Standards (or coding conventions)
Sorry guys I invented my own the camel case is based on Java standards it allow me to read and write code which I understand.

You could get into a big discussion about coding by convention in C which every one does, not using `void*` is a convention the langauge allows it but it is not used by convention.

The use of conventions is to help avoid issues an dmake the intention of our code clear.

There are a few basic rules that I apply, 
* Code by intention - make the intention of the code clear to anyone reading it.
* Comment by exception (comments and documentation are not the same thing) - comments should be used and only be used where you're doing something fancy or doing a work around, where for soomeone reading the code it is not clear what is going on.
* Always consider performanace.
* Think about your future self when you come back to the code in x months time are you going to be able to understand it.

The implementation is a mix of OO and function centric (not functional) where there is a library of predefined function signatures which can the implemented by differnt "classes" to provide a compatable consistant and and predictable interface.

Class function implementation naming <Class>_method
The `$` in member variables is used to indicate for internal use (what would be hidden on OO)
simarly the `$` in a function name is used to indicate internal usage (but probably not nessacay as `static` is also used) 

e.g given:
```
typedef struct ValueTree_data {
    int $objectSize; 
    PFn_compare compare; 
    int length; 
    PValueTreeNode $rootNode;
    PValueAllocator $allocator;
} ValueTree_data;
```
The variables `$objectSize`, `$rootNode` and `$allocator` are intended for internal use only

The formating is standard C formating.

As strong typing as I cound use, there is generous use of `typedef` to get as much type checking by the compiler as possible.

Basic types, I find when creating binary compilable code knowing exactly what data type you are dealing with is a major benefit in removing simple mistakes so the basic Primatives have a `typedef` to indicate extactly what the are
`[u,i,f]<size in bits> (unsigned, signed/integer, float)`

i.e u8 is an unsigned 8 bit.
f32 32 bit floating point.
i64 64 bit integer.

`PChar` is for strings as it has a special meaning in that it is `null` terminated which `char*` could mean but could also mean a few other things too, like a pointer to an array of short short int which is not null terminated, or a pointer to a single short short int not an array of them.

Sadly `Pointer` is also required whihc is equivalent to `void*`

## documentation

## Samples

## Build



