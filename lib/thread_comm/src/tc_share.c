#include "tc_share.h"
#include "tc_directory.h"

void TCShare_create(const char* path, const char* name, CC_obj type_obj)
{
    CCObject_enableThreadSafe(type_obj);
    TCDirectory_mkobj(path, name, type_obj);
}

struct TCShare_writer_t
{
    CC_obj data_obj;
};


TCShare_t TCShare_writer_connect(const char* path, const char* name)
{
    struct TCShare_writer_t* _obj = (struct TCShare_writer_t*)malloc(sizeof(struct TCShare_writer_t));
    _obj->data_obj = TCDirectory_openObj(path, name);
    CCObject_retain(_obj->data_obj);
    return _obj;
}

void TCShare_writer_disconnect(TCShare_t obj)
{
    struct TCShare_writer_t* _obj = (struct TCShare_writer_t*)obj;
    CCObject_release(_obj->data_obj);
    free(_obj);
}

void TCShare_writer_write(TCShare_t obj, CC_obj set_obj)
{
    struct TCShare_writer_t* _obj = (struct TCShare_writer_t*)obj;
    CCObject_replace(_obj->data_obj, set_obj);
}


struct TCShare_reader_t
{
    CC_obj data_obj;
};

TCShare_t TCShare_reader_connect(const char* path, const char* name)
{
    struct TCShare_reader_t* _obj = (struct TCShare_reader_t*)malloc(sizeof(struct TCShare_reader_t));
    _obj->data_obj = TCDirectory_openObj(path, name);
    CCObject_retain(_obj->data_obj);
    return _obj;
}

void TCShare_reader_disconnect(TCShare_t obj)
{
    struct TCShare_reader_t* _obj = (struct TCShare_reader_t*)obj;
    CCObject_release(_obj->data_obj);
    free(_obj);
}

CC_obj TCShare_reader_read(TCShare_t obj)
{
    struct TCShare_reader_t* _obj = (struct TCShare_reader_t*)obj;
    return CCObject_copy(_obj->data_obj);
}
