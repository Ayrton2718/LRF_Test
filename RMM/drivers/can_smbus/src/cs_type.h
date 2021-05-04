#ifndef RMM_CAN_SMBUS_CS_TYPE_H
#define RMM_CAN_SMBUS_CS_TYPE_H

#include <thread_comm.h>

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t CSType_canId_t;
typedef uint8_t CSType_canReg_t;

void CSType_delay(float second);

void CSType_delay_us(size_t us);
void CSType_delay_ms(size_t ms);


// typedef enum
// {
// 	CSType_data_Int32,
// 	CSType_data_UInt32,
// 	CSType_data_Int16,
// 	CSType_data_UInt16,
// 	CSType_data_Int8,
// 	CSType_data_UInt8,

// 	CCType_data_Float
// } CSType_data_t;

// size_t CSType_getDataSize(CSType_data_t data_type);
// CC_bool_t CSType_isBinarryType(CSType_data_t data_type);

// CC_obj CSType_createtDataObj(CSType_data_t data_type);
// void CSType_setDataObj(CC_obj obj, CSType_data_t data_type, void* data);


inline static CSType_canId_t CSType_getIdNumber(CSType_canId_t id)
{
	return id & 0x1FF;
}


inline static CSType_canId_t CSType_getIdSysWrite(CSType_canId_t id)
{
	return CSType_getIdNumber(id);
}

inline static CSType_canId_t CSType_getIdSysRead(CSType_canId_t id)
{
	return CSType_getIdNumber(id) | 0x200;
}

inline static CSType_canId_t CSType_getIdUserWrite(CSType_canId_t id)
{
	return CSType_getIdNumber(id) | 0x400;
}

inline static CSType_canId_t CSType_getIdUserRead(CSType_canId_t id)
{
	return CSType_getIdNumber(id) | 0x600;
}


inline static CC_bool_t CSType_isIdSysWrite(CSType_canId_t id)
{
	return (id & 0x600) == 0x000;
}

inline static CC_bool_t CSType_isIdSysRead(CSType_canId_t id)
{
	return (id & 0x600) == 0x200;
}

inline static CC_bool_t CSType_isIdUserWrite(CSType_canId_t id)
{
	return (id & 0x600) == 0x400;
}

inline static CC_bool_t CSType_isIdUserRead(CSType_canId_t id)
{
	return (id & 0x600) == 0x600;
}



inline static CSType_canReg_t CSType_getRegNumber(CSType_canReg_t reg)
{
	return reg & 0x0F;
}

inline static CSType_canReg_t CSType_getRegWrite(CSType_canReg_t reg)
{
	return CSType_getRegNumber(reg);
}

inline static CSType_canReg_t CSType_getRegRequest(CSType_canReg_t reg)
{
	return CSType_getRegNumber(reg) | 0x10;
}

inline static CSType_canReg_t CSType_getRegResponse(CSType_canReg_t reg)
{
	return CSType_getRegNumber(reg) | 0x20;
}

inline static CC_bool_t CSType_isRegWrite(CSType_canReg_t reg)
{
	return (reg & 0xF0) == 0x00;
}

inline static CC_bool_t CSType_isRegRequest(CSType_canReg_t reg)
{
	return (reg & 0xF0) == 0x10;
}

inline static CC_bool_t CSType_isRegResponse(CSType_canReg_t reg)
{
	return (reg & 0xF0) == 0x20;
}

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_TYPE_H*/
