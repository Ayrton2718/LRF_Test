#include "cc_log.h"

#include "cc_type.h"
#include "cc_locker.h"

#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


struct CCLog_threadInfo_t
{
    pthread_t id;
    char name[256];
};

struct CCLog_t
{
    pthread_mutex_t locker;

    struct timeval  befo_sampled;

    size_t  thread_count;
    struct CCLog_threadInfo_t* thread_list;

    // char file_path[512];
    FILE* out;
};

static struct CCLog_t g_log;

static const char* g_thread_unknown = "unknown";

void CCLog_init(void)
{
    CCLocker_lockerInit(&g_log.locker);

    gettimeofday(&g_log.befo_sampled, NULL);
    g_log.out = stdout;

    g_log.thread_count = 1;
    g_log.thread_list = (struct CCLog_threadInfo_t*)malloc(sizeof(struct CCLog_threadInfo_t) * g_log.thread_count);
    g_log.thread_list[0].id = pthread_self();
    strcpy(g_log.thread_list[0].name, "base");

    CCLOG_INFO_NOFMT("Log started!");
}

void CCLog_setFilePath(const char* path)
{
    FILE* f = fopen(path, "w");

    if(f != NULL)
    {
        CCLocker_lockerLock(&g_log.locker);
        g_log.out = f;
        CCLocker_lockerUnlock(&g_log.locker);
    }

    CCLOG_INFO("Log file changed to %s!", path);
}

void CCLog_setThreadName(pthread_t id, const char* name)
{
    CCLocker_lockerLock(&g_log.locker);
    g_log.thread_count++;
    g_log.thread_list = (struct CCLog_threadInfo_t*)realloc(g_log.thread_list, sizeof(struct CCLog_threadInfo_t) * g_log.thread_count);
    g_log.thread_list[g_log.thread_count - 1].id = id;
    strcpy(g_log.thread_list[g_log.thread_count - 1].name, name);
    CCLocker_lockerUnlock(&g_log.locker);
}


const char* CCLog_getThreadName(pthread_t thread_id)
{
    const char* thread_name = g_thread_unknown;

    CCLocker_lockerLock(&g_log.locker);
    for(size_t i = 0; i < g_log.thread_count; i++)
    {
        if(pthread_equal(g_log.thread_list[i].id, thread_id) != 0)
        {
            thread_name = (const char*)g_log.thread_list[i].name;
            break;
        }
    }
    CCLocker_lockerUnlock(&g_log.locker);

    return thread_name;
}

void CCLog_withFormat(const char* file, size_t line, const char* tag, int diff_time, const char* fmt, ...)
{
    char debug_string[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(debug_string, 4095, fmt, ap);
    debug_string[4095] = 0x00;
    va_end(ap);

    CCLog_noFormat(file, line, tag, diff_time, debug_string);
}

void CCLog_noFormat(const char* file, size_t line, const char* tag, int diff_time, const char* str)
{
    struct timeval  now_time;
    struct tm       now_localtime;
    FILE*           f;

    gettimeofday(&now_time, NULL);

    pthread_t thread_id = pthread_self();
    const char* thread_name = g_thread_unknown;

    CCLocker_lockerLock(&g_log.locker);
    f = g_log.out;
    memcpy(&now_localtime, localtime(&now_time.tv_sec), sizeof(struct tm));

    for(size_t i = 0; i < g_log.thread_count; i++)
    {
        if(pthread_equal(g_log.thread_list[i].id, thread_id) != 0)
        {
            thread_name = (const char*)g_log.thread_list[i].name;
            break;
        }
    }
    CCLocker_lockerUnlock(&g_log.locker);

    if(diff_time == 0)
    {
#ifdef __APPLE__
        fprintf(f, "%02d:%02d:%02d.%06d [%s] <%s>\"%s:%zu\" %s\n",
            now_localtime.tm_hour, now_localtime.tm_min, now_localtime.tm_sec, now_time.tv_usec,
            tag, thread_name, file, line, str);
#else
        fprintf(f, "%02d:%02d:%02d.%06ld [%s] <%s>\"%s:%zu\" %s\n",
            now_localtime.tm_hour, now_localtime.tm_min, now_localtime.tm_sec, now_time.tv_usec,
            tag, thread_name, file, line, str);
#endif
    }else{
        struct timeval  befo_sampled, diff_time;
        CCLocker_lockerLock(&g_log.locker);
        befo_sampled = g_log.befo_sampled;
        g_log.befo_sampled = now_time;
        CCLocker_lockerUnlock(&g_log.locker);

        timersub(&now_time, &befo_sampled, &diff_time);

#ifdef __APPLE__
        fprintf(f, "%02d:%02d:%02d.%06d [%s] <%s>\"%s:%zu\" %s, diff\"%02ld.%06d\" \n",
                now_localtime.tm_hour, now_localtime.tm_min, now_localtime.tm_sec, now_time.tv_usec,
            tag, thread_name, file, line, str, diff_time.tv_sec, diff_time.tv_usec);
#else
        fprintf(f, "%02d:%02d:%02d.%06ld [%s] <%s>\"%s:%zu\" %s, diff\"%02ld.%06ld\" \n",
                now_localtime.tm_hour, now_localtime.tm_min, now_localtime.tm_sec, now_time.tv_usec,
            tag, thread_name, file, line, str, diff_time.tv_sec, diff_time.tv_usec);
#endif
    }
}
