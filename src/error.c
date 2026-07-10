#include <ckit/error.h>
#include <ckit/config.h>
#include <ckit/stdio.h>

static void* ckit_userdata = NULL;
static ErrorAction (*ckit_callback)(void* _userdata, Context _context, ErrorCode _code, const char* msg) = NULL;

void throwcallback(void* userdata, ErrorAction (*callback)(void* _userdata, Context _context, ErrorCode _code, const char* msg)) {
    if (userdata) { ckit_userdata = userdata; }
    if (callback) { ckit_callback = callback; }
}
ErrorAction __throw(Context context, ErrorCode code, const char* msg) {
    ErrorAction action = ckit_callback(ckit_userdata, context, code, msg);
    if (action == ERROR_ACTION_ABORT) {
        exit(-1);
    }
    return action;
}
ErrorAction __throwf(Context context, ErrorCode code, const char* fmt, ...) {
    char buf[CKIT_FORMAT_BUFFER_SIZE] = {0};
    va_list args;
    va_start(args, fmt);
    vsnformat(buf, sizeof(buf), true, fmt, args);
    va_end(args);
    ErrorAction action = ckit_callback(ckit_userdata, context, code, buf);
    if (action == ERROR_ACTION_ABORT) {
        exit(-1);
    }
    return action;
}
