#include "sim.h"

#include <math.h>

void LrfSim_initializer(void)
{
    srand(time(NULL));
}


CC_obj LrfSim_simulate(LrfMap_t map, int x, int y, int yaw)
{
    return LrfSim_simulateNoise(map, x, y, yaw, 0);
}

CC_obj LrfSim_simulateNoise(LrfMap_t map, int x, int y, int yaw, int range)
{
    CC_obj values = CCArray_create();

    for(int ang = yaw; ang < (180 + yaw); ang++)
    {
        double cos_rate = cos(((double)ang / 360) * M_PI * 2);
        double sin_rate = sin(((double)ang / 360) * M_PI * 2);

        for(int dis = 0; dis < 1200; dis++)
        {
            int dx = (int)((double)dis * cos_rate);
            int dy = (int)((double)dis * sin_rate);

            int tar_x = (int)x + dx;
            int tar_y = (int)y + dy;

            if(LrfMap_isWall(map, tar_x, tar_y) == CC_BOOL_TRUE)
            {
                if(0 < range)
                {
                    int noise = (rand() % range) - (range / 2);
                    dis += noise;
                }

                // CCLOG_INFO("%d, %d", tar_x, tar_y);
                CCArray_addObject(values, CCInt32_create(dis));
                // LrfMap_addLine(disp_map, x, y, tar_x, tar_y);
                break;
            }
        }
    }

    return values;
}

void LrfSim_simulateViwe(LrfMap_t map, int x, int y, int yaw, CC_obj values)
{
    for(int ang = yaw; ang < (180 + yaw); ang++)
    {
        CC_obj dis_obj = CCArray_objectAtIndex(values, ang - yaw);

        double cos_rate = cos(((double)ang / 360) * M_PI * 2);
        double sin_rate = sin(((double)ang / 360) * M_PI * 2);
        
        int tar_x = (int)x + (int)((double)CCInt32_get(dis_obj) * cos_rate);
        int tar_y = (int)y + (int)((double)CCInt32_get(dis_obj) * sin_rate);

        LrfMap_addLine(map, x, y, tar_x, tar_y);
    }
}

void LrfSim_simulateViwe2(LrfMap_t map, int x, int y, int yaw, CC_obj values)
{
    for(int ang = yaw; ang < (180 + yaw); ang++)
    {
        CC_obj dis_obj = CCArray_objectAtIndex(values, ang - yaw);

        double cos_rate = cos(((double)ang / 360) * M_PI * 2);
        double sin_rate = sin(((double)ang / 360) * M_PI * 2);
        
        int tar_x = (int)x + (int)((double)CCInt32_get(dis_obj) * cos_rate);
        int tar_y = (int)y + (int)((double)CCInt32_get(dis_obj) * sin_rate);

        LrfMap_addCircle(map, tar_x, tar_y, 2);
    }
}