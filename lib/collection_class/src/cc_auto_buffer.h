#ifndef CC_AUTO_BUFFER_H
#define CC_AUTO_BUFFER_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CCAutoBuffer_t
{
    size_t one_data_size;
    size_t data_count;
    size_t one_block_count;
    size_t block_count;
    uint8_t* buffer;
};

void CCAutoBuffer_create(struct CCAutoBuffer_t* obj, size_t one_data_size);
void CCAutoBuffer_destructor(struct CCAutoBuffer_t* obj);

size_t CCAutoBuffer_count(struct CCAutoBuffer_t* obj);

const void* CCAutoBuffer_getRaw(struct CCAutoBuffer_t* obj);

void CCAutoBuffer_copy(struct CCAutoBuffer_t* obj, struct CCAutoBuffer_t* to_copy_obj);

void CCAutoBuffer_add(struct CCAutoBuffer_t* obj, const void* data_in);
void CCAutoBuffer_add_stream(struct CCAutoBuffer_t* obj, size_t data_count, const void* data_in);
void CCAutoBuffer_insert(struct CCAutoBuffer_t* obj, size_t index, const void* data_in);
void CCAutoBuffer_insert_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count, const void* data_in);
void CCAutoBuffer_replace(struct CCAutoBuffer_t* obj, size_t index, const void* data_in);
void CCAutoBuffer_replace_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count, const void* data_in);

void CCAutoBuffer_readLast(struct CCAutoBuffer_t* obj, void* data_out);
void* CCAutoBuffer_readLast_pointer(struct CCAutoBuffer_t* obj);
void CCAutoBuffer_readAtIndex(struct CCAutoBuffer_t* obj, size_t index, void* data_out);
void* CCAutoBuffer_readAtIndex_pointer(struct CCAutoBuffer_t* obj, size_t index);
void CCAutoBuffer_read_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count, void* data_out);

void CCAutoBuffer_removeLast(struct CCAutoBuffer_t* obj);
void CCAutoBuffer_removeAtIndex(struct CCAutoBuffer_t* obj, size_t index);
void CCAutoBuffer_removeAll(struct CCAutoBuffer_t* obj);
void CCAutoBuffer_remove_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count);

#ifdef __cplusplus
}
#endif

#endif /*CC_AUTO_BUFFER_H*/
