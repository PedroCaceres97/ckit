#ifndef __CKIT_INFO_H__
#define __CKIT_INFO_H__

#include <math.h>
#include <time.h>
#include <wchar.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
    #define INFO_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define INFO_OS_IOS
    #else
        #define INFO_OS_MACOS
    #endif
#elif defined(__linux__)
    #define INFO_OS_LINUX
#elif defined(__unix__) || defined(__unix)
    #define INFO_OS_UNIX
#elif defined(__ANDROID__)
    #define INFO_OS_ANDROID
#else
    #define INFO_OS_UNKNOWN
#endif

#if defined(INFO_OS_LINUX) || defined(INFO_OS_MACOS) || defined(INFO_OS_UNIX) || defined(INFO_OS_ANDROID) || defined(INFO_OS_IOS)
    #define INFO_OS_POSIX
#endif

#if defined(__x86_64__) || defined(_M_X64)
    #define INFO_CPU_X64
#elif defined(__i386__) || defined(_M_IX86)
    #define INFO_CPU_X86
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define INFO_CPU_ARM64
#elif defined(__arm__) || defined(_M_ARM)
    #define INFO_CPU_ARM32
#else
    #define INFO_CPU_UNKNOWN
#endif

#if UINTPTR_MAX == UINT64_MAX
    #define INFO_PTR_64BIT
#elif UINTPTR_MAX == UINT32_MAX
    #define INFO_PTR_32BIT
#else 
    #define INFO_PTR_UNKOWN
#endif

#if (SIZE_MAX == UINT64_MAX)
    #define INFO_SIZE_64BIT
#elif (SIZE_MAX == UINT32_MAX)
    #define INFO_SIZE_32BIT
#else
    #define INFO_SIZE_UNKOWN
#endif

#if (PTRDIFF_MAX == INT64_MAX)
    #define INFO_PTRDIFF_64BIT
#elif (PTRDIFF_MAX == INT32_MAX)
    #define INFO_PTRDIFF_32BIT
#else
    #define INFO_PTRDIFF_UNKOWN
#endif

#if defined(__clang__)
    #define INFO_COMPILER_CLANG
#elif defined(__GNUC__)
    #define INFO_COMPILER_GCC
#elif defined(_MSC_VER)
    #define INFO_COMPILER_MSVC
#else
    #define INFO_COMPILER_UNKNOWN
#endif

#define LINE __LINE__
#define FUNCTION __func__
#if defined(__FILE_NAME__)
    #define FILENAME __FILE_NAME__
#else
    #define FILENAME __FILE__
#endif

typedef struct Context {
    const char* alias;
    const char* file;
    const char* func;
    uint32_t    line;
} Context;

#define ALIAS(alias)    ((Context){(alias), FILENAME, FUNCTION, LINE})
#define CONTEXT_HERE    ((Context){NULL, FILENAME, FUNCTION, LINE})
#define CONTEXT_ARGS(context) context.file, context.line, context.func

#ifdef INFO_COMPILER_MSVC

#pragma section(".CRT$XCU", read)

#define PREMAIN(fn)                     \
    static void fn(void);               \
    __declspec(allocate(".CRT$XCU"))    \
    void (*fn##_)(void) = fn;           \
    static void fn(void)

#else

#define PREMAIN(fn)                                     \
    static void fn(void) __attribute__((constructor));  \
    static void fn(void)

#endif

#endif /* __CKIT_INFO_H__ */