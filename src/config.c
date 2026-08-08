#include <ckit/config.h>
#include <ckit/error.h>
#include <ckit/utils.h>
#include <ckit/info.h>

#include <stdlib.h>

static void printout(const char* msg) { fwrite((void*)msg, sizeof(char), strlen(msg), stdout); }
static void printerr(const char* msg) { fwrite((void*)msg, sizeof(char), strlen(msg), stderr); }

void ckit_platform_init();
void ckit_platform_quit();

#if CKIT_INIT_PREMAIN

CkitInfo ckit_info = {
    .printout = printout,
    .printerr = printerr,

    .free = free,
    .malloc = malloc,
    .calloc = calloc,
    .realloc = realloc
};

PREMAIN(ckit_premain) {
    ckit_init(EMPTY(CkitInfo));
}

void ckit_init(CkitInfo info) {
#if CKIT_QUIT_ATEXIT
    atexit(ckit_quit);
#endif
    (void)info;
    ckit_platform_init();
}

#else

CkitInfo ckit_info = {0};

void ckit_init(CkitInfo info) {
#if CKIT_QUIT_ATEXIT
    atexit(ckit_quit);
#endif
    vthrownull(info.printout);
    vthrownull(info.printerr);
    vthrownull(info.free);
    vthrownull(info.malloc);
    vthrownull(info.calloc);
    vthrownull(info.realloc);

    ckit_info = info;
    ckit_platform_init();
}

#endif

void ckit_quit() {
    ckit_platform_quit();
}

void ckit_printout(const char* msg) {
    ckit_info.printout(msg);
}
void ckit_printerr(const char* msg) {
    ckit_info.printerr(msg);
}

void ckit_free(void* ptr) {
    throwif(!ptr, ERROR_NULLPTR, ERRMSG_NULLPTR(ptr));
    ckit_info.free(ptr);
}
void* ckit_malloc(size_t bytes) {
    throwif(!bytes, ERROR_USER_ZERO_SIZE, ERRMSG_ZERO_SIZE_ALLOCATION);
    void* ptr = ckit_info.malloc(bytes);
    memset(ptr, 0, bytes);
    return ptr;
}
void* ckit_calloc(size_t count, size_t size) {
    throwif(!count || !size, ERROR_USER_ZERO_SIZE, ERRMSG_ZERO_SIZE_ALLOCATION);
    return ckit_info.calloc(count, size);
}
void* ckit_realloc(void* ptr, size_t bytes) {
    throwif(!ptr && !bytes, ERROR_USER_ZERO_SIZE, ERRMSG_ZERO_SIZE_ALLOCATION);
    if (!ptr) { return ckit_malloc(bytes); }
    else if (!bytes) { ckit_free(ptr); return NULL; }
    return ckit_info.realloc(ptr, bytes);
}