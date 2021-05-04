#ifndef CC_FLOAT_H
#define CC_FLOAT_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

CC_obj CCFloat_create(float init_value);

void CCFloat_set(CC_obj obj, float set_value);

float CCFloat_get(CC_obj obj);

#ifdef __cplusplus
}
#endif

#endif /*CC_FLOAT_H*/
