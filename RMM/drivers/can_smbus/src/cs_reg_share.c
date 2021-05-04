#include "cs_reg_share.h"

#include "cs_type.h"
#include "cs_register.h"
#include "cs_data_base.h"


void CSRegShare_initializer(void)
{
	TCDirectory_mkdir("comm/", "reg_share");
    TCDirectory_mkdir("comm/reg_share/", "user");
    TCDirectory_mkdir("comm/reg_share/", "sys");
}


void CSRegShare_callback(CC_obj args, CCVariable_data_t data_type, const void* data)
{
	CCAutoRelease_startScope();
	{
		CCVariable_t tmp;
		tmp.Uint32 = 0;
		CC_obj data_obj = CCAutoRelease_add(CCVariable_create(tmp, data_type));
		CCVariable_setBin(data_obj, data);
		TCShare_writer_write(args, data_obj);
	}
	CCAutoRelease_doneScope();
}


void CSRegShare_bind(const char* dev_name, const char* reg_name)
{
	CCVariable_t none = {0};
	CC_obj obj = CCVariable_create(none, CSDataBase_readRegInType(dev_name, reg_name));

	CC_bool_t is_system = CSDataBase_regInIsSystem(dev_name, reg_name);

	CC_obj tc_writer;

	if(is_system)
	{
		if(TCDirectory_lsObj("comm/reg_share/sys/", dev_name) == CC_BOOL_FALSE)
		{
			TCDirectory_mkdir("comm/reg_share/sys/", dev_name);
		}

		CCAutoRelease_startScope();
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_share/sys/"));
			CCString_add(device_path, dev_name);
			TCShare_create(CCString_getCString(device_path), reg_name, obj);
		
			tc_writer = TCShare_writer_connect(CCString_getCString(device_path), reg_name);
		}
		CCAutoRelease_doneScope();
	}else{
		if(TCDirectory_lsObj("comm/reg_share/user/", dev_name) == CC_BOOL_FALSE)
		{
			TCDirectory_mkdir("comm/reg_share/user/", dev_name);
		}

		CCAutoRelease_startScope();
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_share/user/"));
			CCString_add(device_path, dev_name);
			TCShare_create(CCString_getCString(device_path), reg_name, obj);

			tc_writer = TCShare_writer_connect(CCString_getCString(device_path), reg_name);
		}
		CCAutoRelease_doneScope();
	}

	CSRegister_bind(is_system, CSDataBase_readId(dev_name), CSDataBase_readRegInId(dev_name, reg_name), tc_writer, CSDataBase_readRegInType(dev_name, reg_name), CSRegShare_callback);

}


TCShare_t CSRegShare_open(const char* dev_name, const char* reg_name)
{
	TCShare_t reader;
	CCAutoRelease_startScope();
	{
		if(CSDataBase_regInIsSystem(dev_name, reg_name))
		{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_share/sys/"));
			CCString_add(device_path, dev_name);

			reader = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
		}else{
			CC_obj device_path = CCAutoRelease_add(CCString_create("comm/reg_share/user/"));
			CCString_add(device_path, dev_name);

			reader = TCShare_reader_connect(CCString_getCString(device_path), reg_name);
		}
	}
	CCAutoRelease_doneScope();

	return reader;
}

void CSRegShare_close(TCShare_t share)
{
	TCShare_reader_disconnect(share);
}

CCVariable_t CSRegShare_read(TCShare_t share)
{
	CCVariable_t variable;
	CCAutoRelease_startScope();
	{
		CC_obj var_obj = CCAutoRelease_add(TCShare_reader_read(share));
		variable = CCVariable_get(var_obj);
	}
	CCAutoRelease_doneScope();

	return variable;
}