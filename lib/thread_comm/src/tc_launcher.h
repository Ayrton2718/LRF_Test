#ifndef TC_LAUNCHER_H
#define TC_LAUNCHER_H

#include <collection_class.h>

#ifdef __cplusplus
extern "C" {
#endif

void TCLauncher_init(void);

typedef void (*TCLauncher_launch_function_t)(void);

void TCLauncher_addLaunchFunction(TCLauncher_launch_function_t function_pointer, const char* node_name);

void TCLauncher_launchAll(void);

// void TCLauncher_nodeName(char* name_out);
void TCLauncher_nodeName(char* name_out);

void TCLauncher_waitForever(void);
void TCLauncher_waitOnece(void);

void TCLauncher_waitDuration(int duration_ms);

typedef void (*TCLauncher_callback_t)(CC_obj arg);

void TCLauncher_registerCallback(TCLauncher_callback_t callback, CC_obj args_queue);

#ifdef __cplusplus
}
#endif

#endif /*TC_LAUNCHER_H*/
