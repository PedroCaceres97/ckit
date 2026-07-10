#include <ckit/error.h>

void throwerror(Context context, ErrorCode code, const char* fmt, ...) {
    
}
void callbackerror(void* userdata, ErrorBehaviour(*callback)(void* _userdata, Context _context, ErrorCode _code, const char* msg)) {
    
}