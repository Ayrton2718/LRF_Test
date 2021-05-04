#include "map.h"

// http://opencv.jp/opencv-2.1/cpp/basic_structures.html#mat
#include <opencv2/opencv.hpp>


// width : x,
// hight : y

struct LrfMap_inst_t
{
    cv::Mat* map;

    size_t width;
    size_t height;

    size_t offset_x;
    size_t offset_y;
};

static inline int LrfMap_offsetX(struct LrfMap_inst_t* _obj, int x)
{
    return _obj->offset_x + x;
}

static inline int LrfMap_offsetY(struct LrfMap_inst_t* _obj, int y)
{
    return _obj->offset_y - y;
}

LrfMap_t LrfMap_create(size_t x, size_t y, size_t offset_x, size_t offset_y)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)malloc(sizeof(struct LrfMap_inst_t));
    _obj->width = x;
    _obj->height = y;

    _obj->offset_x = offset_x + 100;
    _obj->offset_y = offset_y + 100;

    x = x + 200;
    y = y + 200;

    _obj->map = new cv::Mat(cv::Size(x, y), CV_8UC1);

    cv::rectangle(*_obj->map, cv::Point(LrfMap_offsetX(_obj, -11), LrfMap_offsetY(_obj, -11)), cv::Point(LrfMap_offsetX(_obj, _obj->width + 10), LrfMap_offsetY(_obj, _obj->height + 10)), 255, 10);
    
    return _obj;
}


LrfMap_t LrfMap_copy(LrfMap_t map)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)malloc(sizeof(struct LrfMap_inst_t));
    struct LrfMap_inst_t* _map = (struct LrfMap_inst_t*)map;

    _obj->map = new cv::Mat();
    *_obj->map = _map->map->clone();//new cv::Mat(*_map->map);

    _obj->width = _map->width;
    _obj->height = _map->height;

    _obj->offset_x = _map->offset_x;
    _obj->offset_y = _map->offset_y;

    return _obj;
}


void LrfMap_destructor(LrfMap_t obj)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;

    delete _obj->map;
}


void LrfMap_addBox(LrfMap_t obj, int leftUp_x, int leftUp_y, int rightDown_x, int rightDown_y)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;
    cv::rectangle(*_obj->map, cv::Point(LrfMap_offsetX(_obj, leftUp_x), LrfMap_offsetY(_obj, leftUp_y)), cv::Point(LrfMap_offsetX(_obj, rightDown_x), LrfMap_offsetY(_obj, rightDown_y)), 255, -1);
}


void LrfMap_addCircle(LrfMap_t obj, int center_x, int center_y, int radius)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;
    cv::circle(*_obj->map, cv::Point(LrfMap_offsetX(_obj, center_x), LrfMap_offsetY(_obj, center_y)), radius, 255, -1);
}


void LrfMap_addLine(LrfMap_t obj, int start_x, int start_y, int end_x, int end_y)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;
    cv::line(
        *_obj->map, 
        cv::Point(LrfMap_offsetX(_obj, start_x), LrfMap_offsetY(_obj, start_y)), 
        cv::Point(LrfMap_offsetX(_obj, end_x), LrfMap_offsetY(_obj, end_y)), 
        255, 1);

}


CC_bool_t LrfMap_isWall(LrfMap_t obj, int x, int y)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;

    if(LrfMap_isContain(obj, x, y) == CC_BOOL_FALSE)
    {
        return CC_BOOL_TRUE;
    }

    if(_obj->map->at<cv::uint8_t>(cv::Point(LrfMap_offsetX(_obj, x), LrfMap_offsetY(_obj, y))) != 0)
    {
        return CC_BOOL_TRUE;
    }else{
        return CC_BOOL_FALSE;
    }
}

void LrfMap_putRobot(LrfMap_t obj, int x, int y, int yaw)
{
    LrfMap_addCircle(obj, x, y, 3);
    float rad = (float)((yaw + 90) * M_PI) / 180;
    LrfMap_addLine(obj, x, y, x + 10 * cos(rad), y + 10 * sin(rad));
}

// void LrfMap_putDot(int x, int y, uint8_t col)
// {
//     if(LrfMap_isContain(x, y) == CC_BOOL_FALSE)
//     {
//         return;
//     }

//     g_root.map.at<cv::uint8_t>(cv::Point(LrfMap_offsetX(x), LrfMap_offsetX(y))) = col;
// }

CC_bool_t LrfMap_isContain(LrfMap_t obj, int x, int y)
{
    // struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;

    // x =  LrfMap_offsetX(_obj, x);
    // y =  LrfMap_offsetY(_obj, y);

    if(x < 0)
    {
        return CC_BOOL_FALSE;
    }else if((int)LrfMap_width(obj) <= x){
        return CC_BOOL_FALSE;
    }
    
    if(y < 0)
    {
        return CC_BOOL_FALSE;
    }else if((int)LrfMap_height(obj) <= y){
        return CC_BOOL_FALSE;
    }

    return CC_BOOL_TRUE;
}


size_t LrfMap_width(LrfMap_t obj)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;
    return _obj->width;
}

size_t LrfMap_height(LrfMap_t obj)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;
    return _obj->height;
}


void LrfMap_show(LrfMap_t obj, const char* tag)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;

    cv::imshow(tag, *_obj->map);
    cv::waitKey(1);
}

void LrfMap_showWait(LrfMap_t obj, const char* tag)
{
    struct LrfMap_inst_t* _obj = (struct LrfMap_inst_t*)obj;

    cv::imshow(tag, *_obj->map);
    cv::waitKey(0);
}
