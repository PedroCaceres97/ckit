#ifndef __CKIT_ERROR_H__
#define __CKIT_ERROR_H__

#include <ckit/info.h>

#define CKIT_ERROR_UNKOWN_OS            "Unable to fetch OS info"
#define CKIT_ERROR_UNSUPPORTED_PTR      "Unsupported pointer size"
#define CKIT_ERROR_UNSUPPORTED_SIZE     "Unsupported size_t size"
#define CKIT_ERROR_UNSUPPORTED_DIFF     "Unsupported ptrdiff_t size"

#define CKIT_ERROR_STDIN_INVALID        "GetStdHandle(STD_INPUT_HANDLE) returned INVALID_HANDLE_VALUE"
#define CKIT_ERROR_STDOUT_INVALID       "GetStdHandle(STD_OUTPUT_HANDLE) returned INVALID_HANDLE_VALUE"
#define CKIT_ERROR_STDERR_INVALID       "GetStdHandle(STD_ERROR_HANDLE) returned INVALID_HANDLE_VALUE"

#define CKIT_ERROR_GET_MODE             "Failed to retrieve console mode through GetConsoleMode(HANDLE, &mode)"
#define CKIT_ERROR_ENABLE_VT            "Failed to enable VIRTUAL_TERMINAL_PROCESSING through SetConsoleMode(HANDLE, mode)"

#define CKIT_ERROR_FILE_NOT_READABLE    "File missing ATTRIBUTE_READ"
#define CKIT_ERROR_FILE_NOT_WRITABLE    "File missing ATTRIBUTE_WRITE"
#define CKIT_ERROR_FILES_UNCLOSED       "Detected unclosed files at terminate"

#define CKIT_ERROR_FOREIGN_PTR          "Memory address does not belong to struct"
#define CKIT_ERROR_ALREADY_FREE         "Memory address is already free"
#define CKIT_ERROR_NOT_ALLOCATED        "Memory address was not allocated"
#define CKIT_ERROR_ZERO_SIZE_ALLOCATION "Requested allocation size is 0"

#define CKIT_ERROR_NON_EMPTY_SLAB       "Destroying non empty slab"
#define CKIT_ERROR_NON_EMPTY_TRACKER    "Destroying non empty tracker"
#define CKIT_ERROR_NON_EMPTY_VECTOR     "Destroying non empty vector"

#define CKIT_ERROR_MEMORY_COPY_ZERO     "Called a memory copying operation with zero size or count"

/*
    32bits

    4bits
    4bits
    4bits
    4bits

    4bits
    4bits
    4bits
    4bits
*/

typedef enum {
    BIT_ERROR_TYPE      = 0,
    BIT_ERROR_PLATFORM  = 2,
    BIT_ERROR_FAULT     = 4,
    BIT_ERROR_FILE      = 8,
    BIT_ERROR_MEMORY    = 12,
    BIT_ERROR_STRUCT    = 16,
    BIT_ERROR_OPERATION = 20,
} ErrorCodeBit;

typedef enum {
    ERROR_NONE          = 0 << 0,

// ERROR_TYPE       2bit field
    ERROR_SOFT          = 1 << BIT_ERROR_TYPE,
    ERROR_HARD          = 2 << BIT_ERROR_TYPE,

// ERROR_PLATFORM   2bit field
    ERROR_WINDOWS       = 1 << BIT_ERROR_PLATFORM,
    ERROR_POSIX         = 2 << BIT_ERROR_PLATFORM,

// ERROR_FAULT      4bit field
    ERROR_OS            = 1 << BIT_ERROR_FAULT,
    ERROR_CKIT          = 2 << BIT_ERROR_FAULT,
    ERROR_USER          = 3 << BIT_ERROR_FAULT,
    ERROR_FUNCTION      = 4 << BIT_ERROR_FAULT,
    ERROR_STDIN         = 5 << BIT_ERROR_FAULT,
    ERROR_STDOUT        = 6 << BIT_ERROR_FAULT,
    ERROR_STDERR        = 7 << BIT_ERROR_FAULT,

// ERROR_FILE       4bit field
    ERROR_UNOPENED      = 1 << BIT_ERROR_FILE,
    ERROR_UNREADABLE    = 2 << BIT_ERROR_FILE,
    ERROR_UNWRITABLE    = 3 << BIT_ERROR_FILE,
    ERROR_UNCLOSED      = 4 << BIT_ERROR_FILE,
    ERROR_INVALID_HND   = 5 << BIT_ERROR_FILE,
    ERROR_GET_MODE      = 6 << BIT_ERROR_FILE,
    ERROR_ENABLE_VT     = 6 << BIT_ERROR_FILE,

// ERROR_MEMORY     4bit field
    ERROR_NULLPTR       = 1 << BIT_ERROR_MEMORY,
    ERROR_FOREIGN       = 2 << BIT_ERROR_MEMORY,
    ERROR_DOUBLE_FREE   = 3 << BIT_ERROR_MEMORY,
    ERROR_OUT_OF_BOUNDS = 4 << BIT_ERROR_MEMORY,
    ERROR_ZERO_SIZE     = 5 << BIT_ERROR_MEMORY,
    ERROR_NON_EMPTY     = 6 << BIT_ERROR_MEMORY,
    ERROR_LEAK          = 7 << BIT_ERROR_MEMORY,

// ERROR_STRUCT     4bit field
    ERROR_VECTOR        = 1 << BIT_ERROR_STRUCT,
    ERROR_ARRAY         = 2 << BIT_ERROR_STRUCT,
    ERROR_LIST          = 3 << BIT_ERROR_STRUCT,
    ERROR_SLAB          = 4 << BIT_ERROR_STRUCT,

// ERROR_OPERATION  5bit field
    ERROR_OPENFILE      = 1 << BIT_ERROR_OPERATION,
    ERROR_CLOSEFILE     = 2 << BIT_ERROR_OPERATION,
    ERROR_DUMPFILE      = 3 << BIT_ERROR_OPERATION,
    ERROR_READFILE      = 4 << BIT_ERROR_OPERATION,
    ERROR_WRITEFILE     = 5 << BIT_ERROR_OPERATION,

    ERROR_CREATE        = 6 << BIT_ERROR_OPERATION,
    ERROR_DESTROY       = 7 << BIT_ERROR_OPERATION,
    ERROR_FREE          = 8 << BIT_ERROR_OPERATION,
    ERROR_ALLOC         = 9 << BIT_ERROR_OPERATION,
    ERROR_REALLOC      = 10 << BIT_ERROR_OPERATION,
    ERROR_DUPLICATE    = 11 << BIT_ERROR_OPERATION,
    ERROR_AT           = 12 << BIT_ERROR_OPERATION,
    ERROR_SET          = 13 << BIT_ERROR_OPERATION,
    ERROR_GET          = 14 << BIT_ERROR_OPERATION,
    ERROR_CLEAR        = 15 << BIT_ERROR_OPERATION,
    ERROR_ERASE        = 16 << BIT_ERROR_OPERATION,
    ERROR_POPBACK      = 17 << BIT_ERROR_OPERATION,
    ERROR_POPFRONT     = 18 << BIT_ERROR_OPERATION,
    ERROR_INSERT       = 19 << BIT_ERROR_OPERATION,
    ERROR_PUSHBACK     = 20 << BIT_ERROR_OPERATION,
    ERROR_PUSHFRONT    = 21 << BIT_ERROR_OPERATION,
    ERROR_MEMCPY       = 22 << BIT_ERROR_OPERATION,
    ERROR_MEMSET       = 23 << BIT_ERROR_OPERATION,
    ERROR_APPEND       = 24 << BIT_ERROR_OPERATION,
    ERROR_REPEAT       = 25 << BIT_ERROR_OPERATION,
} ErrorCode;

typedef enum {
    ERROR_ACTION_IGNORE,
    ERROR_ACTION_RETURN,
    ERROR_ACTION_ABORT
} ErrorAction;

#define ERROR_WIN_STDIN_INVALID (ERROR_HARD | ERROR_WINDOWS | ERROR_OS | ERROR_STDIN | ERROR_INVALID_HND)
#define ERROR_WIN_STDOUT_INVALID (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_INVALID_HND)
#define ERROR_WIN_STDERR_INVALID (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDERR | ERROR_INVALID_HND)
#define ERROR_WIN_STDOUT_MODE    (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_GET_MODE)
#define ERROR_WIN_STDERR_MODE    (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_GET_MODE)
#define ERROR_WIN_STDOUT_VT      (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_ENABLE_VT)
#define ERROR_WIN_STDERR_VT      (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDERR | ERROR_ENABLE_VT)

#define ERROR_WIN_FILES_UNCLOSED (ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNCLOSED | ERROR_LEAK)

// #define 

#define hastype(code)       (((code >> BIT_ERROR_TYPE)      & 3) != 0)
#define hasplatform(code)   (((code >> BIT_ERROR_PLATFORM)  & 3) != 0)
#define hasfault(code)      (((code >> BIT_ERROR_FAULT)     & 15) != 0)
#define hasfile(code)       (((code >> BIT_ERROR_FILE)      & 15) != 0)
#define hasmemory(code)     (((code >> BIT_ERROR_MEMORY)    & 15) != 0)
#define hasstruct(code)     (((code >> BIT_ERROR_STRUCT)    & 15) != 0)
#define hasoperation(code)  (((code >> BIT_ERROR_OPERATION) & 31) != 0)

void throwcallback(void* userdata, ErrorAction (*callback)(void* _userdata, Context _context, ErrorCode _code, const char* msg));
ErrorAction __throw(Context context, ErrorCode code, const char* msg);
ErrorAction __throwf(Context context, ErrorCode code, const char* fmt, ...);

#define throw(code, msg) __throw(CONTEXT, code, msg)
#define throwf(code, fmt, ...) __throwf(CONTEXT, code, fmt, ##__VA_ARGS__)
#define throwif(cond, code, msg) if ((cond)) { throw(code, msg); }
#define throwiff(cond, code, fmt, ...) if ((cond)) { throwf(code, fmt, ##__VA_ARGS__); }

#endif /* __CKIT_ERROR_H__ */