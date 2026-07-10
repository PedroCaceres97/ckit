#ifndef __CKIT_SYMBOLS_H__
#define __CKIT_SYMBOLS_H__

#include <ckit/utils.h>

#define CKIT_PREFIX(a) CONCAT2(ckit_, a)
#define CKIT_NEXT_SYMBOL(a) CKIT_PREFIX(CONCAT2(a, _next))
#define CKIT_INDEX_SYMBOL(a) CKIT_PREFIX(CONCAT2(a, _index))
#define CKIT_BUFFERS_SYMBOL(a) CKIT_PREFIX(CONCAT2(a, _buffers))

#define CKIT_BUFFERS_SYMBOLS(buffers, index, next, type, count, size)   \
    static _Thread_local type buffers[count][size] = {0};               \
    static _Thread_local uint32_t index = 0;                            \
    static inline type* next() {                                        \
        if (index == count) { index = 0; }                              \
        return buffers[index++];                                        \
    }
#define CKIT_BUFFERS(identifier, type, count, size) CKIT_BUFFERS_SYMBOLS(   \
                            CKIT_BUFFERS_SYMBOL(identifier),                \
                            CKIT_INDEX_SYMBOL(identifier),                  \
                            CKIT_NEXT_SYMBOL(identifier), type, count, size)

#endif /* __CKIT_SYMBOLS_H__ */