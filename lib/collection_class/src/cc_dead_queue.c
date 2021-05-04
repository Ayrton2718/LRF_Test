#include "cc_dead_queue.h"

#include "basic_queue.h"

#include "cc_base_object.h"
#include "cc_log.h"
#include "cc_locker.h"
#include <pthread.h>
#include <unistd.h>

#define CC_DEAD_QUEUE_QUEUE_COUNT   (256)
#define CC_DEAD_QUEUE_DEADLINE      (500) //ms


struct CCDeadQueue_objTimeInfo_t
{
    clock_t added_time;
    CC_obj obj;
};

struct CCDeadQueue_t
{
    pthread_mutex_t     locker;

    pthread_t           thread;

    struct BasicQueue_t queue;
    struct CCDeadQueue_objTimeInfo_t queue_buffer[CC_DEAD_QUEUE_QUEUE_COUNT];
};

static struct CCDeadQueue_t g_root;


void* CCDeadQueue_thread(void* data);


void CCDeadQueue_initializer(void)
{
    BasicQueue_init(&g_root.queue, sizeof(struct CCDeadQueue_objTimeInfo_t), CC_DEAD_QUEUE_QUEUE_COUNT, g_root.queue_buffer);

    CCLocker_lockerInit(&g_root.locker);

    if(pthread_create(&g_root.thread, NULL, CCDeadQueue_thread, NULL) != 0)
    {
        CCLOG_ERROR_NOFMT("Dead queue thread start failed.");
    }
    CCLog_setThreadName(g_root.thread, "CCDeadQueue");
}

static inline clock_t CCDeadQueue_getMillis(void)
{
    return clock() / (CLOCKS_PER_SEC / 1000);
}


void CCDeadQueue_add(CC_obj obj)
{
    struct CCDeadQueue_objTimeInfo_t time_info;
    time_info.added_time = CCDeadQueue_getMillis();
    time_info.obj = obj;

    CC_bool_t is_filled = CC_BOOL_FALSE;
    struct CCDeadQueue_objTimeInfo_t fillout;

    CCLocker_lockerLock(&g_root.locker);
    if(BasicQueue_isFill(&g_root.queue))
    {
        is_filled = CC_BOOL_TRUE;
        BasicQueue_get(&g_root.queue, &fillout);
    }    
    BasicQueue_add(&g_root.queue, &time_info);
    CCLocker_lockerUnlock(&g_root.locker);


    if(is_filled == CC_BOOL_TRUE)
    {
        // CCLOG_INFO("fill kill. %p", fillout.obj);
        CCBaseObject_destructor(fillout.obj);
    }
}

void* CCDeadQueue_thread(void* data)
{
    while(1)
    {
        struct CCDeadQueue_objTimeInfo_t time_info;
        CCLocker_lockerLock(&g_root.locker);
        int is_available = BasicQueue_get(&g_root.queue, &time_info);
        CCLocker_lockerUnlock(&g_root.locker);

        if(is_available == 1 && CCBaseObject_isObject(time_info.obj) == CC_BOOL_TRUE)
        {
            if(CCDeadQueue_getMillis() < time_info.added_time + CC_DEAD_QUEUE_DEADLINE)
            {
                usleep(time_info.added_time + CC_DEAD_QUEUE_DEADLINE - CCDeadQueue_getMillis());
            }

            if(CCBaseObject_referenceCount(time_info.obj) <= 0)
            {
                // CCLOG_INFO("kill %p", time_info.obj);
                CCBaseObject_destructor(time_info.obj);
            }else{
                // CCLOG_INFO("no kill %p", time_info.obj);
            }
        }else{
            CCType_yield();
        }
    }

    return NULL;
}
