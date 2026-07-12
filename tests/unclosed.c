#include <ckit/stdio.h>

int main() {
    createdir("ckitfiles");
    createfile("ckitfiles/unclosed");
    openfile("ckitfiles/unclosed", ATTRIBUTE_READ);
    return 0;
}