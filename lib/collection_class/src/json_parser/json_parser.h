#ifndef JSON_PARSER_JSON_PARSER_H
#define JSON_PARSER_JSON_PARSER_H

#include "../cc_type.h"
#include "jsmn.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CCJsonParser_jsonToken_t
{
    size_t index_start;
    size_t count;
};

struct CCJsonParser_t
{
    jsmn_parser             parser;

    char                   *js;
};

CC_bool_t CCJsonParser_parse(struct CCJsonParser_t* obj, const char* js, size_t js_len);
struct CCJsonParser_jsonToken_t CCJsonParser_getRoot(struct CCJsonParser_t* obj);

void CCJsonParser_destructor(struct CCJsonParser_t* obj);

void CCJsonParser_showResult(struct CCJsonParser_t* obj, struct CCJsonParser_jsonToken_t* token);

jsmntype_t CCJsonParser_jsonType(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token);


size_t CCJsonParser_dictionaryCount(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token);
struct CCJsonParser_jsonToken_t CCJsonParser_getDictionary(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, const char* key);
struct CCJsonParser_jsonToken_t CCJsonParser_getDictionaryAtIndex(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, size_t dic_index, char* key);

size_t CCJsonParser_arrayCount(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token);
struct CCJsonParser_jsonToken_t CCJsonParser_getArray(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, size_t index);

int CCJsonParser_parseAsInt(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, int default_value);
double CCJsonParser_parseAsDouble(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, double default_value);
CC_bool_t CCJsonParser_parseAsBoolean(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, CC_bool_t default_value);

size_t CCJsonParser_stringLen(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token);
void CCJsonParser_parseAsString(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token, char* buff_out, size_t copy_len);
const char* CCJsonParser_getStringRaw(struct CCJsonParser_t* obj, const struct CCJsonParser_jsonToken_t* token);

#ifdef __cplusplus
}
#endif

#endif /*JSON_PARSER_JSON_PARSER_H*/