#ifndef RMM_CAN_SMBUS_CS_DATA_BASE_H
#define RMM_CAN_SMBUS_CS_DATA_BASE_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSDataBase_initializer(void);

void CSDataBase_setDevice(const char* dev_name, CSType_canId_t dev_id);

void CSDataBase_setRegIn(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type);
void CSDataBase_setRegOut(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type);

void CSDataBase_setSysRegIn(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type);
void CSDataBase_setSysRegOut(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type);

// const char* CSDataBase_readName(CSType_canId_t id);

CSType_canId_t CSDataBase_readId(const char* name);

CSType_canId_t CSDataBase_readRegInId(const char* dev_name, const char* reg_name);
CC_bool_t CSDataBase_regInIsSystem(const char* dev_name, const char* reg_name);
CCVariable_data_t CSDataBase_readRegInType(const char* dev_name, const char* reg_name);

CSType_canId_t CSDataBase_readRegOutId(const char* dev_name, const char* reg_name);
CC_bool_t CSDataBase_regOutIsSystem(const char* dev_name, const char* reg_name);
CCVariable_data_t CSDataBase_readRegOutType(const char* dev_name, const char* reg_name);

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_DATA_BASE_H*/
