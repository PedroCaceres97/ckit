#ifndef __CKIT_ERROR_H__
#define __CKIT_ERROR_H__

#include <ckit/info.h>

#define ERRMSG_UNKOWN_OS            "Unable to fetch OS info"
#define ERRMSG_UNSUPPORTED_PTR      "Unsupported pointer size"
#define ERRMSG_UNSUPPORTED_SIZE     "Unsupported size_t size"
#define ERRMSG_UNSUPPORTED_DIFF     "Unsupported ptrdiff_t size"

#define ERRMSG_STDIN_INVALID        "GetStdHandle(STD_INPUT_HANDLE) returned INVALID_HANDLE_VALUE"
#define ERRMSG_STDOUT_INVALID       "GetStdHandle(STD_OUTPUT_HANDLE) returned INVALID_HANDLE_VALUE"
#define ERRMSG_STDERR_INVALID       "GetStdHandle(STD_ERROR_HANDLE) returned INVALID_HANDLE_VALUE"

#define ERRMSG_GET_MODE             "Failed to retrieve console mode through GetConsoleMode(HANDLE, &mode)"
#define ERRMSG_ENABLE_VT            "Failed to enable VIRTUAL_TERMINAL_PROCESSING through SetConsoleMode(HANDLE, mode)"

#define ERRMSG_UNOPENED             "Trying to operate over an unopened file"
#define ERRMSG_UNREADABLE           "File missing ATTRIBUTE_READ"
#define ERRMSG_UNWRITEABLE          "File missing ATTRIBUTE_WRITE"
#define ERRMSG_UNCLOSED             "Detected unclosed files"
#define ERRMSG_STDCLOSE             "Standard file is not closeable"
#define ERRMSG_INVALID_FD           "Invalid file descriptor (negative or bigger than 255)"
#define ERRMSG_FILE_DOESNT_EXIST    "File does not exist"
#define ERRMSG_FILES_MAX_REACHED    "Unable to open more files than 256 (std included)"

#define ERRMSG_COPYFILE_REPLACE     "Cant copy a file into an already existent file"
#define ERRMSG_RENAMEFILE_REPLACE   "Cant rename a file into an already existent file"

#define ERRMSG_COPYDIR_REPLACE      "Cant copy a directory into an already existent directory"
#define ERRMSG_RENAMEDIR_REPLACE    "Cant rename a directory into an already existent directory"

#define ERRMSG_NULLPTR(ptr)         "Memory address is a nullptr -> " LITERAL(ptr)
#define ERRMSG_FOREIGN_PTR          "Memory address does not belong to struct"
#define ERRMSG_ALREADY_FREE         "Memory address is already free"
#define ERRMSG_NOT_ALLOCATED        "Memory address was not allocated"
#define ERRMSG_ZERO_SIZE_ALLOCATION "Requested allocation size is 0"

#define ERRMSG_GETINPUT_OVERFLOW    "Input overflowed"

// #define CKIT_ERROR_NON_EMPTY_SLAB       "Destroying non empty slab"
// #define CKIT_ERROR_NON_EMPTY_TRACKER    "Destroying non empty tracker"
// #define CKIT_ERROR_NON_EMPTY_VECTOR     "Destroying non empty vector"

#define ERRMSG_MEMORY_COPY_ZERO     "Called a memory copying operation with zero size or count"

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
    BIT_ERROR_ORIGIN    = 2,
    BIT_ERROR_FILE      = 6,
    BIT_ERROR_MEMORY    = 10,
    BIT_ERROR_STRUCT    = 14,
} ErrorCodeBit;

typedef enum {
    ERROR_NONE          = 0 << 0,

// ERROR_TYPE       2bit field
    ERROR_SOFT          = 1 << BIT_ERROR_TYPE,
    ERROR_HARD          = 2 << BIT_ERROR_TYPE,
    
// ERROR_ORIGIN     4bit field
    ERROR_WINDOWS       = 1 << BIT_ERROR_ORIGIN,
    ERROR_POSIX         = 2 << BIT_ERROR_ORIGIN,
    ERROR_CKIT          = 3 << BIT_ERROR_ORIGIN,
    ERROR_USER          = 4 << BIT_ERROR_ORIGIN,
    ERROR_FUNCTION      = 5 << BIT_ERROR_ORIGIN,
    ERROR_STDIN         = 6 << BIT_ERROR_ORIGIN,
    ERROR_STDOUT        = 7 << BIT_ERROR_ORIGIN,
    ERROR_STDERR        = 8 << BIT_ERROR_ORIGIN,

// ERROR_FILE       4bit field
    ERROR_WINAPI        = 1 << BIT_ERROR_FILE,
    ERROR_UNOPENED      = 2 << BIT_ERROR_FILE,
    ERROR_UNREADABLE    = 3 << BIT_ERROR_FILE,
    ERROR_UNWRITEABLE   = 4 << BIT_ERROR_FILE,
    ERROR_UNCLOSED      = 5 << BIT_ERROR_FILE,
    ERROR_STDCLOSE      = 6 << BIT_ERROR_FILE,
    ERROR_EXISTING      = 7 << BIT_ERROR_FILE,
    ERROR_UNEXISTING    = 8 << BIT_ERROR_FILE,
    ERROR_INVALID_FD    = 9 << BIT_ERROR_FILE,
    ERROR_INVALID_PATH = 10 << BIT_ERROR_FILE,
    ERROR_MAX_REACHED  = 11 << BIT_ERROR_FILE,

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
} ErrorCode;

#ifdef INFO_OS_WINDOWS
    #define ERROR_OS                ERROR_WINDOWS
#else
    #define ERROR_OS                ERROR_POSIX
#endif

#define ERROR_SOFT_OS           (ERROR_SOFT | ERROR_OS)
#define ERROR_HARD_OS           (ERROR_HARD | ERROR_OS)

#define ERROR_SOFT_USER         (ERROR_SOFT | ERROR_USER)
#define ERROR_HARD_USER         (ERROR_HARD | ERROR_USER)

#define ERROR_SOFT_WINAPI       (ERROR_SOFT_OS | ERROR_WINAPI)
#define ERROR_HARD_WINAPI       (ERROR_HARD_OS | ERROR_WINAPI)

#define ERROR_STDIN_INVALID     (ERROR_HARD_WINAPI | ERROR_STDIN)
#define ERROR_STDOUT_INVALID    (ERROR_HARD_WINAPI | ERROR_STDOUT)
#define ERROR_STDERR_INVALID    (ERROR_HARD_WINAPI | ERROR_STDERR)
#define ERROR_STDOUT_MODE       (ERROR_SOFT_WINAPI | ERROR_STDOUT)
#define ERROR_STDERR_MODE       (ERROR_SOFT_WINAPI | ERROR_STDERR)
#define ERROR_STDOUT_VT         (ERROR_SOFT_WINAPI | ERROR_STDOUT)
#define ERROR_STDERR_VT         (ERROR_SOFT_WINAPI | ERROR_STDERR)

#define ERROR_USER_NULLPTR      (ERROR_SOFT_USER | ERROR_NULLPTR)
#define ERROR_USER_UNOPENED     (ERROR_SOFT_USER | ERROR_UNOPENED)
#define ERROR_USER_UNREADABLE   (ERROR_SOFT_USER | ERROR_UNREADABLE)
#define ERROR_USER_UNWRITEABLE  (ERROR_SOFT_USER | ERROR_UNWRITEABLE)
#define ERROR_USER_UNCLOSED     (ERROR_SOFT_USER | ERROR_UNCLOSED)
#define ERROR_USER_STDCLOSE     (ERROR_SOFT_USER | ERROR_STDCLOSE)
#define ERROR_USER_EXISTING     (ERROR_SOFT_USER | ERROR_EXISTING)
#define ERROR_USER_UNEXISTING   (ERROR_SOFT_USER | ERROR_UNEXISTING)
#define ERROR_USER_INVALID_FD   (ERROR_SOFT_USER | ERROR_INVALID_FD)

#define ERROR_FILES_UNCLOSED    (ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNCLOSED | ERROR_LEAK)

// #define 

#define iserror(code, value)        ((code & value) == value)
#define hastype(code)               ((code & (3  << BIT_ERROR_TYPE))        != 0)
#define hasorigin(code)             ((code & (15 << BIT_ERROR_ORIGIN))      != 0)
#define hasfile(code)               ((code & (15 << BIT_ERROR_FILE))        != 0)
#define hasmemory(code)             ((code & (15 << BIT_ERROR_MEMORY))      != 0)
#define hasstruct(code)             ((code & (15 << BIT_ERROR_STRUCT))      != 0)
#define hasoperation(code)          ((code & (63 << BIT_ERROR_OPERATION))   != 0)

void throwcallback(void* userdata, void (*callback)(void* _userdata, Context _context, ErrorCode _code, const char* msg));
void __throw(Context context, ErrorCode code, const char* msg);
void __throwf(Context context, ErrorCode code, const char* fmt, ...);

#define throw(code, msg)                        do { __throw(CONTEXT_HERE, code, msg); } while(0)
#define throwf(code, fmt, ...)                  do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__); } while(0)
#define throwif(cond, code, msg)                if ((cond)) { throw(code, msg); }
#define throwiff(cond, code, fmt, ...)          if ((cond)) { throwf(code, fmt, ##__VA_ARGS__); }

#define vthrow(code, msg)                       do { __throw(CONTEXT_HERE, code, msg);                   return; } while(0)
#define vthrowf(code, fmt, ...)                 do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__);   return; } while(0)
#define vthrowif(cond, code, msg)               if ((cond)) { throw(code, msg);                     return; }
#define vthrowiff(cond, code, fmt, ...)         if ((cond)) { throwf(code, fmt, ##__VA_ARGS__);     return; }

#define nthrow(code, msg)                       do { __throw(CONTEXT_HERE, code, msg);                   return NULL; } while(0)
#define nthrowf(code, fmt, ...)                 do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__);   return NULL; } while(0)
#define nthrowif(cond, code, msg)               if ((cond)) { throw(code, msg);                     return NULL; }
#define nthrowiff(cond, code, fmt, ...)         if ((cond)) { throwf(code, fmt, ##__VA_ARGS__);     return NULL; }

#define zthrow(code, msg)                       do { __throw(CONTEXT_HERE, code, msg);                   return 0; } while(0)
#define zthrowf(code, fmt, ...)                 do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__);   return 0; } while(0)
#define zthrowif(cond, code, msg)               if ((cond)) { throw(code, msg);                     return 0; }
#define zthrowiff(cond, code, fmt, ...)         if ((cond)) { throwf(code, fmt, ##__VA_ARGS__);     return 0; }

#define ithrow(code, msg)                       do { __throw(CONTEXT_HERE, code, msg);                   return -1; } while(0)
#define ithrowf(code, fmt, ...)                 do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__);   return -1; } while(0)
#define ithrowif(cond, code, msg)               if ((cond)) { throw(code, msg);                     return -1; }
#define ithrowiff(cond, code, fmt, ...)         if ((cond)) { throwf(code, fmt, ##__VA_ARGS__);     return -1; }

#define rthrow(rvalue, code, msg)               do { __throw(CONTEXT_HERE, code, msg);                   return rvalue; } while(0)
#define rthrowf(rvalue, code, fmt, ...)         do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__);   return rvalue; } while(0)
#define rthrowif(rvalue, cond, code, msg)       if ((cond)) { throw(code, msg);                     return rvalue; }
#define rthrowiff(rvalue, cond, code, fmt, ...) if ((cond)) { throwf(code, fmt, ##__VA_ARGS__);     return rvalue; }

#define gthrow(gotof, code, msg)                do { __throw(CONTEXT_HERE, code, msg);                   goto gotof; } while(0)
#define gthrowf(gotof, code, fmt, ...)          do { __throwf(CONTEXT_HERE, code, fmt, ##__VA_ARGS__);   goto gotof; } while(0)
#define gthrowif(gotof, cond, code, msg)        if ((cond)) { throw(code, msg);                     goto gotof; }
#define gthrowiff(gotof, cond, code, fmt, ...)  if ((cond)) { throwf(code, fmt, ##__VA_ARGS__);     goto gotof; }

#endif /* __CKIT_ERROR_H__ */