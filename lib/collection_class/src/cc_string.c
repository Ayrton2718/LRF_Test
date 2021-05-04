#include "cc_string.h"

#include "cc_base_object.h"
#include "cc_auto_buffer.h"

#include "cc_log.h"

struct CCString_t
{
    struct CCBaseObject_t object;

    struct CCAutoBuffer_t  string;
    char* cstr_buffer;
};


size_t CCString_length_nolock(CC_obj obj);
const char* CCString_getCString_nolock(CC_obj obj);
void CCString_add_nolock(CC_obj obj, const char* string);
int CCString_intValue_nolock(CC_obj obj, int default_value);
float CCString_floatValue_nolock(CC_obj obj, float default_value);
double CCString_doubleValue_nolock(CC_obj obj, double default_value);
CC_bool_t CCString_boolValue_nolock(CC_obj obj, CC_bool_t default_value);
int CCString_compare_nolock(CC_obj obj, const char* compare_string);
void CCString_writeFile_nolock(CC_obj obj, const char* file_path);

void CCString_destructor_callback_nolock(CC_obj obj);
void CCString_enableLocker_callback_nolock(CC_obj obj);
CC_obj CCString_copy_callback_nolock(CC_obj obj);
CC_bool_t CCString_replace_callback_nolock(CC_obj obj, CC_obj obj_from);
size_t CCString_toHash_callbacdk_nolock(CC_obj obj);
void CCString_debug_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* strings);
void CCString_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCString_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCString_create(const char* string)
{
    return CCString_createWithLength(string, strlen(string));
}


CC_obj CCString_createWithLength(const char* string, size_t len)
{
    struct CCString_t* _obj = (struct CCString_t*)CCBaseObject_create(
        sizeof(struct CCString_t), 
        CC_STRING_TYPE_ID, 
        CCString_destructor_callback_nolock, 
        CCString_enableLocker_callback_nolock,
        CCString_copy_callback_nolock, 
        CCString_replace_callback_nolock,
        CCString_toHash_callbacdk_nolock, 
        CCString_debug_callback_nolock, 
        CCString_toJson_callback_nolock,
        CCString_loadJson_callback_nolock);

    CCAutoBuffer_create(&_obj->string, sizeof(char));
    if(string != NULL)
    {
        CCAutoBuffer_add_stream(&_obj->string, len, string);
    }
    char null_char = '\0';
    CCAutoBuffer_add(&_obj->string, &null_char);

    _obj->cstr_buffer = NULL;

    return _obj;
}


size_t CCString_length(CC_obj obj)
{
    size_t result = 0;
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result = CCString_length_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return result;
}

const char* CCString_getCString(CC_obj obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        const char* result = CCString_getCString_nolock(obj);
        CCBaseObject_unlock(obj);
        return result;
    }
    return "";
}
    
void CCString_add(CC_obj obj, const char* string)
{
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCString_add_nolock(obj, string);
        CCBaseObject_unlock(obj);
    }
}

int CCString_intValue(CC_obj obj, int default_value)
{
    int result = default_value;
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result = CCString_intValue_nolock(obj, default_value);
        CCBaseObject_unlock(obj);
    }
    return result;
}

float CCString_floatValue(CC_obj obj, float default_value)
{
    float result = default_value;
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result = CCString_floatValue_nolock(obj, default_value);
        CCBaseObject_unlock(obj);
    }
    return result;
}

double CCString_doubleValue(CC_obj obj, double default_value)
{
    double result = default_value;
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result = CCString_doubleValue_nolock(obj, default_value);
        CCBaseObject_unlock(obj);
    }
    return result;
}

CC_bool_t CCString_boolValue(CC_obj obj, CC_bool_t default_value)
{
    CC_bool_t result = default_value;
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result = CCString_boolValue_nolock(obj, default_value);
        CCBaseObject_unlock(obj);
    }
    return result;
}

int CCString_compare(CC_obj obj, const char* compare_string)
{
    int result = -1;
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result = CCString_compare_nolock(obj, compare_string);
        CCBaseObject_unlock(obj);
    }
    return result;
}

void CCString_writeFile(CC_obj obj, const char* file_path)
{
    if(CCBaseObject_isTypeObject(obj, CC_STRING_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCString_writeFile_nolock(obj, file_path);
        CCBaseObject_unlock(obj);
    }
}


size_t CCString_length_nolock(CC_obj obj)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    size_t len = CCAutoBuffer_count(&_obj->string) - 1;
    return len;
}

const char* CCString_getCString_nolock(CC_obj obj)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    if(_obj->cstr_buffer != NULL)
    {
        free(_obj->cstr_buffer);
    }
    _obj->cstr_buffer = (char*)malloc(sizeof(char) * CCAutoBuffer_count(&_obj->string));
    CCAutoBuffer_read_stream(&_obj->string, 0, CCAutoBuffer_count(&_obj->string), _obj->cstr_buffer);
    return _obj->cstr_buffer;
}

void CCString_add_nolock(CC_obj obj, const char* string)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    
    CCAutoBuffer_removeLast(&_obj->string);
    CCAutoBuffer_add_stream(&_obj->string, strlen(string) + 1, string);
}
    
int CCString_intValue_nolock(CC_obj obj, int default_value)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    const char* string = (const char*)CCAutoBuffer_getRaw(&_obj->string);
    char *endptr;
    int value = (int)strtol(string, &endptr, (long)default_value);
    if(string == endptr)
    {
        value = default_value;
    }
    return value;
}

float CCString_floatValue_nolock(CC_obj obj, float default_value)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    const char* string = (const char*)CCAutoBuffer_getRaw(&_obj->string);
    char *endptr;
    float value = strtof(string, &endptr);
    if(string == endptr)
    {
        value = default_value;
    }
    return value;
}

double CCString_doubleValue_nolock(CC_obj obj, double default_value)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    const char* string = (const char*)CCAutoBuffer_getRaw(&_obj->string);
    char *endptr;
    double value = strtod(string, &endptr);
    if(string == endptr)
    {
        value = default_value;
    }
    return value;
}

CC_bool_t CCString_boolValue_nolock(CC_obj obj, CC_bool_t default_value)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;

    CC_bool_t result = default_value;

    const char* string_raw = (const char*)CCAutoBuffer_getRaw(&_obj->string);
    
    if(strcmp(string_raw, "true") == 0)
    {
        result = CC_BOOL_TRUE;
    }else if(strcmp(string_raw, "false") == 0){
        result = CC_BOOL_FALSE;
    }

    return result;
}

int CCString_compare_nolock(CC_obj obj, const char* compare_string)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    int result = strcmp((const char*)CCAutoBuffer_getRaw(&_obj->string), compare_string);
    return result;
}


void CCString_writeFile_nolock(CC_obj obj, const char* file_path)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;

    FILE *fp;
    fp = fopen(file_path, "w");
    if(fp == NULL)
    {
        CCLOG_ERROR_NOFMT("File open error.");
    }

    fwrite(CCAutoBuffer_getRaw(&_obj->string), 1, CCAutoBuffer_count(&_obj->string) - 1, fp);
    fprintf(fp, "");

    fclose(fp);
}



void CCString_destructor_callback_nolock(CC_obj obj)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    
    CCAutoBuffer_destructor(&_obj->string);
    
    if(_obj->cstr_buffer != NULL)
    {
        free(_obj->cstr_buffer);
    }
}


void CCString_enableLocker_callback_nolock(CC_obj obj)
{
}

CC_obj CCString_copy_callback_nolock(CC_obj obj)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;

    return CCString_create((const char*)CCAutoBuffer_getRaw(&_obj->string));
}

CC_bool_t CCString_replace_callback_nolock(CC_obj obj, CC_obj obj_from)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    struct CCString_t* _obj_from = (struct CCString_t*)obj_from;

    CCAutoBuffer_removeAll(&_obj->string);
    CCAutoBuffer_add_stream(&_obj->string, CCString_length_nolock(_obj_from), CCAutoBuffer_getRaw(&_obj_from->string));

    return CC_BOOL_TRUE;
}

size_t CCString_toHash_callbacdk_nolock(CC_obj obj)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    size_t sum = 0;

    const char* string_raw = (const char*)CCAutoBuffer_getRaw(&_obj->string);
    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->string) - 1; i++)
    {
        sum += string_raw[i];
    }
    return sum;
}

void CCString_debug_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string)
{
    CCAutoBuffer_add(string, "{");
    CCAutoBuffer_add(string, "}");
}

void CCString_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;
    CCAutoBuffer_add(string, "\"");
    CCAutoBuffer_add_stream(string, CCString_length_nolock(obj), CCAutoBuffer_getRaw(&_obj->string));
    CCAutoBuffer_add(string, "\"");
}

void CCString_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCString_t* _obj = (struct CCString_t*)obj;

    CCAutoBuffer_removeAll(&_obj->string);

    size_t len = CCJsonParser_stringLen(parser, json_token);
    const char* string = CCJsonParser_getStringRaw(parser, json_token);
    if(string != NULL)
    {
        CCAutoBuffer_add_stream(&_obj->string, len, string);
    }
    char null_char = '\0';
    CCAutoBuffer_add(&_obj->string, &null_char);
}
