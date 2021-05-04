#ifndef RMM_CAN_SMBUS_CS_REG_SHARE_H
#define RMM_CAN_SMBUS_CS_REG_SHARE_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSRegShare_initializer(void);

void CSRegShare_bind(const char* dev_name, const char* reg_name);

TCShare_t CSRegShare_open(const char* dev_name, const char* reg_name);

void CSRegShare_close(TCShare_t share);

CCVariable_t CSRegShare_read(TCShare_t share);

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_REG_SHARE_H*/
