#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    int fd = openfile("ckitfiles/unclosed.txt", ATTRIBUTE_WRITE | ATTRIBUTE_TRUNCATE);
    printfile(fd, "This is an unclosed file");

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}