#ifndef RMM_CAN_SMBUS_CS_REG_SIGNAL_H
#define RMM_CAN_SMBUS_CS_REG_SIGNAL_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSRegSignal_initializer(void);

void CSRegSignal_bind(const char* dev_name, const char* reg_name);

void CSRegSignal_close(TCSignal_t signal);


TCSignal_t CSRegSignal_open(const char* dev_name, const char* reg_name, const char* listen_name);

void CSRegSignal_request(TCSignal_t sig);

CCVariable_t CSRegSignal_recv(TCSignal_t sig);


#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_REG_SIGNAL_H*/
