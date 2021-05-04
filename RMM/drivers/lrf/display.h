#ifndef RMM_DRIVERS_LRF_DISPLAY_H
#define RMM_DRIVERS_LRF_DISPLAY_H

#include "map.h"


#ifdef __cplusplus
extern "C" {
#endif

LrfMap_t LrfDisplay_create(LrfMap_t map);

void LrfDisplay_reload(LrfMap_t* obj, LrfMap_t map);

#ifdef __cplusplus
}
#endif

#endif /*RMM_DRIVERS_LRF_DISPLAY_H*/
