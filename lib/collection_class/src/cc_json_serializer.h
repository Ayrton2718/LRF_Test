#ifndef CC_JSON_SERIALIZER_H
#define CC_JSON_SERIALIZER_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

CC_obj CCJsonSerializer_dumps(CC_obj obj, CC_bool_t visible);

void CCJsonSerializer_staticLoads(CC_obj json, CC_obj obj);
CC_obj CCJsonSerializer_dynamicLoads(CC_obj json);

#ifdef __cplusplus
}
#endif

#endif /*CC_JSON_SERIALIZER_H*/
