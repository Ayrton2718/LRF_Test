#include "cs_sender.h"

#include "cs_type.h"
#include "cs_io.h"
#include "cs_data_base.h"


struct CSSender_t
{
    TCSignal_t send_talker;
    TCSignal_t send_listener;
};

static struct CSSender_t g_obj;


void CSSender_thread(void);

// static void CSender_rawSend(CSType_canId_t id, CSType_canReg_t reg, size_t len, const void* data);

static CC_bool_t CSSender_orderReceive(CSType_canId_t* id, CSType_canReg_t* reg, size_t* len, uint8_t* data);


void CSSender_initializer(void)
{
    TCSignal_create("comm/", "send_bus");

    g_obj.send_talker = TCSignal_talker_connect("comm/", "send_bus");
    g_obj.send_listener = TCSignal_listener_connect("comm/", "send_bus", "sender_task");

    TCLauncher_addLaunchFunction(CSSender_thread, "CommTask_sender");
}


CC_obj CSSender_open(const char* dev_name, const char* reg_name)
{
    int32_t id = 0;
    int32_t reg = 0;

    if(CSDataBase_regOutIsSystem(dev_name, reg_name))
    {
        id = CSType_getIdSysWrite(CSDataBase_readId(dev_name));
    }else{
        id = CSType_getIdUserWrite(CSDataBase_readId(dev_name));
    }
    reg = CSType_getRegWrite(CSDataBase_readRegOutId(dev_name, reg_name));

    CC_obj _obj = CCDictionary_create();
    CCObject_retain(_obj);

    CCDictionary_setObject(_obj, CCInt32_create(id), "id");
    CCDictionary_setObject(_obj, CCInt32_create(reg), "reg");
    CCDictionary_setObject(_obj, CCInt32_create(CCVariable_dataSizeFromType(CSDataBase_readRegOutType(dev_name, reg_name))), "len");
    CCDictionary_setObject(_obj, CCBytes_create(4), "data");

    return _obj;
}


void CSSender_close(CC_obj obj)
{
    CCObject_release(obj);
}


void CSSender_write(CC_obj obj, CCVariable_t variable)
{
    CC_obj data_obj = CCDictionary_objectForKey(obj, "data");
    CCBytes_setBuff(data_obj, (uint8_t*)&variable);

    TCSignal_talker_send(g_obj.send_talker, obj);
}


CC_obj CSSender_openRequester(const char* dev_name, const char* reg_name)
{
    int32_t id = 0;
    int32_t reg = 0;

    if(CSDataBase_regOutIsSystem(dev_name, reg_name))
    {
        id = CSType_getIdSysWrite(CSDataBase_readId(dev_name));
    }else{
        id = CSType_getIdUserWrite(CSDataBase_readId(dev_name));
    }
    reg = CSType_getRegWrite(CSDataBase_readRegOutId(dev_name, reg_name));

    CC_obj _obj = CCDictionary_create();
    CCObject_retain(_obj);

    CCDictionary_setObject(_obj, CCInt32_create(id), "id");
    CCDictionary_setObject(_obj, CCInt32_create(reg), "reg");
    CCDictionary_setObject(_obj, CCInt32_create(CCVariable_dataSizeFromType(CSDataBase_readRegInType(dev_name, reg_name))), "len");
    CCDictionary_setObject(_obj, CCBytes_create(4), "data");

    return _obj;
}


typedef enum
{
    CSRequester_mode_Request = 0,
    CSRequester_mode_SetSend,
    CSRequester_mode_Interval
} CSRequester_mode_t;


void CSRequester_setModeRequest(const char* dev_name, const char* reg_name)
{
    int32_t id = 0;
    int32_t reg = 0;

    if(CSDataBase_regInIsSystem(dev_name, reg_name))
    {
        id = CSType_getIdSysWrite(CSDataBase_readId(dev_name));
    }else{
        id = CSType_getIdUserWrite(CSDataBase_readId(dev_name));
    }
    reg = CSType_getRegRequest(CSDataBase_readRegInId(dev_name, reg_name));

    uint8_t data[4];
    data[0] = CSRequester_mode_Request;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    CCAutoRelease_startScope();
    {
        CC_obj send_dic = CCAutoRelease_add(CCDictionary_create());
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(id)), "id");
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(reg)), "reg");
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(4)), "len");
        CC_obj data_bytes = CCAutoRelease_add(CCBytes_create(4));
        CCBytes_setBuff(data_bytes, data);
        CCDictionary_setObject(send_dic, data_bytes, "data");
        
        TCSignal_talker_send(g_obj.send_talker, send_dic);
    }
    CCAutoRelease_doneScope();
}


void CSRequester_setModeSetSend(const char* dev_name, const char* reg_name)
{
    int32_t id = 0;
    int32_t reg = 0;

    if(CSDataBase_regInIsSystem(dev_name, reg_name))
    {
        id = CSType_getIdSysWrite(CSDataBase_readId(dev_name));
    }else{
        id = CSType_getIdUserWrite(CSDataBase_readId(dev_name));
    }
    reg = CSType_getRegRequest(CSDataBase_readRegInId(dev_name, reg_name));

    uint8_t data[4];
    data[0] = CSRequester_mode_SetSend;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    CCAutoRelease_startScope();
    {
        CC_obj send_dic = CCAutoRelease_add(CCDictionary_create());
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(id)), "id");
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(reg)), "reg");
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(4)), "len");
        CC_obj data_bytes = CCAutoRelease_add(CCBytes_create(4));
        CCBytes_setBuff(data_bytes, data);
        CCDictionary_setObject(send_dic, data_bytes, "data");
        
        TCSignal_talker_send(g_obj.send_talker, send_dic);
    }
    CCAutoRelease_doneScope();

}


void CSRequester_setModeInterval(const char* dev_name, const char* reg_name, uint16_t ms)
{
    int32_t id = 0;
    int32_t reg = 0;

    if(CSDataBase_regInIsSystem(dev_name, reg_name))
    {
        id = CSType_getIdSysWrite(CSDataBase_readId(dev_name));
    }else{
        id = CSType_getIdUserWrite(CSDataBase_readId(dev_name));
    }
    reg = CSType_getRegRequest(CSDataBase_readRegInId(dev_name, reg_name));

    uint8_t data[4];
    data[0] = CSRequester_mode_Interval;
    data[1] = (uint8_t)(ms >> 8);
    data[2] = (uint8_t)ms;
    data[3] = 0;

    CCAutoRelease_startScope();
    {
        CC_obj send_dic = CCAutoRelease_add(CCDictionary_create());
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(id)), "id");
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(reg)), "reg");
        CCDictionary_setObject(send_dic, CCAutoRelease_add(CCInt32_create(4)), "len");
        CC_obj data_bytes = CCAutoRelease_add(CCBytes_create(4));
        CCBytes_setBuff(data_bytes, data);
        CCDictionary_setObject(send_dic, data_bytes, "data");
        
        TCSignal_talker_send(g_obj.send_talker, send_dic);
    }
    CCAutoRelease_doneScope();
}


CC_obj CSRequester_open(const char* dev_name, const char* reg_name)
{
    int32_t id = 0;
    int32_t reg = 0;

    if(CSDataBase_regInIsSystem(dev_name, reg_name))
    {
        id = CSType_getIdSysWrite(CSDataBase_readId(dev_name));
    }else{
        id = CSType_getIdUserWrite(CSDataBase_readId(dev_name));
    }
    reg = CSType_getRegRequest(CSDataBase_readRegInId(dev_name, reg_name));

    CC_obj _obj = CCDictionary_create();
    CCObject_retain(_obj);

    CCDictionary_setObject(_obj, CCInt32_create(id), "id");
    CCDictionary_setObject(_obj, CCInt32_create(reg), "reg");
    CCDictionary_setObject(_obj, CCInt32_create(0), "len");
    CCDictionary_setObject(_obj, CCBytes_create(4), "data");

    return _obj;
}


void CSRequester_close(CC_obj obj)
{
    CCObject_release(obj);
}

void CSRequester_request(CC_obj obj)
{
    TCSignal_talker_send(g_obj.send_talker, obj);
}


CC_bool_t CSSender_orderReceive(CSType_canId_t* id, CSType_canReg_t* reg, size_t* len, uint8_t* data)
{
    CCAutoRelease_startScope();
    {
        CC_obj send_packet = CCAutoRelease_add(TCSignal_listener_receiveWait(g_obj.send_listener));
        *id = CCInt32_get(CCDictionary_objectForKey(send_packet, "id"));
        *reg = CCInt32_get(CCDictionary_objectForKey(send_packet, "reg"));
        *len = CCInt32_get(CCDictionary_objectForKey(send_packet, "len"));
        CCBytes_getBuff(CCDictionary_objectForKey(send_packet, "data"), data);
    }
    CCAutoRelease_doneScope();

    return CC_BOOL_TRUE;
}


void CSSender_thread(void)
{
    while(1)
    {
        CSType_canId_t id;
        CSType_canReg_t reg;
        size_t data_len;
        uint8_t data[4];
        if(CC_BOOL_TRUE == CSSender_orderReceive(&id, &reg, &data_len, data))
        {       
            CCLOG_INFO("Send to %x, %x, %d [%x, %x, %x, %x]", id, reg, data_len, data[0], data[1], data[2], data[3]);
            CSIo_send(id, reg, data_len, data);
            CSType_delay_ms(1);
        }
    }
}
