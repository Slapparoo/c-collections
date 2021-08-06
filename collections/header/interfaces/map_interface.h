#ifndef __MAPINTERFACE_HH_
#define __MAPINTERFACE_HH_

#include "collection_base.h"
/**
 * @brief "MapInterface" definitions 
 * 
 * Map key/value pairs
 * 
 */

typedef struct MapInterface {
  PFn_put put;
  PFn_get get;
  PFn_deleteEntry delete;
  PFn_contains contains;
  PFn_clear clear;
} MapInterface;

typedef MapInterface * Map;

#define Fn_mapInterface(class) Map class ## _mapInterface()
typedef Map (*PFn_map)(void);

#define implementMap(class) struct MapInterface class ## _MapInterface = { \
    &class ## _put, \
    &class ## _get, \
    &class ## _deleteEntry, \
    &class ## _contains, \
    &class ## _clear, \
}; \
 \
Fn_mapInterface(class) { \
    return &class ## _MapInterface; \
}

#endif