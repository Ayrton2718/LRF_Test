#ifndef CC_BASE_OBJECT_H
#define CC_BASE_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cc_type.h"
#include "cc_auto_buffer.h"
#include "json_parser/json_parser.h"

//object.c
typedef void (*CCBaseObject_lock_callback_t)(CC_obj obj);
typedef void (*CCBaseObject_unlock_callback_t)(CC_obj obj);
typedef void (*CCBaseObject_destructor_calback_nolock_t)(CC_obj obj);
typedef void (*CCBaseObject_enableLocker_callback_t)(CC_obj obj);
typedef CC_obj (*CCBaseObject_copy_callback_nolock_t)(CC_obj obj);
typedef CC_bool_t (*CCBaseObject_replace_callback_nolock_t)(CC_obj obj, CC_obj obj_from);
typedef size_t (*CCBaseObject_toHash_callback_nolock_t)(CC_obj obj);
typedef void (*CCBaseObject_debug_callback_nolock_t)(CC_obj obj, struct CCAutoBuffer_t* string);
typedef void (*CCBaseObject_toJson_callback_nolock_t)(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
typedef void (*CCBaseObject_loadJson_callback_nolock_t)(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);

struct CCBaseObject_t
{
    CC_type_t   is_object_id;
    CC_type_t   type_id;

    size_t      retain;
    size_t      release;

    pthread_mutex_t*    locker;

    CCBaseObject_lock_callback_t     lock_callback;
    CCBaseObject_unlock_callback_t   unlock_callback;

    CCBaseObject_destructor_calback_nolock_t        destructor_callback;
    CCBaseObject_enableLocker_callback_t            enableLocker_callback;
    CCBaseObject_copy_callback_nolock_t             copy_callback;
    CCBaseObject_replace_callback_nolock_t          replace_callback;
    CCBaseObject_toHash_callback_nolock_t           toHash_callback;
    CCBaseObject_debug_callback_nolock_t            debug_callback;
    CCBaseObject_toJson_callback_nolock_t           toJson_callback;
    CCBaseObject_loadJson_callback_nolock_t         loadJson_callback;
};

CC_obj CCBaseObject_create(size_t size, CC_type_t type_id, CCBaseObject_destructor_calback_nolock_t destraructor, CCBaseObject_enableLocker_callback_t enableLocker, CCBaseObject_copy_callback_nolock_t copy, CCBaseObject_replace_callback_nolock_t replace, CCBaseObject_toHash_callback_nolock_t toHash, CCBaseObject_debug_callback_nolock_t debug, CCBaseObject_toJson_callback_nolock_t toJson, CCBaseObject_loadJson_callback_nolock_t loadJson);

CC_bool_t CCBaseObject_isTypeObject(CC_obj obj, CC_type_t id);
CC_bool_t CCBaseObject_isObject(CC_obj obj);
CC_type_t CCBaseObject_getTypeId(CC_obj obj);

CC_bool_t CCBaseObject_checkObject(CC_obj obj);

void CCBaseObject_retain_nolock(CC_obj obj);
void CCBaseObject_retain(CC_obj obj);
void CCBaseObject_release(CC_obj obj);

int CCBaseObject_referenceCount(CC_obj obj);
void CCBaseObject_destructor(CC_obj obj);

CC_obj CCBaseObject_copy(CC_obj obj);
CC_bool_t CCBaseObject_replace(CC_obj obj, CC_obj obj_from);
size_t CCBaseObject_toHash(CC_obj obj);
void CCBaseObject_debug(CC_obj obj, struct CCAutoBuffer_t* string);
void CCBaseObject_toJson(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCBaseObject_loadJson(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);

CC_bool_t CCBaseObject_isLockerEnable(CC_obj obj);
void CCBaseObject_enableLocker(CC_obj obj);
void CCBaseObject_lock(CC_obj obj);
void CCBaseObject_unlock(CC_obj obj);

#ifdef __cplusplus
}
#endif

#endif /*CC_BASE_OBJECT_H*/
