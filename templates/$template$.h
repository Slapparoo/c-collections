#ifndef _$TEMPLATE$_HH_
#define _$TEMPLATE$_HH_

#include "$extends$.h"

#define $Template$_name "$Template$"
#define $Template$_instance_name "$Template$_data"

struct $Template$_code $Template$;

#define __C_$TEMPLATE$_DATA_ \
    __C_$EXTENDS$_DATA_ 
    // -- insert data items here --


typedef struct $Template$_data {
    __C_$TEMPLATE$_DATA_
} $Template$_data;

typedef $Template$_data * P$Template$;

#define __C_$TEMPLATE$_CODE_ \
    __C_$EXTENDS$_CODE_  \
    $Template$_data * (*create)(); \
    void (*init)($Template$_data *this);
    // -- add functions here --

typedef struct $Template$_code {
    __C_$TEMPLATE$_CODE_
} $Template$_code;

#endif