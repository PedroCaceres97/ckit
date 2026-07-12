#include <ckit/ansi.h>
#include <ckit/stdio.h>

int main() {
    printlog(PRINT_INFO,    "This is a info");
    printlog(PRINT_DEBUG,   "This is a debug");
    printlog(PRINT_SUCCESS, "This is a success");
    printlog(PRINT_WARNING, "This is a warning");
    printlog(PRINT_ERROR,   "This is a error");
    printlog(PRINT_FATAL,   "This is a fatal\n");
    
    print("Press enter: ");
    getinput();
    printformat("%a", "!a");

    printinform(CONTEXT_HERE, PRINT_INFO,     "This is a info");
    printinform(CONTEXT_HERE, PRINT_DEBUG,    "This is a debug");
    printinform(CONTEXT_HERE, PRINT_SUCCESS,  "This is a success");
    printinform(CONTEXT_HERE, PRINT_WARNING,  "This is a warning");
    printinform(CONTEXT_HERE, PRINT_ERROR,    "This is a error");
    printinform(CONTEXT_HERE, PRINT_FATAL,    "This is a fatal");
}