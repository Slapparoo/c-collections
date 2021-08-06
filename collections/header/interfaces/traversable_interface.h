#ifndef __TRAVERSABLEINTERFACE_
#define __TRAVERSABLEINTERFACE_

#include "collection_base.h"

typedef struct TraversableInterface {
  PFn_traverse traverse;
} TraversableInterface;

typedef TraversableInterface * Traversable;

#define implementTraversable(class)  struct TraversableInterface class ## _TraversableInterface = { \
    &class ## _traverse, \
}; \
 \
Fn_traversableInterface(class) { \
    return &class ## _TraversableInterface; \
}

#define Fn_traversableInterface(class) Traversable class ## _traversableInterface()
typedef Traversable (*PFn_traversable)(void);

#endif