#include "cc_variable.h"

#include "cc_raw_binary.h"

void CCVariable_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCVariable_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);

static size_t CCVariable_dataSize_nolock(CC_obj obj);


struct CCVariable_info_t
{
    CCVariable_t var;
    CCVariable_data_t  type;
};


CC_obj CCVariable_create(CCVariable_t init_value, CCVariable_data_t data_type)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(struct CCVariable_info_t), CC_VARIABLE_TYPE_ID, CCVariable_toJson_callback_nolock, CCVariable_loadJson_callback_nolock);
    struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;
    info->var = init_value;
    info->type = data_type;

    return _obj;
}

void CCVariable_set(CC_obj obj, CCVariable_t set_value)
{
    if(CCBaseObject_isTypeObject(obj, CC_VARIABLE_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;
        info->var = set_value;
        CCBaseObject_unlock(obj);
    }
}


void CCVariable_setBin(CC_obj obj, const void* binarry)
{
    if(CCBaseObject_isTypeObject(obj, CC_VARIABLE_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;
        memcpy(&info->var, binarry, CCVariable_dataSize_nolock(obj));
        CCBaseObject_unlock(obj);
    }
}


CCVariable_t CCVariable_get(CC_obj obj)
{
    CCVariable_t value = {0};
    if(CCBaseObject_isTypeObject(obj, CC_VARIABLE_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;
        value = info->var;
        CCBaseObject_unlock(obj);
    }
    return value;
}


size_t CCVariable_dataSize(CC_obj obj)
{
    size_t data_size = 0;
    if(CCBaseObject_isTypeObject(obj, CC_VARIABLE_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        data_size = CCVariable_dataSize_nolock(obj);
        CCBaseObject_unlock(obj);
    }

    return data_size;
}

size_t CCVariable_dataSizeFromType(CCVariable_data_t data_type)
{
    switch(data_type)
    {
        case CCVariable_data_Int64:
        case CCVariable_data_Uint64:
            return 8;

        case CCVariable_data_Int32:
        case CCVariable_data_Uint32:
            return 4;

        case CCVariable_data_Int16:
        case CCVariable_data_Uint16:
            return 2;

        case CCVariable_data_Int8:
        case CCVariable_data_Uint8:
            return 1;

        case CCVariable_data_Float:
            return 4;

        case CCVariable_data_Double:
            return 8;
        
        default:
            return 0;
    }
}


void CCVariable_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;

    char value_str[16];
    size_t value_str_len = 0;

    switch(info->type)
	{
		case CCVariable_data_Int64:
#ifdef __APPLE__
            value_str_len = sprintf(value_str, "%lld", info->var.Int64);
#else
            value_str_len = sprintf(value_str, "%ld", info->var.Int64);
#endif
            break;

		case CCVariable_data_Uint64:
#ifdef __APPLE__
            value_str_len = sprintf(value_str, "%lld", info->var.Uint64);
#else
            value_str_len = sprintf(value_str, "%ld", info->var.Uint64);
#endif            
            break;

		case CCVariable_data_Int32:
            value_str_len = sprintf(value_str, "%d", info->var.Int32);
            break;

		case CCVariable_data_Uint32:
            value_str_len = sprintf(value_str, "%d", info->var.Uint32);
            break;

		case CCVariable_data_Int16:
            value_str_len = sprintf(value_str, "%d", info->var.Int16);
            break;

		case CCVariable_data_Uint16:
            value_str_len = sprintf(value_str, "%d", info->var.Uint16);
            break;

		case CCVariable_data_Int8:
            value_str_len = sprintf(value_str, "%d", info->var.Int8);
            break;

		case CCVariable_data_Uint8:
            value_str_len = sprintf(value_str, "%d", info->var.Uint8);
            break;

		case CCVariable_data_Float:
            value_str_len = sprintf(value_str, "%f", info->var.Float);
            break;

		case CCVariable_data_Double:
            value_str_len = sprintf(value_str, "%lf", info->var.Double);
            break;

		default:
            break;
	}

    CCAutoBuffer_add_stream(string, value_str_len, value_str);
}


void CCVariable_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;
    
    switch(info->type)
	{
		case CCVariable_data_Int64:
            info->var.Int64 = CCJsonParser_parseAsInt(parser, json_token, info->var.Int64);
            break;

		case CCVariable_data_Uint64:
            info->var.Uint64 = CCJsonParser_parseAsInt(parser, json_token, info->var.Uint64);
            break;

		case CCVariable_data_Int32:
            info->var.Int32 = CCJsonParser_parseAsInt(parser, json_token, info->var.Int32);
            break;

		case CCVariable_data_Uint32:
            info->var.Uint32 = CCJsonParser_parseAsInt(parser, json_token, info->var.Uint32);
            break;

		case CCVariable_data_Int16:
            info->var.Int16 = CCJsonParser_parseAsInt(parser, json_token, info->var.Int16);
            break;

		case CCVariable_data_Uint16:
            info->var.Uint16 = CCJsonParser_parseAsInt(parser, json_token, info->var.Uint16);
            break;

		case CCVariable_data_Int8:
            info->var.Int8 = CCJsonParser_parseAsInt(parser, json_token, info->var.Int8);
            break;

		case CCVariable_data_Uint8:
            info->var.Uint8 = CCJsonParser_parseAsInt(parser, json_token, info->var.Uint8);
            break;

		case CCVariable_data_Float:
            info->var.Float = CCJsonParser_parseAsDouble(parser, json_token, info->var.Float);
            break;

		case CCVariable_data_Double:
            info->var.Double = CCJsonParser_parseAsDouble(parser, json_token, info->var.Double);
            break;

		default:
            break;
	}
}



static size_t CCVariable_dataSize_nolock(CC_obj obj)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    struct CCVariable_info_t* info = (struct CCVariable_info_t*)_obj->data_bin;

	return CCVariable_dataSizeFromType(info->type);
}