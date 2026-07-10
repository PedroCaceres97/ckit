#include <ckit/ansi.h>
#include <ckit/string.h>
#include <ckit/parser.h>
#include <ckit/config.h>
#include <ckit/symbols.h>

CKIT_BUFFERS(ansi, char, CKIT_ANSI_BUFFER_COUNT, CKIT_ANSI_BUFFER_SIZE)
CKIT_BUFFERS(format, char, CKIT_ANSI_FORMAT_BUFFER_COUNT, CKIT_ANSI_FORMAT_BUFFER_SIZE)

const char* ansi256(int n) {
    char* buffer = ckit_ansi_next();
    char* start = buffer;
    bool bg = n & ANSI_BG;
    *buffer++ = '\x1b';
    *buffer++ = '[';
    *buffer++ = bg ? '4' : '3';
    *buffer++ = '8';
    *buffer++ = ';';
    *buffer++ = '5';
    *buffer++ = ';';
    buffer += u32str_out(buffer, n & 0xFF, false, false);
    *buffer++ = 'm';
    *buffer++ = '\0';
    return start;
}
const char* ansirgb(int r, int g, int b) {
    char* buffer = ckit_ansi_next();
    char* start = buffer;
    bool bg = r & ANSI_BG || g & ANSI_BG || b & ANSI_BG;
    *buffer++ = '\x1b';
    *buffer++ = '[';
    *buffer++ = bg ? '4' : '3';
    *buffer++ = '8';
    *buffer++ = ';';
    *buffer++ = '2';
    *buffer++ = ';';

    buffer += u32str_out(buffer, r & 0xFF, false, false);
    *buffer++ = ';';
    buffer += u32str_out(buffer, g & 0xFF, false, false);
    *buffer++ = ';';
    buffer += u32str_out(buffer, b & 0xFF, false, false);
    *buffer++ = 'm';

    *buffer++ = '\0';
    return start;
}
const char* ansipos(int x, int y) {
    char* buffer = ckit_ansi_next();
    char* start = buffer;
    *buffer++ = '\x1b';
    *buffer++ = '[';
    
    buffer += u32str_out(buffer, y & 0xFFFF, false, false);
    *buffer++ = ';';
    buffer += u32str_out(buffer, x & 0xFFFF, false, false);
    *buffer++ = 'H';

    *buffer++ = '\0';
    return start;
}
const char* ansimove(AnsiDirection direction, int n) {
    char* buffer = ckit_ansi_next();
    char* start = buffer;
    *buffer++ = '\x1b';
    *buffer++ = '[';

    buffer += u32str_out(buffer, (uint32_t)n, false, false);

    // char directions[4] = {'A', 'B', 'C', 'D'};
    // *buffer++ = directions[direction];
    *buffer++ = 'A' + direction;
    *buffer++ = '\0';
    return start;
}

typedef struct {
    const char* ansi;
    const char* word;
    char        single;
} Token;

static const Token clears[] = {
    {ANSI_CLEAR_ALL,            "all",      'a'},
    {ANSI_CLEAR_SCREEN,         "screen",   's'},
    {ANSI_CLEAR_SCREEN_START,   "head",     'h'},
    {ANSI_CLEAR_SCREEN_END,     "tail",     't'},
    {ANSI_CLEAR_LINE,           "line",     'l'},
    {ANSI_CLEAR_LINE_START,     "begin",    'b'},
    {ANSI_CLEAR_LINE_END,       "end",      'e'}
};
static const Token cursors[] = {
    {ANSI_CURSOR_HOME,          "home",     'h'},
    {ANSI_CURSOR_SAVE,          "save",     's'},
    {ANSI_CURSOR_RESTORE,       "restore",  'r'},
    {ANSI_CURSOR_SHOW,          "visible",  'v'},
    {ANSI_CURSOR_HIDE,          "invisible",'i'}
};
static const Token positions[] = {
    {NULL,                      "up",       'u'},
    {NULL,                      "down",     'd'},
    {NULL,                      "forward",  'f'},
    {NULL,                      "backward", 'b'},
    {NULL,                      "next",     'n'},
    {NULL,                      "prev",     'p'},
    {NULL,                      "column",   'c'}
};
static const Token styles[] = {
    {ANSI_BOLD,                 "bold",     'b'},
    {ANSI_DIM,                  "dim",      'd'},
    {ANSI_ITALIC,               "italic",   'i'},
    {ANSI_UNDERLINE,            "underline",'u'},
    {ANSI_BLINK,                "blink",    'k'},
    {ANSI_REVERSE,              "reverse",  'r'},
    {ANSI_HIDDEN,               "hidden",   'h'},
    {ANSI_STRIKE,               "strike",   's'},
    {ANSI_DOUBLE,               "double",   'e'},
    {ANSI_OVERLINE,             "overline", 'o'}
};
static const Token resets[] = {
    {ANSI_RESET_BOLD,                 "bold",     'b'},
    {ANSI_RESET_DIM,                  "dim",      'd'},
    {ANSI_RESET_ITALIC,               "italic",   'i'},
    {ANSI_RESET_UNDERLINE,            "underline",'u'},
    {ANSI_RESET_BLINK,                "blink",    'k'},
    {ANSI_RESET_REVERSE,              "reverse",  'r'},
    {ANSI_RESET_HIDDEN,               "hidden",   'h'},
    {ANSI_RESET_STRIKE,               "strike",   's'},
    {ANSI_RESET_UNDERLINE,            "double",   'e'},
    {ANSI_RESET_OVERLINE,             "overline", 'o'}
};
static const Token foregrounds[] = {
    {ANSI_BLACK,                "black",    'k'},
    {ANSI_RED,                  "red",      'r'},
    {ANSI_GREEN,                "green",    'g'},
    {ANSI_YELLOW,               "yellow",   'y'},
    {ANSI_BLUE,                 "blue",     'b'},
    {ANSI_MAGENTA,              "magenta",  'm'},
    {ANSI_CYAN,                 "cyan",     'c'},
    {ANSI_WHITE,                "white",    'w'}
};
static const Token backgrounds[] = {
    {ANSI_BG_BLACK,             "black",    'k'},
    {ANSI_BG_RED,               "red",      'r'},
    {ANSI_BG_GREEN,             "green",    'g'},
    {ANSI_BG_YELLOW,            "yellow",   'y'},
    {ANSI_BG_BLUE,              "blue",     'b'},
    {ANSI_BG_MAGENTA,           "magenta",  'm'},
    {ANSI_BG_CYAN,              "cyan",     'c'},
    {ANSI_BG_WHITE,             "white",    'w'}
};

static bool match(CkitParser* parser, Token token) {
    if (!isalpha((unsigned char)parser->source[1])) { 
        return ckit_parser_lowadvance(parser, token.single);
    }

    size_t i = 0;
    size_t len = strlen(token.word);
    for (; i < len; i++) {
        if (tolower((unsigned char)parser->source[i]) != token.word[i]) {
            return false;
        }
    }
    parser->source += i;
    return true;
}
static int __find(CkitParser* parser, const Token* tokens, int count) {
    if (*parser->source == '\0') { 
        return -1; 
    }
    for (int i = 0; i < count; i++) {
        if (!match(parser, tokens[i])) {
            continue;
        }
        if (!isalnum((unsigned char)*parser->source)) {
            return i;
        }
        return -1;
    }
    return -1;
}
static const char* __map(CkitParser* parser, const Token* tokens, int count) {
    int i = __find(parser, tokens, count);
    if (i == -1) { return NULL; }
    return tokens[i].ansi;
}
#define map(parse, tokens) __map(parse, tokens, ARRAY_COUNT(tokens))
#define find(parse, tokens) __find(parse, tokens, ARRAY_COUNT(tokens))

static void clear(CkitParser* parser) {
    ckit_parser_skipspace(parser);
    ckit_parser_writestr(parser, map(parser, clears)); 
}
static void cursor(CkitParser* parser) {
    int x = 0;
    int y = 0;
    const char* ansi = NULL;

    ckit_parser_skipspace(parser);
    if (!ckit_parser_unsigned(parser, &x)) {
        ansi = map(parser, cursors);
    } else if (!ckit_parser_nextunsigned(parser, &y)) {
        int mode = find(parser, positions);
        if (mode == -1) { return; }
        ansi = ansimove(mode, x);
    } else {
        ansi = ansipos(x, y);
    }

    ckit_parser_writestr(parser, ansi);
    ckit_parser_skipspace(parser);
    ckit_parser_eqadvance(parser, '>');
}
static void enable(CkitParser* parse) {
    ckit_parser_skipspace(parse);
    ckit_parser_writestr(parse, map(parse, styles)); 
}
static void disable(CkitParser* parse) {
    ckit_parser_skipspace(parse);
    ckit_parser_writestr(parse, map(parse, resets)); 
}
static void foreground(CkitParser* parser) {
    int r = 0;
    int g = 0;
    int b = 0;
    const char* ansi = NULL;

    ckit_parser_skipspace(parser);
    if (!ckit_parser_unsigned(parser, &r)) {
        ansi = map(parser, foregrounds);
    } else if (!ckit_parser_nextunsigned(parser, &g)) {
        ansi = ansi256(r); 
    } else {
        ckit_parser_nextunsigned(parser, &b);
        ansi = ansirgb(r, g, b);
    }

    ckit_parser_writestr(parser, ansi);
    ckit_parser_skipspace(parser);
    ckit_parser_eqadvance(parser, ']');
}
static void background(CkitParser* parser) {
    int r = 0;
    int g = 0;
    int b = 0;
    const char* ansi = NULL;

    ckit_parser_skipspace(parser);
    if (!ckit_parser_unsigned(parser, &r)) {
        ansi = map(parser, backgrounds);
    } else if (!ckit_parser_nextunsigned(parser, &g)) {
        ansi = ansi256(r | ANSI_BG); 
    } else {
        ckit_parser_nextunsigned(parser, &b);
        ansi = ansirgb(r | ANSI_BG, g, b);
    }

    ckit_parser_writestr(parser, ansi);
    ckit_parser_skipspace(parser);
    ckit_parser_eqadvance(parser, '}');
}

const char* ansiformat(const char* fmt, void* userdata, int(*nextint)(void*)) {
    CkitParser parser = {0};
    parser.buf = ckit_format_next();
    parser.count = CKIT_ANSI_FORMAT_BUFFER_SIZE - 1;
    parser.source = fmt;
    parser.nextint = nextint;
    parser.userdata = userdata;

    while (*parser.source) {
        ckit_parser_skipspace(&parser);
        if (ckit_parser_eqadvance(&parser, '!')) { clear(&parser); }
        else if (ckit_parser_eqadvance(&parser, '<')) { cursor(&parser); }
        else if (ckit_parser_eqadvance(&parser, '+')) { enable(&parser); }
        else if (ckit_parser_eqadvance(&parser, '-')) { disable(&parser); }
        else if (ckit_parser_eqadvance(&parser, '[')) { foreground(&parser); }
        else if (ckit_parser_eqadvance(&parser, '{')) { background(&parser); }
        else { parser.source++; }
    }

    ckit_parser_terminate(&parser);
    return parser.buf;
}