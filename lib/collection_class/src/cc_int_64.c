#include "cc_int_64.h"

#include "cc_raw_binary.h"

void CCInt64_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCInt64_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCInt64_create(int64_t init_value)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(int64_t), CC_INT64_TYPE_ID, CCInt64_toJson_callback_nolock, CCInt64_loadJson_callback_nolock);
    *(int64_t*)_obj->data_bin = init_value;
    return _obj;
}

void CCInt64_set(CC_obj obj, int64_t set_value)
{
    if(CCBaseObject_isTypeObject(obj, CC_INT64_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        *(int64_t*)_obj->data_bin = set_value;
        CCBaseObject_unlock(obj);
    }
}

int64_t CCInt64_get(CC_obj obj)
{
    int64_t value = 0;
    if(CCBaseObject_isTypeObject(obj, CC_INT64_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        value = *(int64_t*)_obj->data_bin;
        CCBaseObject_unlock(obj);
    }
    return value;
}


void CCInt64_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    char value_str[16];
#ifdef __APPLE__
    size_t value_str_len = sprintf(value_str, "%lld", *(int64_t*)_obj->data_bin);
#else
    size_t value_str_len = sprintf(value_str, "%ld", *(int64_t*)_obj->data_bin);
#endif
    CCAutoBuffer_add_stream(string, value_str_len, value_str);
}


void CCInt64_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    *(int64_t*)_obj->data_bin = CCJsonParser_parseAsInt(parser, json_token, *(int64_t*)_obj->data_bin);
}

