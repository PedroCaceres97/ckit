#ifndef __CKIT_SYMBOLS_H__
#define __CKIT_SYMBOLS_H__

#include <ckit/utils.h>

#define CKIT_PREFIX(a) CONCAT2(ckit_, a)
#define CKIT_NEXT_SUFFIX(a) CKIT_PREFIX(CONCAT2(a, _next))
#define CKIT_INDEX_SUFFIX(a) CKIT_PREFIX(CONCAT2(a, _index))
#define CKIT_BUFFERS_SUFFIX(a) CKIT_PREFIX(CONCAT2(a, _buffers))

#define CKIT_BUFFERS_SYMBOLS(buffers, index, next, type, count, size)   \
    static type buffers[count][size] = {0};                             \
    static uint32_t index = 0;                                          \
    static inline type* next() {                                        \
        if (index == count) { index = 0; }                              \
        return buffers[index++];                                        \
    }
#define CKIT_BUFFERS(identifier, type, count, size) \
    CKIT_BUFFERS_SYMBOLS(   CKIT_BUFFERS_SUFFIX(identifier),                \
                            CKIT_INDEX_SUFFIX(identifier),                  \
                            CKIT_NEXT_SUFFIX(identifier),                   \
                            type, count, size)

#define CKIT_CREATE(ptr, type)  FNLIKE( if (!ptr) { ptr = (type*)ckit_calloc(1, sizeof(type)); ptr->allocated = true; } )
#define CKIT_DESTROY(ptr)       FNLIKE( if (ptr->allocated) { ckit_free((void*)ptr); ptr = NULL; } )    

#endif /* __CKIT_SYMBOLS_H__ */