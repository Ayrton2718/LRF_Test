#include "tc_signal.h"
#include "tc_directory.h"

#include "tc_type.h"

void TCSignal_create(const char* path, const char* name)
{
    CC_obj receivers = CCDictionary_create();
    CCObject_enableThreadSafe(receivers);
    TCDirectory_mkobj(path, name, receivers);
}

struct TCSignal_writer_t
{
    CC_obj receivers;
};

TCSignal_t TCSignal_talker_connect(const char* path, const char* name)
{
    struct TCSignal_writer_t* _obj = (struct TCSignal_writer_t*)malloc(sizeof(struct TCSignal_writer_t));
    _obj->receivers = TCDirectory_openObj(path, name);
    CCObject_retain(_obj->receivers);
    return _obj;
}

void TCSignal_talker_disconnect(TCSignal_t obj)
{
    struct TCSignal_writer_t* _obj = (struct TCSignal_writer_t*)obj;
    CCObject_release(_obj->receivers);
    free(_obj);
}

void TCSignal_talker_send(TCSignal_t obj, CC_obj set_obj)
{
    struct TCSignal_writer_t* _obj = (struct TCSignal_writer_t*)obj;

    CC_obj clone_send_obj = CCObject_copy(set_obj);
    CCObject_enableThreadSafe(clone_send_obj);

    for(size_t i = 0; i < CCDictionary_count(_obj->receivers); i++)
    {
        CC_obj queue = CCDictionary_objectAtIndex(_obj->receivers, i);
        CCQueue_add(queue, clone_send_obj);
    }
}

struct TCSignal_listener_t
{
    CC_obj receivers;
    CC_obj queue;
};

TCSignal_t TCSignal_listener_connect(const char* path, const char* name, const char* listener_name)
{
    struct TCSignal_listener_t* _obj = (struct TCSignal_listener_t*)malloc(sizeof(struct TCSignal_listener_t));
    _obj->receivers = TCDirectory_openObj(path, name);
    CCObject_retain(_obj->receivers);

    _obj->queue = CCQueue_create();
    CCObject_enableThreadSafe(_obj->queue);
    CCDictionary_setObject(_obj->receivers, _obj->queue, listener_name);

    return _obj;
}

void TCSignal_listener_disconnect(TCSignal_t obj)
{
    struct TCSignal_listener_t* _obj = (struct TCSignal_listener_t*)obj;
    CCDictionary_removeObjectForObj(_obj->receivers, _obj->queue);
    CCObject_release(_obj->receivers);
    free(_obj);
}

size_t TCSignal_listener_readbleCount(TCSignal_t obj)
{
    struct TCSignal_listener_t* _obj = (struct TCSignal_listener_t*)obj;

    return CCQueue_count(_obj->queue);
}

CC_obj TCSignal_listener_receive(TCSignal_t obj)
{
    struct TCSignal_listener_t* _obj = (struct TCSignal_listener_t*)obj;
    CC_obj receive_obj = CCObject_copy(CCQueue_peek(_obj->queue));
    CCQueue_remove(_obj->queue);

    return receive_obj;
}


CC_obj TCSignal_listener_receiveWait(TCSignal_t obj)
{
    struct TCSignal_listener_t* _obj = (struct TCSignal_listener_t*)obj;

    while(CCQueue_readble(_obj->queue) == CC_BOOL_FALSE)
    {
        CCType_yield();
    }

    CC_obj receive_obj = CCObject_copy(CCQueue_peek(_obj->queue));
    CCQueue_remove(_obj->queue);

    return receive_obj;
}


CC_bool_t TCSignal_listener_isAvailable(TCSignal_t obj)
{
    struct TCSignal_listener_t* _obj = (struct TCSignal_listener_t*)obj;

    CC_obj receive_obj = CCQueue_peek(_obj->queue);
    if(CCObject_isObject(receive_obj))
    {
        return CC_BOOL_TRUE;
    }else{
        return CC_BOOL_FALSE;
    }
}
