#include "display.h"

#include "map.h"

#include <opencv2/opencv.hpp>


LrfMap_t LrfDisplay_create(LrfMap_t map)
{
    return LrfMap_copy(map);
}

void LrfDisplay_reload(LrfMap_t* obj, LrfMap_t map)
{
    LrfMap_destructor(*obj);
    *obj = LrfMap_copy(map);
}
