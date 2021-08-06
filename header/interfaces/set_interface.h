#ifndef __SETINTERFACE_
#define __SETINTERFACE_

#include "collection_base.h"

typedef struct SetInterface {
  PFn_putEntry putEntry;
  PFn_get get;
  PFn_deleteEntry delete;
  PFn_contains contains;
  PFn_clear clear;
} SetInterface;

typedef SetInterface * Set;

#define implementSet(class)  struct SetInterface class ## _SetInterface = { \
    &class ## _putEntry, \
    &class ## _get, \
    &class ## _delete, \
    &class ## _contains, \
    &class ## _clear, \
}; \
 \
Fn_setInterface(class) { \
    return &class ## _SetInterface; \
}

#define Fn_setInterface(class) Pool class ## _setInterface()
typedef Set (*PFn_set)(void);

#endif