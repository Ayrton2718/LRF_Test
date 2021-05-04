#include "RMM/rmm.hpp"

// #define IS_MAKE_DATASET

#ifndef IS_MAKE_DATASET
int main(void)
{
    CCInitializer_init();
    TCInitializer_init();
    PCInitializer_init();
    CSInitializer_init();

    srand(time(NULL));

    TCLauncher_launchAll();

    LrfMap_t map = LrfMap_create(500, 500, 0, 499);
    LrfMap_addBox(map, 50, 180, 80, 150);
    LrfMap_addBox(map, 450, 180, 480, 150);
    LrfMap_addBox(map, 50, 280, 80, 250);
    LrfMap_addBox(map, 450, 280, 480, 250);
    LrfMap_addBox(map, 50, 380, 80, 350);
    LrfMap_addBox(map, 450, 380, 480, 350);

    LrfMap_addBox(map, 0, 0, 10, 100);

    LrfMap_t disp = LrfDisplay_create(map);

    LrfPf_t pf = LrfPf_create({250, 250, 0});

    CCAutoRelease_startScope();
    {
        LrfPf_predict(pf, {50, 50, 0});
        // LrfPf_predict(pf, {50, 0, 80});
        // LrfPf_predict(pf, {50, 0, 0});

        // CC_obj ans = CCAutoRelease_add(LrfSim_simulateNoise(map, 295, 300, 0, 2));
        CC_obj ans = CCAutoRelease_add(LrfSim_simulate(map, 300, 300, -5));
        LrfSim_simulateViwe2(disp, 300, 300, -5, ans);
        LrfMap_showWait(disp, "nodes");
        LrfDisplay_reload(&disp, map);

        CC_obj weights = CCAutoRelease_add(LrfEst_calcWeights(map, pf, ans, 12000));
        CCLOG_INFO("%s", CCString_getCString(CCAutoRelease_add(CCJsonSerializer_dumps(weights, CC_BOOL_TRUE))));

        // LrfPf_show(pf, disp);
        // LrfMap_showWait(disp, "nodes");

        for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
        {
            LrfPf_setWeight(pf, i, CCFloat_get(CCArray_objectAtIndex(weights, i)));
        }
        LrfPf_pos_t est = LrfPf_estimate(pf);
        CCLOG_INFO("%d, %d, %d", est.x, est.y, est.yaw);

        LrfDisplay_reload(&disp, map);
        LrfMap_putRobot(disp, 300, 300, -5);
        LrfMap_putRobot(disp, est.x, est.y, est.yaw);
        LrfMap_showWait(disp, "result");
    }
    CCAutoRelease_doneScope();
    // LrfMap_show();

    return 0;
}

#else /*IS_MAKE_DATASET*/
int main(void)
{
    CCInitializer_init();
    TCInitializer_init();
    PCInitializer_init();
    CSInitializer_init();

    TCLauncher_launchAll();

    LrfMap_t map = LrfMap_create(500, 500, 0, 499);
    LrfMap_addBox(map, 50, 180, 80, 150);
    LrfMap_addBox(map, 450, 180, 480, 150);
    LrfMap_addBox(map, 50, 280, 80, 250);
    LrfMap_addBox(map, 450, 280, 480, 250);
    LrfMap_addBox(map, 50, 380, 80, 350);
    LrfMap_addBox(map, 450, 380, 480, 350);

    // LrfMap_addBox(map, 200, 200, 300, 300);
    LrfMap_addBox(map, 0, 0, 10, 100);
    // LrfMap_addLine(0, 0, 300, 300);
    // CCLOG_INFO("%d", LrfMap_isWall(250, 0));
    CCAutoRelease_startScope();
    {
        CC_obj data_set = CCAutoRelease_add(CCDictionary_create());
        CC_obj pos_array = CCArray_create();
        CC_obj distances_array = CCArray_create();
        CCDictionary_setObject(data_set, distances_array, "distances_array");
        CCDictionary_setObject(data_set, pos_array, "pos_array");

        for(size_t x = 0; x < 500; x+=10)
        {
            CCLOG_INFO("%d", x);
            for(size_t y = 0; y < 500; y+=10)
            {
                if(LrfMap_isWall(map, x, y) == CC_BOOL_FALSE)
                {
                    for(size_t ang = 0; ang < 360; ang+=10)
                    {
                        CC_obj pos_data = CCArray_create();
                        CCArray_addObject(pos_data, CCInt32_create(x));
                        CCArray_addObject(pos_data, CCInt32_create(y));
                        CCArray_addObject(pos_data, CCInt32_create(ang));

                        CCArray_addObject(pos_array, pos_data);
                        CCArray_addObject(distances_array, LrfSim_simulate(map, x, y, ang));
                    }
                } 
            }
        }

        CC_obj str = CCAutoRelease_add(CCJsonSerializer_dumps(data_set, CC_BOOL_FALSE));
        CCString_writeFile(str, "data_set.json");

        // CC_obj distances = CCAutoRelease_add(LrfSim_simulate(map, 250, 250, 0));
        // for(size_t x = 0; x < 500; x+=25)
        // {
        //     for(size_t y = 0; y < 500; y+=25)
        //     {
        //         if(LrfMap_isWall(map, x, y) == CC_BOOL_FALSE)
        //         {
        //             size_t diff = LrfEst_culcError(map, x, y, 100, distances);
        //             CCLOG_INFO("%d", diff);
        //             LrfMap_addCircle(map, x, y, diff / 10000); 
        //         } 
        //     }
        // }
        // LrfMap_show(map);
        // LrfSim_simulateViwe(map, 250, 250, 0, distances);
    }
    CCAutoRelease_doneScope();
    // LrfMap_show();

    return 0;
}

#endif /*IS_MAKE_DATASET*/
