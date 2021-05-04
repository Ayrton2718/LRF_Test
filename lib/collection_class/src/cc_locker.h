#ifndef CC_LOCKER_H
#define CC_LOCKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

void CCLocker_lockerLock(pthread_mutex_t* locker);

void CCLocker_lockerUnlock(pthread_mutex_t* locker);

void CCLocker_lockerInit(pthread_mutex_t* locker);

void CCLocker_lockerDestroy(pthread_mutex_t* locker);

#ifdef __cplusplus
}
#endif

#endif /*CC_LOCKER_H*/
