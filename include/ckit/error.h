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

#define CKIT_ERROR_CLOSE_STD            "Standard file is not closeable"
#define CKIT_ERROR_INVALID_FD           "Invalid file descriptor (negative or bigger than 255)"
#define CKIT_ERROR_FILE_UNOPENED        "Trying to operate over an unopened file"
#define CKIT_ERROR_FILE_UNREADABLE      "File missing ATTRIBUTE_READ"
#define CKIT_ERROR_FILE_UNWRITEABLE     "File missing ATTRIBUTE_WRITE"
#define CKIT_ERROR_FILE_DOESNT_EXIST    "File does not exist"
#define CKIT_ERROR_FILES_MAX_REACHED    "Unable to open more files than 256 (std included)"
#define CKIT_ERROR_FILES_UNCLOSED       "Detected unclosed files at terminate"

#define CKIT_ERROR_COPYFILE_REPLACE     "Cant copy a file into an already existent file"
#define CKIT_ERROR_RENAMEFILE_REPLACE   "Cant rename a file into an already existent file"

#define CKIT_ERROR_COPYDIR_REPLACE      "Cant copy a directory into an already existent directory"
#define CKIT_ERROR_RENAMEDIR_REPLACE    "Cant rename a directory into an already existent directory"

#define CKIT_ERROR_NULLPTR              "Memory address is a nullptr -> "
#define CKIT_ERROR_FOREIGN_PTR          "Memory address does not belong to struct"
#define CKIT_ERROR_ALREADY_FREE         "Memory address is already free"
#define CKIT_ERROR_NOT_ALLOCATED        "Memory address was not allocated"
#define CKIT_ERROR_ZERO_SIZE_ALLOCATION "Requested allocation size is 0"

#define CKIT_ERROR_GETINPUT_OVERFLOW    "Input overflowed"

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
    ERROR_UNWRITEABLE   = 3 << BIT_ERROR_FILE,
    ERROR_UNCLOSED      = 4 << BIT_ERROR_FILE,
    ERROR_STDCLOSE      = 5 << BIT_ERROR_FILE,
    ERROR_INVALID_FD    = 6 << BIT_ERROR_FILE,
    ERROR_INVALID_PATH  = 7 << BIT_ERROR_FILE,
    ERROR_MAX_REACHED   = 8 << BIT_ERROR_FILE,
    ERROR_WINAPI_FILE   = 9 << BIT_ERROR_FILE,

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

// ERROR_OPERATION  6bit field
    ERROR_COPYDIR       = 1 << BIT_ERROR_OPERATION,
    ERROR_CHECKDIR      = 2 << BIT_ERROR_OPERATION,
    ERROR_CREATEDIR     = 3 << BIT_ERROR_OPERATION,
    ERROR_REMOVEDIR     = 4 << BIT_ERROR_OPERATION,
    ERROR_RENAMEDIR     = 5 << BIT_ERROR_OPERATION,

    ERROR_COPYFILE      = 6 << BIT_ERROR_OPERATION,
    ERROR_CHECKFILE     = 7 << BIT_ERROR_OPERATION,
    ERROR_CREATEFILE    = 8 << BIT_ERROR_OPERATION,
    ERROR_REMOVEFILE    = 9 << BIT_ERROR_OPERATION,
    ERROR_RENAMEFILE   = 10 << BIT_ERROR_OPERATION,

    ERROR_OPENFILE     = 11 << BIT_ERROR_OPERATION,
    ERROR_CLOSEFILE    = 12 << BIT_ERROR_OPERATION,
    ERROR_DUMPFILE     = 13 << BIT_ERROR_OPERATION,
    ERROR_READFILE     = 14 << BIT_ERROR_OPERATION,
    ERROR_WRITEFILE    = 15 << BIT_ERROR_OPERATION,
    ERROR_SIZEFILE     = 16 << BIT_ERROR_OPERATION,
    ERROR_TELLFILE     = 17 << BIT_ERROR_OPERATION,
    ERROR_SEEKFILE     = 18 << BIT_ERROR_OPERATION,

    ERROR_CREATE       = 19 << BIT_ERROR_OPERATION,
    ERROR_DESTROY      = 20 << BIT_ERROR_OPERATION,
    ERROR_FREE         = 21 << BIT_ERROR_OPERATION,
    ERROR_ALLOC        = 22 << BIT_ERROR_OPERATION,
    ERROR_REALLOC      = 23 << BIT_ERROR_OPERATION,
    ERROR_DUPLICATE    = 24 << BIT_ERROR_OPERATION,
    ERROR_AT           = 25 << BIT_ERROR_OPERATION,
    ERROR_SET          = 26 << BIT_ERROR_OPERATION,
    ERROR_GET          = 27 << BIT_ERROR_OPERATION,
    ERROR_CLEAR        = 28 << BIT_ERROR_OPERATION,
    ERROR_ERASE        = 29 << BIT_ERROR_OPERATION,
    ERROR_POPBACK      = 30 << BIT_ERROR_OPERATION,
    ERROR_POPFRONT     = 31 << BIT_ERROR_OPERATION,
    ERROR_INSERT       = 32 << BIT_ERROR_OPERATION,
    ERROR_PUSHBACK     = 33 << BIT_ERROR_OPERATION,
    ERROR_PUSHFRONT    = 34 << BIT_ERROR_OPERATION,
    ERROR_MEMCPY       = 35 << BIT_ERROR_OPERATION,
    ERROR_MEMSET       = 36 << BIT_ERROR_OPERATION,
    ERROR_APPEND       = 37 << BIT_ERROR_OPERATION,
    ERROR_REPEAT       = 38 << BIT_ERROR_OPERATION,
} ErrorCode;

#define ERROR_WIN_STDIN_INVALID     (ERROR_HARD | ERROR_WINDOWS | ERROR_OS | ERROR_STDIN | ERROR_WINAPI_FILE)
#define ERROR_WIN_STDOUT_INVALID    (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_WINAPI_FILE)
#define ERROR_WIN_STDERR_INVALID    (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDERR | ERROR_WINAPI_FILE)
#define ERROR_WIN_STDOUT_MODE       (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_WINAPI_FILE)
#define ERROR_WIN_STDERR_MODE       (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_WINAPI_FILE)
#define ERROR_WIN_STDOUT_VT         (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDOUT | ERROR_WINAPI_FILE)
#define ERROR_WIN_STDERR_VT         (ERROR_SOFT | ERROR_WINDOWS | ERROR_OS | ERROR_STDERR | ERROR_WINAPI_FILE)

#define ERROR_WIN_FILES_UNCLOSED (ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNCLOSED | ERROR_LEAK)

// #define 

#define iserror(code, value)        ((code & value) == value)
#define hastype(code)               ((code & (3  << BIT_ERROR_TYPE))        != 0)
#define hasplatform(code)           ((code & (3  << BIT_ERROR_PLATFORM))    != 0)
#define hasfault(code)              ((code & (15 << BIT_ERROR_FAULT))       != 0)
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