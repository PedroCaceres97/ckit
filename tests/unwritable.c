#include <ckit/stdio.h>

int main() {
#if !CKIT_INIT_PREMAIN
    ckit_init();
#endif

    int fd = openfile("ckitfiles/unwritable.txt", ATTRIBUTE_WRITE | ATTRIBUTE_TRUNCATE);
    printfile(fd, "This is an unwritable file");
    closefile(fd);
    
    fd = openfile("ckitfiles/unwritable.txt", ATTRIBUTE_NONE);
    printfile(fd, "\nThis text shold not be seen");
    closefile(fd);

#if !CKIT_QUIT_ATEXIT
    ckit_quit();
#endif
    return 0;
}