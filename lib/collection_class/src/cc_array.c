#include "cc_array.h"

#include "cc_base_object.h"
#include "cc_auto_buffer.h"
#include "cc_log.h"

struct CCArray_t
{
    struct CCBaseObject_t object;
    struct CCAutoBuffer_t objs;
};


static size_t CCArray_count_nolock(CC_obj obj);
static CC_bool_t CCArray_containsObject_nolock(CC_obj obj, CC_obj target_obj);
static size_t CCArray_indexOfObject_nolock(CC_obj obj, CC_obj target_obj);
static CC_obj CCArray_objectAtIndex_nolock(CC_obj obj, size_t index);
static CC_obj CCArray_lastObject_nolock(CC_obj obj);
static void CCArray_addObject_nolock(CC_obj obj, CC_obj target_obj);
static void CCArray_addObjectsFromArray_nolock(CC_obj obj, CC_obj target_array);
static void CCArray_insertObject_nolock(CC_obj obj, size_t index, CC_obj target_obj);
static void CCArray_replaceObjectAtIndex_nolock(CC_obj obj, size_t index, CC_obj target_obj);
static void CCArray_removeLastObject_nolock(CC_obj obj);
static void CCArray_removeObject_nolock(CC_obj obj, CC_obj target_obj);
static void CCArray_removeObjectAtIndex_nolock(CC_obj obj, size_t index);
static void CCArray_removeAllObject_nolock(CC_obj obj);

void CCArray_destructor_callback_nolock(CC_obj obj);
void CCArray_enableLocker_callback_nolock(CC_obj obj);
CC_obj CCArray_copy_callback_nolock(CC_obj obj);
CC_bool_t CCArray_replace_callback_nolock(CC_obj obj, CC_obj obj_from);
size_t CCArray_toHash_callback_nolock(CC_obj obj);
void CCArray_debug_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* strings);
void CCArray_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCArray_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCArray_create(void)
{
    struct CCArray_t* _obj = (struct CCArray_t*)CCBaseObject_create(
        sizeof(struct CCArray_t), 
        CC_ARRAY_TYPE_ID, 
        CCArray_destructor_callback_nolock, 
        CCArray_enableLocker_callback_nolock,
        CCArray_copy_callback_nolock, 
        CCArray_replace_callback_nolock,
        CCArray_toHash_callback_nolock, 
        CCArray_debug_callback_nolock, 
        CCArray_toJson_callback_nolock,
        CCArray_loadJson_callback_nolock);

    CCAutoBuffer_create(&_obj->objs, sizeof(CC_obj));
    return _obj;
}

size_t CCArray_count(CC_obj obj)
{
    size_t count = 0;
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        count = CCArray_count_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return count;
}

CC_bool_t CCArray_containsObject(CC_obj obj, CC_obj target_obj)
{
    CC_bool_t is_contain = CC_BOOL_FALSE;
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        is_contain = CCArray_containsObject_nolock(obj, target_obj);
        CCBaseObject_unlock(obj);
    }
    return is_contain;
}

size_t CCArray_indexOfObject(CC_obj obj, CC_obj target_obj)
{
    size_t index = 0;
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        index = CCArray_indexOfObject_nolock(obj, target_obj);
        CCBaseObject_unlock(obj);
    }
    return index;
}

CC_obj CCArray_objectAtIndex(CC_obj obj, size_t index)
{
    CC_obj result_obj = NULL;
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result_obj = CCArray_objectAtIndex_nolock(obj, index);
        CCBaseObject_unlock(obj);
    }
    return result_obj;
}

CC_obj CCArray_lastObject(CC_obj obj)
{
    CC_obj result_obj = NULL;
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        result_obj = CCArray_lastObject_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return result_obj;
}

void CCArray_addObject(CC_obj obj, CC_obj target_obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_addObject_nolock(obj, target_obj);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_addObjectsFromArray(CC_obj obj, CC_obj target_array)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_addObjectsFromArray_nolock(obj, target_array);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_insertObject(CC_obj obj, size_t index, CC_obj target_obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_insertObject_nolock(obj, index, target_obj);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_replaceObjectAtIndex(CC_obj obj, size_t index, CC_obj target_obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_replaceObjectAtIndex_nolock(obj, index, target_obj);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_removeLastObject(CC_obj obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_removeLastObject_nolock(obj);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_removeObject(CC_obj obj, CC_obj target_obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_removeObject_nolock(obj, target_obj);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_removeObjectAtIndex(CC_obj obj, size_t index)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_removeObjectAtIndex_nolock(obj, index);
        CCBaseObject_unlock(obj);
    }
}

void CCArray_removeAllObject(CC_obj obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_ARRAY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCArray_removeAllObject_nolock(obj);
        CCBaseObject_unlock(obj);
    }
}


static size_t CCArray_count_nolock(CC_obj obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;
    return CCAutoBuffer_count(&_obj->objs);
}

static CC_bool_t CCArray_containsObject_nolock(CC_obj obj, CC_obj target_obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    size_t target_obj_hash = CCBaseObject_toHash(target_obj);

    CC_bool_t is_contain = CC_BOOL_FALSE;

    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->objs); i++)
    {
        CC_obj current_obj = *(CC_obj*)CCAutoBuffer_readAtIndex_pointer(&_obj->objs, i);
        if(CCBaseObject_toHash(current_obj) == target_obj_hash)
        {
            is_contain = CC_BOOL_TRUE;
            break;
        }
    }
    
    return is_contain;
}

static size_t CCArray_indexOfObject_nolock(CC_obj obj, CC_obj target_obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    size_t target_obj_hash = CCBaseObject_toHash(target_obj);

    size_t index = UINT_MAX;

    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->objs); i++)
    {
        CC_obj current_obj = *(CC_obj*)CCAutoBuffer_readAtIndex_pointer(&_obj->objs, i);

        if(CCBaseObject_toHash(current_obj) == target_obj_hash)
        {
            index = i;
            break;
        }
    }
    
    return index;
}

static CC_obj CCArray_objectAtIndex_nolock(CC_obj obj, size_t index)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;
    if(CCAutoBuffer_count(&_obj->objs) <= index)
    {
        CCLOG_ERROR_NOFMT("Index out of range!");
        return NULL;
    }

    CC_obj result_obj;
    CCAutoBuffer_readAtIndex(&_obj->objs, index, &result_obj);
    return result_obj;
}

static CC_obj CCArray_lastObject_nolock(CC_obj obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;
    if(CCAutoBuffer_count(&_obj->objs) == 0)
    {
        CCLOG_ERROR_NOFMT("This array is empty.");
        return NULL;
    }

    CC_obj result_obj;
    CCAutoBuffer_readLast(&_obj->objs, &result_obj);
    return result_obj;
}

static void CCArray_addObject_nolock(CC_obj obj, CC_obj target_obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    CCBaseObject_retain(target_obj);
    CCAutoBuffer_add(&_obj->objs, &target_obj);
}

static void CCArray_addObjectsFromArray_nolock(CC_obj obj, CC_obj target_array)
{
    for(size_t i = 0; i < CCArray_count_nolock(target_array); i++)
    {
        CC_obj add_obj = CCArray_objectAtIndex_nolock(target_array, i);
        CCArray_addObject_nolock(obj, add_obj);
    }
}

static void CCArray_insertObject_nolock(CC_obj obj, size_t index, CC_obj target_obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    if(CCAutoBuffer_count(&_obj->objs) < index)
    {
        CCLOG_ERROR_NOFMT("Index out of range!");
        return;
    }
    
    CCBaseObject_retain(target_obj);
    CCAutoBuffer_insert(&_obj->objs, index, &target_obj);
}

static void CCArray_replaceObjectAtIndex_nolock(CC_obj obj, size_t index, CC_obj target_obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    if(CCAutoBuffer_count(&_obj->objs) <= index)
    {
        CCLOG_ERROR_NOFMT("Index out of range!");
        return;
    }

    if(index < CCAutoBuffer_count(&_obj->objs))
    {
        CC_obj befo_obj;
        CCAutoBuffer_readAtIndex(&_obj->objs, index, &befo_obj);
        CCBaseObject_release(befo_obj);

        CCBaseObject_retain(target_obj);
        CCAutoBuffer_replace(&_obj->objs, index, &target_obj);
    }
}

static void CCArray_removeLastObject_nolock(CC_obj obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    if(CCAutoBuffer_count(&_obj->objs) == 0)
    {
        CCLOG_ERROR_NOFMT("This array is empty.");
        return;
    }

    CC_obj remove_obj;
    CCAutoBuffer_readLast(&_obj->objs, &remove_obj);
    CCBaseObject_release(remove_obj);
    CCAutoBuffer_removeLast(&_obj->objs);
}

static void CCArray_removeObject_nolock(CC_obj obj, CC_obj target_obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    size_t target_obj_hash = CCBaseObject_toHash(target_obj);

    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->objs); i++)
    {
        CC_obj current_obj = *(CC_obj*)CCAutoBuffer_readAtIndex_pointer(&_obj->objs, i);
        if(CCBaseObject_toHash(current_obj) == target_obj_hash)
        {
            CCBaseObject_release(current_obj);
            CCAutoBuffer_removeAtIndex(&_obj->objs, i);
        }
    }
}

static void CCArray_removeObjectAtIndex_nolock(CC_obj obj, size_t index)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;
    if(CCAutoBuffer_count(&_obj->objs) <= index)
    {
        CCLOG_ERROR_NOFMT("Index out of range!");
        return;
    }

    CC_obj remove_obj;
    CCAutoBuffer_readAtIndex(&_obj->objs, index, &remove_obj);
    CCBaseObject_release(remove_obj);
    CCAutoBuffer_removeAtIndex(&_obj->objs, index);
}

static void CCArray_removeAllObject_nolock(CC_obj obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;

    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->objs); i++)
    {
        CC_obj current_obj = *(CC_obj*)CCAutoBuffer_readAtIndex_pointer(&_obj->objs, i);
        CCBaseObject_release(current_obj);
    }
    CCAutoBuffer_removeAll(&_obj->objs);
}


void CCArray_destructor_callback_nolock(CC_obj obj)
{
    struct CCArray_t* _obj = (struct CCArray_t*)obj;
    CCArray_removeAllObject_nolock(obj);
    CCAutoBuffer_destructor(&_obj->objs);
}

void CCArray_enableLocker_callback_nolock(CC_obj obj)
{
    for(size_t i = 0; i < CCArray_count_nolock(obj); i++)
    {
        CCBaseObject_enableLocker(CCArray_objectAtIndex_nolock(obj, i));
    }
}

CC_obj CCArray_copy_callback_nolock(CC_obj obj)
{
    CC_obj copy_obj = CCArray_create();
    CCArray_addObjectsFromArray_nolock(copy_obj, obj);
    return copy_obj;
}

CC_bool_t CCArray_replace_callback_nolock(CC_obj obj, CC_obj obj_from)
{
    for(size_t i = 0; i < CCArray_count_nolock(obj_from); i++)
    {
        CC_obj current_obj_from = CCArray_objectAtIndex_nolock(obj_from, i);
        CC_obj current_replaced_obj = CCArray_objectAtIndex_nolock(obj, i);

        if(current_replaced_obj == NULL)
        {
            return CC_BOOL_FALSE;
        }

        if(CCBaseObject_replace(current_replaced_obj, current_obj_from) == CC_BOOL_FALSE)
        {
            return CC_BOOL_FALSE;
        }
    }

    return CC_BOOL_TRUE;
}

size_t CCArray_toHash_callback_nolock(CC_obj obj)
{
    size_t sum = 0;
    for(size_t i = 0; i < CCArray_count_nolock(obj); i++)
    {
        sum += CCBaseObject_toHash((CC_obj)CCArray_objectAtIndex_nolock(obj, i)) ^ i;
    }

    return sum;
}

void CCArray_debug_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string)
{
    CCAutoBuffer_add(string, "[");
    for(size_t i = 0; i < CCArray_count_nolock(obj); i++)
    {
        CCBaseObject_debug(CCArray_objectAtIndex_nolock(obj, i), string);
        if(i < CCArray_count_nolock(obj) - 1)
        {
            CCAutoBuffer_add(string, ",");
        }
    }
    CCAutoBuffer_add(string, "]");
}

void CCArray_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    CCAutoBuffer_add(string, "[");
    for(size_t i = 0; i < CCArray_count_nolock(obj); i++)
    {
        CCBaseObject_toJson(CCArray_objectAtIndex_nolock(obj, i), string, visible);
        if(i < CCArray_count_nolock(obj) - 1)
        {
            if(visible)
            {
                CCAutoBuffer_add_stream(string, 2, ", ");
            }else{
                CCAutoBuffer_add(string, ",");
            }
        }
    }
    CCAutoBuffer_add(string, "]");
}

void CCArray_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    for(size_t i = 0; i < CCArray_count_nolock(obj); i++)
    {
        struct CCJsonParser_jsonToken_t child_token = CCJsonParser_getArray(parser, json_token, i);
        CCBaseObject_loadJson(CCArray_objectAtIndex_nolock(obj, i), parser, &child_token);
    }
}
