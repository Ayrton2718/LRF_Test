#ifndef RMM_DRIVERS_LRF_PF_H
#define RMM_DRIVERS_LRF_PF_H

#include <thread_comm.h>

#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LRFPF_NODE_COUNT (100)


typedef void* LrfPf_t;


typedef struct
{
    int x;
    int y;
    int yaw;
} LrfPf_pos_t;


LrfPf_t LrfPf_create(LrfPf_pos_t init_pos);

// for predict
void LrfPf_predict(LrfPf_t obj, LrfPf_pos_t d_move);

// for weight set
size_t LrfPf_nodeCount(LrfPf_t obj);

LrfPf_pos_t LrfPf_nodePos(LrfPf_t obj, size_t node_num);
void LrfPf_setWeight(LrfPf_t obj, size_t node_num, float weight);

// calculation
LrfPf_pos_t LrfPf_estimate(LrfPf_t obj);

// for resampling
void LrfPf_resampling(LrfPf_t obj);

void LrfPf_show(LrfPf_t obj, LrfMap_t disp);

#ifdef __cplusplus
}
#endif

#endif /*RMM_DRIVERS_LRF_PF_H*/
