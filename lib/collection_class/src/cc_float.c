#include "cc_float.h"

#include "cc_raw_binary.h"

void CCFloat_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCFloat_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);

CC_obj CCFloat_create(float init_value)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(float), CC_FLOAT_TYPE_ID, CCFloat_toJson_callback_nolock, CCFloat_loadJson_callback_nolock);
    *(float*)_obj->data_bin = init_value;
    return _obj;
}

void CCFloat_set(CC_obj obj, float set_value)
{
    if(CCBaseObject_isTypeObject(obj, CC_FLOAT_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        *(float*)_obj->data_bin = set_value;
        CCBaseObject_unlock(obj);
    }
}

float CCFloat_get(CC_obj obj)
{
    float value = 0;
    if(CCBaseObject_isTypeObject(obj, CC_FLOAT_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        value = *(float*)_obj->data_bin;
        CCBaseObject_unlock(obj);
    }
    return value;
}


void CCFloat_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    char value_str[16];
    size_t value_str_len = sprintf(value_str, "%lf", *(float*)_obj->data_bin);
    CCAutoBuffer_add_stream(string, value_str_len, value_str);
}


void CCFloat_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    *(float*)_obj->data_bin = (double)CCJsonParser_parseAsDouble(parser, json_token, (double)*(float*)_obj->data_bin);

}

