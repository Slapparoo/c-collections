#ifndef __QUEUEINTERFACE_HH_
#define __QUEUEINTERFACE_HH_

#include "collection_base.h"
#include "interfaces/pushpop_interface.h"
/**
 * @brief "Interface" definitions 
 * 
 * Fifo is a queue but a queue doesn't have to be a fifo (a queue may be priority sorted etc)
 * 
 */

#define Fn_queueInterface(class) PushPop class ## _queueInterface()
typedef PushPop (*PFn_queue)(void);

#define implementQueueAs(class, push, pop, peek) struct PushPopInterface class ## _QueueInterface = { \
    push, \
    pop, \
    peek, \
}; \
 \
Fn_queueInterface(class) { \
    return &class ## _QueueInterface; \
}

#define implementQueue(class) struct PushPopInterface class ## _QueueInterface = { \
    &class ## _push, \
    &class ## _pop, \
    &class ## _peek, \
}; \
 \
Fn_queueInterface(class) { \
    return &class ## _QueueInterface; \
}


#endif