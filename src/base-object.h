/**
 * @brief Group diffent functional areas together 
 * 
 */

#ifndef _OBJECTS_HH
#define _OBJECTS_HH

#include "object-types.h"
#include "hashcodes.h"

#define BaseObject_name "BaseObject"
#define BaseObject_instance_name "BaseObject_data"

typedef struct BaseObject_code BaseObject_code;
typedef struct BaseObject_data BaseObject_data;
typedef BaseObject_code* Class;
typedef BaseObject_data* Object;

// Base Generic Object
#define __C_CORE_CODE_  \
    Class parentClass; \
    PChar (*className)();  \
    i32 (*hashCode)(const Object);  \
    boolean (*equals)(const Object, const Object);  \
    PChar (*toString)(Object);  \
    void (*free)(Object);  \
    void (*freeChildren)(const Object);  

typedef struct BaseObject_code {
    __C_CORE_CODE_ 
} BaseObject_code;

#define __C_CORE_DATA_  \
    Class classMethods; \
    Pointer privateData; \
    PChar instanceName; \
    PChar toStringPString; 

typedef struct BaseObject_data {
    __C_CORE_DATA_ 
} BaseObject_data;

BaseObject_code BaseObject;

#endif
