#include <ckit/ansi.h>
#include <ckit/stdio.h>
#include <ckit/string.h>
#include <ckit/parser.h>
#include <ckit/config.h>
#include <ckit/utils.h>

typedef struct {
    int         width;
    int         precision;
    bool        setWidth;
    bool        setPrecision;
    bool        alt;
    bool        zero;
    bool        plus;
    bool        minus;
    bool        space;
    bool        mayus;
    bool        lengthL;
    bool        lengthZ;

    char        ch;
    const char* str;
    size_t      length;
} CkitSpec;

typedef struct {
    CkitSpec        spec;
    va_list         args;
    CkitParser*     parser;
    
    bool            ansi;
    bool            reset;
    bool            enabled;
} CkitFormat;

static thread_local char slot[CKIT_ANSI_FORMAT_BUFFER_SIZE] = {0};
static thread_local char slots[CKIT_ANSI_FORMAT_BUFFER_SIZE][CKIT_FORMAT_ANSI_SLOT_BUFFER_COUNT] = {0};

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint32_t x32;
typedef uint64_t x64;
typedef float f32;
typedef double f64;
typedef void* ptr;
typedef size_t size;
typedef ptrdiff_t diff;

#define parsetype(type, ...) do { type value = va_arg(f->args, type); f->spec.str = CONCAT2(type, str)(value, ##__VA_ARGS__);} while(0)
static int nextint(void* userdata) {
    return va_arg(((CkitFormat*)userdata)->args, int);
}

static void ppflag(CkitFormat* f) {
    f->spec.ch = '%';
    f->spec.str = &f->spec.ch;
    f->spec.length = 1;
}
static void cflag(CkitFormat* f) {
    f->spec.ch = (char)va_arg(f->args, int);
    f->spec.str = &f->spec.ch;
    f->spec.length = 1;
}
static void sflag(CkitFormat* f) {
    f->spec.str = va_arg(f->args, const char*);
    if (!f->spec.str) { f->spec.str = "(null)"; }
    f->spec.length = strlen(f->spec.str);
    if (f->spec.setPrecision) { SETMIN(f->spec.length, (size_t)f->spec.precision); }
}
static void iflag(CkitFormat* f) {
    if (f->spec.lengthZ) { parsetype(diff); } 
    else if (f->spec.lengthL) { parsetype(i64, f->spec.plus, f->spec.space); } 
    else { parsetype(i32, f->spec.plus, f->spec.space); }
    f->spec.length = strlen(f->spec.str);
}
static void uflag(CkitFormat* f) {
    if (f->spec.lengthZ) { parsetype(size); } 
    else if (f->spec.lengthL) { parsetype(u64, f->spec.plus, f->spec.space); } 
    else { parsetype(u32, f->spec.plus, f->spec.space); }
    f->spec.length = strlen(f->spec.str);
}
static void xflag(CkitFormat* f) {
    if (f->spec.lengthZ || f->spec.lengthL) { parsetype(x64, f->spec.mayus); } 
    else { parsetype(x32, true); }
    f->spec.length = strlen(f->spec.str);
}
static void fflag(CkitFormat* f) {
    if (!f->spec.setPrecision) { f->spec.precision = CKIT_FORMAT_DEFAULT_PRECISION; }
    parsetype(f64, f->spec.precision, f->spec.plus, f->spec.space);
    f->spec.length = strlen(f->spec.str);
}
static void nflag(CkitFormat* f) {
    f->spec.str = "";
    f->spec.length = 0;
    size_t* n = (size_t*)va_arg(f->args, void*);
    *n = f->parser->written;
}
static void pflag(CkitFormat* f) {
    parsetype(ptr);
    f->spec.length = strlen(f->spec.str);
}
static void aflag(CkitFormat* f) {
    if (f->spec.zero) { 
        f->spec.str = ANSI_RESET;
        f->spec.length = STRLEN_LITERAL(ANSI_RESET);
        return; 
    }

    if (f->spec.minus) { f->reset = true; }
    if (!(f->spec.plus && f->spec.alt) || f->spec.lengthL) { f->ansi = true; }

    bool saveOnly = false;
    if (f->spec.lengthL) {
        f->spec.alt = false;
        f->spec.str = TERNARY(f->spec.setPrecision, slots[MIN(f->spec.precision, CKIT_FORMAT_ANSI_SLOT_BUFFER_COUNT - 1)], slot);
        f->spec.length = strlen(f->spec.str);
        if (!f->spec.plus) { return; }
        saveOnly = true;
    }

    const char* script = ansiformat(va_arg(f->args, const char*), f, nextint);
    size_t len = strlen(script);
    if (f->spec.plus) {
        char* dest = TERNARY(f->spec.setPrecision, slots[MIN(f->spec.precision, CKIT_FORMAT_ANSI_SLOT_BUFFER_COUNT - 1)], slot);
        strcpy_s(dest, CKIT_ANSI_FORMAT_BUFFER_SIZE, script);
        if (saveOnly || f->spec.alt) { return; }
    }
    f->spec.str = script;
    f->spec.length = len;
}

static void (*formatters[256])(CkitFormat*) = {
    [0 ... '%' - 1] = NULL,
    ['%' + 1 ... 'a' - 1] = NULL,
    ['a' + 1 ... 'c' - 1] = NULL,
    ['d' + 1 ... 'f' - 1] = NULL,
    ['f' + 1 ... 'i' - 1] = NULL,
    ['i' + 1 ... 'n' - 1] = NULL,
    ['n' + 1 ... 'p' - 1] = NULL,
    ['p' + 1 ... 's' - 1] = NULL,
    ['s' + 1 ... 'u' - 1] = NULL,
    ['u' + 1 ... 'x' - 1] = NULL,
    ['x' + 1 ... 255] = NULL,
    ['%'] = ppflag,
    ['a'] = aflag,
    ['c'] = cflag,
    ['d'] = iflag,
    ['f'] = fflag,
    ['i'] = iflag,
    ['n'] = nflag,
    ['p'] = pflag,
    ['s'] = sflag,
    ['u'] = uflag,
    ['x'] = xflag
};

#define calcpad(f)  (f->spec.width - f->spec.length)
#define leftpad(f)  (f->spec.setWidth && f->spec.length < (size_t)f->spec.width && !f->spec.minus)
#define rightpad(f) (f->spec.setWidth && f->spec.length < (size_t)f->spec.width && f->spec.minus)
static void commit(CkitFormat* f) {
    if (leftpad(f)) { ckit_parser_fill(f->parser, ' ', calcpad(f)); }
    ckit_parser_write(f->parser, f->spec.str, f->spec.length);
    if (rightpad(f)) { ckit_parser_fill(f->parser, ' ', calcpad(f)); }
    f->spec.str = "";
    f->spec.length = 0;
}

static void parseflags(CkitFormat* f) {
    while (true) {
        if (ckit_parser_eqadvance(f->parser, '#')) { f->spec.alt = true; }
        else if (ckit_parser_eqadvance(f->parser, '0')) { f->spec.zero = true; }
        else if (ckit_parser_eqadvance(f->parser, '+')) { f->spec.plus = true; }
        else if (ckit_parser_eqadvance(f->parser, '-')) { f->spec.minus = true; }
        else if (ckit_parser_eqadvance(f->parser, ' ')) { f->spec.space = true; }
        else { break; }
    }
    if (f->spec.plus) { f->spec.space = false; }
    if (f->spec.minus) { f->spec.zero = false; }
}
static void parsewidth(CkitFormat* f) {
    f->spec.setWidth = ckit_parser_unsigned(f->parser, &f->spec.width);
}
static void parseprecision(CkitFormat* f) {
    if (ckit_parser_eqadvance(f->parser, '.')) { 
        f->spec.setPrecision = ckit_parser_unsigned(f->parser, &f->spec.precision);
    }
}
static void parselength(CkitFormat* f) {
    if (ckit_parser_eqadvance(f->parser, 'z')) {
        f->spec.lengthZ = true;
    } else if (ckit_parser_eqadvance(f->parser, 'l')) {
        ckit_parser_eqadvance(f->parser, 'l');
        f->spec.lengthL = true;
    } else {
        ckit_parser_eqadvance(f->parser, 'h');
        ckit_parser_eqadvance(f->parser, 'h');
    }
}
static char parsespecifier(CkitFormat* f) {
    char c = (char)tolower((unsigned char)*f->parser->source);
    f->spec.mayus = *f->parser->source++ != c;
    return c;
}

size_t vsnformat(char* buf, size_t count, bool enableAnsi, const char* fmt, va_list args) {    
    CkitParser parser = {0};
    parser.buf = buf;
    parser.count = count ? count - 1 : 0;
    parser.written = 0;
    parser.source = fmt;

    CkitFormat f = {0};
    va_copy(f.args, args);
    f.spec = EMPTY(CkitSpec);
    f.parser = &parser;
    f.enabled = enableAnsi;
    parser.nextint = nextint;
    parser.userdata = &f;

    while (true) {
        f.spec = EMPTY(CkitSpec);
        
        if (!ckit_parser_copyuntil(&parser, '%')) {
            ckit_parser_writestr(&parser, parser.source);
            break; 
        }

        if (*parser.source == '\0') { break; }
        parseflags(&f);
        parsewidth(&f);
        parseprecision(&f);
        parselength(&f);

        unsigned char c = parsespecifier(&f);
        void (*formatter)(CkitFormat*) = formatters[c];
        if (!formatter) { continue; }
        formatter(&f);
        if (formatter == aflag && !enableAnsi) { continue; }
        
        commit(&f);
        if (f.reset && c != 'a') { 
            f.reset = false;
            ckit_parser_writestr(&parser, ANSI_RESET);
        }
    }

    va_end(f.args);
    if (f.ansi && enableAnsi) { ckit_parser_writestr(&parser, ANSI_RESET); }
    ckit_parser_terminate(&parser);
    return parser.written;
}