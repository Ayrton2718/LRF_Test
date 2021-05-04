#include "cc_auto_buffer.h"

#include "cc_log.h"

#define CC_AUTO_BUFFER_ONE_BLOCK_SIZE   (128)

void CCAutoBuffer_realloc(struct CCAutoBuffer_t* obj)
{
    size_t befo_block_count = obj->block_count;
    size_t allocating_count = obj->one_block_count * obj->block_count;
    
    if(allocating_count < obj->data_count)
    {
        size_t append_block_count = ((obj->data_count - allocating_count) / obj->one_block_count) + 1;
        obj->block_count += append_block_count;
    }else{
        size_t detach_block_count = ((allocating_count - obj->data_count) / obj->one_block_count);
        obj->block_count -= detach_block_count;
        if(obj->block_count <= 0)
        {
            obj->block_count = 1;
        }
    }
    
    if(befo_block_count != obj->block_count)
    {
        obj->buffer = (uint8_t*)realloc(obj->buffer, obj->one_data_size * obj->one_block_count * obj->block_count);
        if(obj->buffer == NULL)
        {
            CCLOG_ERROR_NOFMT("Memory allocation failure! Program exit.");
            exit(-1);
        }
    }
}


void CCAutoBuffer_create(struct CCAutoBuffer_t* obj, size_t one_data_size)
{
    obj->one_data_size = one_data_size;
    obj->data_count = 0;
    obj->block_count = 1;
    obj->one_block_count = CC_AUTO_BUFFER_ONE_BLOCK_SIZE / one_data_size;

    obj->buffer = (uint8_t*)malloc(obj->one_data_size * obj->one_block_count * obj->block_count);
    if(obj->buffer == NULL)
    {
        CCLOG_ERROR_NOFMT("Memory allocation failure! Program exit.");
        exit(-1);
    }
}


void CCAutoBuffer_destructor(struct CCAutoBuffer_t* obj)
{
    free(obj->buffer);
}


const void* CCAutoBuffer_getRaw(struct CCAutoBuffer_t* obj)
{
    return (const void*)obj->buffer;
}

size_t CCAutoBuffer_count(struct CCAutoBuffer_t* obj)
{
    return obj->data_count;
}


void CCAutoBuffer_add(struct CCAutoBuffer_t* obj, const void* data_in)
{
    CCAutoBuffer_add_stream(obj, 1, data_in);
}

void CCAutoBuffer_add_stream(struct CCAutoBuffer_t* obj, size_t data_count, const void* data_in)
{
    CCAutoBuffer_insert_stream(obj, obj->data_count, data_count, data_in);
}

void CCAutoBuffer_insert(struct CCAutoBuffer_t* obj, size_t index, const void* data_in)
{
    CCAutoBuffer_insert_stream(obj, index, 1, data_in);
}

void CCAutoBuffer_insert_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count, const void* data_in)
{
    if(obj->data_count < index)
    {
        CCLOG_ERROR_NOFMT("Index out of range!");
        return;
    }
    size_t befo_count = obj->data_count;
    obj->data_count += data_count;
    CCAutoBuffer_realloc(obj);

    memmove(&obj->buffer[obj->one_data_size * (index + data_count)], &obj->buffer[obj->one_data_size * index], obj->one_data_size * (befo_count - index));
    memcpy(&obj->buffer[obj->one_data_size * index], data_in, obj->one_data_size * data_count);
}

void CCAutoBuffer_replace(struct CCAutoBuffer_t* obj, size_t index, const void* data_in)
{
    CCAutoBuffer_replace_stream(obj, index, 1, data_in);
}

void CCAutoBuffer_replace_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count, const void* data_in)
{
    if(obj->data_count < index + data_count)
    {
        CCLOG_ERROR_NOFMT("Index and data_count out of range!");
        return;
    }
    memcpy(&obj->buffer[obj->one_data_size * index], data_in, obj->one_data_size * data_count);
}

void* CCAutoBuffer_readLast_pointer(struct CCAutoBuffer_t* obj)
{
    if(obj->data_count == 0)
    {
        CCLOG_ERROR_NOFMT("This buffer is empty.");
        return NULL;
    }

    return CCAutoBuffer_readAtIndex_pointer(obj, obj->data_count - 1);
}

void* CCAutoBuffer_readAtIndex_pointer(struct CCAutoBuffer_t* obj, size_t index)
{
    if(obj->data_count <= index)
    {
        CCLOG_ERROR_NOFMT("index out of range.");
        return NULL;
    }
    return &obj->buffer[obj->one_data_size * index];
}

void CCAutoBuffer_readLast(struct CCAutoBuffer_t* obj, void* data_out)
{
    if(obj->data_count == 0)
    {
        CCLOG_ERROR_NOFMT("This buffer is empty.");
        data_out = NULL;
        return;
    }
    CCAutoBuffer_read_stream(obj, obj->data_count - 1, 1, data_out);
}

void CCAutoBuffer_readAtIndex(struct CCAutoBuffer_t* obj, size_t index, void* data_out)
{
    CCAutoBuffer_read_stream(obj, index, 1, data_out);
}

void CCAutoBuffer_read_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count, void* data_out)
{
    if(obj->data_count < index + data_count)
    {
        CCLOG_ERROR_NOFMT("index out of range.");
        data_out = NULL;
        return;
    }

    memcpy(data_out, &obj->buffer[obj->one_data_size * index], obj->one_data_size * data_count);
}


void CCAutoBuffer_removeLast(struct CCAutoBuffer_t* obj)
{
    if(obj->data_count == 0)
    {
        CCLOG_ERROR_NOFMT("This buffer is empty.");
        return;
    }

    CCAutoBuffer_removeAtIndex(obj, obj->data_count - 1);
}

void CCAutoBuffer_removeAtIndex(struct CCAutoBuffer_t* obj, size_t index)
{
    CCAutoBuffer_remove_stream(obj, index, 1);
}

void CCAutoBuffer_removeAll(struct CCAutoBuffer_t* obj)
{
    CCAutoBuffer_remove_stream(obj, 0, obj->data_count);
}

void CCAutoBuffer_remove_stream(struct CCAutoBuffer_t* obj, size_t index, size_t data_count)
{
    if(obj->data_count < index + data_count)
    {
        CCLOG_ERROR_NOFMT("index out of range.");
        return;
    }

    size_t befo_count = obj->data_count;
    obj->data_count -= data_count;

    memcpy(&obj->buffer[obj->one_data_size * index], &obj->buffer[obj->one_data_size * (index + data_count)], obj->one_data_size * (befo_count - (index + data_count)));
    // memmove(&obj->buffer[obj->one_data_size * index], &obj->buffer[obj->one_data_size * (index + data_count)], obj->one_data_size * (befo_count - (index + data_count)));

    CCAutoBuffer_realloc(obj);
}
