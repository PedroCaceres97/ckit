#include <ckit/info.h>

#ifdef INFO_OS_WINDOWS

#include <io.h>
#include <direct.h>
#include <windows.h>
#include <sys/stat.h>

#include <ckit/stdio.h>
#include <ckit/error.h>
#include <ckit/utils.h>
#include <ckit/config.h>
#include <ckit/symbols.h>

#define CKIT_LONGPATH_BUFFER_COUNT  2
#define CKIT_LONGPATH_BUFFER_SIZE   4096

#define CKIT_LASTERROR_BUFFER_COUNT  2
#define CKIT_LASTERROR_BUFFER_SIZE   2048

CKIT_BUFFERS(longpath, wchar_t, CKIT_LONGPATH_BUFFER_COUNT, CKIT_LONGPATH_BUFFER_SIZE)
CKIT_BUFFERS(lasterror, char, CKIT_LASTERROR_BUFFER_COUNT, CKIT_LASTERROR_BUFFER_SIZE)

#define win_ithrow() ithrow(ERROR_HARD_OS | ERROR_WINAPI, lasterror())

#define win_vthrowif(cond) vthrowif(cond, ERROR_HARD_OS | ERROR_WINAPI, lasterror())
#define win_zthrowif(cond) zthrowif(cond, ERROR_HARD_OS | ERROR_WINAPI, lasterror())
#define win_ithrowif(cond) ithrowif(cond, ERROR_HARD_OS | ERROR_WINAPI, lasterror())
#define win_nthrowif(cond) nthrowif(cond, ERROR_HARD_OS | ERROR_WINAPI, lasterror())
#define win_gthrowif(gotof, cond) gthrowif(gotof, cond, ERROR_HARD_OS | ERROR_WINAPI, lasterror())
#define win_rthrowif(rvalue, cond) rthrowif(rvalue, cond, ERROR_HARD_OS | ERROR_WINAPI, lasterror())

struct File {
    HANDLE          handle;
    FileAttributes  attributes;
};

static DWORD stdout_mode = 0;
static DWORD stderr_mode = 0;

#define FILES_COUNT 256
#define FILES_STACK FILES_COUNT - 3

static int back = FILES_STACK;
static int stack[FILES_STACK] = { 255, 254, 253, 252, 251, 250, 249,
    248, 247, 246, 245, 244, 243, 242, 241, 240, 239,
    238, 237, 236, 235, 234, 233, 232, 231, 230, 229,
    228, 227, 226, 225, 224, 223, 222, 221, 220, 219,
    218, 217, 216, 215, 214, 213, 212, 211, 210, 209,
    208, 207, 206, 205, 204, 203, 202, 201, 200, 199,
    198, 197, 196, 195, 194, 193, 192, 191, 190, 189,
    188, 187, 186, 185, 184, 183, 182, 181, 180, 179,
    178, 177, 176, 175, 174, 173, 172, 171, 170, 169,
    168, 167, 166, 165, 164, 163, 162, 161, 160, 159,
    158, 157, 156, 155, 154, 153, 152, 151, 150, 149,
    148, 147, 146, 145, 144, 143, 142, 141, 140, 139,
    138, 137, 136, 135, 134, 133, 132, 131, 130, 129,
    128, 127, 126, 125, 124, 123, 122, 121, 120, 119,
    118, 117, 116, 115, 114, 113, 112, 111, 110, 109,
    108, 107, 106, 105, 104, 103, 102, 101, 100, 99,
    98, 97, 96, 95, 94, 93, 92, 91, 90, 89,
    88, 87, 86, 85, 84, 83, 82, 81, 80, 79,
    78, 77, 76, 75, 74, 73, 72, 71, 70, 69,
    68, 67, 66, 65, 64, 63, 62, 61, 60, 59,
    58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
    48, 47, 46, 45, 44, 43, 42, 41, 40, 39,
    38, 37, 36, 35, 34, 33, 32, 31, 30, 29,
    28, 27, 26, 25, 24, 23, 22, 21, 20, 19,
    18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
    8, 7, 6, 5, 4, 3
};
static File files[FILES_COUNT] = {
    [3 ... 255] = {0},
    [0] = { .handle = INVALID_HANDLE_VALUE, .attributes = ATTRIBUTE_READ },
    [1] = { .handle = INVALID_HANDLE_VALUE, .attributes = ATTRIBUTE_WRITE },
    [2] = { .handle = INVALID_HANDLE_VALUE, .attributes = ATTRIBUTE_WRITE }
};

void ckit_init() {
#if CKIT_QUIT_ATEXIT
    atexit(ckit_quit);
#endif
    files[0].handle = GetStdHandle(STD_INPUT_HANDLE);
    files[1].handle = GetStdHandle(STD_OUTPUT_HANDLE);
    files[2].handle = GetStdHandle(STD_ERROR_HANDLE);
    vthrowif(files[0].handle == INVALID_HANDLE_VALUE, ERROR_STDIN_INVALID, ERRMSG_STDIN_INVALID);
    vthrowif(files[1].handle == INVALID_HANDLE_VALUE, ERROR_STDOUT_INVALID, ERRMSG_STDOUT_INVALID);
    vthrowif(files[2].handle == INVALID_HANDLE_VALUE, ERROR_STDERR_INVALID, ERRMSG_STDERR_INVALID);
    files[0].attributes |= ATTRIBUTE_ISOPEN;
    files[1].attributes |= ATTRIBUTE_ISOPEN;
    files[2].attributes |= ATTRIBUTE_ISOPEN;
#if !CKIT_DONT_ENABLE_ANSI_FOR_WINDOWS
    vthrowif(!GetConsoleMode(files[1].handle, &stdout_mode), ERROR_STDOUT_MODE, ERRMSG_GET_MODE);
    vthrowif(!GetConsoleMode(files[2].handle, &stderr_mode), ERROR_STDERR_MODE, ERRMSG_GET_MODE);
    vthrowif(!SetConsoleMode(files[1].handle, stdout_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT), ERROR_STDOUT_VT, ERRMSG_ENABLE_VT);
    vthrowif(!SetConsoleMode(files[2].handle, stderr_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT), ERROR_STDERR_VT, ERRMSG_ENABLE_VT);
    files[1].attributes |= ATTRIBUTE_ANSI;
    files[2].attributes |= ATTRIBUTE_ANSI;
#endif
}
void ckit_quit() {
    throwif(back < FILES_STACK, ERROR_USER_UNCLOSED, ERRMSG_UNCLOSED);
    SetConsoleMode(files[1].handle, stdout_mode);
    SetConsoleMode(files[2].handle, stderr_mode);
    files[0].handle = INVALID_HANDLE_VALUE;
    files[1].handle = INVALID_HANDLE_VALUE;
    files[2].handle = INVALID_HANDLE_VALUE;
    files[0].attributes &= ~ATTRIBUTE_ISOPEN;
    files[1].attributes &= ~(ATTRIBUTE_ISOPEN | ATTRIBUTE_ANSI);
    files[2].attributes &= ~(ATTRIBUTE_ISOPEN | ATTRIBUTE_ANSI);
#if !CKIT_DONT_ENABLE_ANSI_FOR_WINDOWS
    SetConsoleMode(files[1].handle, stdout_mode);
    SetConsoleMode(files[2].handle, stderr_mode);
#endif
}

#if CKIT_INIT_PREMAIN
PREMAIN(premain) {
    ckit_init();
}
#endif

static const char* lasterror() {
    DWORD code = GetLastError();
    char* buffer = ckit_lasterror_next();
    bool formatted = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, CKIT_LASTERROR_BUFFER_SIZE, NULL);
    if (!formatted) { strcpy(buffer, "Failed to format windows error"); return buffer; }
     
    char* end = buffer;
    while (*end) { end++; }
    while (end > buffer && *end <= 32) { *end-- = 0; };
    return buffer;
}
static wchar_t* longpath(const char* path) {
    wchar_t utf16[CKIT_LONGPATH_BUFFER_SIZE] = {0};
    int result = MultiByteToWideChar(CP_UTF8, 0, path, -1, utf16, CKIT_LONGPATH_BUFFER_SIZE);
    nthrowif(!result, ERROR_HARD_OS | ERROR_INVALID_PATH, lasterror());

    wchar_t* buffer = ckit_longpath_next();
    wmemset(buffer, 0, CKIT_LONGPATH_BUFFER_SIZE);

    wchar_t* cursor = buffer;
    *cursor++ = L'\\';
    *cursor++ = L'\\';
    *cursor++ = L'?';
    *cursor++ = L'\\';
    DWORD len = GetFullPathNameW(utf16, CKIT_LONGPATH_BUFFER_SIZE - 4, cursor, NULL);
    nthrowif(len == 0 || len > CKIT_LONGPATH_BUFFER_SIZE, ERROR_HARD_OS | ERROR_INVALID_PATH, lasterror());
    return buffer;
}

static bool isdotfolder(wchar_t* pathname) {
    return !wcscmp(pathname, L".") || !wcscmp(pathname, L"..");
}

static int countlongdir(wchar_t* dirpath) {
    size_t len = wcslen(dirpath);
    dirpath[len] = L'\\';
    dirpath[len + 1] = L'*';
    dirpath[len + 2] = 0;

    WIN32_FIND_DATAW data = {0};
    HANDLE first = FindFirstFileW(dirpath, &data);
    if (first == INVALID_HANDLE_VALUE) {
        win_gthrowif(failure, GetLastError() != ERROR_FILE_NOT_FOUND);
        goto success;
    }

    int count = 0;
    do { 
        if (isdotfolder(data.cFileName)) { continue; }
        count++;
    } while(FindNextFileW(first, &data));
    win_gthrowif(failure, GetLastError() != ERROR_NO_MORE_FILES);
    
success:
    dirpath[len] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first)); }
    return count;

failure:
    dirpath[len] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first)); }
    return -1;
}
static bool copylongdir(wchar_t* dirpath, wchar_t* destpath) {
    size_t lendir = wcslen(dirpath);
    size_t lendest = wcslen(destpath);

    dirpath[lendir] = L'\\';
    dirpath[lendir + 1] = L'*';
    dirpath[lendir + 2] = 0;
    WIN32_FIND_DATAW data = {0};
    HANDLE first = FindFirstFileW(dirpath, &data);
    if (first == INVALID_HANDLE_VALUE) {
        win_gthrowif(failure, GetLastError() != ERROR_FILE_NOT_FOUND);
        goto success;
    }

    dirpath[lendir] = 0;
    dirpath[lendir + 1] = 0;

    size_t i = 0;
    size_t j = 0;
    for (i = 0; dirpath[lendir - i] != L'\\'; i++);
    for (j = 0; i > 0; j++) { destpath[lendest + j] = dirpath[lendir - i--]; }
    win_gthrowif(failure, !CreateDirectoryW(destpath, NULL));

    do { 
        if (isdotfolder(data.cFileName)) { continue; }
        dirpath[lendir] = '\\';
        dirpath[lendir + 1] = 0;
        destpath[lendest + j] = '\\';
        destpath[lendest + j + 1] = 0;
        wcscpy(&dirpath[lendir + 1], data.cFileName);
        wcscpy(&destpath[lendest + j + 1], data.cFileName);
        bool isdir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT);
        if (isdir) { if (!copylongdir(dirpath, destpath)) { goto failure; } }
        else { win_gthrowif(failure, !CopyFileW(dirpath, destpath, true)); }
    } while(FindNextFileW(first, &data));
    win_gthrowif(failure, GetLastError() != ERROR_NO_MORE_FILES);

success:
    dirpath[lendir] = 0;
    destpath[lendest] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first)); }
    return true;
    
failure:
    dirpath[lendir] = 0;
    destpath[lendest] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first)); }
    return false;
}
static bool checklongdir(wchar_t* dirpath) {
    DWORD attributes = GetFileAttributesW(dirpath);
    if (attributes == INVALID_FILE_ATTRIBUTES) { return false; }
    return attributes & FILE_ATTRIBUTE_DIRECTORY;
}
static bool createlongdir(wchar_t* dirpath) {
    // Skip "\\\\?\\C:\\"
    WCHAR* next = &dirpath[7];
    while (*next) {
        if (*next != L'\\' && *next != L'/') { next++; continue; }
        *next = L'\0';
        BOOL result = CreateDirectoryW(dirpath, NULL);
        win_zthrowif(!result && GetLastError() != ERROR_ALREADY_EXISTS);
        *next++ = L'\\';
    }
    BOOL result = CreateDirectoryW(dirpath, NULL);
    win_zthrowif(!result && GetLastError() != ERROR_ALREADY_EXISTS);
    return true;
}
static bool removelongdir(wchar_t* dirpath, bool force) {
    size_t len = wcslen(dirpath);
    dirpath[len] = L'\\';
    dirpath[len + 1] = L'*';
    dirpath[len + 2] = 0;

    WIN32_FIND_DATAW data = {0};
    HANDLE first = FindFirstFileW(dirpath, &data);
    if (first == INVALID_HANDLE_VALUE) {
        win_gthrowif(failure, GetLastError() != ERROR_FILE_NOT_FOUND);
        goto success;
    }

    do { 
        if (isdotfolder(data.cFileName)) { continue; }
        if (!force) { goto failure; }
        dirpath[len] = '\\';
        dirpath[len + 1] = 0;
        wcscpy(&dirpath[len + 1], data.cFileName);
        bool isdir = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        if (isdir) { if (!removelongdir(dirpath, true)) { goto failure; } }
        else { win_gthrowif(failure, !DeleteFileW(dirpath)); }
    } while(FindNextFileW(first, &data));
    win_gthrowif(failure, GetLastError() != ERROR_NO_MORE_FILES);
    
success:
    dirpath[len] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first)); }
    win_zthrowif(!RemoveDirectoryW(dirpath));
    return true;

failure:
    dirpath[len] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first)); }
    return false;
}

static bool checklongfile(wchar_t* filepath) {
    DWORD attributes = GetFileAttributesW(filepath);
    if (attributes == INVALID_FILE_ATTRIBUTES) { return false; }
    return !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

FileAttributes attributes(int fd) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    return files[fd].attributes;
}

int countdir(const char* dirpath) {
    ithrowif(!dirpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(dirpath));
    wchar_t* path = longpath(dirpath);
    return countlongdir(path);
}
bool copydir(const char* dirpath, const char* destpath) {
    zthrowif(!dirpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    zthrowif(!destpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(destpath));
    
    wchar_t* longdir = longpath(dirpath);
    wchar_t* longdest = longpath(destpath);
    return copylongdir(longdir, longdest);
}
bool checkdir(const char* dirpath) {
    zthrowif(!dirpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(dirpath));
    wchar_t* path = longpath(dirpath);
    return checklongdir(path);
}
bool createdir(const char* dirpath) {
    zthrowif(!dirpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(dirpath));
    wchar_t* path = longpath(dirpath);
    return createlongdir(path);
}
bool removedir(const char* dirpath, bool force) {
    zthrowif(!dirpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(dirpath));
    wchar_t* path = longpath(dirpath);
    return removelongdir(path, force);
}
bool renamedir(const char* dirpath, const char* destpath) { 
    zthrowif(!dirpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    zthrowif(!destpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(destpath));
    
    wchar_t* longdir = longpath(dirpath);
    wchar_t* longdest = longpath(destpath);
    zthrowif(checklongdir(longdest), ERROR_USER_EXISTING, ERRMSG_RENAMEDIR_REPLACE);
    zthrowif(!checklongdir(longdir), ERROR_USER_UNEXISTING, ERRMSG_FILE_DOESNT_EXIST);
    win_zthrowif(!MoveFileW(longdir, longdest));
    return true;
}

bool copyfile(const char* filepath, const char* destpath) {
    zthrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    zthrowif(!destpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(destpath));
    
    wchar_t* longfile = longpath(filepath);
    wchar_t* longdest = longpath(destpath);
    size_t lendir = wcslen(longfile);
    size_t lendest = wcslen(longdest);

    size_t i = 0;
    size_t j = 0;
    for (i = 0; longfile[lendir - i] != L'\\'; i++);
    for (j = 0; i > 0; j++) { longdest[lendest + j] = longfile[lendir - i--]; }
    longdest[lendest + j] = 0;

    zthrowif(checklongfile(longdest), ERROR_USER_EXISTING, ERRMSG_COPYFILE_REPLACE);
    zthrowif(!checklongfile(longfile), ERROR_USER_UNEXISTING, ERRMSG_FILE_DOESNT_EXIST);
    win_zthrowif(!CopyFileW(longfile, longdest, true));
    return true;
}
bool checkfile(const char* filepath) {
    zthrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));

    wchar_t* path = longpath(filepath);
    return checklongfile(path);
}
bool createfile(const char* filepath) {
    zthrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));

    DWORD access = GENERIC_READ;
    DWORD creation = CREATE_ALWAYS;
    wchar_t* path = longpath(filepath);
    HANDLE file = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    win_zthrowif(file == INVALID_HANDLE_VALUE);
    win_zthrowif(!CloseHandle(file));
    return true;
}
bool removefile(const char* filepath) {
    zthrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    
    wchar_t* path = longpath(filepath);
    win_zthrowif(!DeleteFileW(path) && GetLastError() != ERROR_FILE_NOT_FOUND);
    return true;
}
bool renamefile(const char* filepath, const char* destpath) {
    zthrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    zthrowif(!destpath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(destpath));
    
    wchar_t* longfile = longpath(filepath);
    wchar_t* longdest = longpath(destpath);
    zthrowif(checklongfile(longdest), ERROR_USER_EXISTING, ERRMSG_RENAMEFILE_REPLACE);
    zthrowif(!checklongfile(longfile), ERROR_USER_UNEXISTING, ERRMSG_FILE_DOESNT_EXIST);
    win_zthrowif(!MoveFileW(longfile, longdest));
    return true;
}

int openfile(const char* filepath, FileAttributes attributes) {
    ithrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    
    ithrowif(back == 0, ERROR_HARD_USER | ERROR_MAX_REACHED, ERRMSG_FILES_MAX_REACHED);
    int fd = stack[--back];
    stack[back] = 0;
    files[fd].attributes = attributes & ~ATTRIBUTE_ISOPEN;

    DWORD access = 0;
    DWORD creation = OPEN_EXISTING;

    if (attributes & ATTRIBUTE_READ) { access |= GENERIC_READ; }
    if (attributes & ATTRIBUTE_WRITE) { access |= GENERIC_WRITE; }
    if (attributes & ATTRIBUTE_CREATE) { creation = CREATE_NEW; }
    if (attributes & ATTRIBUTE_TRUNCATE) { creation = CREATE_ALWAYS; attributes &= ~ATTRIBUTE_CREATE; }

    wchar_t* path = longpath(filepath);
    files[fd].handle = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    if (GetLastError() == ERROR_ALREADY_EXISTS && attributes & ATTRIBUTE_IFNEW) { return fd; }
    if (files[fd].handle == INVALID_HANDLE_VALUE) {
        stack[back++] = fd;
        win_ithrow();
        return -1;
    }
    files[fd].attributes |= ATTRIBUTE_ISOPEN;
    return fd;
}
void closefile(int fd) {
    vthrowif(fd == 0 || fd == 1 || fd == 2, ERROR_USER_STDCLOSE, ERRMSG_STDCLOSE);
    vthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    vthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    
    if (files[fd].attributes & ATTRIBUTE_ISOPEN) { win_vthrowif(!CloseHandle(files[fd].handle)); }
    files[fd].attributes = 0;
    stack[back++] = fd;
}
size_t dumpfile(const char* filepath, char* buf, size_t max) {
    ithrowif(!filepath, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(filepath));
    ithrowif(!buf, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(buf));
    if (max == 0) { return 0; }

    DWORD access = GENERIC_READ;
    DWORD creation = CREATE_ALWAYS;
    wchar_t* path = longpath(filepath);
    HANDLE file = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    win_zthrowif(file == INVALID_HANDLE_VALUE);
    
    DWORD readed = 0;
    win_zthrowif(!ReadFile(file, buf, max - 1, &readed, NULL));
    win_zthrowif(!CloseHandle(file));
    buf[readed] = '\0';
    return (size_t)readed;
}
size_t readfile(int fd, char* buf, size_t max) {
    zthrowif(!buf, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(buf));
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    zthrowif(!(files[fd].attributes & ATTRIBUTE_READ), ERROR_USER_UNREADABLE, ERRMSG_UNREADABLE);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN) || !max) { return 0; }

    DWORD readed = 0;
    win_zthrowif(!ReadFile(files[fd].handle, buf, max - 1, &readed, NULL));
    buf[readed] = '\0';
    return (size_t)readed;
}
size_t writefile(int fd, const char* buf, size_t max) {
    zthrowif(!buf, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(buf));
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    zthrowif(!(files[fd].attributes & ATTRIBUTE_WRITE), ERROR_USER_UNWRITEABLE, ERRMSG_UNWRITEABLE);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN) || !max) { return 0; }

    DWORD written = 0;
    win_zthrowif(!WriteFile(files[fd].handle, buf, max, &written, NULL));
    return (size_t)written;
}
size_t printfile(int fd, const char* buf) {
    zthrowif(!buf, ERROR_USER_NULLPTR, ERRMSG_NULLPTR(buf));
    return writefile(fd, buf, strlen(buf));
}
size_t sizefile(int fd) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN)) { return 0; }

    LARGE_INTEGER LISize = {0};
    win_zthrowif(!GetFileSizeEx(files[fd].handle, &LISize));
    return (size_t)LISize.QuadPart;
}
size_t tellfile(int fd) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN)) { return 0; }

    LARGE_INTEGER LINew = {0};
    LARGE_INTEGER LIOffset = {0};
    win_zthrowif(SetFilePointerEx(files[fd].handle, LIOffset, &LINew, SEEK_CUR));
    return (size_t)LINew.QuadPart;
}
size_t seekfile(int fd, ptrdiff_t offset, FileSeek origin) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_USER_INVALID_FD, ERRMSG_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_USER_UNOPENED, ERRMSG_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN)) { return 0; }

    // DWORD method = 0;
    // switch (origin) {
    //     case FSEEK_SET: method = FILE_BEGIN;   break;
    //     case FSEEK_CUR: method = FILE_CURRENT; break;
    //     case FSEEK_END: method = FILE_END;     break;
    // }
    DWORD method = origin;
    LARGE_INTEGER LINew = {0};
    LARGE_INTEGER LIOffset = {0};
    LIOffset.QuadPart = offset;
    win_zthrowif(!SetFilePointerEx(files[fd].handle, LIOffset, &LINew, method));
    return (size_t)LINew.QuadPart;
}

void flushinput() {
    FlushConsoleInputBuffer(files[0].handle);
}

#endif /* INFO_OS_WINDOWS */