#ifndef __CKIT_ANSI_H__
#define __CKIT_ANSI_H__

#include <ckit/info.h>

#define ANSI_ESC  "\x1b["
#define ANSI_TEXT(ansi, text)   ansi text MY_ANSI_RESET

// Ansi Format: !

#define ANSI_CLEAR_ALL          ANSI_CLEAR_SCREEN ANSI_CURSOR_HOME
#define ANSI_CLEAR_SCREEN       ANSI_ESC "2J"
#define ANSI_CLEAR_SCREEN_END   ANSI_ESC "0J"
#define ANSI_CLEAR_SCREEN_START ANSI_ESC "1J"
#define ANSI_CLEAR_LINE         ANSI_ESC "2K"
#define ANSI_CLEAR_LINE_END     ANSI_ESC "0K"
#define ANSI_CLEAR_LINE_START   ANSI_ESC "1K"

// Ansi Format: []

#define ANSI_BLACK              ANSI_ESC "30m"
#define ANSI_RED                ANSI_ESC "31m"
#define ANSI_GREEN              ANSI_ESC "32m"
#define ANSI_YELLOW             ANSI_ESC "33m"
#define ANSI_BLUE               ANSI_ESC "34m"
#define ANSI_MAGENTA            ANSI_ESC "35m"
#define ANSI_CYAN               ANSI_ESC "36m"
#define ANSI_WHITE              ANSI_ESC "37m"
#define ANSI_256(n)             ANSI_ESC "38;5;" #n "m"
#define ANSI_RGB(r, g, b)       ANSI_ESC "38;2;" #r ";" #g ";" #b "m"

// Ansi Format: {}

#define ANSI_BG_BLACK           ANSI_ESC "40m"
#define ANSI_BG_RED             ANSI_ESC "41m"
#define ANSI_BG_GREEN           ANSI_ESC "42m"
#define ANSI_BG_YELLOW          ANSI_ESC "43m"
#define ANSI_BG_BLUE            ANSI_ESC "44m"
#define ANSI_BG_MAGENTA         ANSI_ESC "45m"
#define ANSI_BG_CYAN            ANSI_ESC "46m"
#define ANSI_BG_WHITE           ANSI_ESC "47m"
#define ANSI_BG_256(n)          ANSI_ESC "48;5;" #n "m"
#define ANSI_BG_RGB(r, g, b)    ANSI_ESC "48;2;" #r ";" #g ";" #b "m"

// Ansi Format: +

#define ANSI_BOLD               ANSI_ESC "1m"
#define ANSI_DIM                ANSI_ESC "2m"
#define ANSI_ITALIC             ANSI_ESC "3m"
#define ANSI_UNDERLINE          ANSI_ESC "4m"
#define ANSI_BLINK              ANSI_ESC "5m"
#define ANSI_REVERSE            ANSI_ESC "7m"
#define ANSI_HIDDEN             ANSI_ESC "8m"
#define ANSI_STRIKE             ANSI_ESC "9m"
#define ANSI_DOUBLE             ANSI_ESC "21m"
#define ANSI_OVERLINE           ANSI_ESC "53m"

// Ansi Format: -

#define ANSI_RESET              ANSI_ESC "0m"
#define ANSI_RESET_BOLD         ANSI_ESC "22m"
#define ANSI_RESET_DIM          ANSI_ESC "22m"
#define ANSI_RESET_ITALIC       ANSI_ESC "23m"
#define ANSI_RESET_UNDERLINE    ANSI_ESC "24m"
#define ANSI_RESET_BLINK        ANSI_ESC "25m"
#define ANSI_RESET_REVERSE      ANSI_ESC "27m"
#define ANSI_RESET_HIDDEN       ANSI_ESC "28m"
#define ANSI_RESET_STRIKE       ANSI_ESC "29m"
#define ANSI_RESET_OVERLINE     ANSI_ESC "55m"

// Ansi Format: <>

#define ANSI_CURSOR_UP(n)       ANSI_ESC #n "A"
#define ANSI_CURSOR_DOWN(n)     ANSI_ESC #n "B"
#define ANSI_CURSOR_FORWARD(n)  ANSI_ESC #n "C"
#define ANSI_CURSOR_BACK(n)     ANSI_ESC #n "D"
#define ANSI_CURSOR_NEXT(n)     ANSI_ESC #n "E"
#define ANSI_CURSOR_PREV(n)     ANSI_ESC #n "F"
#define ANSI_CURSOR_COLUMN(n)   ANSI_ESC #n "G"
#define ANSI_CURSOR_POS(x, y)   ANSI_ESC #y ";" #x "H"
#define ANSI_CURSOR_HOME        ANSI_ESC "H"
#define ANSI_CURSOR_SAVE        ANSI_ESC "s"
#define ANSI_CURSOR_RESTORE     ANSI_ESC "u"
#define ANSI_CURSOR_SHOW        ANSI_ESC "?25h"
#define ANSI_CURSOR_HIDE        ANSI_ESC "?25l"

typedef enum {
    ANSI_UP,
    ANSI_DOWN,
    ANSI_FORWARD,
    ANSI_BACK,
    ANSI_NEXT,
    ANSI_PREV,
    ANSI_COLUMN
} AnsiDirection;

// To apply bg color:
//  ansi256(n | ANSI_BG);
//  ansirgb(r | ANSI_BG, g, b); (or any combination of g | ANSI_BG, b | ANSI_BG)
#define ANSI_BG 65536 // 0b00010000000000000000

const char* ansi256     (int n);
const char* ansirgb     (int r, int g, int b);
const char* ansipos     (int x, int y);
const char* ansimove    (AnsiDirection direction, int n);
const char* ansiformat  (const char* fmt, void* userdata, int(*getint)(void*));

#endif /* __CKIT_ANSI_H__ */