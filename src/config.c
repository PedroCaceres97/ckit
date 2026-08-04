#include <ckit/config.h>
#include <ckit/utils.h>
#include <ckit/info.h>

#include <stdlib.h>

CkitInfo ckit_info = {
    .free = free,
    .malloc = malloc,
    .calloc = calloc,
    .realloc = realloc
};

void ckit_platform_init();
void ckit_platform_quit();

#if CKIT_INIT_PREMAIN
PREMAIN(premain) {
    ckit_init(EMPTY(CkitInfo));
}
#endif

void ckit_init(CkitInfo info) {
#if !CKIT_INIT_PREMAIN
    ckit_info = info;
#endif
#if CKIT_QUIT_ATEXIT
    atexit(ckit_quit);
#endif
    (void)info;
    ckit_platform_init();
}
void ckit_quit() {
    ckit_platform_quit();
}

void ckit_free(void* ptr);
void* ckit_malloc(size_t bytes);
void* ckit_calloc(size_t count, size_t size);
void* ckit_realloc(void* ptr, size_t bytes);