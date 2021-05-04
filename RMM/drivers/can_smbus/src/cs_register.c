#include "cs_register.h"

#include "cs_io.h"


struct CSRegister_dictionary_t
{
    CC_obj user_output;
    CC_obj user_input;
    CC_obj sys_output;
    CC_obj sys_input;
};

static struct CSRegister_dictionary_t g_root;

void CSRegister_thread(void);


inline static void CSRegister_setInstance(CC_obj type, CSType_canId_t id, CSType_canReg_t reg, CC_obj instance)
{
    reg = CSType_getRegNumber(reg);
    char id_name[32];
    char reg_name[32];
    sprintf(id_name, "0x%03x", id);
    sprintf(reg_name, "%1d", reg);

    CC_obj reg_list = CCDictionary_objectForKey(type, id_name);
    if(CCObject_isObject(reg_list) == CC_BOOL_FALSE)
    {
        reg_list = CCDictionary_create();
        CCObject_enableThreadSafe(reg_list);
        CCDictionary_setObject(type, reg_list, id_name);
    }

    CCObject_enableThreadSafe(instance);
    CCDictionary_setObject(reg_list, instance, reg_name);
}


inline static CC_obj CSRegister_getInstance(CC_obj type, CSType_canId_t id, CSType_canReg_t reg)
{
    reg = CSType_getRegNumber(reg);
    char id_name[32];
    char reg_name[32];
    sprintf(id_name, "0x%03x", id);
    sprintf(reg_name, "%1d", reg);
        
    CC_obj instance = NULL;
    CCAutoRelease_startScope();
    {
        CC_obj reg_list = CCAutoRelease_add(CCDictionary_objectForKey(type, id_name));
        if(CCObject_isObject(reg_list) == CC_BOOL_TRUE)
        {
            instance = CCAutoRelease_add(CCDictionary_objectForKey(reg_list, reg_name));
        }
    }
    CCAutoRelease_doneScope();

    return instance;
}


struct CSRegister_info_t
{
    CC_obj args;
    CCVariable_data_t data_type;
    CSRegister_callback_t func;
};


void CSRegister_bind(CC_bool_t is_system, CSType_canId_t id, CSType_canReg_t reg, CC_obj args, CCVariable_data_t data_type, CSRegister_callback_t func)
{
    struct CSRegister_info_t instance;
    instance.args = args;
    instance.data_type = data_type;
    instance.func = func;

    CC_obj callback_info = CCBinary_create(sizeof(struct CSRegister_info_t));
    CCBinary_set(callback_info, &instance);

    if(is_system == CC_BOOL_TRUE)
    {
        CSRegister_setInstance(g_root.sys_input, id, reg, callback_info);
    }else{
        CSRegister_setInstance(g_root.user_input, id, reg, callback_info);
    }
}


void CSRegister_initializer(void)
{
    TCDirectory_mkdir("comm/", "register");
    TCDirectory_mkdir("comm/register/", "user");
    TCDirectory_mkdir("comm/register/", "sys");

    TCDirectory_mkdir("comm/register/user/", "output");
    TCDirectory_mkdir("comm/register/user/", "input");

    TCDirectory_mkdir("comm/register/sys/", "output");
    TCDirectory_mkdir("comm/register/sys/", "input");

    g_root.user_output = TCDirectory_openObj("comm/register/user/", "output");
    g_root.user_input = TCDirectory_openObj("comm/register/user/", "input");
    g_root.sys_output = TCDirectory_openObj("comm/register/sys/", "output");
    g_root.sys_input = TCDirectory_openObj("comm/register/sys/", "input");

    TCLauncher_addLaunchFunction(CSRegister_thread, "CommTask_register");
}

inline static void CSRegister_write(CC_obj type, CSType_canId_t id, CSType_canReg_t reg, size_t data_len, const uint8_t* data)
{
    CC_obj instance_obj = CSRegister_getInstance(type, id, reg);

    if(CCObject_isObject(instance_obj) == CC_BOOL_FALSE)
    {
        CCLOG_ERROR("Receive a unregistered id and reg!");
        return;
    }

    struct CSRegister_info_t info;
    CCBinary_get(instance_obj, &info);
    
    if(CCVariable_dataSizeFromType(info.data_type) != data_len)
    {
        CCLOG_ERROR_NOFMT("Invited data size!");
        return;
    }

    info.func(info.args, info.data_type, data);
}

static void CSRegister_userInputWrite(CSType_canId_t id, CSType_canReg_t reg, uint8_t len, const uint8_t* data)
{
    CSRegister_write(g_root.user_input, id, reg, len, data);
}


static void CSRegister_sysInputWrite(CSType_canId_t id, CSType_canReg_t reg, uint8_t len, const uint8_t* data)
{
    CSRegister_write(g_root.sys_input, id, reg, len, data);
}


void CSRegister_thread(void)
{
    while(1)
    {
        CSType_canId_t id;
        CSType_canReg_t reg;
        size_t data_len;
        uint8_t data[4];

        if(CSIo_read(&id, &reg, &data_len, data) == CC_BOOL_TRUE)
        {
            CCLOG_INFO("Get from %x, %x, %d [%x, %x, %x, %x]", id, reg, data_len, data[0], data[1], data[2], data[3]);
            if(CSType_isIdUserRead(id))
            {
                if(CSType_isRegResponse(reg))
                {
                    CSRegister_userInputWrite(CSType_getIdNumber(id), CSType_getRegNumber(reg), data_len, data);
                }
            }else if(CSType_isIdSysRead(id))
            {
                if(CSType_isRegResponse(reg))
                {
                    CSRegister_sysInputWrite(CSType_getIdNumber(id), CSType_getRegNumber(reg), data_len, data);
                }
            }
        }

        CCType_yield();
    }
}
