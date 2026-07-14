#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    renamedir("ckitfiles/torenamedir", "ckitfiles/renameddir");
    renamefile("ckitfiles/torename.txt", "ckitfiles/renamed.txt");

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}