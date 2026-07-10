#ifndef __CKIT_PARSER_H__
#define __CKIT_PARSER_H__

#include <ckit/info.h>

// written = total virtual output size,
// not necessarily bytes copied into buf
typedef struct {
    char*       buf;
    size_t      count;
    size_t      written;
    const char* source;
    void*       userdata;
    int         (*nextint)(void*);
} CkitParser;

void ckit_parser_fill(CkitParser* parser, char ch, size_t count);
void ckit_parser_write(CkitParser* parser, const char* src, size_t count);
void ckit_parser_writestr(CkitParser* parser, const char* src);
void ckit_parser_terminate(CkitParser* parser);

void ckit_parser_skipspace(CkitParser* parser);
bool ckit_parser_eqadvance(CkitParser* parser, char ch);
bool ckit_parser_lowadvance(CkitParser* parser, char ch);
bool ckit_parser_copyuntil(CkitParser* parser, char ch);

bool ckit_parser_unsigned(CkitParser* parser, int* value);
bool ckit_parser_nextunsigned(CkitParser* parser, int* value);

#endif /* __CKIT_PARSER_H__ */