#include "cc_null_object.h"

#include "cc_raw_binary.h"

void CCNullObject_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCNullObject_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCNullObject_create(void)
{
    struct CCRawBinary_t* _obj = (struct CCRawBinary_t*)CCRawBinary_create(sizeof(uint8_t), CC_NULL_TYPE_ID, CCNullObject_toJson_callback_nolock, CCNullObject_loadJson_callback_nolock);
    return _obj;
}


void CCNullObject_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    CCAutoBuffer_add_stream(string, strlen("None"), "None");
}


void CCNullObject_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
}

