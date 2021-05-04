#include "cc_base_object.h"

#include "cc_mem_debugger.h"
#include "cc_log.h"
#include "cc_locker.h"
#include "cc_dead_queue.h"

static void CCBaseObject_destructor_nolock(CC_obj obj);
static int CCBaseObject_referenceCount_nolock(CC_obj obj);
static CC_obj CCBaseObject_copy_nolock(CC_obj obj);
static CC_bool_t CCBaseObject_replace_nolock(CC_obj obj, CC_obj obj_from);
static size_t CCBaseObject_toHash_nolock(CC_obj obj);
static void CCBaseObject_debug_nolock(CC_obj obj, struct CCAutoBuffer_t* string);
static void CCBaseObject_toJson_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
static void CCBaseObject_loadJson_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);

static CC_bool_t CCBaseObject_isLockerEnable_nolock(CC_obj obj);

void CCBaseObject_using_lock_callback(CC_obj obj);
void CCBaseObject_unusinge_lock_callback(CC_obj obj);
void CCBaseObject_using_unlock_callback(CC_obj obj);
void CCBaseObject_unusinge_unlock_callback(CC_obj obj);

CC_obj CCBaseObject_create(size_t size, CC_type_t type_id, CCBaseObject_destructor_calback_nolock_t destraructor, CCBaseObject_enableLocker_callback_t enableLocker, CCBaseObject_copy_callback_nolock_t copy, CCBaseObject_replace_callback_nolock_t replace, CCBaseObject_toHash_callback_nolock_t toHash, CCBaseObject_debug_callback_nolock_t debug, CCBaseObject_toJson_callback_nolock_t toJson, CCBaseObject_loadJson_callback_nolock_t loadJson)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)CCMALLOC(size, "object");
    _obj->is_object_id = CC_OBJECT_ID;
    _obj->type_id = type_id;
    _obj->retain = 0;
    _obj->release = 0;

    _obj->locker = NULL;

    _obj->lock_callback = CCBaseObject_unusinge_lock_callback;
    _obj->unlock_callback = CCBaseObject_unusinge_unlock_callback;

    _obj->destructor_callback = destraructor;
    _obj->enableLocker_callback = enableLocker;
    _obj->copy_callback = copy;
    _obj->replace_callback = replace;
    _obj->toHash_callback = toHash;
    _obj->debug_callback = debug;
    _obj->toJson_callback = toJson;
    _obj->loadJson_callback = loadJson;

    return _obj;
}

CC_bool_t CCBaseObject_isTypeObject(CC_obj obj, CC_type_t id)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    if(CCBaseObject_isObject(obj))
    {
        if(_obj->type_id != id)
        {
            CCLOG_ERROR("Type error. %s cannot use as %s!", CCType_toName(_obj->type_id), CCType_toName(id));
            return CC_BOOL_FALSE;
        }
    }else{
        return CC_BOOL_FALSE;
    }

    return CC_BOOL_TRUE;
}

CC_bool_t CCBaseObject_isObject(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    if(obj == NULL)
    {
        CCLOG_ERROR_NOFMT("Type error. NULL pointer detected!");
        return CC_BOOL_FALSE;
    }

    if(_obj->is_object_id != CC_OBJECT_ID)
    {
        CCLOG_ERROR_NOFMT("Type error. invited object!");
        return CC_BOOL_FALSE;
    }

    return CC_BOOL_TRUE;
}


CC_type_t CCBaseObject_getTypeId(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    CC_type_t type_id = CC_UNKNOWN_TYPE_ID;

    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        type_id = _obj->type_id;
        CCBaseObject_unlock(obj);
    }
    return type_id;
}


CC_bool_t CCBaseObject_checkObject(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    if(obj == NULL)
    {
        return CC_BOOL_FALSE;
    }

    if(_obj->is_object_id != CC_OBJECT_ID)
    {
        return CC_BOOL_FALSE;
    }

    return CC_BOOL_TRUE;
}


void CCBaseObject_retain_nolock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    _obj->retain++;
}

void CCBaseObject_retain(CC_obj obj)
{
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        CCBaseObject_retain_nolock(obj);
        CCBaseObject_unlock(obj);
    }
}

void CCBaseObject_release(CC_obj obj)
{
    if(CCBaseObject_isObject(obj))
    {
        struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
        CCBaseObject_lock(obj);
        _obj->release++;
        
        if(CCBaseObject_referenceCount_nolock(obj) <= 0)
        {
            if(CCBaseObject_isLockerEnable_nolock(obj) == CC_BOOL_TRUE)
            {
                CCBaseObject_unlock(obj);
                CCDeadQueue_add(obj);
            }else{
                CCBaseObject_destructor_nolock(obj);
            }
        }else{
            CCBaseObject_unlock(obj);
        }
    }
}

int CCBaseObject_referenceCount(CC_obj obj)
{
    int count = 0;
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        count = CCBaseObject_referenceCount_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return count;
}

void CCBaseObject_lock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    _obj->lock_callback(obj);
}

void CCBaseObject_unlock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    _obj->unlock_callback(obj);
}

CC_obj CCBaseObject_copy(CC_obj obj)
{
    CC_obj copy_obj = NULL;
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        copy_obj = CCBaseObject_copy_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return copy_obj;
}

CC_bool_t CCBaseObject_replace(CC_obj obj, CC_obj obj_from)
{
    if(CCBaseObject_isObject(obj) && CCBaseObject_isObject(obj_from))
    {
        CCBaseObject_lock(obj);
        CCBaseObject_lock(obj_from);
        CC_bool_t result = CCBaseObject_replace_nolock(obj, obj_from);
        CCBaseObject_unlock(obj_from);
        CCBaseObject_unlock(obj);

        return result;
    }else{
        return CC_BOOL_FALSE;
    }
}

size_t CCBaseObject_toHash(CC_obj obj)
{
    size_t hash = 0;
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        hash = CCBaseObject_toHash_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return hash;
}

void CCBaseObject_debug(CC_obj obj, struct CCAutoBuffer_t* string)
{
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        CCBaseObject_debug_nolock(obj, string);
        CCBaseObject_unlock(obj);
    }
}

void CCBaseObject_toJson(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        CCBaseObject_toJson_nolock(obj, string, visible);
        CCBaseObject_unlock(obj);
    }
}

void CCBaseObject_loadJson(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        CCBaseObject_loadJson_nolock(obj, parser, json_token);
        CCBaseObject_unlock(obj);
    }
}

void CCBaseObject_destructor(CC_obj obj)
{
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        CCBaseObject_destructor_nolock(obj);
    }
}

static void CCBaseObject_destructor_nolock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;

    if(_obj->locker != NULL)
    {
        _obj->destructor_callback(obj);
        _obj->is_object_id = 0;

        CCLocker_lockerUnlock(_obj->locker);
        CCLocker_lockerDestroy(_obj->locker);
        CCFREE(_obj->locker, "locker");

        CCFREE(obj, "object");
    }else{
        _obj->destructor_callback(obj);
        _obj->is_object_id = 0;
        CCFREE(obj, "object");
    }
}

static int CCBaseObject_referenceCount_nolock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    return _obj->retain - _obj->release;
}

static CC_obj CCBaseObject_copy_nolock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    CC_obj clone_obj = _obj->copy_callback(obj);
    return clone_obj;
}

CC_bool_t CCBaseObject_replace_nolock(CC_obj obj, CC_obj obj_from)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    struct CCBaseObject_t* _obj_from = (struct CCBaseObject_t*)obj_from;

    if(_obj->type_id != _obj_from->type_id)
    {
        return CC_BOOL_FALSE;
    }
    return _obj->replace_callback(_obj, _obj_from);
}

static size_t CCBaseObject_toHash_nolock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    size_t hash = _obj->toHash_callback(obj);
    return hash;
}

static void CCBaseObject_debug_nolock(CC_obj obj, struct CCAutoBuffer_t* string)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    char info[64];
    char* is_enable_locker;
    if(_obj->locker == NULL)
    {
        is_enable_locker = (char*)"disable";
    }else{
        is_enable_locker = (char*)"enable";
    }
    sprintf(info, "{\"%s(%zu,%zu, %s)\":", CCType_toName(_obj->type_id), _obj->retain, _obj->release, is_enable_locker);
    CCAutoBuffer_add_stream(string, strlen(info), info);
    
    _obj->debug_callback(obj, string);
    CCAutoBuffer_add(string, "}");
}

static void CCBaseObject_toJson_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    // char info[64];
    // sprintf(info, "{\"%s(%zu,%zu)\":", CCType_toName(_obj->type_id), _obj->retain, _obj->release);
    // CCAutoBuffer_add_stream(string, strlen(info), info);
    
    _obj->toJson_callback(obj, string, visible);
    // CCAutoBuffer_add(string, "}");
}


static void CCBaseObject_loadJson_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    _obj->loadJson_callback(obj, parser, json_token);
}


CC_bool_t CCBaseObject_isLockerEnable(CC_obj obj)
{
    CC_bool_t is_enable = CC_BOOL_FALSE;
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);
        is_enable = CCBaseObject_isLockerEnable_nolock(obj);
        CCBaseObject_unlock(obj);
    }

    return is_enable;
}

CC_bool_t CCBaseObject_isLockerEnable_nolock(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    if(_obj->locker == NULL)
    {
        return CC_BOOL_FALSE;
    }else{
        return CC_BOOL_TRUE;
    }
}

void CCBaseObject_enableLocker(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    if(CCBaseObject_isObject(obj))
    {
        CCBaseObject_lock(obj);

        if(CCBaseObject_isLockerEnable_nolock(obj) == CC_BOOL_FALSE)
        {
            _obj->locker = (pthread_mutex_t*)CCMALLOC(sizeof(pthread_mutex_t), "locker");
            CCLocker_lockerInit(_obj->locker);
            _obj->lock_callback = CCBaseObject_using_lock_callback;
            _obj->unlock_callback = CCBaseObject_using_unlock_callback;

            CCBaseObject_lock(obj);
        }

        _obj->enableLocker_callback(obj);
        CCBaseObject_unlock(obj);
    }
}

void CCBaseObject_using_lock_callback(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    CCLocker_lockerLock(_obj->locker);
}

void CCBaseObject_unusinge_lock_callback(CC_obj obj)
{
}

void CCBaseObject_using_unlock_callback(CC_obj obj)
{
    struct CCBaseObject_t* _obj = (struct CCBaseObject_t*)obj;
    CCLocker_lockerUnlock(_obj->locker);
}

void CCBaseObject_unusinge_unlock_callback(CC_obj obj)
{
}
