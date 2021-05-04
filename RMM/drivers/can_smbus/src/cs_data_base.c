#include "cs_data_base.h"

void CSDataBase_initializer(void)
{
    TCDirectory_mkdir("comm/", "data_base");
    TCDirectory_mkdir("comm/data_base/", "id_list");
    TCDirectory_mkdir("comm/data_base/", "reg_list");
    TCDirectory_mkdir("comm/data_base/reg_list/", "out");
    TCDirectory_mkdir("comm/data_base/reg_list/", "in");
}

void CSDataBase_setDevice(const char* name, CSType_canId_t id)
{
    TCShare_create("comm/data_base/id_list/", name, CCInt32_create(id));
}

static void __CSDataBase_setRegIn(CC_bool_t is_system, const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
{
    if(TCDirectory_lsObj("comm/data_base/reg_list/in/", dev_name) == CC_BOOL_FALSE)
    {
        TCDirectory_mkdir("comm/data_base/reg_list/in/", dev_name);
    }

    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/in/"));
        CCString_add(device_path, dev_name);

        CC_obj info = CCDictionary_create();
        CCDictionary_setObject(info, CCInt32_create(reg_id), "id");
        CCDictionary_setObject(info, CCBool_create(is_system), "is_sys");
        CCDictionary_setObject(info, CCInt32_create(data_type), "type");
        TCShare_create(CCString_getCString(device_path), reg_name, info);
    }
    CCAutoRelease_doneScope();
}


static void __CSDataBase_setRegOut(CC_bool_t is_system, const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
{
    if(TCDirectory_lsObj("comm/data_base/reg_list/out/", dev_name) == CC_BOOL_FALSE)
    {
        TCDirectory_mkdir("comm/data_base/reg_list/out/", dev_name);
    }

    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/out/"));
        CCString_add(device_path, dev_name);

        CC_obj info = CCDictionary_create();
        CCDictionary_setObject(info, CCInt32_create(reg_id), "id");
        CCDictionary_setObject(info, CCBool_create(is_system), "is_sys");
        CCDictionary_setObject(info, CCInt32_create(data_type), "type");
        TCShare_create(CCString_getCString(device_path), reg_name, info);
    }
    CCAutoRelease_doneScope();
}


void CSDataBase_setRegIn(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
{
    __CSDataBase_setRegIn(CC_BOOL_FALSE, dev_name, reg_name, reg_id, data_type);
}

void CSDataBase_setRegOut(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
{
    __CSDataBase_setRegOut(CC_BOOL_FALSE, dev_name, reg_name, reg_id, data_type);
}

void CSDataBase_setSysRegIn(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
{
    __CSDataBase_setRegIn(CC_BOOL_TRUE, dev_name, reg_name, reg_id, data_type);
}

void CSDataBase_setSysRegOut(const char* dev_name, const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
{
    __CSDataBase_setRegOut(CC_BOOL_TRUE, dev_name, reg_name, reg_id, data_type);
}


CSType_canId_t CSDataBase_readId(const char* name)
{
    int32_t id;
    CCAutoRelease_startScope();
    {
        TCShare_t share = TCShare_reader_connect("comm/data_base/id_list/", name);

        CC_obj reg_id_obj = CCAutoRelease_add(TCShare_reader_read(share));
        id = CCInt32_get(reg_id_obj);
        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();
    return id;
}


CSType_canId_t CSDataBase_readRegInId(const char* dev_name, const char* reg_name)
{
    CSType_canId_t reg_id;
    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/in/"));
        CCString_add(device_path, dev_name);

        TCShare_t share = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
        CC_obj info_dic = CCAutoRelease_add(TCShare_reader_read(share));

        reg_id = CCInt32_get(CCDictionary_objectForKey(info_dic, "id"));

        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();

    return reg_id;
}


CC_bool_t CSDataBase_regInIsSystem(const char* dev_name, const char* reg_name)
{
    CC_bool_t is_system;
    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/in/"));
        CCString_add(device_path, dev_name);

        TCShare_t share = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
        CC_obj info_dic = CCAutoRelease_add(TCShare_reader_read(share));

        is_system = CCBool_get(CCDictionary_objectForKey(info_dic, "is_sys"));

        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();

    return is_system;
}

CCVariable_data_t CSDataBase_readRegInType(const char* dev_name, const char* reg_name)
{
    CCVariable_data_t reg_type;
    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/in/"));
        CCString_add(device_path, dev_name);

        TCShare_t share = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
        CC_obj info_dic = CCAutoRelease_add(TCShare_reader_read(share));

        reg_type = (CCVariable_data_t)CCInt32_get(CCDictionary_objectForKey(info_dic, "type"));

        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();

    return reg_type;
}


CSType_canId_t CSDataBase_readRegOutId(const char* dev_name, const char* reg_name)
{
    CSType_canId_t reg_id;
    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/out/"));
        CCString_add(device_path, dev_name);

        TCShare_t share = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
        CC_obj info_dic = CCAutoRelease_add(TCShare_reader_read(share));

        reg_id = CCInt32_get(CCDictionary_objectForKey(info_dic, "id"));

        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();

    return reg_id;
}


CC_bool_t CSDataBase_regOutIsSystem(const char* dev_name, const char* reg_name)
{
    CC_bool_t is_system;
    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/out/"));
        CCString_add(device_path, dev_name);

        TCShare_t share = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
        CC_obj info_dic = CCAutoRelease_add(TCShare_reader_read(share));

        is_system = CCBool_get(CCDictionary_objectForKey(info_dic, "is_sys"));

        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();

    return is_system;
}


CCVariable_data_t CSDataBase_readRegOutType(const char* dev_name, const char* reg_name)
{
    CCVariable_data_t reg_type;
    CCAutoRelease_startScope();
    {
        CC_obj device_path = CCAutoRelease_add(CCString_create("comm/data_base/reg_list/out/"));
        CCString_add(device_path, dev_name);

        TCShare_t share = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
        CC_obj info_dic = CCAutoRelease_add(TCShare_reader_read(share));

        reg_type = (CCVariable_data_t)CCInt32_get(CCDictionary_objectForKey(info_dic, "type"));

        TCShare_reader_disconnect(share);
    }
    CCAutoRelease_doneScope();

    return reg_type;
}
