#include "cc_object.h"

#include "cc_base_object.h"
#include "cc_auto_buffer.h"
#include "cc_string.h"

void CCObject_retain(CC_obj obj)
{
    CCBaseObject_retain(obj);
}

void CCObject_release(CC_obj obj)
{
    CCBaseObject_release(obj);
}

void CCObject_enableThreadSafe(CC_obj obj)
{
    CCBaseObject_enableLocker(obj);
}

CC_obj CCObject_copy(CC_obj obj)
{
    return CCBaseObject_copy(obj);
}

CC_bool_t CCObject_replace(CC_obj obj, CC_obj obj_from)
{
    return CCBaseObject_replace(obj, obj_from);
}

CC_bool_t CCObject_isObject(CC_obj obj)
{
    return CCBaseObject_checkObject(obj);
}

CC_obj CCObject_info(CC_obj obj)
{
    struct CCAutoBuffer_t string;
    CCAutoBuffer_create(&string, sizeof(char));
    CCBaseObject_debug(obj, &string);
    CCAutoBuffer_add(&string, "");
    CC_obj str_obj = CCString_create((const char*)CCAutoBuffer_getRaw(&string));
    CCAutoBuffer_destructor(&string);
    return str_obj;
}
