#ifndef CC_BINARY_H
#define CC_BINARY_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

CC_obj CCBinary_create(size_t data_size);

void CCBinary_set(CC_obj obj, const void* data_in);

void CCBinary_get(CC_obj obj, void* data_out);

// size_t CCBinary_dataSize(CC_obj obj);

#ifdef __cplusplus
}
#endif

#endif /*CC_BINARY_H*/
