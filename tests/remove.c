#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    removedir("ckitfiles", true);

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}