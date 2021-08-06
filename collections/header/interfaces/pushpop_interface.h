#ifndef __PUSHPOPINTERFACE_HH_
#define __PUSHPOPINTERFACE_HH_

#include "collection_base.h"

/**
 * @brief no implementtions used for quaue and stack etc.
 * 
 */

typedef struct PushPopInterface {
  PFn_push push;
  PFn_pop pop;
  PFn_peek peek;
} PushPopInterface;

typedef PushPopInterface * PushPop;

#endif