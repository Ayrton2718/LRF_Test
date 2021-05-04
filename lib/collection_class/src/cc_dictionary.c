#include "cc_dictionary.h"

#include "cc_base_object.h"
#include "cc_auto_buffer.h"

#include "cc_array.h"
#include "cc_string.h"

#include "cc_log.h"

struct CCDictionary_pair_t
{
    char        key[64];
    CC_obj      obj;
};

struct CCDictionary_t
{
    struct CCBaseObject_t object;

    struct CCAutoBuffer_t pair_array;
};


static size_t CCDictionary_count_nolock(CC_obj obj);
static CC_obj CCDictionary_allKeys_nolock(CC_obj obj);
static CC_bool_t CCDictionary_containsKey_nolock(CC_obj obj, const char* key);
static CC_obj CCDictionary_allKeysForObject_nolock(CC_obj obj, CC_obj target_obj);
static CC_obj CCDictionary_objectForKey_nolock(CC_obj obj, const char* key);
static CC_obj CCDictionary_objectAtIndex_nolock(CC_obj obj, size_t index);
static void CCDictionary_setObject_nolock(CC_obj obj, CC_obj set_obj, const char* key);
static void CCDictionary_removeObjectForKey_nolock(CC_obj obj, const char* key);
static void CCDictionary_removeObjectForObj_nolock(CC_obj obj, CC_obj target_obj);
static void CCDictionary_removeAllObjects_nolock(CC_obj obj);
static CC_obj CCDictionary_objectForKey_withIndex_nolock(CC_obj obj, const char* key, size_t* index_out);

void CCDictionary_destructor_callback_nolock(CC_obj obj);
void CCDictionary_enableLocker_callback_nolock(CC_obj obj);
CC_obj CCDictionary_copy_callback_nolock(CC_obj obj);
CC_bool_t CCDictionary_replace_callback_nolock(CC_obj obj, CC_obj obj_from);
size_t CCDictionary_toHash_callback_nolock(CC_obj obj);
void CCDIctionary_debug_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* strings);
void CCDictionary_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible);
void CCDictionary_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token);


CC_obj CCDictionary_create(void)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)CCBaseObject_create(
        sizeof(struct CCDictionary_t),
        CC_DICTIONARY_TYPE_ID,
        CCDictionary_destructor_callback_nolock,
        CCDictionary_enableLocker_callback_nolock,
        CCDictionary_copy_callback_nolock,
        CCDictionary_replace_callback_nolock,
        CCDictionary_toHash_callback_nolock,
        CCDIctionary_debug_callback_nolock,
        CCDictionary_toJson_callback_nolock,
        CCDictionary_loadJson_callback_nolock);

    CCAutoBuffer_create(&_obj->pair_array, sizeof(struct CCDictionary_pair_t));

    return _obj;
}

size_t CCDictionary_count(CC_obj obj)
{
    size_t count = 0;
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        count = CCDictionary_count_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return count;
}

CC_obj CCDictionary_allKeys(CC_obj obj)
{
    CC_obj key_array = NULL;
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        key_array = CCDictionary_allKeys_nolock(obj);
        CCBaseObject_unlock(obj);
    }
    return key_array;
}

CC_bool_t CCDictionary_containsKey(CC_obj obj, const char* key)
{
    CC_bool_t is_contain = CC_BOOL_FALSE;
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        is_contain = CCDictionary_containsKey_nolock(obj, key);
        CCBaseObject_unlock(obj);
    }
    return is_contain;
}

CC_obj CCDictionary_allKeysForObject(CC_obj obj, CC_obj target_obj)
{
    CC_obj key_array = NULL;
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCDictionary_allKeysForObject_nolock(obj, target_obj);
        CCBaseObject_unlock(obj);
    }
    return key_array;
}

CC_obj CCDictionary_objectForKey(CC_obj obj, const char* key)
{
    CC_obj same_key_obj = NULL;
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        same_key_obj = CCDictionary_objectForKey_nolock(obj, key);
        CCBaseObject_unlock(obj);
    }
    return same_key_obj;
}

CC_obj CCDictionary_objectAtIndex(CC_obj obj, size_t index)
{
    CC_obj same_key_obj = NULL;
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        same_key_obj = CCDictionary_objectAtIndex_nolock(obj, index);
        CCBaseObject_unlock(obj);
    }
    return same_key_obj;
}

void CCDictionary_setObject(CC_obj obj, CC_obj set_obj, const char* key)
{
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCDictionary_setObject_nolock(obj, set_obj, key);
        CCBaseObject_unlock(obj);
    }
}

void CCDictionary_removeObjectForKey(CC_obj obj, const char* key)
{
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCDictionary_removeObjectForKey_nolock(obj, key);
        CCBaseObject_unlock(obj);
    }
}

void CCDictionary_removeObjectForObj(CC_obj obj, CC_obj target_obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCDictionary_removeObjectForObj_nolock(obj, target_obj);
        CCBaseObject_unlock(obj);
    }
}

void CCDictionary_removeAllObjects(CC_obj obj)
{
    if(CCBaseObject_isTypeObject(obj, CC_DICTIONARY_TYPE_ID))
    {
        CCBaseObject_lock(obj);
        CCDictionary_removeAllObjects_nolock(obj);
        CCBaseObject_unlock(obj);
    }
}


static size_t CCDictionary_count_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;
    return CCAutoBuffer_count(&_obj->pair_array);
}

static CC_bool_t CCDictionary_containsKey_nolock(CC_obj obj, const char* key)
{
    size_t index_tmp;
    CC_obj same_key_obj = CCDictionary_objectForKey_withIndex_nolock(obj, key, &index_tmp);
    CC_bool_t result;
    if(same_key_obj == NULL)
    {
        result = CC_BOOL_FALSE;
    }else{
        result = CC_BOOL_TRUE;
    }
    return result;
}

static CC_obj CCDictionary_allKeys_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    CC_obj array_obj = CCArray_create();
    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->pair_array); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        CCArray_addObject(array_obj, CCString_create(pair->key));
    }

    return array_obj;
}

static CC_obj CCDictionary_allKeysForObject_nolock(CC_obj obj, CC_obj target_obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    size_t target_hash = CCBaseObject_toHash(target_obj);

    CC_obj array_obj = CCArray_create();
    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->pair_array); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        if(CCBaseObject_toHash(pair->obj) == target_hash)
        {
            CCArray_addObject(array_obj, CCString_create(pair->key));
        }
    }

    return array_obj;
}

static CC_obj CCDictionary_objectForKey_nolock(CC_obj obj, const char* key)
{
    size_t index_tmp;
    return CCDictionary_objectForKey_withIndex_nolock(obj, key, &index_tmp);
}

static CC_obj CCDictionary_objectAtIndex_nolock(CC_obj obj, size_t index)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;
    
    struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, index);
    return pair->obj;
}

static void CCDictionary_setObject_nolock(CC_obj obj, CC_obj set_obj, const char* key)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    size_t index;
    CC_obj same_key_obj = CCDictionary_objectForKey_withIndex_nolock(obj, key, &index);

    if(same_key_obj == NULL)
    {
        struct CCDictionary_pair_t pair;
        size_t key_len = strlen(key);
        memcpy(pair.key, key, sizeof(char) * key_len);
        pair.key[key_len] = 0x00;
        pair.obj = set_obj;

        CCBaseObject_retain(set_obj);
        CCAutoBuffer_add(&_obj->pair_array, &pair);
    }else{
        CCBaseObject_release(same_key_obj);

        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, index);
        CCBaseObject_retain(set_obj);
        pair->obj = set_obj;
    }
}

static void CCDictionary_removeObjectForKey_nolock(CC_obj obj, const char* key)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    size_t index;
    CC_obj same_key_obj = CCDictionary_objectForKey_withIndex_nolock(obj, key, &index);

    if(same_key_obj != NULL)
    {
        CCBaseObject_release(same_key_obj);

        CCAutoBuffer_removeAtIndex(&_obj->pair_array, index);
    }
}

static void CCDictionary_removeObjectForObj_nolock(CC_obj obj, CC_obj target_obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    for(size_t i = 0; i < CCDictionary_count_nolock(obj); i++)
    {
        if(CCDictionary_objectAtIndex_nolock(obj, i) == target_obj)
        {
            CCBaseObject_release(target_obj);
            CCAutoBuffer_removeAtIndex(&_obj->pair_array, i);
        }
    }
}

static void CCDictionary_removeAllObjects_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;
    
    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->pair_array); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        CCBaseObject_release(pair->obj);
    }

    CCAutoBuffer_removeAll(&_obj->pair_array);
}

static CC_obj CCDictionary_objectForKey_withIndex_nolock(CC_obj obj, const char* key, size_t* index_out)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    CC_obj same_key_obj = NULL;
    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->pair_array); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        if(pair->key[0] == key[0])
        {
            size_t pair_key_len = strlen(pair->key);
            if(pair_key_len == strlen(key))
            {
                if(memcmp(pair->key, key, pair_key_len) == 0)
                {
                    *index_out = i;
                    same_key_obj = pair->obj;
                    break;
                }
            }
        }
    }
    return same_key_obj;
}


void CCDictionary_destructor_callback_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;
    CCDictionary_removeAllObjects_nolock(obj);

    CCAutoBuffer_destructor(&_obj->pair_array);
}

void CCDictionary_enableLocker_callback_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    for(size_t i = 0; i < CCDictionary_count_nolock(obj); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        CCBaseObject_enableLocker(pair->obj);
    }
}

CC_obj CCDictionary_copy_callback_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    struct CCDictionary_t*  copy_obj = (struct CCDictionary_t*)CCDictionary_create();

    for(size_t i = 0; i < CCDictionary_count_nolock(obj); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        CC_obj current_copy_obj = CCBaseObject_copy(pair->obj);
        CCDictionary_setObject_nolock(copy_obj, current_copy_obj, pair->key);
    }

    return copy_obj;
}


CC_bool_t CCDictionary_replace_callback_nolock(CC_obj obj, CC_obj obj_from)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;
    struct CCDictionary_t* _obj_from = (struct CCDictionary_t*)obj_from;

    for(size_t i = 0; i < CCDictionary_count(_obj_from); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj_from->pair_array, i);

        CC_obj replace_obj = CCDictionary_objectForKey_nolock(_obj, pair->key);
        if(replace_obj == NULL)
        {
            return CC_BOOL_FALSE;
        }

        if(CCBaseObject_replace(replace_obj, pair->obj) == CC_BOOL_FALSE)
        {
            return CC_BOOL_FALSE;
        }

    }

    return CC_BOOL_TRUE;
}


size_t CCDictionary_toHash_callback_nolock(CC_obj obj)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    size_t hash_sum = 0;

    for(size_t i = 0; i < CCAutoBuffer_count(&_obj->pair_array); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        size_t key_hash = 0;
        for(size_t key_index = 0; pair->key[key_index] != 0; i++)
        {
            key_hash += pair->key[key_index];
        }
        
        hash_sum += CCBaseObject_toHash(pair->obj) ^ key_hash;
    }
    return hash_sum;
}

void CCDIctionary_debug_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    CCAutoBuffer_add(string, "{");
    for(size_t i = 0; i < CCDictionary_count_nolock(obj); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        char info[68];
        sprintf(info, "\"%s\":", pair->key);
        CCAutoBuffer_add_stream(string, strlen(info), info);
        
        CCBaseObject_debug(pair->obj, string);
        if(i < CCDictionary_count_nolock(obj) - 1)
        {
            CCAutoBuffer_add(string, ",");
        }
    }
    CCAutoBuffer_add(string, "}");
}

void CCDictionary_toJson_callback_nolock(CC_obj obj, struct CCAutoBuffer_t* string, CC_bool_t visible)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    if(visible)
    {
        CCAutoBuffer_add_stream(string, 2, "{\n");
    }else{
        CCAutoBuffer_add(string, "{");
    }

    for(size_t i = 0; i < CCDictionary_count_nolock(obj); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        char info[128];
        if(visible)
        {
            sprintf(info, "\"%s\" : ", pair->key);
        }else{
            sprintf(info, "\"%s\":", pair->key);
        }
        CCAutoBuffer_add_stream(string, strlen(info), info);
        
        CCBaseObject_toJson(pair->obj, string, visible);
        if(i < CCDictionary_count_nolock(obj) - 1)
        {
            if(visible)
            {
                CCAutoBuffer_add_stream(string, 2, ", ");
            }else{
                CCAutoBuffer_add(string, ",");
            }
        }
    }

    if(visible)
    {
        CCAutoBuffer_add_stream(string, 2, "\n}");
    }else{
        CCAutoBuffer_add(string, "}");
    }
}

void CCDictionary_loadJson_callback_nolock(CC_obj obj, struct CCJsonParser_t* parser, struct CCJsonParser_jsonToken_t* json_token)
{
    struct CCDictionary_t* _obj = (struct CCDictionary_t*)obj;

    for(size_t i = 0; i < CCDictionary_count_nolock(obj); i++)
    {
        struct CCDictionary_pair_t* pair = (struct CCDictionary_pair_t*)CCAutoBuffer_readAtIndex_pointer(&_obj->pair_array, i);
        struct CCJsonParser_jsonToken_t child_token = CCJsonParser_getDictionary(parser, json_token, pair->key);
        CCBaseObject_loadJson(pair->obj, parser, &child_token);
    }
}
