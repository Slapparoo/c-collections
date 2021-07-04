#ifndef _LIST_HH_
#define _LIST_HH_

#include "objects.h"

// simple list
struct List_code List;
#define List_Growby 10
#define List_data_name "List_data"

// utility macro 
#define ListAdd(list, value ) *(Pointer*)List.add(list) = value;    

typedef struct List_data {
    __C_CORE_DATA_
    Pointer items;
    u64 length;
    u64 capacity;
    u64 itemSize;
} List_data;

/**
 * @brief more like a stack
 * 
 */
typedef struct List_code {
    __C_CORE_CODE_ 
    List_data * (*create)(u64 initialSize, u64 itemSize);
    void (*init)(const List_data *list, u64 initialSize, u64 itemSize);
    /** returns a pointer to the Object to be stored 
     * *(PChar*)List.add(myList) = "A string";
    */
    Pointer (*add)(List_data *list);
    Pointer (*value)(const List_data *list, u64 index);
    Pointer (*remove)(const List_data *list);
    void (*clear)(const List_data *list);
    void (*sort)(const List_data *list, int (*compar)(const Pointer, const Pointer));
    Pointer (*search)(const List_data *list, const Pointer key, int (*compar)(const Pointer, const Pointer));
} List_code;

#endif