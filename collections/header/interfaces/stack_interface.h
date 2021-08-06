#ifndef __STACKINTERFACE_HH_
#define __STACKINTERFACE_HH_

#include "collection_base.h"
#include "interfaces/pushpop_interface.h"
/**
 * Lifo Stack
 * 
 */

#define implementStackAs(class, push, pop, peek) struct PushPopInterface class ## _StackInterface = { \
    push, \
    pop, \
    peek, \
}; \
 \
Fn_stackInterface(class) { \
    return &class ## _StackInterface; \
}

#define implementStack(class) struct PushPopInterface class ## _StackInterface = { \
    &class ## _push, \
    &class ## _pop, \
    &class ## _peek, \
}; \
 \
Fn_stackInterface(class) { \
    return &class ## _StackInterface; \
}


#define Fn_stackInterface(class) PushPop class ## _stackInterface()
typedef PushPop (*PFn_stack)(void);

#endif