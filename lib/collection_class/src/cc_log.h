#ifndef CC_LOG_H
#define CC_LOG_H

#include <stdio.h>
#include <pthread.h>
#include <signal.h> 

#ifdef __cplusplus
extern "C" {
#endif

#define CCLOG_ERROR(...)        CCLog_withFormat(__FILE__, __LINE__, "ERROR", 0, __VA_ARGS__)//; raise(SIGTRAP)
#define CCLOG_ERROR_NOFMT(str)  CCLog_noFormat(__FILE__, __LINE__, "ERROR", 0, str)//; raise(SIGTRAP)
#define CCLOG_WARNNING(...)         CCLog_withFormat(__FILE__, __LINE__, "WARNNING", 0, __VA_ARGS__)
#define CCLOG_WARNNING_NOFMT(str)   CCLog_noFormat(__FILE__, __LINE__, "WARNNING", 0, str)
#define CCLOG_INFO(...)         CCLog_withFormat(__FILE__, __LINE__, "INFO", 0, __VA_ARGS__)
#define CCLOG_INFO_NOFMT(str)   CCLog_noFormat(__FILE__, __LINE__, "INFO", 0, str)
#define CCLOG_SPEED(...)         CCLog_withFormat(__FILE__, __LINE__, "SPEED", 1, __VA_ARGS__)
#define CCLOG_SPEED_NOFMT(str)   CCLog_noFormat(__FILE__, __LINE__, "SPEED", 1, str)

#define CCLOG_INFO_MANUAL(file, name, ...)         CCLog_withFormat(file, name, "INFO", 0, __VA_ARGS__)
#define CCLOG_WARNNING_MANUAL(file, name, ...)     CCLog_withFormat(file, name, "WARNNING", 0, __VA_ARGS__)
#define CCLOG_ERROR_MANUAL(file, name, ...)        CCLog_withFormat(file, name, "ERROR", 0, __VA_ARGS__)

void CCLog_init(void);
void CCLog_setFilePath(const char* path);

void CCLog_setThreadName(pthread_t id, const char* name);

const char* CCLog_getThreadName(pthread_t id);

void CCLog_withFormat(const char* file, size_t line, const char* tag, int diff_time, const char* fmt, ...);
void CCLog_noFormat(const char* file, size_t line, const char* tag, int diff_time, const char* str);

#ifdef __cplusplus
}
#endif

#endif /*CC_LOG_H*/
