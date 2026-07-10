#ifndef __CKIT_ERROR_H__
#define __CKIT_ERROR_H__

#include <ckit/info.h>

#define CKIT_ERROR_UNKOWN_OS            "Unable to fetch OS info"
#define CKIT_ERROR_UNSUPPORTED_PTR      "Unsupported pointer size"
#define CKIT_ERROR_UNSUPPORTED_SIZE     "Unsupported size_t size"
#define CKIT_ERROR_UNSUPPORTED_DIFF     "Unsupported ptrdiff_t size"

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

typedef enum {
    BIT_ERROR_SOFT      = 0,
    BIT_ERROR_HARD      = 1,
    BIT_ERROR_WINDOWS   = 2,
    BIT_ERROR_LINUX     = 3,
    BIT_ERROR_FAULT     = 4,
    BIT_ERROR_FILE      = 8,
    BIT_ERROR_MEMORY    = 12,
    BIT_ERROR_STRUCT    = 16,
} ErrorCodeBit;

typedef enum {
    ERROR_NONE          = 0 << 0,
    ERROR_SOFT          = 1 << BIT_ERROR_SOFT,
    ERROR_HARD          = 1 << BIT_ERROR_HARD,
    ERROR_WINDOWS       = 1 << BIT_ERROR_WINDOWS,
    ERROR_LINUX         = 1 << BIT_ERROR_LINUX,
    ERROR_FAULT         = 1 << BIT_ERROR_FAULT,
    ERROR_FILE          = 1 << BIT_ERROR_FILE,
    ERROR_MEMORY        = 1 << BIT_ERROR_MEMORY,
    ERROR_STRUCT        = 1 << BIT_ERROR_STRUCT,
} ErrorCode;

typedef enum {
    ERROR_OS            = 1 << BIT_ERROR_FAULT,
    ERROR_CKIT          = 2 << BIT_ERROR_FAULT,
    ERROR_USER          = 3 << BIT_ERROR_FAULT,
    ERROR_FUNCTION      = 4 << BIT_ERROR_FAULT,
    ERROR_STDIN         = 5 << BIT_ERROR_FAULT,
    ERROR_STDOUT        = 6 << BIT_ERROR_FAULT,
    ERROR_STDERR        = 7 << BIT_ERROR_FAULT
} ErrorOwner;

typedef enum {
    ERROR_UNOPENED      = 1 << BIT_ERROR_FILE,
    ERROR_UNREADABLE    = 2 << BIT_ERROR_FILE,
    ERROR_UNWRITABLE    = 3 << BIT_ERROR_FILE,
    ERROR_UNCLOSED      = 4 << BIT_ERROR_FILE,
    ERROR_INVALID       = 5 << BIT_ERROR_FILE
} ErrorFile;

typedef enum {
    ERROR_UNFREED       = 1 << BIT_ERROR_MEMORY,
    ERROR_EXCEED        = 2 << BIT_ERROR_MEMORY
} ErrorMemory;

typedef enum {
    ERROR_VECTOR        = 1 << BIT_ERROR_STRUCT,
    ERROR_ARRAY         = 2 << BIT_ERROR_STRUCT,
    ERROR_LIST          = 3 << BIT_ERROR_STRUCT,
    ERROR_SLAB          = 4 << BIT_ERROR_STRUCT,
} ErrorStruct;

typedef enum {
    BIT_ERROR_DO_ABORT
} ErrorBehaviourBit;

typedef enum {
    ERROR_ACTION_IGNORE,
    ERROR_ACTION_RETURN,
    ERROR_ACTION_ABORT
} ErrorAction;

#define ERROR_WIN_INVALID_STDIN (ERROR_HARD | ERROR_WINDOWS | ERROR_STDIN | ERROR_INVALID)
#define ERROR_WIN_FILES_UNCLOSED (ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNCLOSED)

#define structerror(code) ((code >> BIT_ERROR_STRUCT) & 15)
#define memoryerror(code) ((code >> BIT_ERROR_MEMORY) & 15)

void throwcallback(void* userdata, ErrorAction (*callback)(void* _userdata, Context _context, ErrorCode _code, const char* msg));
ErrorAction __throw(Context context, ErrorCode code, const char* msg);
ErrorAction __throwf(Context context, ErrorCode code, const char* fmt, ...);

#define throw(code, msg) __throw(CONTEXT, code, msg)
#define throwf(code, fmt, ...) __throwf(CONTEXT, code, fmt, ##__VA_ARGS__)
#define throwif(cond, code, msg) if ((cond)) { throw(code, msg); }
#define throwiff(cond, code, fmt, ...) if ((cond)) { throwf(code, fmt, ##__VA_ARGS__); }

#endif /* __CKIT_ERROR_H__ */