#include "json_parser.h"

#include "../cc_mem_debugger.h"
#include "../cc_log.h"


static struct CCJsonParser_jsonToken_t CCJsonParser_searchTokCurRange(struct CCJsonParser_t* obj, size_t tokenIndex)
{
    struct CCJsonParser_jsonToken_t range = {tokenIndex, obj->parser.toknext-tokenIndex};
    int parent = obj->parser.tokens[tokenIndex].parent;

    for( size_t i=1; i< obj->parser.toknext - tokenIndex; i++ )
    {
        if( obj->parser.tokens[tokenIndex+i].parent <= parent )
        {
            range.count = i;//-1;
            break;
        }
    }

    return range;
}

CC_bool_t CCJsonParser_parse(struct CCJsonParser_t* obj, const char* js, size_t js_len)
{
    obj->js = (char*)malloc(sizeof(char) * js_len + 1);
    memcpy(obj->js, js, js_len);
    obj->js[js_len] = '\0';

    jsmn_init(&obj->parser);

    // struct CCJsonParser_jsonToken_t token;
    int parse_result = jsmn_parse(&obj->parser, obj->js, js_len);
    if(parse_result < 0)
    {
        if( parse_result == JSMN_ERROR_INVAL || parse_result == JSMN_ERROR_PART )
        {
            CCLOG_ERROR_NOFMT("Inval json format");
        }

        CCLOG_ERROR_NOFMT("Json parse failed");
        return CC_BOOL_FALSE;
    }

    return CC_BOOL_TRUE;
}


struct CCJsonParser_jsonToken_t CCJsonParser_getRoot(struct CCJsonParser_t* obj)
{
    struct CCJsonParser_jsonToken_t token;
    token.index_start = 0;
    token.count = obj->parser.toknext;

    return token;
}


void CCJsonParser_showResult(struct CCJsonParser_t* obj, struct CCJsonParser_jsonToken_t* token)
{
    for(size_t i = 0; i < token->count; i++)
    {
        CCLOG_INFO("(index:%d) %d, %d", i, obj->parser.tokens[i + token->index_start].type, obj->parser.tokens[i + token->index_start].parent);
    }
}


void CCJsonParser_destructor(struct CCJsonParser_t* obj)
{
    jsmn_destructor(&obj->parser);
    free(obj->js);
}


jsmntype_t CCJsonParser_jsonType(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token)
{
    return obj->parser.tokens[token->index_start].type;
}


struct CCJsonParser_jsonToken_t CCJsonParser_getDictionary(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, const char* key)
{
    struct CCJsonParser_jsonToken_t result_token = {0, 0};
    size_t tokenLen = strlen( key );

    for(size_t i = 0; i < token->count; i++)
    {
        if(obj->parser.tokens[i + token->index_start].parent == (int)token->index_start)
        {
            size_t keyLength = (size_t)(obj->parser.tokens[i + token->index_start].end - obj->parser.tokens[i + token->index_start].start);
            if(tokenLen == keyLength && (strncmp( &obj->js[obj->parser.tokens[i + token->index_start].start], key, keyLength )== 0))
            {
                result_token = CCJsonParser_searchTokCurRange(obj, i + token->index_start + 1);
                break;
            }
        }
    }

    return result_token;
}


size_t CCJsonParser_dictionaryCount(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token)
{
    size_t count = 0;

    for(size_t i = 0; i < token->count; i++)
    {
        if(obj->parser.tokens[i + token->index_start].parent == (int)token->index_start)
        {
            count++;
        }
    }

    return count;
}


struct CCJsonParser_jsonToken_t CCJsonParser_getDictionaryAtIndex(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, size_t dic_index, char* key)
{
    struct CCJsonParser_jsonToken_t result_token = {0, 0};

    size_t count = 0;

    for(size_t i = 0; i < token->count; i++)
    {
        if(obj->parser.tokens[i + token->index_start].parent == (int)token->index_start)
        {
            if(dic_index == count)
            {
                size_t key_length = (size_t)(obj->parser.tokens[i + token->index_start].end - obj->parser.tokens[i + token->index_start].start);
                strncpy(key, &obj->js[obj->parser.tokens[i + token->index_start].start], key_length);
                key[key_length] = '\0';
                result_token = CCJsonParser_searchTokCurRange(obj, i + token->index_start + 1);
                break;
            }

            count++;
        }
    }

    return result_token;
}


size_t CCJsonParser_arrayCount(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token)
{
    size_t count = 0;

    for(size_t i = 0; i < token->count; i++)
    {
        if(obj->parser.tokens[i + token->index_start].parent == (int)token->index_start)
        {
            count++;
        }
    }

    return count;
}


struct CCJsonParser_jsonToken_t CCJsonParser_getArray(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, size_t index)
{
    size_t count = 0;
    struct CCJsonParser_jsonToken_t result_token = {0, 0};

    for(size_t i = 0; i < token->count; i++)
    {
        if(obj->parser.tokens[i + token->index_start].parent == (int)token->index_start)
        {
            if(count == index)
            {
                result_token = CCJsonParser_searchTokCurRange( obj, i + token->index_start);
                break;
            }

            count++;
        }
    }

    return result_token;
}


int CCJsonParser_parseAsInt(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, int default_value)
{
    int result = default_value;
    if ( (token->count == 1) && (obj->parser.tokens[token->index_start].type == JSMN_INT) )
    {
        const char* buffer = &obj->js[obj->parser.tokens[token->index_start].start];
        char tmp_char = obj->js[obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start];
        obj->js[obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start] = 0x00;

        result = atoi(buffer);

        obj->js[obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start] = tmp_char;
    }

    return result;
}


double CCJsonParser_parseAsDouble(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, double default_value)
{
    double result = default_value;
    if ( (token->count == 1) && (obj->parser.tokens[token->index_start].type == JSMN_DOUBLE) )
    {
        const char* buffer = &obj->js[obj->parser.tokens[token->index_start].start];
        char tmp_char = obj->js[obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start];
        obj->js[obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start] = 0x00;

        result = atof(buffer);

        obj->js[obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start] = tmp_char;
    }

    return result;
}


CC_bool_t CCJsonParser_parseAsBoolean(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, CC_bool_t default_value)
{
    CC_bool_t result = default_value;

    if(token->count == 1)
    {
        if(obj->parser.tokens[token->index_start].type == JSMN_TRUE)
        {
            result = CC_BOOL_TRUE;
        }else if(obj->parser.tokens[token->index_start].type == JSMN_FALSE){
            result = CC_BOOL_FALSE;
        }
    }

    return result;
}


size_t CCJsonParser_stringLen(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token)
{
    size_t resault = 0;

    if ( (token->count == 1) && (obj->parser.tokens[token->index_start].type == JSMN_STRING) )
    {
        resault = obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start;
    }

    return resault;
}


void CCJsonParser_parseAsString(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, char* buff_out, size_t copy_len)
{
    if ( (token->count == 1) && (obj->parser.tokens[token->index_start].type == JSMN_STRING) )
    {
        if( (obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start) < ((int)copy_len - 1) )
        {
            copy_len = obj->parser.tokens[token->index_start].end - obj->parser.tokens[token->index_start].start;
        }

        copy_len -= 1;

        memcpy(buff_out, &obj->js[obj->parser.tokens[token->index_start].start], sizeof(char) * copy_len);
        buff_out[copy_len] = 0x00;
    }
}

const char* CCJsonParser_getStringRaw(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token)
{
    const char* str = NULL;
    if ( (token->count == 1) && (obj->parser.tokens[token->index_start].type == JSMN_STRING) )
    {
        str = &obj->js[obj->parser.tokens[token->index_start].start];
    }

    return str;
}
