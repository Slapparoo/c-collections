#ifndef _HASHMAP_HH_
#define _HASHMAP_HH_

#include "objects.h"

// simple Hashmap
struct Hashmap_code Hashmap;
#define Hashmap_Growby 10
#define Hashmap_data_name "Hashmap_data"

typedef struct Hashmap_data {
    __C_CORE_DATA_
    Pointer keys; 
    Pointer values;
    u64 length;
    u64 capacity;
    u64 itemSize = sizeof(Pointer);
} Hashmap_data;

/**
 * @brief Simple Hashmap implementation
 * 
 */
typedef struct Hashmap_code {
    __C_CORE_CODE_ 
    Hashmap_data * (*create)(u64 initialSize);
    void (*init)(const Hashmap_data *hashmap, u64 initialSize);
    void (*add)(Hashmap_data *hashmap, BaseObject_data * key, BaseObject_data * object);
    Pointer (*get)(const Hashmap_data *hashmap, BaseObject_data * key);
    void (*remove)(Hashmap_data *hashmap, BaseObject_data * key);
    void (*clear)(Hashmap_data *hashmap);
} Hashmap_code;


#endif