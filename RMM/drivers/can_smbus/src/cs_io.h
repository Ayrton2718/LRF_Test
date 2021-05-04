#ifndef RMM_CAN_SMBUS_CS_IO_H
#define RMM_CAN_SMBUS_CS_IO_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSIo_initializer(void);

CC_bool_t CSIo_read(CSType_canId_t* id, CSType_canReg_t* reg, size_t* len, uint8_t* data);

void CSIo_send(CSType_canId_t id, CSType_canReg_t reg, size_t len, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_IO_H*/
