#include <ckit/error.h>
#include <ckit/config.h>
#include <ckit/stdio.h>

static void default_callback(void* userdata, Context context, ErrorCode code, const char* msg) {
#ifdef INFO_OS_WINDOWS
    if (code == ERROR_STDIN_INVALID || 
        code == ERROR_STDOUT_INVALID ||
        code == ERROR_STDERR_INVALID) {
            fprintf(stderr, "[WINSTART]: %s", msg);
    }
#endif
    (void)userdata;
    if (code & ERROR_SOFT) { printlog(PRINT_WARNING, msg); }
    else if (code & ERROR_HARD) { printinform(context, PRINT_ERROR, msg); exit(-1); }
}

static void* ckit_userdata = NULL;
static void (*ckit_callback)(void* _userdata, Context _context, ErrorCode _code, const char* _msg) = default_callback;

void throwcallback(void* userdata, void (*callback)(void* _userdata, Context _context, ErrorCode _code, const char* _msg)) {
    if (userdata) { ckit_userdata = userdata; }
    if (callback) { ckit_callback = callback; }
}
void __throw(Context context, ErrorCode code, const char* msg) {
    ckit_callback(ckit_userdata, context, code, msg);
}
void __throwf(Context context, ErrorCode code, const char* fmt, ...) {
    char buf[CKIT_FORMAT_BUFFER_SIZE] = {0};
    va_list args;
    va_start(args, fmt);
    vsnformat(buf, sizeof(buf), true, fmt, args);
    va_end(args);
    ckit_callback(ckit_userdata, context, code, buf);
}