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

#define win_vthrowif(cond, code) vthrowif(cond, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | code, lasterror())
#define win_zthrowif(cond, code) zthrowif(cond, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | code, lasterror())
#define win_ithrowif(cond, code) ithrowif(cond, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | code, lasterror())
#define win_nthrowif(cond, code) nthrowif(cond, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | code, lasterror())
#define win_gthrowif(gotof, cond, code) gthrowif(gotof, cond, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | code, lasterror())
#define win_rthrowif(rvalue, cond, code) rthrowif(rvalue, cond, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | code, lasterror())

struct File {
    HANDLE          handle;
    FileAttributes  attributes;
};

static DWORD stdout_mode = 0;
static DWORD stderr_mode = 0;

#define FILES_COUNT 256
#define FILES_STACK FILES_COUNT - 3

static int back = FILES_STACK;
static bool stack[FILES_STACK] = { 255, 254, 253, 252, 251, 250, 249,
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

static void winckit_quit() {
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
    throwif(back < FILES_STACK, ERROR_WIN_FILES_UNCLOSED, CKIT_ERROR_FILES_UNCLOSED);
}
PREMAIN(winckit_init) {
    atexit(winckit_quit);
    files[0].handle = GetStdHandle(STD_INPUT_HANDLE);
    files[1].handle = GetStdHandle(STD_OUTPUT_HANDLE);
    files[2].handle = GetStdHandle(STD_ERROR_HANDLE);
    vthrowif(files[0].handle == INVALID_HANDLE_VALUE, ERROR_WIN_STDIN_INVALID, CKIT_ERROR_STDIN_INVALID);
    vthrowif(files[1].handle == INVALID_HANDLE_VALUE, ERROR_WIN_STDOUT_INVALID, CKIT_ERROR_STDOUT_INVALID);
    vthrowif(files[2].handle == INVALID_HANDLE_VALUE, ERROR_WIN_STDERR_INVALID, CKIT_ERROR_STDERR_INVALID);
    files[0].attributes |= ATTRIBUTE_ISOPEN;
    files[1].attributes |= ATTRIBUTE_ISOPEN;
    files[2].attributes |= ATTRIBUTE_ISOPEN;
#if !CKIT_DONT_ENABLE_ANSI_FOR_WINDOWS
    vthrowif(!GetConsoleMode(files[1].handle, &stdout_mode), ERROR_WIN_STDOUT_MODE, CKIT_ERROR_GET_MODE);
    vthrowif(!GetConsoleMode(files[2].handle, &stderr_mode), ERROR_WIN_STDERR_MODE, CKIT_ERROR_GET_MODE);
    vthrowif(!SetConsoleMode(files[1].handle, stdout_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT), ERROR_WIN_STDOUT_VT, CKIT_ERROR_ENABLE_VT);
    vthrowif(!SetConsoleMode(files[2].handle, stderr_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT), ERROR_WIN_STDERR_VT, CKIT_ERROR_ENABLE_VT);
    files[1].attributes |= ATTRIBUTE_ANSI;
    files[2].attributes |= ATTRIBUTE_ANSI;
#endif
}

static const char* lasterror() {
    DWORD code = GetLastError();
    char* buffer = ckit_lasterror_next();
    bool formatted = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, sizeof(buffer), NULL);
    if (!formatted) { strcpy(buffer, "Failed to format windows error"); }
    return buffer;
}
static wchar_t* longpath(const char* path) {
    wchar_t utf16[CKIT_LONGPATH_BUFFER_SIZE] = {0};
    win_nthrowif(!MultiByteToWideChar(CP_UTF8, 0, path, -1, utf16, CKIT_LONGPATH_BUFFER_SIZE), ERROR_INVALID_PATH);

    wchar_t* buffer = ckit_longpath_next();
    wmemset(buffer, 0, CKIT_LONGPATH_BUFFER_SIZE);

    wchar_t* cursor = buffer;
    *cursor++ = L'\\';
    *cursor++ = L'\\';
    *cursor++ = L'?';
    *cursor++ = L'\\';
    DWORD len = GetFullPathNameW(utf16, CKIT_LONGPATH_BUFFER_SIZE - 4, cursor, NULL);
    win_nthrowif(len == 0 || len > CKIT_LONGPATH_BUFFER_SIZE, ERROR_INVALID_PATH);
    return buffer;
}

static bool isdotfolder(wchar_t* pathname) {
    return !wcscmp(pathname, L".") || !wcscmp(pathname, L"..");
}

static bool longcopydir(wchar_t* dirpath, wchar_t* destpath) {
    size_t lendir = wcslen(dirpath);
    size_t lendest = wcslen(destpath);

    dirpath[lendir] = L'\\';
    dirpath[lendir + 1] = L'*';
    dirpath[lendir + 2] = 0;
    WIN32_FIND_DATAW data = {0};
    HANDLE first = FindFirstFileW(dirpath, &data);
    if (first == INVALID_HANDLE_VALUE) {
        win_gthrowif(failure, GetLastError() != ERROR_FILE_NOT_FOUND, ERROR_WINAPI_FILE | ERROR_COPYDIR);
        goto success;
    }

    dirpath[lendir] = 0;
    dirpath[lendir + 1] = 0;

    size_t i = 0;
    size_t j = 0;
    for (i = 0; dirpath[lendir - i] != L'\\'; i++);
    for (j = 0; i > 0; j++) { destpath[lendest + j] = dirpath[lendir - i--]; }
    win_gthrowif(failure, !CreateDirectoryW(destpath, NULL), ERROR_WINAPI_FILE | ERROR_COPYDIR);

    do { 
        if (isdotfolder(data.cFileName)) { continue; }
        dirpath[lendir] = '\\';
        dirpath[lendir + 1] = 0;
        destpath[lendest + j] = '\\';
        destpath[lendest + j + 1] = 0;
        wcscpy(&dirpath[lendir + 1], data.cFileName);
        wcscpy(&destpath[lendest + j + 1], data.cFileName);
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) { if (!longcopydir(dirpath, destpath)) { goto failure; } }
        else { win_gthrowif(failure, !CopyFileW(dirpath, destpath, true), ERROR_WINAPI_FILE | ERROR_COPYDIR); }
    } while(FindNextFileW(first, &data));
    win_gthrowif(failure, GetLastError() != ERROR_NO_MORE_FILES, ERROR_WINAPI_FILE | ERROR_REMOVEDIR);

success:
    dirpath[lendir] = 0;
    destpath[lendest] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first), ERROR_WINAPI_FILE | ERROR_COPYDIR); }
    return true;
    
failure:
    dirpath[lendir] = 0;
    destpath[lendest] = 0;
    if (first != INVALID_HANDLE_VALUE) { win_zthrowif(!FindClose(first), ERROR_WINAPI_FILE | ERROR_COPYDIR); }
    return false;
}
static bool longcheckdir(wchar_t* dirpath) {
    DWORD attributes = GetFileAttributesW(dirpath);
    if (attributes == INVALID_FILE_ATTRIBUTES) { return false; }
    return attributes & FILE_ATTRIBUTE_DIRECTORY;
}
static bool longremovedir(wchar_t* dirpath, bool force) {
    if (!longcheckdir(dirpath)) { return true; }

    size_t len = wcslen(dirpath);
    dirpath[len] = L'\\';
    dirpath[len + 1] = L'*';
    dirpath[len + 2] = 0;

    WIN32_FIND_DATAW data = {0};
    HANDLE first = FindFirstFileW(dirpath, &data);
    if (first == INVALID_HANDLE_VALUE) {
        win_zthrowif(GetLastError() != ERROR_FILE_NOT_FOUND, ERROR_WINAPI_FILE | ERROR_REMOVEDIR);
        goto success;
    } else if (!force) { 
        goto failure;
    }

    do { 
        if (isdotfolder(data.cFileName)) { continue; }
        dirpath[len] = '\\';
        dirpath[len + 1] = 0;
        wcscpy(&dirpath[len + 1], data.cFileName);
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { if (!longremovedir(dirpath, true)) { goto failure; } }
        else { win_gthrowif(failure, !DeleteFileW(dirpath), ERROR_WINAPI_FILE | ERROR_REMOVEDIR); }
    } while(FindNextFileW(first, &data));
    win_gthrowif(failure, GetLastError() != ERROR_NO_MORE_FILES, ERROR_WINAPI_FILE | ERROR_REMOVEDIR);
    
success:
    dirpath[len] = 0;
    win_zthrowif(!FindClose(first), ERROR_WINAPI_FILE | ERROR_REMOVEDIR);
    win_zthrowif(!RemoveDirectoryW(dirpath), ERROR_WINAPI_FILE | ERROR_REMOVEDIR);
    return true;

failure:
    win_zthrowif(!FindClose(first), ERROR_WINAPI_FILE | ERROR_REMOVEDIR);
    return false;
}

static bool longcheckfile(wchar_t* filepath) {
    DWORD attributes = GetFileAttributesW(filepath);
    if (attributes == INVALID_FILE_ATTRIBUTES) { return false; }
    return !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

FileAttributes attributes(int fd) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD, CKIT_ERROR_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED, CKIT_ERROR_FILE_UNOPENED);
    return files[fd].attributes;
}

bool copydir(const char* dirpath, const char* destpath) {
    zthrowif(!dirpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_COPYDIR, CKIT_ERROR_NULLPTR LITERAL(filepath));
    zthrowif(!destpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_COPYDIR, CKIT_ERROR_NULLPTR LITERAL(destpath));
    
    wchar_t* longdir = longpath(dirpath);
    wchar_t* longdest = longpath(destpath);
    zthrowif(longcheckdir(longdest), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_COPYDIR, CKIT_ERROR_COPYDIR_REPLACE);
    zthrowif(!longcheckdir(longdir), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_COPYDIR, CKIT_ERROR_FILE_DOESNT_EXIST);
    return longcopydir(longdir, longdest);
}
bool checkdir(const char* dirpath) {
    zthrowif(!dirpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_CHECKDIR, CKIT_ERROR_NULLPTR LITERAL(dirpath));
    return longcheckdir(longpath(dirpath));
}
bool createdir(const char* dirpath) {
    zthrowif(!dirpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_CREATEDIR, CKIT_ERROR_NULLPTR LITERAL(dirpath));

    wchar_t* path = longpath(dirpath);
    if (longcheckdir(path)) { return true; }

    // Skip "\\\\?\\C:\\"
    WCHAR* next = &path[7];
    while (*next) {
        if (*next != L'\\' && *next != L'/') { next++; continue; }
        *next = L'\0';
        if (!longcheckdir(path)) {
            win_zthrowif(!CreateDirectoryW(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS, ERROR_WINAPI_FILE | ERROR_CREATEDIR);
        }
        *next++ = L'\\';
    }
    win_zthrowif(!CreateDirectoryW(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS, ERROR_WINAPI_FILE | ERROR_CREATEDIR);
    return true;
}
bool removedir(const char* dirpath, bool force) {
    zthrowif(!dirpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_REMOVEDIR, CKIT_ERROR_NULLPTR LITERAL(dirpath));
    return longremovedir(longpath(dirpath), force);
}
bool renamedir(const char* dirpath, const char* destpath) {
    zthrowif(!dirpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_RENAMEDIR, CKIT_ERROR_NULLPTR LITERAL(filepath));
    zthrowif(!destpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_RENAMEDIR, CKIT_ERROR_NULLPTR LITERAL(destpath));
    
    wchar_t* longdir = longpath(dirpath);
    wchar_t* longdest = longpath(destpath);
    zthrowif(longcheckfile(longdest), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_RENAMEDIR, CKIT_ERROR_RENAMEDIR_REPLACE);
    zthrowif(!longcheckfile(longdir), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_RENAMEDIR, CKIT_ERROR_FILE_DOESNT_EXIST);
    win_zthrowif(!MoveFileW(longdir, longdest), ERROR_WINAPI_FILE | ERROR_RENAMEDIR);
    return true;
}

bool copyfile(const char* filepath, const char* destpath) {
    zthrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_COPYFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));
    zthrowif(!destpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_COPYFILE, CKIT_ERROR_NULLPTR LITERAL(destpath));
    
    wchar_t* longfile = longpath(filepath);
    wchar_t* longdest = longpath(destpath);
    zthrowif(longcheckfile(longdest), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_COPYFILE, CKIT_ERROR_COPYFILE_REPLACE);
    zthrowif(!longcheckfile(longfile), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_COPYFILE, CKIT_ERROR_FILE_DOESNT_EXIST);
    win_zthrowif(!CopyFileW(longfile, longdest, true), ERROR_WINAPI_FILE | ERROR_COPYFILE);
    return true;
}
bool checkfile(const char* filepath) {
    zthrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_CHECKFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));
    return longcheckfile(longpath(filepath));
}
bool createfile(const char* filepath) {
    zthrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_CREATEFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));

    DWORD access = GENERIC_READ;
    DWORD creation = CREATE_ALWAYS;
    wchar_t* path = longpath(filepath);
    HANDLE file = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    win_zthrowif(file == INVALID_HANDLE_VALUE, ERROR_WINAPI_FILE | ERROR_CREATEFILE);
    win_zthrowif(!CloseHandle(file), ERROR_WINAPI_FILE | ERROR_CREATEFILE);
    return true;
}
bool removefile(const char* filepath) {
    zthrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_REMOVEFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));
    
    wchar_t* path = longpath(filepath);
    if (!longcheckfile(path)) { return true; }
    win_zthrowif(!DeleteFileW(path), ERROR_WINAPI_FILE | ERROR_REMOVEFILE);
    return true;
}
bool renamefile(const char* filepath, const char* destpath) {
    zthrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_RENAMEFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));
    zthrowif(!destpath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_RENAMEFILE, CKIT_ERROR_NULLPTR LITERAL(destpath));
    
    wchar_t* longfile = longpath(filepath);
    wchar_t* longdest = longpath(destpath);
    zthrowif(longcheckfile(longdest), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_RENAMEFILE, CKIT_ERROR_RENAMEFILE_REPLACE);
    zthrowif(!longcheckfile(longfile), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_RENAMEFILE, CKIT_ERROR_FILE_DOESNT_EXIST);
    win_zthrowif(!MoveFileW(longfile, longdest), ERROR_WINAPI_FILE | ERROR_RENAMEFILE);
    return true;
}

int openfile(const char* filepath, FileAttributes attributes) {
    ithrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_OPENFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));
    
    ithrowif(back == 0, ERROR_HARD | ERROR_WINDOWS | ERROR_USER | ERROR_MAX_REACHED | ERROR_OPENFILE, CKIT_ERROR_FILES_MAX_REACHED);
    int fd = stack[--back];
    stack[back] = 0;
    files[fd].attributes = attributes & ~ATTRIBUTE_ISOPEN;

    DWORD access = GENERIC_READ;
    DWORD creation = OPEN_EXISTING;

    if (attributes & ATTRIBUTE_WRITE) { access |= GENERIC_WRITE; }
    if (attributes & ATTRIBUTE_CREATE) { creation = CREATE_NEW; }
    if (attributes & ATTRIBUTE_TRUNCATE) { creation = CREATE_ALWAYS; attributes &= ~ATTRIBUTE_CREATE; }

    wchar_t* path = longpath(filepath);
    files[fd].handle = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    if (GetLastError() == ERROR_ALREADY_EXISTS && attributes & ATTRIBUTE_IFNEW) { return fd; }
    win_ithrowif(files[fd].handle == INVALID_HANDLE_VALUE, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | ERROR_WINAPI_FILE | ERROR_OPENFILE);
    files[fd].attributes |= ATTRIBUTE_ISOPEN;
    return fd;
}
void closefile(int fd) {
    vthrowif(fd == 0 || fd == 1 || fd == 2, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_STDCLOSE | ERROR_CLOSEFILE, CKIT_ERROR_CLOSE_STD);
    vthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD | ERROR_CLOSEFILE, CKIT_ERROR_INVALID_FD);
    vthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED | ERROR_CLOSEFILE, CKIT_ERROR_FILE_UNOPENED);
    if (files[fd].attributes & ATTRIBUTE_ISOPEN) { 
        win_vthrowif(!CloseHandle, ERROR_HARD | ERROR_WINDOWS | ERROR_OS | ERROR_WINAPI_FILE | ERROR_CLOSEFILE);
    }
    files[fd].attributes = 0;
    stack[back++] = fd;
}
size_t dumpfile(const char* filepath, char* buf, size_t max) {
    ithrowif(!filepath, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_DUMPFILE, CKIT_ERROR_NULLPTR LITERAL(filepath));
    ithrowif(!buf, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_DUMPFILE, CKIT_ERROR_NULLPTR LITERAL(buf));
    if (max == 0) { return 0; }

    DWORD access = GENERIC_READ;
    DWORD creation = CREATE_ALWAYS;
    wchar_t* path = longpath(filepath);
    HANDLE file = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    win_zthrowif(file == INVALID_HANDLE_VALUE, ERROR_WINAPI_FILE | ERROR_DUMPFILE);
    
    DWORD readed = 0;
    win_zthrowif(!ReadFile(file, buf, max - 1, &readed, NULL), ERROR_WINAPI_FILE | ERROR_DUMPFILE);
    win_zthrowif(!CloseHandle(file), ERROR_WINAPI_FILE | ERROR_DUMPFILE);
    buf[readed] = '\0';
    return (size_t)readed;
}
size_t readfile(int fd, char* buf, size_t max) {
    zthrowif(!buf, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_READFILE, CKIT_ERROR_NULLPTR LITERAL(buf));
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD | ERROR_READFILE, CKIT_ERROR_INVALID_FD);
    zthrowif(!(files[fd].attributes & ATTRIBUTE_READ), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNREADABLE | ERROR_READFILE, CKIT_ERROR_FILE_UNREADABLE);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED | ERROR_READFILE, CKIT_ERROR_FILE_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN) || !max) { return 0; }

    DWORD readed = 0;
    win_zthrowif(!ReadFile(files[fd].handle, buf, max - 1, &readed, NULL), ERROR_WINAPI_FILE | ERROR_READFILE);
    buf[readed] = '\0';
    return (size_t)readed;
}
size_t writefile(int fd, const char* buf, size_t max) {
    zthrowif(!buf, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_WRITEFILE, CKIT_ERROR_NULLPTR LITERAL(buf));
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD | ERROR_WRITEFILE, CKIT_ERROR_INVALID_FD);
    zthrowif(!(files[fd].attributes & ATTRIBUTE_WRITE), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNWRITEABLE | ERROR_WRITEFILE, CKIT_ERROR_FILE_UNWRITEABLE);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED | ERROR_WRITEFILE, CKIT_ERROR_FILE_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN) || !max) { return 0; }

    DWORD written = 0;
    win_zthrowif(!WriteFile(files[fd].handle, buf, max, &written, NULL), ERROR_WINAPI_FILE | ERROR_WRITEFILE);
    return (size_t)written;
}
size_t printfile(int fd, const char* buf) {
    zthrowif(!buf, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_NULLPTR | ERROR_WRITEFILE, CKIT_ERROR_NULLPTR LITERAL(buf));
    return writefile(fd, buf, strlen(buf));
}
size_t sizefile(int fd) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD | ERROR_SIZEFILE, CKIT_ERROR_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED | ERROR_SIZEFILE, CKIT_ERROR_FILE_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN)) { return 0; }

    LARGE_INTEGER LISize = {0};
    win_zthrowif(!GetFileSizeEx(files[fd].handle, &LISize), ERROR_WINAPI_FILE | ERROR_SIZEFILE);
    return (size_t)LISize.QuadPart;
}
size_t tellfile(int fd) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD | ERROR_TELLFILE, CKIT_ERROR_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED | ERROR_TELLFILE, CKIT_ERROR_FILE_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN)) { return 0; }

    LARGE_INTEGER LINew = {0};
    LARGE_INTEGER LIOffset = {0};
    win_zthrowif(SetFilePointerEx(files[fd].handle, LIOffset, &LINew, SEEK_CUR), ERROR_WINAPI_FILE | ERROR_TELLFILE);
    return (size_t)LINew.QuadPart;
}
size_t seekfile(int fd, ptrdiff_t offset, FileSeek origin) {
    zthrowif(fd < 0 || fd >= FILES_COUNT, ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_INVALID_FD | ERROR_SEEKFILE, CKIT_ERROR_INVALID_FD);
    zthrowif(!(files[fd].attributes & (ATTRIBUTE_ISOPEN | ATTRIBUTE_IFNEW)), ERROR_SOFT | ERROR_WINDOWS | ERROR_USER | ERROR_UNOPENED | ERROR_SEEKFILE, CKIT_ERROR_FILE_UNOPENED);
    if (!(files[fd].attributes & ATTRIBUTE_ISOPEN)) { return 0; }

    DWORD method = origin;
    // DWORD method = 0;
    // switch (origin) {
    //     case FSEEK_SET: method = FILE_BEGIN;   break;
    //     case FSEEK_CUR: method = FILE_CURRENT; break;
    //     case FSEEK_END: method = FILE_END;     break;
    // }

    LARGE_INTEGER LINew = {0};
    LARGE_INTEGER LIOffset = {0};
    LIOffset.QuadPart = offset;
    win_zthrowif(!SetFilePointerEx(files[fd].handle, LIOffset, &LINew, method), ERROR_WINAPI_FILE | ERROR_SEEKFILE);
    return (size_t)LINew.QuadPart;
}

void flushinput() {
    FlushConsoleInputBuffer(files[0].handle);
}

#endif /* INFO_OS_WINDOWS */