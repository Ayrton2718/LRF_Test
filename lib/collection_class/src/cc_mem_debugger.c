#include "cc_mem_debugger.h"

#include <pthread.h>

#include "cc_log.h"
#include "cc_locker.h"
#include "cc_auto_buffer.h"

struct CCMemDebugger_info_t
{
    void* addr;
    pthread_t   thread_id;
    size_t size;
    const char* tag;
};

struct CCMemDebugger_t
{
    pthread_mutex_t         locker;
    struct CCAutoBuffer_t   memorys;
};

static struct CCMemDebugger_t  g_record;


void CCMemDebugger_init(void)
{
    CCLocker_lockerInit(&g_record.locker);
    CCAutoBuffer_create(&g_record.memorys, sizeof(struct CCMemDebugger_info_t));
}

void* CCMemDebugger_malloc(const char* file, size_t line, size_t malloc_size, const char* tag)
{
    struct CCMemDebugger_info_t record_info;
    void* malloc_ptr = malloc(malloc_size);
    
#ifdef CCMEM_LOGOUT_ON
    CCLOG_INFO_MANUAL(file, line, "%s malloc! %p", tag, malloc_ptr);
#endif /*CCMEM_LOGOUT_ON*/

    record_info.addr = malloc_ptr;
    record_info.thread_id = pthread_self();
    record_info.size = malloc_size;
    record_info.tag = tag;

    CCLocker_lockerLock(&g_record.locker);
    CCAutoBuffer_add(&g_record.memorys, &record_info);
    CCLocker_lockerUnlock(&g_record.locker);

    return malloc_ptr;
}

void* CCMemDebugger_realloc(const char* file, size_t line, void* malloc_ptr, size_t size, const char* tag)
{
    void* realloc_ptr = realloc(malloc_ptr, size);
    
#ifdef CCMEM_LOGOUT_ON
    CCLOG_INFO_MANUAL(file, line, "%s reallooc! from %p to %p", tag, malloc_ptr, realloc_ptr);
#endif /*CCMEM_LOGOUT_ON*/

    CCLocker_lockerLock(&g_record.locker);
    for(size_t i = 0; i < CCAutoBuffer_count(&g_record.memorys); i++)
    {
        struct CCMemDebugger_info_t* current_record = (struct CCMemDebugger_info_t*)CCAutoBuffer_readAtIndex_pointer(&g_record.memorys, i);
        if(current_record->addr == malloc_ptr)
        {
            current_record->addr = realloc_ptr;
            current_record->thread_id = pthread_self();
            break;
        }
    }
    CCLocker_lockerUnlock(&g_record.locker);

    return realloc_ptr;
}

void CCMemDebugger_free(const char* file, size_t line, void* malloc_ptr, const char* tag)
{
    free(malloc_ptr);

#ifdef CCMEM_LOGOUT_ON
    CCLOG_INFO_MANUAL(file, line, "%s free! %p", tag, malloc_ptr);
#endif /*CCMEM_LOGOUT_ON*/

    CCLocker_lockerLock(&g_record.locker);
    for(size_t i = 0; i < CCAutoBuffer_count(&g_record.memorys); i++)
    {
        struct CCMemDebugger_info_t* current_record = (struct CCMemDebugger_info_t*)CCAutoBuffer_readAtIndex_pointer(&g_record.memorys, i);
        if(current_record->addr == malloc_ptr)
        {
            CCAutoBuffer_removeAtIndex(&g_record.memorys, i);
            break;
        }
    }
    CCLocker_lockerUnlock(&g_record.locker);
}

void CCMemDebugger_showOwn(const char* tag)
{
    size_t count = 0;
    pthread_t this_thread_id = pthread_self();

    CCLocker_lockerLock(&g_record.locker);
    CCLOG_INFO_NOFMT("Memory state show.");
    for(size_t i = 0; i < CCAutoBuffer_count(&g_record.memorys); i++)
    {
        struct CCMemDebugger_info_t* current_record = (struct CCMemDebugger_info_t*)CCAutoBuffer_readAtIndex_pointer(&g_record.memorys, i);
        if(0 < pthread_equal(current_record->thread_id, this_thread_id))
        {
            if(current_record->tag[0] == tag[0])
            {
                if(strncmp(current_record->tag, tag, 32) == 0)
                {
                    count++;
                    CCLOG_INFO("%s malloced %p, %zu[byte]", current_record->tag, current_record->addr, current_record->size);
                }
            }
        }
    }
    CCLOG_INFO("sum...%zu malloced!", count);
    CCLocker_lockerUnlock(&g_record.locker);
}

void CCMemDebugger_showAll(const char* tag)
{
    size_t count = 0;

    CCLocker_lockerLock(&g_record.locker);
    CCLOG_INFO_NOFMT("Memory state show.");
    for(size_t i = 0; i < CCAutoBuffer_count(&g_record.memorys); i++)
    {
        struct CCMemDebugger_info_t* current_record = (struct CCMemDebugger_info_t*)CCAutoBuffer_readAtIndex_pointer(&g_record.memorys, i);
       if(current_record->tag[0] == tag[0])
        {
            if(strncmp(current_record->tag, tag, 32) == 0)
            {
                count++;
                CCLOG_INFO("%s malloced %p, %zu[byte] from %s", current_record->tag, current_record->addr, current_record->size, CCLog_getThreadName(current_record->thread_id));
            }
        }
    }
    CCLOG_INFO("sum...%zu malloced!", count);
    CCLocker_lockerUnlock(&g_record.locker);
}
