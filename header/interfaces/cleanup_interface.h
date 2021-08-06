#ifndef __CLEANUPINTERFACE_
#define __CLEANUPINTERFACE_

#include "collection_base.h"

typedef struct CleanupInterface {
  PFn_free free;
  PFn_freeChildren freeChildren;
} CleanupInterface;

typedef CleanupInterface * Cleanup;

#define implementCleanup(class)  struct CleanupInterface class ## _CleanupInterface = { \
    &class ## _free, \
    &class ## _freeChildren, \
}; \
 \
Fn_cleanupInterface(class) { \
    return &class ## _CleanupInterface; \
}

#define Fn_cleanupInterface(class) Cleanup class ## _cleanupInterface()
typedef Cleanup (*PFn_cleanup)(void);

#endif