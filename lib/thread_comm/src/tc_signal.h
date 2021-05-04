#ifndef TC_SIGNAL_H
#define TC_SIGNAL_H

#include <collection_class.h>

#ifdef __cplusplus
extern "C" {
#endif

void TCSignal_create(const char* path, const char* name);

typedef void* TCSignal_t;

TCSignal_t TCSignal_talker_connect(const char* path, const char* name);
void TCSignal_talker_disconnect(TCSignal_t obj);
void TCSignal_talker_send(TCSignal_t obj, CC_obj set_obj);

TCSignal_t TCSignal_listener_connect(const char* path, const char* name, const char* listener_name);
void TCSignal_listener_disconnect(TCSignal_t obj);
size_t TCSignal_listener_readbleCount(TCSignal_t obj);
CC_obj TCSignal_listener_receive(TCSignal_t obj);
CC_obj TCSignal_listener_receiveWait(TCSignal_t obj);
CC_bool_t TCSignal_listener_isAvailable(TCSignal_t obj);

#ifdef __cplusplus
}
#endif

#endif /*TC_SIGNAL_H*/
