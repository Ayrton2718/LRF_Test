#ifndef CC_BOOL_H
#define CC_BOOL_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

CC_obj CCBool_create(CC_bool_t init_value);

void CCBool_set(CC_obj obj, CC_bool_t set_value);

CC_bool_t CCBool_get(CC_obj obj);

#ifdef __cplusplus
}
#endif

#endif /*CC_BOOL_H*/
