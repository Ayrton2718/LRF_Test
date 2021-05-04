#ifndef RMM_DRIVERS_LRF_MAP_H
#define RMM_DRIVERS_LRF_MAP_H

#include <thread_comm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* LrfMap_t;

 // mm? cm?
LrfMap_t LrfMap_create(size_t x, size_t y, size_t offset_x, size_t offset_y);

LrfMap_t LrfMap_copy(LrfMap_t map);

void LrfMap_destructor(LrfMap_t map);

void LrfMap_addBox(LrfMap_t obj, int leftUp_x, int leftUp_y, int rightDown_x, int rightDown_y);

void LrfMap_addCircle(LrfMap_t obj, int center_x, int center_y, int radius);

void LrfMap_addLine(LrfMap_t obj, int start_x, int start_y, int end_x, int end_y);

void LrfMap_show(LrfMap_t obj, const char* tag);
void LrfMap_showWait(LrfMap_t obj, const char* tag);

CC_bool_t LrfMap_isWall(LrfMap_t obj, int x, int y);

void LrfMap_putRobot(LrfMap_t obj, int x, int y, int yaw);

CC_bool_t LrfMap_isContain(LrfMap_t obj, int x, int y);

size_t LrfMap_width(LrfMap_t obj);
size_t LrfMap_height(LrfMap_t obj);

#ifdef __cplusplus
}
#endif

#endif /*RMM_DRIVERS_LRF_MAP_H*/
