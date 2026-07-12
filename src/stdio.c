#include <ckit/stdio.h>
#include <ckit/error.h>
#include <ckit/config.h>
#include <ckit/symbols.h>

CKIT_BUFFERS(format, char, CKIT_FORMAT_BUFFER_COUNT, CKIT_FORMAT_BUFFER_SIZE)
CKIT_BUFFERS(getinput, char, CKIT_GETLINE_BUFFER_COUNT, CKIT_GETLINE_BUFFER_SIZE + 64)

typedef struct {
    const char* title;
    int         color;
    int         fd;
} LogData;

static LogData LogDatas[6] = {
    [PRINT_INFO]       = (LogData){.title = CKIT_INFO_TITLE,     .color = CKIT_INFO_COLOR,      .fd = 1, },
    [PRINT_DEBUG]      = (LogData){.title = CKIT_DEBUG_TITLE,    .color = CKIT_DEBUG_COLOR,     .fd = 1, },
    [PRINT_SUCCESS]    = (LogData){.title = CKIT_SUCCESS_TITLE,  .color = CKIT_SUCCESS_COLOR,   .fd = 1, },
    [PRINT_WARNING]    = (LogData){.title = CKIT_WARNING_TITLE,  .color = CKIT_WARNING_COLOR,   .fd = 2, },
    [PRINT_ERROR]      = (LogData){.title = CKIT_ERROR_TITLE,    .color = CKIT_ERROR_COLOR,     .fd = 2, },
    [PRINT_FATAL]      = (LogData){.title = CKIT_FATAL_TITLE,    .color = CKIT_FATAL_COLOR,     .fd = 2, }
};

const char* format(const char* fmt, ...) {
    if (!fmt) { return NULL; }

    va_list args;
    va_start(args, fmt);
    char* buf = ckit_format_next();
    vsnformat(buf, CKIT_FORMAT_BUFFER_SIZE, true, fmt, args);

    va_end(args);
    return buf;
}
size_t fformat(int fd, const char* fmt, ...) {
    if (!fmt) { return 0; }
    va_list args;
    va_start(args, fmt);
    char buf[CKIT_FORMAT_BUFFER_SIZE] = {0};
    size_t written = vsnformat(buf, CKIT_FORMAT_BUFFER_SIZE, attributes(fd) & ATTRIBUTE_ANSI, fmt, args);
    writefile(fd, buf, written);

    va_end(args);
    return written;
}
size_t sformat(char* buf, const char* fmt, ...) {
    if (!buf || !fmt) { return 0; }

    va_list args;
    va_start(args, fmt);
    size_t written = vsnformat(buf, SIZE_MAX, true, fmt, args);

    va_end(args);
    return written;
}
size_t snformat(char* buf, size_t count, bool enableAnsi, const char* fmt, ...) {
    if (!fmt) { return 0; }

    va_list args;
    va_start(args, fmt);
    size_t written = vsnformat(buf, count, enableAnsi, fmt, args);

    va_end(args);
    return written;
}

void print(const char* buf) {
    writefile(1, buf, strlen(buf));
}
void printlog(PrintLog level, const char* fmt, ...) {
    const char* title = LogDatas[level].title;
    int color = LogDatas[level].color;
    int fd = LogDatas[level].fd;
    
    char buf[CKIT_FORMAT_BUFFER_SIZE] = {0};
    va_list args;
    va_start(args, fmt);
    vsnformat(buf, CKIT_FORMAT_BUFFER_SIZE, true, fmt, args);
    va_end(args);
    fformat(fd, "%-a%-8s%-a%s\n",
        "[*]",      color, title,
        "[*]",      CKIT_MESSAGE_COLOR, buf
    );
}
void printinform(Context context, PrintLog level, const char* fmt, ...) {
    const char* title = LogDatas[level].title;
    int color = LogDatas[level].color;
    int fd = LogDatas[level].fd;
    
    char buf[CKIT_FORMAT_BUFFER_SIZE] = {0};
    va_list args;
    va_start(args, fmt);
    vsnformat(buf, CKIT_FORMAT_BUFFER_SIZE, true, fmt, args);
    va_end(args);
    fformat(fd, "%-a%s\n%-a%s  %a%s:%u -> %s()%0a\n%-a%s  %-a%s\n\n",
        "[*]",      color,          title,
        "[*]",      CKIT_LABEL_COLOR,    CKIT_LABEL_CONTEXT,
        "[*]+i",    CKIT_CONTEXT_COLOR,  CONTEXT_ARGS(context),
        "[*]",      CKIT_LABEL_COLOR,    CKIT_LABEL_MESSAGE,
        "[*]",      CKIT_MESSAGE_COLOR,  buf
    );
}
void printformat(const char* fmt, ...) {
    if (!fmt) { return; }

    va_list args;
    va_start(args, fmt);
    char buf[CKIT_FORMAT_BUFFER_SIZE] = {0};
    size_t written = vsnformat(buf, CKIT_FORMAT_BUFFER_SIZE, true, fmt, args);
    writefile(1, buf, written);
    va_end(args);
}

size_t readinput(char* buf, size_t max) {
    return readfile(0, buf, max);
}
const char* getinput() {
    char* buffer = ckit_getinput_next();
    size_t readed = readfile(0, buffer, CKIT_GETLINE_BUFFER_SIZE + 64);
    memset(&buffer[CKIT_GETLINE_BUFFER_SIZE], 0, 64);
    throwif(readed > CKIT_GETLINE_BUFFER_SIZE, ERROR_SOFT | ERROR_STDIN, ERRMSG_GETINPUT_OVERFLOW);
    return buffer;
}