#include "cc_locker.h"

#include "cc_type.h"

#include <stdio.h>
#include <signal.h>

void CCLocker_lockerLock(pthread_mutex_t* locker)
{
    while(pthread_mutex_trylock(locker) != 0)
    {
        CCType_yield();
    }
}

void CCLocker_lockerUnlock(pthread_mutex_t* locker)
{
    int result = pthread_mutex_unlock(locker); 
    if(result != 0)
    {
        printf("locker unlock : %d\n", result);
        fflush(stdout);
        exit(1);
        // CCLOG_ERROR_NOFMT("locker init failed.");
    }
}

void CCLocker_lockerInit(pthread_mutex_t* locker)
{
    int result = pthread_mutex_init(locker, NULL); 
    if(result != 0)
    {
        printf("locker init : %d\n", result);
        fflush(stdout);
        exit(1);
        // CCLOG_ERROR_NOFMT("locker init failed.");
    }
}

// #define CCLocker_lockerDestroy(locker) pthread_mutex_destroy(locker)
void CCLocker_lockerDestroy(pthread_mutex_t* locker)
{
    int result = pthread_mutex_destroy(locker); 
    if(result != 0)
    {
        printf("locker destroy : %d\n", result);
        fflush(stdout);
        exit(1);
        // CCLOG_ERROR_NOFMT("locker destroy failed.");
    }
}