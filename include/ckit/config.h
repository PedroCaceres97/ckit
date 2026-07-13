#ifndef __CKIT_CONFIG_H__
#define __CKIT_CONFIG_H__

/* --------------------------------------------------------------------------
 * Features
 * -------------------------------------------------------------------------- */

#define CKIT_DONT_ENABLE_ANSI_FOR_WINDOWS 0

#define CKIT_TRACK_MEMORY 1
#define CKIT_TRACKER_FREE(ptr) free((void*)ptr)
#define CKIT_TRACKER_MALLOC(type, bytes) (type*)malloc(bytes)
#define CKIT_TRACKER_CALLOC(type, count) (type*)calloc(count, sizeof(type))
#define CKIT_TRACKER_REALLOC(type, ptr, bytes) (type*)realloc(ptr, bytes)

#define CKIT_ENABLE_RWLOCK 0

#define CKIT_ENSURE_DONT_EXIT 0
#define CKIT_ENSURE_SHORT_LOG 0

#define CKIT_EXPECT_DONT_EXIT 1
#define CKIT_EXPECT_SHORT_LOG 1

#define CKIT_FORMAT_DEFAULT_PRECISION 6

/* --------------------------------------------------------------------------
 * Buffers
 * -------------------------------------------------------------------------- */

#define CKIT_TOSTR_BUFFER_COUNT             32
#define CKIT_TOSTR_BUFFER_SIZE              128

#define CKIT_ANSI_BUFFER_COUNT              32
#define CKIT_ANSI_BUFFER_SIZE               128
#define CKIT_ANSI_FORMAT_BUFFER_COUNT       8
#define CKIT_ANSI_FORMAT_BUFFER_SIZE        512

#define CKIT_FORMAT_BUFFER_COUNT            8
#define CKIT_FORMAT_BUFFER_SIZE             4096
#define CKIT_FORMAT_ANSI_SLOT_BUFFER_COUNT  32

#define CKIT_GETLINE_BUFFER_COUNT           4
#define CKIT_GETLINE_BUFFER_SIZE            4096

/* --------------------------------------------------------------------------
 * Structs
 * -------------------------------------------------------------------------- */

#define CKIT_VECTOR_MINIMUM_SIZE 10

/* --------------------------------------------------------------------------
 * Log
 * -------------------------------------------------------------------------- */

#define CKIT_INFO_COLOR     39
#define CKIT_DEBUG_COLOR    104
#define CKIT_SUCCESS_COLOR  46
#define CKIT_WARNING_COLOR  220
#define CKIT_ERROR_COLOR    196
#define CKIT_FATAL_COLOR    199

#define CKIT_INFO_TITLE     "INFO"
#define CKIT_DEBUG_TITLE    "DEBUG"
#define CKIT_SUCCESS_TITLE  "SUCCESS"
#define CKIT_WARNING_TITLE  "WARNING"
#define CKIT_ERROR_TITLE    "ERROR"
#define CKIT_FATAL_TITLE    "FATAL"
#define CKIT_WINAPI_TITLE   "[WINAPI]"
#define CKIT_POSIX_TITLE    "[POSIX]"

#define CKIT_CONTEXT_COLOR  108
#define CKIT_MESSAGE_COLOR  255
#define CKIT_LABEL_COLOR    245

#define CKIT_LABEL_CONTEXT  "Context"
#define CKIT_LABEL_MESSAGE  "Message"

#define CKIT_QUIT_ATEXIT 1
#define CKIT_INIT_PREMAIN 1

void ckit_init();
void ckit_quit();

#endif /* __CKIT_CONFIG_H__ */