#ifndef CC_OBJECT_H
#define CC_OBJECT_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CCObject_retain(CC_obj obj);
void CCObject_release(CC_obj obj);

void CCObject_enableThreadSafe(CC_obj obj);

CC_obj CCObject_copy(CC_obj obj);
CC_bool_t CCObject_replace(CC_obj obj, CC_obj obj_from);

CC_bool_t CCObject_isObject(CC_obj obj);

CC_obj CCObject_info(CC_obj obj);

#ifdef __cplusplus
}
#endif

#endif /*CC_OBJECT_H*/
