#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    createdir("ckitfiles");
    createdir("ckitfiles/copy");
    createdir("ckitfiles/tocopydir");
    createdir("ckitfiles/torenamedir");
    createfile("ckitfiles/tocopy.txt");
    createfile("ckitfiles/torename.txt");
    createfile("ckitfiles/tocopydir/subcopy1.txt");
    createfile("ckitfiles/tocopydir/subcopy2.txt");

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}