// *Case sensitive replace*
// replace $TEMPLATE$ with your name All uppers
// replace $Template$ with your name First upper cammel case

#ifndef _$TEMPLATE$_HH_
#define _$TEMPLATE$_HH_

#include "base-object.h"

#define $Template$_name "$Template$"
#define $Template$_instance_name "$Template$_data"

struct $Template$_code $Template$;

typedef struct $Template$_data {
    __C_CORE_DATA_
} $Template$_data;

typedef struct $Template$_code {
    __C_CORE_CODE_ 
    $Template$_data * (*create)();
    void (*init)($Template$_data *this);
} $Template$_code;

#endif