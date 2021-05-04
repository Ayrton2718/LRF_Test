#ifndef RMM_CAN_SMBUS_CS_SENDER_H
#define RMM_CAN_SMBUS_CS_SENDER_H

#include <thread_comm.h>
#include "cs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSSender_initializer(void);

CC_obj CSSender_open(const char* dev_name, const char* reg_name);
void CSSender_close(CC_obj obj);
void CSSender_write(CC_obj obj, CCVariable_t variable);

void CSRequester_setModeRequest(const char* dev_name, const char* reg_name);
void CSRequester_setModeSetSend(const char* dev_name, const char* reg_name);
void CSRequester_setModeInterval(const char* dev_name, const char* reg_name, uint16_t ms);

CC_obj CSRequester_open(const char* dev_name, const char* reg_name);
void CSRequester_close(CC_obj obj);
void CSRequester_request(CC_obj obj);

#ifdef __cplusplus
}
#endif

#endif /*RMM_CAN_SMBUS_CS_SENDER_H*/
