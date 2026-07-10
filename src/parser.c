#include <ckit/parser.h>
#include <ckit/utils.h>

static inline bool writable(CkitParser* parser) {
    return parser->written < parser->count && parser->buf != NULL;
}
static inline char* position(CkitParser* parser) {
    return &parser->buf[parser->written];
}
static inline size_t available(CkitParser* parser, size_t count) {
    return MIN(count, parser->count - parser->written);
}

static inline char lower(char ch) {
    return (ch >= 'A' && ch <= 'Z') ? ch + 32 : ch;
}

void ckit_parser_fill(CkitParser* parser, char ch, size_t count) {
    if (count == 0) { return; }
    if (writable(parser)) { memset(position(parser), ch, available(parser, count)); }
    parser->written += count;
}
void ckit_parser_write(CkitParser* parser, const char* src, size_t count) {
    if (!src) { return; }
    if (writable(parser)) { memcpy(position(parser), src, available(parser, count)); }
    parser->written += count;
}
void ckit_parser_writestr(CkitParser* parser, const char* src) {
    if (src == NULL) { return; }
    ckit_parser_write(parser, src, strlen(src));
}
void ckit_parser_terminate(CkitParser* parser) {
    parser->count++;
    if (writable(parser)) { *position(parser) = '\0'; }
}

void ckit_parser_skipspace(CkitParser* parser) {
    while (isspace((unsigned char)*parser->source)) { parser->source++; }
}
bool ckit_parser_eqadvance(CkitParser* parser, char ch) {
    if (*parser->source == ch) {
        parser->source++;
        return true;
    }
    return false;
}
bool ckit_parser_lowadvance(CkitParser* parser, char ch) {
    if (lower(*parser->source) == lower(ch)) {
        parser->source++;
        return true;
    }
    return false;
}
bool ckit_parser_copyuntil(CkitParser* parser, char ch) {
    const char* until = strchr(parser->source, ch);
    if (until == NULL) { return false; }
    if (until != parser->source) { ckit_parser_write(parser, parser->source, PTRDST(until, parser->source)); }
    parser->source = until + 1;
    return true;
}

bool ckit_parser_unsigned(CkitParser* parser, int* value) {
    *value = 0;
    if (ckit_parser_eqadvance(parser, '*')) {
        *value = parser->nextint(parser->userdata);
        if (*value < 0) {
            value = 0;
            return false;
        }
        return true;
    }

    if (*parser->source == '-') { while(isdigit((unsigned char)*++parser->source)) {} return false; }
    if (!isdigit((unsigned char)*parser->source)) { return false; }
    do { *value = *value * 10 + (*parser->source - '0'); } while(isdigit((unsigned char)*++parser->source));
    return true;
}
bool ckit_parser_nextunsigned(CkitParser* parser, int* value) {
    ckit_parser_skipspace(parser);
    if (ckit_parser_eqadvance(parser, ',')) {
        ckit_parser_skipspace(parser);
        ckit_parser_unsigned(parser, value);
        return true;
    }

    return false;
}