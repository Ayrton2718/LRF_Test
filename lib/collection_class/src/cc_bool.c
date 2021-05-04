#include "cc_bool.h"

#include "cc_raw_binary.h"

void CCBool_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCBool_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCBool_create(CC_bool_t init_value)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(CC_bool_t), CC_BOOLEAN_TYPE_ID, CCBool_toJson_callback_nolock, CCBool_loadJson_callback_nolock);
    *(CC_bool_t*)_obj->data_bin = init_value;
    return _obj;
}

void CCBool_set(CC_obj obj, CC_bool_t set_value)
{
    if(CCBaseObject_isTypeObject(obj, CC_BOOLEAN_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        *(CC_bool_t*)_obj->data_bin = set_value;
        CCBaseObject_unlock(obj);
    }
}

CC_bool_t CCBool_get(CC_obj obj)
{
    CC_bool_t value = 0;
    if(CCBaseObject_isTypeObject(obj, CC_BOOLEAN_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        value = *(CC_bool_t*)_obj->data_bin;
        CCBaseObject_unlock(obj);
    }
    return value;
}


void CCBool_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    if(*(CC_bool_t*)_obj->data_bin == CC_BOOL_TRUE)
    {
        CCAutoBuffer_add_stream(string, strlen("true"), "true");
    }
    else if(*(CC_bool_t*)_obj->data_bin == CC_BOOL_FALSE)
    {
        CCAutoBuffer_add_stream(string, strlen("false"), "false");
    }
}


void CCBool_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
    *(CC_bool_t*)_obj->data_bin = CCJsonParser_parseAsBoolean(parser, json_token, *(CC_bool_t*)_obj->data_bin);
}

