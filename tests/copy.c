#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    removedir("ckitfiles/copy", true);
    createdir("ckitfiles/copy");

    copydir("ckitfiles/tocopydir", "ckitfiles/copy");
    copyfile("ckitfiles/tocopy.txt", "ckitfiles/copy");

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}