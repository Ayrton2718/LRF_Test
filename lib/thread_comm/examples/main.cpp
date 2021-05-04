// #include "stdio.h"
// #include "rmm_thread_comm.h"
// #include "collection_class/rmm_collection_class.hpp"
// #include <unistd.h>

// void runner1(void)
// {
//     {
//         rmm_cc::CCAutoRelease scope;

//         rmm_cc::CCArray array;
//         scope.add(array);
//         rmm_cc::CCString string("started1");
//         scope.add(string);
//         array.addObject(string);
//         TCShare_Talker_write("share", array.m_obj);
//         CCLOG_INFO("start1");
//     }
// }

// void runner2(void)
// {
//     {
//         rmm_cc::CCAutoRelease scope;
        
//         rmm_cc::CCDictionary dic;
//         scope.add(dic);
//         rmm_cc::CCString string("started2");
//         scope.add(string);
//         dic.setObject(string, "name");
//         TCShare_Talker_write("share", dic.m_obj);
//         CCLOG_INFO("start2");
//     }
// }

// int main(void)
// {
//     CCLog_init();
//     CCAutoRelease_ownerInit();
//     TCLauncher_init();
//     TCShare_init();

//     TCShare_makeChannel("share");

//     TCLauncher_addLaunchFunction(runner1, "runner1");
//     TCLauncher_addLaunchFunction(runner2, "runner2");
//     TCLauncher_launchAll();
//     CCLOG_INFO("finish");

//     sleep(1);

//     CCAutoRelease_startScope();
//     {
//         CC_obj obj = CCAutoRelease_add(TCShare_Listener_read("share"));
//         // CCLOG_INFO("%s", CCString_getCString(obj);
//         CCLOG_INFO("%s", CCString_getCString(CCAutoRelease_add(CCJsonSerializer_dumps(obj, CC_BOOL_TRUE))));
//         // CCLOG_INFO("%s", CCString_getCString(obj));
//     }
//     CCAutoRelease_doneScope();

//     TCShare_removeChannel("share");

//     return 0;
// }
