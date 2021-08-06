#ifndef __POOLINTERFACE_
#define __POOLINTERFACE_

#include "collection_base.h"

typedef struct PoolInterface {
  PFn_borrow borrow;
  PFn_returnIt returnIt;
} PoolInterface;

typedef PoolInterface * Pool;

#define implementPool(class)  struct PoolInterface class ## _PoolInterface = { \
    &class ## _borrow, \
    &class ## _returnIt, \
}; \
 \
Fn_poolInterface(class) { \
    return &class ## _PoolInterface; \
}

#define Fn_poolInterface(class) Pool class ## _poolInterface()
typedef Pool (*PFn_pool)(void);

#endif