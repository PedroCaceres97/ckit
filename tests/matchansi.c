#include <ckit/ansi.h>
#include <ckit/stdio.h>

int main() {
    printformat(ANSI_RED                  "%10s "     ANSI_RESET, "RED");
    printformat(ANSI_GREEN                "%10s "     ANSI_RESET, "GREEN");
    printformat(ANSI_YELLOW               "%10s "     ANSI_RESET, "YELLOW");
    printformat(ANSI_BLUE                 "%10s "     ANSI_RESET, "BLUE");
    printformat(ANSI_MAGENTA              "%10s "     ANSI_RESET, "MAGENTA");
    printformat(ANSI_CYAN                 "%10s\n\n"  ANSI_RESET, "CYAN");

    printformat(ANSI_RED     ANSI_BOLD    "%10s "     ANSI_RESET, "RED");
    printformat(ANSI_GREEN   ANSI_BOLD    "%10s "     ANSI_RESET, "GREEN");
    printformat(ANSI_YELLOW  ANSI_BOLD    "%10s "     ANSI_RESET, "YELLOW");
    printformat(ANSI_BLUE    ANSI_BOLD    "%10s "     ANSI_RESET, "BLUE");
    printformat(ANSI_MAGENTA ANSI_BOLD    "%10s "     ANSI_RESET, "MAGENTA");
    printformat(ANSI_CYAN    ANSI_BOLD    "%10s\n\n"  ANSI_RESET, "CYAN");

    for (int i = 1; i < 256; i++) {
        printformat("%aColor: %03i ", "[*]", i, i);
        if (!(i % 8)) { printformat("\n"); }
    }
}