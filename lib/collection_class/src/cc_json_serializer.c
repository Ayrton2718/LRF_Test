#include "cc_json_serializer.h"

#include "cc_log.h"
#include "cc_base_object.h"
#include "cc_string.h"
#include "cc_object.h"
#include "cc_bool.h"
#include "cc_double.h"
#include "cc_int_32.h"
#include "cc_dictionary.h"
#include "cc_array.h"
#include "json_parser/json_parser.h"

static CC_obj CCJsonSerializer_objectMaker(struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* token);


CC_obj CCJsonSerializer_dumps(CC_obj obj, CC_bool_t visible)
{
    struct CCAutoBuffer_t string;
    CCAutoBuffer_create(&string, sizeof(char));
    CCBaseObject_toJson(obj, &string, visible);
    CCAutoBuffer_add(&string, "");
    CC_obj str_obj = CCString_create((const char*)CCAutoBuffer_getRaw(&string));
    CCAutoBuffer_destructor(&string);
    return str_obj;
}


static inline CC_obj CCJsonSerializer_stringMaker(struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* token)
{
    size_t len = CCJsonParser_stringLen(parser, token);
    const char* string = CCJsonParser_getStringRaw(parser, token);
    return CCString_createWithLength(string, len);
}


static inline CC_obj CCJsonSerializer_dictionaryMaker(struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* token)
{
    CC_obj dictionary = CCDictionary_create();
    for(size_t i = 0; i < CCJsonParser_dictionaryCount(parser, token); i++)
    {
        char key[64];
        struct CCJsonParser_jsonToken_t current_token = CCJsonParser_getDictionaryAtIndex(parser, token, i, key);
        CCDictionary_setObject(dictionary, CCJsonSerializer_objectMaker(parser, &current_token), key);
    }

    return dictionary;
}


static inline CC_obj CCJsonSerializer_arrayMaker(struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* token)
{
    CC_obj array = CCArray_create();
    for(size_t i = 0; i < CCJsonParser_arrayCount(parser, token); i++)
    {
        struct CCJsonParser_jsonToken_t current_token = CCJsonParser_getArray(parser, token, i);

        CCArray_addObject(array, CCJsonSerializer_objectMaker(parser, &current_token));
    }
    return array;
}


static CC_obj CCJsonSerializer_objectMaker(struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* token)
{
    CC_obj obj;

    switch(CCJsonParser_jsonType(parser, token))
    {
    case JSMN_OBJECT:
        obj = CCJsonSerializer_dictionaryMaker(parser, token);
        break;

    case JSMN_ARRAY:
        obj = CCJsonSerializer_arrayMaker(parser, token);
        break;
    
    case JSMN_INT:
        obj = CCInt32_create(CCJsonParser_parseAsInt(parser, token, 0));
        break;

    case JSMN_DOUBLE:
        obj = CCDouble_create(CCJsonParser_parseAsDouble(parser, token, 0));
        break;
    
    case JSMN_TRUE:
        obj = CCBool_create(CC_BOOL_TRUE);
        break;

    case JSMN_FALSE:
        obj = CCBool_create(CC_BOOL_FALSE);
        break;

    case JSMN_STRING:
        obj = CCJsonSerializer_stringMaker(parser, token);
        break;
    
    default:
        obj = NULL;
        break;
    }

    return obj;
}


CC_obj CCJsonSerializer_dynamicLoads(CC_obj json)
{
    struct CCJsonParser_t parser;
    CC_obj obj;
    if(CCJsonParser_parse(&parser, CCString_getCString(json), CCString_length(json)) == CC_BOOL_TRUE)
    {
        struct CCJsonParser_jsonToken_t root = CCJsonParser_getRoot(&parser);
        obj = CCJsonSerializer_objectMaker(&parser, &root);
    }else{
        obj = NULL;
    }

    CCJsonParser_destructor(&parser);
    return obj;
}


void CCJsonSerializer_staticLoads(CC_obj json, CC_obj obj)
{
    struct CCJsonParser_t parser;
    if(CCJsonParser_parse(&parser, CCString_getCString(json), CCString_length(json)) == CC_BOOL_TRUE)
    {
        struct CCJsonParser_jsonToken_t root = CCJsonParser_getRoot(&parser);
        CCBaseObject_loadJson(obj, &parser, &root);
    }

    CCJsonParser_destructor(&parser);
}
