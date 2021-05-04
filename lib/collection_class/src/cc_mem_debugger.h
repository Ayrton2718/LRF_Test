#ifndef CC_MEM_DEBUGGER_H
#define CC_MEM_DEBUGGER_H

#include "cc_type.h"

#ifdef __cplusplus
extern "C" {
#endif


void CCMemDebugger_init(void);

void* CCMemDebugger_malloc(const char* file, size_t line, size_t malloc_size, const char* tag);
void* CCMemDebugger_realloc(const char* file, size_t line, void* malloc_ptr, size_t size, const char* tag);

void CCMemDebugger_free(const char* file, size_t line, void* malloc_ptr, const char* tag);

void CCMemDebugger_showOwn(const char* tag);

void CCMemDebugger_showAll(const char* tag);

#ifdef CCMEM_DEBUG_ON

// #define CCMEM_LOGOUT_ON

#define CCMALLOC(malloc_size, tag) CCMemDebugger_malloc(__FILE__, __LINE__, malloc_size, tag)
#define CCREALLOC(malloc_ptr, size, tag) CCMemDebugger_realloc(__FILE__, __LINE__, malloc_ptr, size, tag)
#define CCFREE(malloc_ptr, tag) CCMemDebugger_free(__FILE__, __LINE__, malloc_ptr, tag)

#else /*CCMEM_DEBUG_ON*/

#define CCMALLOC(malloc_size, tag) malloc(malloc_size)
#define CCREALLOC(malloc_ptr, size, tag) realloc(malloc_ptr, size)
#define CCFREE(malloc_ptr, tag) free(malloc_ptr)

#endif /*CCMEM_DEBUG_ON*/

#ifdef __cplusplus
}
#endif

#endif /*CC_MEM_DEBUGGER_H*/
