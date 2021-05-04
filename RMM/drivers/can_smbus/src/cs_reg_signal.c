#include "cs_reg_signal.h"

#include "cs_type.h"
#include "cs_register.h"
#include "cs_data_base.h"


void CSRegSignal_initializer(void)
{
    TCDirectory_mkdir("comm/", "reg_signal");
    TCDirectory_mkdir("comm/reg_signal/", "user");
    TCDirectory_mkdir("comm/reg_signal/", "sys");
}


void CSRegSignal_callback(CC_obj args, CCVariable_data_t data_type, const void* data)
{
    CCAutoRelease_startScope();
	{
		CCVariable_t tmp;
		tmp.Uint32 = 0;
		CC_obj data_obj = CCAutoRelease_add(CCVariable_create(tmp, data_type));
		CCVariable_setBin(data_obj, data);
        TCSignal_talker_send(args, data_obj);
	}
	CCAutoRelease_doneScope();
}


void CSRegSignal_bind(const char* dev_name, const char* reg_name)
{
	CC_bool_t is_system = CSDataBase_regInIsSystem(dev_name, reg_name);
	CC_obj tc_sender;

	if(is_system)
	{
		if(TCDirectory_lsObj("comm/reg_signal/sys/", dev_name) == CC_BOOL_FALSE)
		{
			TCDirectory_mkdir("comm/reg_signal/sys/", dev_name);
		}

		CCAutoRelease_startScope();
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_signal/sys/"));
			CCString_add(device_path, dev_name);
            TCSignal_create(CCString_getCString(device_path), reg_name);
		
			tc_sender = TCSignal_talker_connect(CCString_getCString(device_path), reg_name);
		}
		CCAutoRelease_doneScope();
	}else{
		if(TCDirectory_lsObj("comm/reg_signal/user/", dev_name) == CC_BOOL_FALSE)
		{
			TCDirectory_mkdir("comm/reg_signal/user/", dev_name);
		}

		CCAutoRelease_startScope();
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_signal/user/"));
			CCString_add(device_path, dev_name);
            TCSignal_create(CCString_getCString(device_path), reg_name);
		
			tc_sender = TCSignal_talker_connect(CCString_getCString(device_path), reg_name);
		}
		CCAutoRelease_doneScope();
	}

	CSRegister_bind(is_system, CSDataBase_readId(dev_name), CSDataBase_readRegInId(dev_name, reg_name), tc_sender, CSDataBase_readRegInType(dev_name, reg_name), CSRegSignal_callback);

}


TCSignal_t CSRegSignal_open(const char* dev_name, const char* reg_name, const char* listen_name)
{
	TCSignal_t listener;

	if(CSDataBase_regInIsSystem(dev_name, reg_name))
	{
		CCAutoRelease_startScope();
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_signal/sys/"));
			CCString_add(device_path, dev_name);

			listener = TCSignal_listener_connect(CCString_getCString(device_path), reg_name, listen_name);
		}
		CCAutoRelease_doneScope();
	}else{
		CCAutoRelease_startScope();
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_signal/user/"));
			CCString_add(device_path, dev_name);

			listener = TCSignal_listener_connect(CCString_getCString(device_path), reg_name, listen_name);
		}
		CCAutoRelease_doneScope();
	}

	return listener;
}


void CSRegSignal_close(TCSignal_t signal)
{
    TCSignal_listener_disconnect(signal);
}


void CSRegSignal_request(TCSignal_t sig)
{

}


CCVariable_t CSRegSignal_recv(TCSignal_t sig)
{
	CCVariable_t variable;
	CCAutoRelease_startScope();
	{
		CC_obj var_obj = CCAutoRelease_add(TCSignal_listener_receiveWait(sig));
		variable = CCVariable_get(var_obj);
	}
	CCAutoRelease_doneScope();

	return variable;
}
