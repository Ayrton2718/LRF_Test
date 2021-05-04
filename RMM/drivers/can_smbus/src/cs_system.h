#ifndef RMM_CAN_SMBUS_CS_SYSTEM_H
#define RMM_CAN_SMBUS_CS_SYSTEM_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSSystem_initializer(void);

void CSSystem_setDevice(const char* dev_name);

void CSSystem_arp(void);

void CSSystem_safety_on(void);
void CSSystem_safety_off(void);

void CSSystem_getDebugMsg(const char* dev_name);

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_SYSTEM_H*/
