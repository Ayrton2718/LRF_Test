#include "cc_int_32.h"

#include "cc_raw_binary.h"

void CCInt32_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCInt32_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCInt32_create(int32_t init_value)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(int32_t), CC_INT32_TYPE_ID, CCInt32_toJson_callback_nolock, CCInt32_loadJson_callback_nolock);
    *(int32_t*)_obj->data_bin = init_value;
    return _obj;
}

void CCInt32_set(CC_obj obj, int32_t set_value)
{
    if(CCBaseObject_isTypeObject(obj, CC_INT32_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        *(int32_t*)_obj->data_bin = set_value;
        CCBaseObject_unlock(obj);
    }
}

int32_t CCInt32_get(CC_obj obj)
{
    int32_t value = 0;
    if(CCBaseObject_isTypeObject(obj, CC_INT32_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        value = *(int32_t*)_obj->data_bin;
        CCBaseObject_unlock(obj);
    }
    return value;
}


void CCInt32_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    char value_str[16];
    size_t value_str_len = sprintf(value_str, "%d", *(int32_t*)_obj->data_bin);
    CCAutoBuffer_add_stream(string, value_str_len, value_str);
}


void CCInt32_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    *(int32_t*)_obj->data_bin = CCJsonParser_parseAsInt(parser, json_token, *(int32_t*)_obj->data_bin);
}

