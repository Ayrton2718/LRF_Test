#ifndef CC_BYTES_H
#define CC_BYTES_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

CC_obj CCBytes_create(size_t count);

void CCBytes_setBuff(CC_obj obj, const uint8_t* bytes);

void CCBytes_getBuff(CC_obj obj, uint8_t* bytes);

#ifdef __cplusplus
}
#endif

#endif /*CC_BYTES_H*/
