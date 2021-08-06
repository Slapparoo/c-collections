#ifndef __RUNNABLEINTERFACE_
#define __RUNNABLEINTERFACE_

#include "collection_base.h"

typedef struct RunnableInterface {
  PFn_run run;
} RunnableInterface;

typedef RunnableInterface * Runnable;

#define Fn_runnableInterface(class) Runnable class ## _runnableInterface()
typedef Runnable (*PFn_runnable)(void);

#define implementRunnable(class)  struct RunnableInterface class ## _RunnableInterface = { \
    &class ## _runnable, \
}; \
 \
Fn_runnableInterface(class) { \
    return &class ## _RunnableInterface; \
}

#endif