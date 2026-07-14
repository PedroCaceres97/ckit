#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    openfile("ckitfiles/unexisting.txt", ATTRIBUTE_NONE);

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}