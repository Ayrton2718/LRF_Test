#ifndef TC_DIRECTORY_H
#define TC_DIRECTORY_H

#include <collection_class.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RMM_TC_DIRECTORY_NAME_LENGTH    (32)

void TCDirectory_init(void);

// max 16 dipth, One dir name is 31 charactors.
CC_obj TCDirectory_splitPath(const char* path);
void TCDirectory_mkdir(const char* path, const char* name);
void TCDirectory_mkobj(const char* path, const char* name, CC_obj obj);
CC_obj TCDirectory_openObj(const char* path, const char* name);
CC_obj TCDirectory_ls(const char* path);
CC_bool_t TCDirectory_lsObj(const char* path, const char* name);


void TCDirectory_info(const char* path);

#ifdef __cplusplus
}
#endif

#endif /*TC_DIRECTORY_H*/
