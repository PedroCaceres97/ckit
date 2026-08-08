#ifndef __CKIT_STDIO_H__
#define __CKIT_STDIO_H__

#include <ckit/info.h>
#include <ckit/config.h>

typedef enum {
    ATTRIBUTE_NONE      = 0 << 0,
    ATTRIBUTE_READ      = 1 << 0,
    ATTRIBUTE_WRITE     = 1 << 1,
    ATTRIBUTE_CREATE    = 1 << 2,
    ATTRIBUTE_TRUNCATE  = 1 << 3,
    ATTRIBUTE_IFNEW     = 1 << 4,
    ATTRIBUTE_ANSI      = 1 << 5,
    ATTRIBUTE_ISOPEN    = 1 << 16
} FileAttributes;

typedef enum {
    FSEEK_SET,
    FSEEK_CUR,
    FSEEK_END
} FileSeek;

typedef struct File File;

FileAttributes attributes(int fd);

int countdir(const char* dirpath);
bool copydir(const char* dirpath, const char* destpath);
bool checkdir(const char* dirpath);
bool createdir(const char* dirpath);
bool removedir(const char* dirpath, bool force);
bool renamedir(const char* dirpath, const char* destpath);

bool copyfile(const char* filepath, const char* destpath);
bool checkfile(const char* filepath);
bool createfile(const char* filepath);
bool removefile(const char* filepath);
bool renamefile(const char* filepath, const char* destpath);

int openfile(const char* filepath, FileAttributes attributes);
void closefile(int fd);
size_t dumpfile(const char* filepath, char* buf, size_t max);
size_t readfile(int fd, char* buf, size_t max);
size_t writefile(int fd, const char* buf, size_t max);
size_t printfile(int fd, const char* buf);
size_t sizefile(int fd);
size_t tellfile(int fd);
size_t seekfile(int fd, ptrdiff_t offset, FileSeek origin);

const char* format(const char* fmt, ...);
size_t fformat(int fd, const char* fmt, ...);
size_t sformat(char* buf, const char* fmt, ...);
size_t snformat(char* buf, size_t max, bool enableAnsi, const char* fmt, ...);
size_t vsnformat(char* buf, size_t max, bool enableAnsi, const char* fmt, va_list args);

typedef enum {
    PRINT_INFO,
    PRINT_DEBUG,
    PRINT_SUCCESS,
    PRINT_WARNING,
    PRINT_ERROR,
    PRINT_FATAL,
    PRINT_WINAPI = 16,
    PRINT_POSIX = 32
} PrintLog;

void print(const char* buf);
void printlog(PrintLog level, const char* fmt, ...);
void printinform(Context context, PrintLog level, const char* fmt, ...);
void printformat(const char* fmt, ...);

void flushinput();
size_t readinput(char* buf, size_t max);
const char* getinput();

#endif /* __CKIT_STDIO_H__ */