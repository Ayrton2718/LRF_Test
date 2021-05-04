#include "cc_bytes.h"

#include "cc_raw_binary.h"


void CCBytes_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCBytes_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCBytes_create(size_t count)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(count, CC_BYTES_TYPE_ID, CCBytes_toJson_callback_nolock, CCBytes_loadJson_callback_nolock);
    memset(_obj->data_bin, 0x00, _obj->data_size);
    return _obj;
}

void CCBytes_setBuff(CC_obj obj, const uint8_t* bytes)
{
    if(CCBaseObject_isTypeObject(obj, CC_BYTES_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        memcpy(_obj->data_bin, bytes, _obj->data_size);
        CCBaseObject_unlock(obj);
    }
}

void CCBytes_getBuff(CC_obj obj, uint8_t* bytes)
{
    if(CCBaseObject_isTypeObject(obj, CC_BYTES_TYPE_ID))
    {
        struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;
        CCBaseObject_lock(obj);
        memcpy(bytes, _obj->data_bin, _obj->data_size);
        CCBaseObject_unlock(obj);
    }
}


void CCBytes_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;

    CCAutoBuffer_add(string, "[");

    for(size_t i = 0; i < _obj->data_size; i++)
    {
        char byte_str[16];
        size_t byte_str_len = sprintf(byte_str, "%x,", ((uint8_t*)_obj->data_bin)[i]);
        CCAutoBuffer_add_stream(string, byte_str_len, byte_str);
    }
    CCAutoBuffer_removeLast(string);
    CCAutoBuffer_add(string, "]");
}


void CCBytes_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)obj;

    for(size_t i = 0; i < _obj->data_size; i++)
    {
        struct CCJsonParser_jsonToken_t child_token = CCJsonParser_getArray(parser, json_token, i);
        ((uint8_t*)_obj->data_bin)[i] = CCJsonParser_parseAsInt(parser, &child_token, ((uint8_t*)_obj->data_bin)[i]);
    }
}



