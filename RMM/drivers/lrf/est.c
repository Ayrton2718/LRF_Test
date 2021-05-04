#include "est.h"

#include <math.h>
#include "display.h"


CC_obj LrfEst_calcWeights(LrfMap_t map, LrfPf_t pf, CC_obj distance, size_t max_distance)
{
    float diff_buff[LRFPF_NODE_COUNT];
    float diff_sum = 0;

    CCAutoRelease_startScope();
    {
        for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
        {
            LrfPf_pos_t current_pos = LrfPf_nodePos(pf, i);
            CC_obj sim = CCAutoRelease_add(LrfSim_simulate(map, current_pos.x, current_pos.y, current_pos.yaw));

            LrfMap_t disp = LrfDisplay_create(map);
            LrfMap_putRobot(disp, current_pos.x, current_pos.y, current_pos.yaw);
            // LrfSim_simulateViwe2(disp, 295, 300, 0, sim);
            LrfSim_simulateViwe2(disp, current_pos.x, current_pos.y, current_pos.yaw, sim);
            // CCLOG_INFO("(%d, %d, %d), %f", current_pos.x, current_pos.y, current_pos.yaw, diff_buff[i]);
            LrfMap_showWait(disp, "tmp");
            LrfMap_destructor(disp);

            size_t current_diff = 0;
            for(int ang = 0; ang < 180; ang++)
            {
                int result_val = CCInt32_get(CCArray_objectAtIndex(distance, ang));
                int sim_val = CCInt32_get(CCArray_objectAtIndex(sim, ang));
                // current_diff += abs(result_val - sim_val);
                current_diff += (result_val - sim_val) * (result_val - sim_val);
            }

            diff_sum += 1 / (float)current_diff;
            diff_buff[i] = 1 / (float)current_diff;

            CCLOG_INFO("(%d, %d, %d), %d", current_pos.x, current_pos.y, current_pos.yaw, current_diff);
        }
    }
    CCAutoRelease_doneScope();

    CC_obj weights = CCArray_create();
    for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
    {
        CCArray_addObject(weights, CCFloat_create(diff_buff[i] / diff_sum));
    }

    return weights;
}


// float LrfEst_culcWeight(LrfMap_t map, int x, int y, int yaw, CC_obj result, int max_distance)
// {
//     float er_sum = 0;

//     CCAutoRelease_startScope();
//     {
//         CC_obj sim = CCAutoRelease_add(LrfSim_simulate(map, x, y, yaw));
        
//         for(int ang = yaw; ang < 180 + yaw; ang++)
//         {
//             CC_obj result_val = CCArray_objectAtIndex(result, ang - yaw);
//             CC_obj sim_val = CCArray_objectAtIndex(sim, ang - yaw);
//             er_sum += (float)abs(CCInt32_get(result_val) - CCInt32_get(sim_val)) / max_distance;
//             // CCLOG_INFO("%d, %d", CCInt32_get(result_val), CCInt32_get(sim_val));
//         }
//     }
//     CCAutoRelease_doneScope();

//     er_sum /= 360;

//     return 1 - er_sum;
// }
