#include "cc_initializer.h"
#include "cc_auto_release.h"
#include "cc_log.h"
#include "cc_locker.h"
#include "cc_dead_queue.h"
#include "cc_mem_debugger.h"

void CCInitializer_init(void)
{
    CCLog_init();
    CCMemDebugger_init();

    CCDeadQueue_initializer();
    CCAutoRelease_ownerInit();
}
