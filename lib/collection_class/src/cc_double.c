#include "cc_double.h"

#include "cc_raw_binary.h"

void CCDouble_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCDouble_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCDouble_create(double init_value)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(double), CC_DOUBLE_TYPE_ID, CCDouble_toJson_callback_nolock, CCDouble_loadJson_callback_nolock);
    *(double*)_obj->data_bin = init_value;
    return _obj;
}

void CCDouble_set(CC_obj obj, double set_value)
{
    if(CCBaseObject_isTypeObject(obj, CC_DOUBLE_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        *(double*)_obj->data_bin = set_value;
        CCBaseObject_unlock(obj);
    }
}

double CCDouble_get(CC_obj obj)
{
    double value = 0;
    if(CCBaseObject_isTypeObject(obj, CC_DOUBLE_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        value = *(double*)_obj->data_bin;
        CCBaseObject_unlock(obj);
    }
    return value;
}


void CCDouble_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    char value_str[16];
    size_t value_str_len = sprintf(value_str, "%lf", *(double*)_obj->data_bin);
    CCAutoBuffer_add_stream(string, value_str_len, value_str);
}


void CCDouble_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    *(double*)_obj->data_bin = CCJsonParser_parseAsDouble(parser, json_token, *(double*)_obj->data_bin);

}

