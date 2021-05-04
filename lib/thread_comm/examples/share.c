#include "stdio.h"
#include "rmm_thread_comm.h"
#include <unistd.h>

void runner1(void)
{
    CCAutoRelease_startScope();
    {
        CC_obj array = CCAutoRelease_add(CCArray_create());
        CCArray_addObject(array, CCAutoRelease_add(CCString_create("started1")));
        TCShare_Talker_write("share", array);
        CCLOG_INFO("start1");
    }
    CCAutoRelease_doneScope();
    return;
}

void runner2(void)
{
    CCAutoRelease_startScope();
    {
        CC_obj dic = CCAutoRelease_add(CCDictionary_create());
        CCDictionary_setObject(dic, CCAutoRelease_add(CCString_create("started2")), "name");
        TCShare_Talker_write("share", dic);
        CCLOG_INFO("start2");
    }
    CCAutoRelease_doneScope();
    return;
}

int main(void)
{
    CCLog_init();
    CCAutoRelease_ownerInit();
    TCLauncher_init();
    TCShare_init();

    TCShare_makeChannel("share");

    TCLauncher_addLaunchFunction(runner1, "runner1");
    TCLauncher_addLaunchFunction(runner2, "runner2");
    TCLauncher_launchAll();
    CCLOG_INFO("finish");

    sleep(1);

    CCAutoRelease_startScope();
    {
        CC_obj obj = CCAutoRelease_add(TCShare_Listener_read("share"));
        // CCLOG_INFO("%s", CCString_getCString(obj);
        CCLOG_INFO("%s", CCString_getCString(CCAutoRelease_add(CCJsonSerializer_dumps(obj, CC_BOOL_TRUE))));
        // CCLOG_INFO("%s", CCString_getCString(obj));
    }
    CCAutoRelease_doneScope();

    TCShare_removeChannel("share");

    return 0;
}
