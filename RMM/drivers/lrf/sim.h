#ifndef RMM_DRIVERS_LRF_SIM_H
#define RMM_DRIVERS_LRF_SIM_H

#include <thread_comm.h>
#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif

void LrfSim_initializer(void);

CC_obj LrfSim_simulate(LrfMap_t map, int x, int y, int yaw);

CC_obj LrfSim_simulateNoise(LrfMap_t map, int x, int y, int yaw, int range);

void LrfSim_simulateViwe(LrfMap_t map, int x, int y, int yaw, CC_obj values);

void LrfSim_simulateViwe2(LrfMap_t map, int x, int y, int yaw, CC_obj values);

#ifdef __cplusplus
}
#endif

#endif /*RMM_DRIVERS_LRF_SIM_H*/
