#ifndef __CKIT_UTILS_H__
#define __CKIT_UTILS_H__

#include <ckit/info.h>

#define __LITERAL(x) #x
#define __CONCAT2(a, b) a##b
#define __CONCAT3(a, b, c) a##b##c

#define LITERAL(x)       __LITERAL(x)
#define CONCAT2(a, b)    __CONCAT2(a, b)
#define CONCAT3(a, b, c) __CONCAT3(a, b, c)

#define TERNARY(cond, t, f) ((cond) ? (t) : (f))
#define FNLIKE(body)        do { body } while(0)
#define CAST(cast, data)    ((cast)(data))
#define NOOP                FNLIKE(((void)0))

#define MIN(x, y)           TERNARY(x > y, y, x)
#define MAX(x, y)           TERNARY(x > y, x, y)
#define MID(x, y, z)        (x + y + z - MAX(x, MAX(y, z)) - MIN(x, MIN(y, z)))
#define CLAMP(x, min, max)  TERNARY(x < min, min, TERNARY(x > max, max, x))
#define DISTANCE(x, y)      TERNARY(x > y, x - y, y - x)
#define ARRAY_COUNT(x)      (sizeof(x) / sizeof((x)[0]))
#define STRLEN_LITERAL(str) (sizeof(str) - 1)

#define SETMIN(x, y)        FNLIKE( x = MIN(x, y); )
#define SETMAX(x, y)        FNLIKE( x = MAX(x, y); )

#define EMPTY(type)         ((type){0})
#define VOIDPTR(ptr)        ((void*)(ptr))
#define BYTEPTR(ptr)        ((uint8_t*)(ptr))
#define UINTPTR(ptr)        ((uintptr_t)(ptr))
#define TYPEDIFF(cast)      ((ptrdiff_t)(cast))
#define TYPESIZE(cast)      ((size_t)(cast))

#define PTRADD(ptr, value)  VOIDPTR(BYTEPTR(ptr) + (value))
#define PTRSUB(ptr, value)  VOIDPTR(BYTEPTR(ptr) - (value))
#define PTRDST(ptr1, ptr2)  TYPESIZE(DISTANCE(UINTPTR(ptr1), UINTPTR(ptr2)))
#define PTRDIF(ptr1, ptr2)  TYPEDIFF(UINTPTR(ptr1) >= UINTPTR(ptr2) ? UINTPTR(ptr1) - UINTPTR(ptr2) : -TYPEDIFF(UINTPTR(ptr2) - UINTPTR(ptr1)))
#define PTRPTR(ptr1, ptr2)  TYPESIZE(BYTEPTR(ptr1) - BYTEPTR(ptr2))

#endif /* __CKIT_UTILS_H__ */