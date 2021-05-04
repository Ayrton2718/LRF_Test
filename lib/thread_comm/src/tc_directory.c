#include "tc_directory.h"

#include <string.h>
#include <pthread.h>

struct TCDirectory_home_t
{
    CC_obj home;
};

struct TCDirectory_home_t g_home;

static CC_obj TCDirectory_cd(const char* path);

void TCDirectory_init(void)
{
    g_home.home = CCDictionary_create();
    CCObject_enableThreadSafe(g_home.home);
}

void TCDirectory_mkdir(const char* path, const char* name)
{
    CC_obj current_dir = TCDirectory_cd(path);

    CC_obj new_dir = CCDictionary_create();
    CCObject_enableThreadSafe(new_dir);
    CCDictionary_setObject(current_dir, new_dir, name);
}

static CC_obj TCDirectory_cd(const char* path)
{
    CC_obj current_dir = g_home.home;

    CCAutoRelease_startScope();
    {
        CC_obj array = CCAutoRelease_add(TCDirectory_splitPath(path));

        // CCLOG_INFO_NOFMT(CCString_getCString(CCAutoRelease_add(CCJsonSerializer_dumps(array, CC_BOOL_FALSE))));

        for(size_t i = 0; i < CCArray_count(array); i++)
        {
            CC_obj tmp_obj = CCDictionary_objectForKey(current_dir, CCString_getCString(CCArray_objectAtIndex(array, i)));
            if(tmp_obj != NULL)
            {
                current_dir = tmp_obj;
            }else{
                break;
            }
        }
    }
    CCAutoRelease_doneScope();

    return current_dir;
}


CC_obj TCDirectory_splitPath(const char* path)
{
    CC_obj array = CCArray_create();

    size_t path_len = strlen(path);
    size_t dir_start_index = 0;
    for(size_t i = 0; i < path_len; i++)
    {
        if(path[i] == '/')
        {
            size_t len = i - dir_start_index;
            if(0 < len)
            {
                CCArray_addObject(array, CCString_createWithLength(&path[dir_start_index], len));
            }
            dir_start_index = i + 1;
        }
    }

    // if(path_len != dir_start_index)
    // {

    // }
    int last_index = path_len - 1;
    if(0 < last_index)
    {
        if(path[last_index] != '/')
        {
            size_t len = last_index + 1 - dir_start_index;
            if(0 < len)
            {
                CCArray_addObject(array, CCString_createWithLength(&path[dir_start_index], len));
            }
        }

    }

    return array;
}

void TCDirectory_mkobj(const char* path, const char* name, CC_obj obj)
{
    CC_obj current_dir = TCDirectory_cd(path);

    CCObject_enableThreadSafe(obj);
    CCDictionary_setObject(current_dir, obj, name);
}

CC_obj TCDirectory_openObj(const char* path, const char* name)
{
    CC_obj current_dir = TCDirectory_cd(path);

    return CCDictionary_objectForKey(current_dir, name);
}

CC_obj TCDirectory_ls(const char* path)
{
    CC_obj current_dir = TCDirectory_cd(path);
    return CCJsonSerializer_dumps(current_dir, CC_BOOL_TRUE);//CCDictionary_objectForKey(current_dir, name);
}


void TCDirectory_info(const char* path)
{
    CC_obj current_dir = TCDirectory_cd(path);
    CCAutoRelease_startScope();
    {
        CC_obj info_str = CCAutoRelease_add(CCObject_info(current_dir));
        CCLOG_INFO_NOFMT(CCString_getCString(info_str));
    }
    CCAutoRelease_doneScope();
}


CC_bool_t TCDirectory_lsObj(const char* path, const char* name)
{
    CC_obj current_dir = TCDirectory_cd(path);

    if(current_dir == NULL)
    {
        return CC_BOOL_FALSE;
    }

    if(CCDictionary_objectForKey(current_dir, name) == NULL)
    {
        return CC_BOOL_FALSE;
    }else{
        return CC_BOOL_TRUE;
    }
}