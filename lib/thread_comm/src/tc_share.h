#ifndef TC_SAHRE_H
#define TC_SAHRE_H

#include <collection_class.h>

#ifdef __cplusplus
extern "C" {
#endif

void TCShare_create(const char* path, const char* name, CC_obj type_obj);

// void TCShare_isConnective(const char* path, const char* name);

typedef void* TCShare_t;

TCShare_t TCShare_writer_connect(const char* path, const char* name);
void TCShare_writer_disconnect(TCShare_t obj);
void TCShare_writer_write(TCShare_t obj, CC_obj set_obj);

TCShare_t TCShare_reader_connect(const char* path, const char* name);
void TCShare_reader_disconnect(TCShare_t obj);
CC_obj TCShare_reader_read(TCShare_t obj);

#ifdef __cplusplus
}
#endif

#endif /*TC_SAHRE_H*/
