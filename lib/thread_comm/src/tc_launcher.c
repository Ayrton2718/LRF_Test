#include "tc_launcher.h"
#include "tc_directory.h"

#include <pthread.h>

struct TCLauncher_info_t
{
    pthread_t thread_id;
    char name[64];
    TCLauncher_launch_function_t f_ptr;
    CC_obj callbacks;
};

struct TCLauncher_t
{
    CC_obj threads;
};


static void* TCLauncher_launchFunc(void* arg);
static CC_bool_t TCLauncher_objectAtThread(struct TCLauncher_info_t* info);

//threads---Array
//              |---bin(thread_id)
//              |---String(name)
//              |---bin(TCLauncher_launch_function_t)
//              |---Array
//                    |---callbacks

struct TCLauncher_t g_launcher;

void TCLauncher_init(void)
{
    g_launcher.threads = CCArray_create();
    TCDirectory_mkobj("", "threads", g_launcher.threads);
}

void TCLauncher_addLaunchFunction(TCLauncher_launch_function_t function_pointer, const char* node_name)
{
    struct TCLauncher_info_t info;
    strcpy(info.name, node_name);
    info.f_ptr = function_pointer;
    info.callbacks = CCArray_create();

    CC_obj info_obj = CCBinary_create(sizeof(struct TCLauncher_info_t));
    CCBinary_set(info_obj, &info);

    CCObject_enableThreadSafe(info_obj);
    CCArray_addObject(g_launcher.threads, info_obj);
}

static void* TCLauncher_launchFunc(void* arg)
{
    pthread_mutex_t* mutex = (pthread_mutex_t*)arg;
    CCLocker_lockerLock(mutex);
    CCLocker_lockerUnlock(mutex);
    CCLocker_lockerDestroy(mutex);
    free(mutex);

    struct TCLauncher_info_t info;
    if(CC_BOOL_TRUE == TCLauncher_objectAtThread(&info))
    {
        CCLog_setThreadName(info.thread_id, info.name);
        CCLOG_INFO("tasks %s start!", info.name);
        info.f_ptr();
        CCLOG_INFO("tasks %s done!", info.name);
    }

    return NULL;
}

static CC_bool_t TCLauncher_objectAtThread(struct TCLauncher_info_t* info)
{
    pthread_t this_thread_id = pthread_self();

    for(size_t i = 0; i < CCArray_count(g_launcher.threads); i++)
    {
        CC_obj info_obj = CCArray_objectAtIndex(g_launcher.threads, i);
        CCBinary_get(info_obj, info);

        if(0 < pthread_equal(this_thread_id, info->thread_id))
        {
            return CC_BOOL_TRUE;
        }
    }
    
    return CC_BOOL_FALSE;
}

void TCLauncher_launchAll(void)
{
    for(size_t i = 0; i < CCArray_count(g_launcher.threads); i++)
    {
        struct TCLauncher_info_t info;
        
        CC_obj info_obj = CCArray_objectAtIndex(g_launcher.threads, i);
        CCBinary_get(info_obj, &info);

        pthread_t thread_id;
        pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        CCLocker_lockerInit(mutex);
        CCLocker_lockerLock(mutex);

        pthread_create(&thread_id, NULL, TCLauncher_launchFunc, mutex);
        info.thread_id = thread_id;
        CCBinary_set(info_obj, &info);
        
        pthread_mutex_unlock(mutex);
    }
}

void TCLauncher_nodeName(char* name_out)
{
    struct TCLauncher_info_t info;
    if(CC_BOOL_TRUE == TCLauncher_objectAtThread(&info))
    {
        strcpy(name_out, info.name);
    }
    strcpy(name_out, "Unknown");
}

void TCLauncher_waitForever(void);
void TCLauncher_waitOnece(void);

void TCLauncher_waitDuration(int duration_ms);

void TCLauncher_registerCallback(TCLauncher_callback_t callback, CC_obj args_queue);
