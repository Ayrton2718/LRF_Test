#ifndef RMM_DRIVERS_LRF_EST_H
#define RMM_DRIVERS_LRF_EST_H

#include <thread_comm.h>

#include "pf.h"
#include "map.h"
#include "sim.h"


#ifdef __cplusplus
extern "C" {
#endif


CC_obj LrfEst_calcWeights(LrfMap_t map, LrfPf_t pf, CC_obj distance, size_t max_distance);

#ifdef __cplusplus
}
#endif

#endif /*RMM_DRIVERS_LRF_EST_H*/
