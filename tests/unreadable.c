#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    char buffer[256] = {0};

    int fd = openfile("ckitfiles/unreadable.txt", ATTRIBUTE_WRITE | ATTRIBUTE_TRUNCATE);
    printfile(fd, "This is an unreadable file");
    closefile(fd);
    
    fd = openfile("ckitfiles/unreadable.txt", ATTRIBUTE_NONE);
    readfile(fd, buffer, sizeof(buffer));
    closefile(fd);

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}