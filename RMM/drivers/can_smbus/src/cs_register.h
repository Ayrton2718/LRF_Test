#ifndef RMM_CAN_SMBUS_CS_REGISTER_H
#define RMM_CAN_SMBUS_CS_REGISTER_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSRegister_initializer(void);


typedef void (*CSRegister_callback_t)(CC_obj args, CCVariable_data_t data_type, const void* data);

void CSRegister_bind(CC_bool_t is_system, CSType_canId_t id, CSType_canReg_t reg, CC_obj args, CCVariable_data_t data_type, CSRegister_callback_t func);

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_REGISTER_H*/
