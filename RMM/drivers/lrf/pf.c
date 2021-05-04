#include "pf.h"

#include <math.h>

struct LrfPf_info_t
{
    LrfPf_pos_t node[LRFPF_NODE_COUNT];
    float weights[LRFPF_NODE_COUNT];
};

LrfPf_t LrfPf_create(LrfPf_pos_t init_pos)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)malloc(sizeof(struct LrfPf_info_t));
    memset(_obj, 0x00, sizeof(struct LrfPf_info_t));
    for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
    {
        _obj->node[i] = init_pos;
    }
    return _obj;
}


void LrfPf_predict(LrfPf_t obj, LrfPf_pos_t d_move)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)obj;
    for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
    {
        _obj->node[i].x += d_move.x + ((rand() % 40) - 20);
        _obj->node[i].y += d_move.y + ((rand() % 40) - 20);
        _obj->node[i].yaw += d_move.yaw + ((rand() % 10) - 5);

        // _obj->node[i].x += d_move.x + ((rand() % 20) - 10);
        // _obj->node[i].y += d_move.y + ((rand() % 20) - 10);
        // _obj->node[i].yaw += d_move.yaw + ((rand() % 10) - 5);

        // _obj->node[i].x += d_move.x + ((rand() % 10) - 5);
        // _obj->node[i].y += d_move.y + ((rand() % 10) - 5);
        // _obj->node[i].yaw += d_move.yaw + ((rand() % 4) - 2);
    }
}


size_t LrfPf_nodeCount(LrfPf_t obj)
{
    return LRFPF_NODE_COUNT;
}


LrfPf_pos_t LrfPf_nodePos(LrfPf_t obj, size_t node_num)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)obj;
    return _obj->node[node_num];
}


void LrfPf_setWeight(LrfPf_t obj, size_t node_num, float weight)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)obj;
    _obj->weights[node_num] = weight;
}


LrfPf_pos_t LrfPf_estimate(LrfPf_t obj)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)obj;
    
    float calc_x = 0;
    float calc_y = 0;
    float calc_yaw = 0;

    float max_weights[5] = {0};
    size_t max_weights_index[5] = {0};
    // float calc_x = 0;
    // float calc_y = 0;
    // float calc_yaw = 0;

    float cutoff_weight_sum = 0;
    float cutoff_calc_x = 0;
    float cutoff_calc_y = 0;
    float cutoff_calc_yaw = 0;

    for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
    {
        calc_x += _obj->node[i].x * _obj->weights[i];
        calc_y += _obj->node[i].y * _obj->weights[i];
        calc_yaw += _obj->node[i].yaw * _obj->weights[i];

        if(0.030 < _obj->weights[i])
        {
            cutoff_weight_sum += _obj->weights[i];
            cutoff_calc_x += _obj->node[i].x * _obj->weights[i];
            cutoff_calc_y += _obj->node[i].y * _obj->weights[i];
            cutoff_calc_yaw += _obj->node[i].yaw * _obj->weights[i];
            CCLOG_INFO("cutoff : (%d, %d, %d), %f", _obj->node[i].x, _obj->node[i].y, _obj->node[i].yaw, _obj->weights[i]);
        }

        // for(size_t )
        if(max_weights[4] < _obj->weights[i])
        {
            if(max_weights[3] < _obj->weights[i])
            {
                if(max_weights[2] < _obj->weights[i])
                {
                    if(max_weights[1] < _obj->weights[i])
                    {
                        if(max_weights[0] < _obj->weights[i])
                        {
                            max_weights[0] = _obj->weights[i];
                            max_weights_index[0] = i;
                        }else{
                            max_weights[1] = _obj->weights[i];
                            max_weights_index[1] = i;
                        }
                    }else{
                        max_weights[2] = _obj->weights[i];
                        max_weights_index[2] = i;
                    }
                }else{
                    max_weights[3] = _obj->weights[i];
                    max_weights_index[3] = i;
                }
            }else{
                max_weights[4] = _obj->weights[i];
                max_weights_index[4] = i;
            }
        }
        // if(max_weight < _obj->weights[i])
        // {
        //     max_weight = _obj->weights[i];
        //     max_weight_index = i;
        // }
    }

    cutoff_calc_x = cutoff_calc_x / cutoff_weight_sum;    
    cutoff_calc_y = cutoff_calc_y / cutoff_weight_sum;    
    cutoff_calc_yaw = cutoff_calc_yaw / cutoff_weight_sum;    
    CCLOG_INFO("cutoff : (%f, %f, %f)", cutoff_calc_x, cutoff_calc_y, cutoff_calc_yaw);

    float top_calc_x = 0;
    float top_calc_y = 0;
    float top_calc_yaw = 0;
    float weight_sum = 0;

    for(size_t i = 0; i < 5; i++)
    {
        weight_sum += max_weights[i];
        top_calc_x += _obj->node[max_weights_index[i]].x * max_weights[i];
        top_calc_y += _obj->node[max_weights_index[i]].y * max_weights[i];
        top_calc_yaw += _obj->node[max_weights_index[i]].yaw * max_weights[i];

        CCLOG_INFO("(%d, %d, %d), %f", _obj->node[max_weights_index[i]].x, _obj->node[max_weights_index[i]].y, _obj->node[max_weights_index[i]].yaw, max_weights[i]);
    }

    top_calc_x = top_calc_x / weight_sum;
    top_calc_y = top_calc_y / weight_sum;
    top_calc_yaw = top_calc_yaw / weight_sum;
    CCLOG_INFO("top : (%f, %f, %f)", top_calc_x, top_calc_y, top_calc_yaw);

    LrfPf_pos_t result;
    result.x = roundtol(calc_x);//(int)calc_x;
    result.y = roundtol(calc_y);//(int)calc_y;
    result.yaw = roundtol(calc_yaw);//(int)calc_yaw;

    return result;
}


void LrfPf_resampling(LrfPf_t obj)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)obj;

    float recip_ess = 0;
    for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
    {
        recip_ess += _obj->weights[i] * _obj->weights[i];
    }

    float ess = 1 / recip_ess;
    if(ess < (LRFPF_NODE_COUNT / 2))
    {
        LrfPf_pos_t befo_nodes[LRFPF_NODE_COUNT];
        memcpy(befo_nodes, _obj->node, sizeof(LrfPf_pos_t) * LRFPF_NODE_COUNT);
        float sp = 1 / 2 * LRFPF_NODE_COUNT;
        for(size_t target_node = 0; target_node < LRFPF_NODE_COUNT; target_node++)
        {
            float w_sum = 0;
            for(size_t search_node = 0; search_node < LRFPF_NODE_COUNT; search_node++)
            {
                if(sp < w_sum)
                {
                    _obj->node[target_node] = befo_nodes[search_node - 1];
                }
                w_sum += _obj->weights[search_node];
            }

            sp += 1 / LRFPF_NODE_COUNT;
        }
    }
}

void LrfPf_show(LrfPf_t obj, LrfMap_t disp)
{
    struct LrfPf_info_t* _obj = (struct LrfPf_info_t*)obj;

    for(size_t i = 0; i < LRFPF_NODE_COUNT; i++)
    {
        LrfMap_putRobot(disp, _obj->node[i].x, _obj->node[i].y, _obj->node[i].yaw);
    }
}
