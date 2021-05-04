#include "thread_comm.h"
#include <unistd.h>

int main(void)
{
    TCInitializer_init();

    CCAutoRelease_startScope();
    {
        CC_obj obj = CCAutoRelease_add(CCBinary_create(sizeof(uint64_t)));
        uint64_t t = 987293847;
        CCBinary_set(obj, &t);
        CCLOG_INFO_NOFMT(CCString_getCString(CCAutoRelease_add(CCJsonSerializer_dumps(obj, 0))));
    }
    CCAutoRelease_doneScope();
    CCMemDebugger_showAll("object");

    return 0;
}

// void runner1(void)
// {
//     CCAutoRelease_startScope();
//     {
//         TCDirectory_mkdir("", "drivers");
//         TCDirectory_mkdir("drivers/", "sensor1");

//         TCShare_create("drivers/sensor1/", "status", CCInt64_create(0));
//         TCShare_create("drivers/sensor1/", "value", CCInt64_create(0));
//         TCSignal_create("drivers/sensor1/", "send");

//         CC_obj value_write = TCShare_writer_connect("drivers/sensor1/", "value");
//         CC_obj send_listener = TCSignal_listener_connect("drivers/sensor1/", "send", "sender");

//         CC_obj status_write = TCShare_writer_connect("drivers/sensor1/", "status");
//         TCShare_writer_write(status_write, CCAutoRelease_add(CCInt64_create(1)));
        
//         CC_obj listen_data = CCAutoRelease_add(CCInt64_create(0));
//         int i = 0;
//         while(i < 2)
//         {
//             if(TCSignal_listener_receiveCheck(send_listener, listen_data))
//             {
//                 TCShare_writer_write(value_write, listen_data);
//                 i++;
//             }
//         }

//         TCShare_writer_disconnect(value_write);
//         TCSignal_listener_disconnect(send_listener);
//         TCShare_writer_disconnect(status_write);
//     }
//     CCAutoRelease_doneScope();

//     CCMemDebugger_show("object");
//     CCMemDebugger_show("locker");
// }

// int main(void)
// {
//     TCInitializer_init();

//     CCAutoRelease_startScope();
//     {
//         CC_obj ls; 
//         ls = CCAutoRelease_add(TCDirectory_ls(""));
//         printf("%s\n", CCString_getCString(ls));

//         TCLauncher_addLaunchFunction(runner1, "runner1");
//         TCLauncher_launchAll();

//         while(TCDirectory_lsObj("drivers/sensor1/", "status") == CC_BOOL_FALSE)
//         {    
//             sleep(1);
//         }

//         ls = CCAutoRelease_add(TCDirectory_ls(""));
//         printf("%s\n", CCString_getCString(ls));
        
//         CC_obj status_read =  TCShare_reader_connect("drivers/sensor1/", "status");
//         // CC_obj result = CCAutoRelease_add(CCInt64_create(0));
//         CCAutoRelease_startScope();
//         {
//             while(1)
//             {
//                 int a;
//                 CCAutoRelease_startScope();
//                 {
//                     CC_obj result = CCAutoRelease_add(TCShare_reader_read(status_read));
//                     a = CCInt64_get(result);
//                 }
//                 CCAutoRelease_doneScope();
//                 if(a == 1)
//                 {
//                     break;
//                 }
//             }
//         }
//         CCAutoRelease_doneScope();

//         ls = CCAutoRelease_add(TCDirectory_ls(""));
//         printf("%s\n", CCString_getCString(ls));

//         CC_obj signal_talker = TCSignal_talker_connect("drivers/sensor1/", "send");
//         TCSignal_talker_send(signal_talker, CCAutoRelease_add(CCInt64_create(64)));
//         TCSignal_talker_send(signal_talker, CCAutoRelease_add(CCInt64_create(56)));
//         TCSignal_talker_disconnect(signal_talker);

//         sleep(1);

//         // while(1)
//         {
//             CC_obj sensor1_read =  TCShare_reader_connect("drivers/sensor1/", "value");
//             CC_obj result = CCAutoRelease_add(TCShare_reader_read(sensor1_read));
//             printf("%lld\n", CCInt64_get(result));    
//         }

//         sleep(1);

//         ls = CCAutoRelease_add(TCDirectory_ls(""));
//         printf("%s\n", CCString_getCString(ls));  
//     }
//     CCAutoRelease_doneScope();

//     CCMemDebugger_show("object");
//     CCMemDebugger_show("locker");
//     return 0;
// }
