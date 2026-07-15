#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    printformat("ckitfiles: %i\nckitfiles/copy: %i\n", countdir("ckitfiles"), countdir("ckitfiles/copy"));

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}