#include "cs_system.h"

#include "cs_type.h"
#include "cs_io.h"
#include "cs_sender.h"
#include "cs_register.h"
#include "cs_data_base.h"
#include "cs_reg_share.h"
#include "cs_reg_signal.h"

struct CSSystem_t
{
    CC_obj arp_devices;
};

static struct CSSystem_t g_root;

void CSSystem_thread(void);

void CSSystem_initializer(void)
{
    TCSignal_create("comm/", "system");

    g_root.arp_devices = CCDictionary_create();
    TCShare_create("comm/system", "arp_result", g_root.arp_devices); 
    // g_root.send_talker = TCSignal_talker_connect("comm/", "send_bus");
    // g_root.send_listener = TCSignal_listener_connect("comm/", "send_bus", "sender_task");

    TCLauncher_addLaunchFunction(CSSystem_thread, "CSSystem_thread");
}

void CSSystem_setDevice(const char* dev_name)
{
    // CSDataBase_setSysRegIn(dev_name, "degMsg1", 4, CCVariable_data_Uint32);
    // CSDataBase_setSysRegIn(dev_name, "degMsg2", 5, CCVariable_data_Uint32);
    // CSDataBase_setSysRegIn(dev_name, "degMsg3", 6, CCVariable_data_Uint32);
    // CSDataBase_setSysRegIn(dev_name, "degMsg4", 7, CCVariable_data_Uint32);

    // CSRegSignal_bind(dev_name, "degMsg1");
    // CSRegSignal_bind(dev_name, "degMsg2");
    // CSRegSignal_bind(dev_name, "degMsg3");
    // CSRegSignal_bind(dev_name, "degMsg4");
}

void CSSystem_getDebugMsg(const char* dev_name)
{
    // TCShare_t sh1 = CSRegSignal_openSys(dev_name, "degMsg1", "getter1");
    // TCShare_t sh2 = CSRegSignal_openSys(dev_name, "degMsg2", "getter2");
    // TCShare_t sh3 = CSRegSignal_openSys(dev_name, "degMsg3", "getter3");
    // TCShare_t sh4 = CSRegSignal_openSys(dev_name, "degMsg4", "getter4");

    // CSSender_request(dev_name, "degMsg1");
    // CSSender_request(dev_name, "degMsg2");
    // CSSender_request(dev_name, "degMsg3");
    // CSSender_request(dev_name, "degMsg4");

    // char debug_msg[32];

    // CCVariable_t tmp;
    // tmp = CSRegSignal_recv(sh1);
    // memcpy(&debug_msg[0], &tmp, 4);
    // tmp = CSRegSignal_recv(sh2);
    // memcpy(&debug_msg[4], &tmp, 4);
    // tmp = CSRegSignal_recv(sh3);
    // memcpy(&debug_msg[8], &tmp, 4);
    // tmp = CSRegSignal_recv(sh4);
    // memcpy(&debug_msg[12], &tmp, 4);

    // debug_msg[16] = '\0';
    // CCLOG_INFO_NOFMT(debug_msg);

    // CSRegSignal_close(sh1);
    // CSRegSignal_close(sh2);
    // CSRegSignal_close(sh3);
    // CSRegSignal_close(sh4);
}

void CSSystem_thread(void)
{
    CCType_yield();
}
