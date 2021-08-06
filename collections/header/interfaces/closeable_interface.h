#ifndef __CLOSEABLEINTERFACE_
#define __CLOSEABLEINTERFACE_

#include "collection_base.h"

typedef struct CloseableInterface {
  PFn_close close;
} CloseableInterface;

typedef CloseableInterface * Closeable;

#define implementCloseable(class)  struct CloseableInterface class ## _CloseableInterface = { \
    &class ## _close, \
}; \
 \
Fn_closeableInterface(class) { \
    return &class ## _CloseableInterface; \
}

#define Fn_closeableInterface(class) Closeable class ## _closeableInterface()
typedef Closeable (*PFn_closeable)(void);

#endif