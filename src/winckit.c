#include <ckit/info.h>
#include <handleapi.h>

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
CKIT_BUFFERS(longpath, wchar_t, CKIT_LONGPATH_BUFFER_COUNT, CKIT_LONGPATH_BUFFER_SIZE)

#define winensure(cond, action) do { if (!(cond)) { printlast(CONTEXT); ensure_action(action); } } while(0)
#define winexpect(cond, action) do { if (!(cond)) { printlast(CONTEXT); expect_action(action); } } while(0)
#define winfindclose(first, action) do { winensure(FindClose(first), action); action; } while(0)

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
    [0] = {
        .handle = INVALID_HANDLE_VALUE,
        .attributes = ATTRIBUTE_READ
    },
    [1] = {
        .handle = INVALID_HANDLE_VALUE,
        .attributes = ATTRIBUTE_WRITE | ATTRIBUTE_ANSI
    },
    [2] = {
        .handle = INVALID_HANDLE_VALUE,
        .attributes = ATTRIBUTE_WRITE | ATTRIBUTE_ANSI
    }
};
static bool initialized = false;

void winckit_quit() {
    SetConsoleMode(files[1].handle, stdout_mode);
    SetConsoleMode(files[2].handle, stderr_mode);
    files[0].handle = INVALID_HANDLE_VALUE;
    files[1].handle = INVALID_HANDLE_VALUE;
    files[2].handle = INVALID_HANDLE_VALUE;
    files[0].attributes &= ~ATTRIBUTE_ISOPEN;
    files[1].attributes &= ~ATTRIBUTE_ISOPEN;
    files[2].attributes &= ~ATTRIBUTE_ISOPEN;
#if !CKIT_DONT_ENABLE_ANSI_FOR_WINDOWS
    GetConsoleMode(files[1].handle, &stdout_mode);
    GetConsoleMode(files[2].handle, &stderr_mode);
    SetConsoleMode(files[1].handle, stdout_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    SetConsoleMode(files[2].handle, stderr_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
#endif
    throwif(back < FILES_STACK, ERROR_WIN_FILES_UNCLOSED, CKIT_ERROR_FILES_UNCLOSED);
}

PREMAIN(winckit_init) {
    files[0].handle = GetStdHandle(STD_INPUT_HANDLE);
    files[1].handle = GetStdHandle(STD_OUTPUT_HANDLE);
    files[2].handle = GetStdHandle(STD_ERROR_HANDLE);
    files[0].attributes |= ATTRIBUTE_ISOPEN;
    files[1].attributes |= ATTRIBUTE_ISOPEN;
    files[2].attributes |= ATTRIBUTE_ISOPEN;
#if !CKIT_DONT_ENABLE_ANSI_FOR_WINDOWS
    failif(files[0].handle == INVALID_HANDLE_VALUE, );
    if (files[1].handle == INVALID_HANDLE_VALUE) {

    }
    GetConsoleMode(files[1].handle, &stdout_mode);
    GetConsoleMode(files[2].handle, &stderr_mode);
    SetConsoleMode(files[1].handle, stdout_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    SetConsoleMode(files[2].handle, stderr_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
#endif
}

void pedrolib_os_quit() {
    if (files.base.size) {
        log(LOG_WARNING, ERROR_FILES_NOT_CLOSED " (%zu)", files.base.size);
    }
    fslab_foreach(&files, NULL, pedrolib_files_close);
    fslab_destroy(&files);
#if ENABLE_ANSI_ON_INIT
    SetConsoleMode(pedrolib_stdout.handle, stdout_mode);
    SetConsoleMode(pedrolib_stderr.handle, stderr_mode);
#endif
    pedrolib_stdin.handle = INVALID_HANDLE_VALUE;
    pedrolib_stdout.handle = INVALID_HANDLE_VALUE;
    pedrolib_stderr.handle = INVALID_HANDLE_VALUE;
    pedrolib_stdin.open = false;
    pedrolib_stdout.open = false;
    pedrolib_stderr.open = false;
}

File*       file_in() {
    return &pedrolib_stdin;
}
File*       file_out() {
    return &pedrolib_stdout;
}
File*       file_err() {
    return &pedrolib_stderr;
}

bool        file_standard   (File* file) {
    return file == &pedrolib_stdin || file == &pedrolib_stdout || file == &pedrolib_stderr;
}
bool        file_readable   (File* file) {
    return file->attributes & ATTRIBUTE_READ;
}
bool        file_writable   (File* file) {
    return file->attributes & ATTRIBUTE_WRITE;
}

bool        file_isopen     (File* file) {
    return file->open;
}
bool        file_isansi     (File* file) {
    return file->attributes & ATTRIBUTE_ANSI;
}

void        flushinput  () {
    FlushConsoleInputBuffer(pedrolib_stdin.handle);
}

static void printlast(Context context) {
    char buffer[2048] = {0};
    DWORD code = GetLastError();
    pedrolib_log(LOG_ERROR, "Windows API error, next message will provide OS error message");
    bool formatted = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, sizeof(buffer), NULL);
    if (!formatted) { strcpy(buffer, "Failed to format windows error"); }
    pedrolib_inform(LOG_ERROR, context, buffer);
}
static wchar_t* longpath(char* path) {
    wchar_t utf16[PATH_BUFFER_SIZE];
    winensure(MultiByteToWideChar(CP_UTF8, 0, path, -1, utf16, PATH_BUFFER_SIZE), return NULL);

    wchar_t* buffer = pedrolib_widestr_next();
    wmemset(buffer, 0, PATH_BUFFER_SIZE);

    wchar_t* cursor = buffer;
    *cursor++ = L'\\';
    *cursor++ = L'\\';
    *cursor++ = L'?';
    *cursor++ = L'\\';
    DWORD len = GetFullPathNameW(
        utf16,
        PATH_BUFFER_SIZE - 4,
        cursor,
        NULL
    );
    winensure(len != 0 && len < PATH_BUFFER_SIZE, return NULL);
    return buffer;
}

static bool win_isdotfolder(wchar_t* pathname) {
    return !wcscmp(pathname, L".") || !wcscmp(pathname, L"..");
}

static bool win_isdir(wchar_t* pathname) {
    DWORD attributes = GetFileAttributesW(pathname);
    if (attributes == INVALID_FILE_ATTRIBUTES) { return false; }
    return attributes & FILE_ATTRIBUTE_DIRECTORY;
}
static bool win_isfile(wchar_t* pathname) {
    DWORD attributes = GetFileAttributesW(pathname);
    if (attributes == INVALID_FILE_ATTRIBUTES) { return false; }
    return !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}
static bool win_removedir (wchar_t* pathname, bool force) {
    if (!win_isdir(pathname)) { return true; }

    size_t len = wcslen(pathname);
    pathname[len] = L'\\';
    pathname[len + 1] = L'*';
    pathname[len + 2] = 0;

    WIN32_FIND_DATAW data = {0};
    HANDLE first = FindFirstFileW(pathname, &data);
    if (first == INVALID_HANDLE_VALUE) {
        winensure(GetLastError() == ERROR_FILE_NOT_FOUND, return false);
        goto success;
    } else if (!force) { 
        goto failure;
    }

    do { 
        if (win_isdotfolder(data.cFileName)) { continue; }

        pathname[len] = '\\';
        pathname[len + 1] = 0;
        wcscat(pathname, data.cFileName);

        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { 
            if (!win_removedir(pathname, true)) { goto failure; }
        }
        else {
            winensure(DeleteFileW(pathname), goto failure);
        }
    } while(FindNextFileW(first, &data));
    winensure(GetLastError() == ERROR_NO_MORE_FILES, goto failure);
    
success:
    winensure(FindClose(first), return false);
    pathname[len] = 0;
    winexpect(RemoveDirectoryW(pathname), return false);
    return true;

failure:
    winensure(FindClose(first), return false);
    return false;
}

File*       file_open   (char* pathname, FileAttributes attributes) {
    File* file = fslab_alloc(&files);
    file->attributes = attributes;

    DWORD access = GENERIC_READ;
    DWORD creation = OPEN_EXISTING;

    if (attributes & ATTRIBUTE_WRITE) { access |= GENERIC_WRITE; }
    if (attributes & ATTRIBUTE_CREATE) { creation = CREATE_NEW; }
    if (attributes & ATTRIBUTE_TRUNCATE) { creation = CREATE_ALWAYS; attributes &= ~ATTRIBUTE_CREATE; }

    wchar_t* path = longpath(pathname);
    file->handle = CreateFileW(path, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file->handle == INVALID_HANDLE_VALUE && attributes & ATTRIBUTE_NOERROR) {
        file->open = false;
        return file;
    }
    winensure(file->handle != INVALID_HANDLE_VALUE, files_free(file); return NULL);
    file->open = true;
    return file;
}
void        file_close  (File* file) {
    expect(!file_standard(file), return, "Standard file is not closeable");
    if (file->open) {
        BOOL result = CloseHandle(file->handle);
        winensure(result, return);
    }
    fslab_free(&files, file);
}

void        file_create (char* pathname, bool truncate) {
    nonnull(expect, pathname, return);
    file_close(file_open(pathname, truncate ? ATTRIBUTE_TRUNCATE : ATTRIBUTE_CREATE | ATTRIBUTE_NOERROR));
}
bool        file_remove (char* pathname) {
    nonnull(expect, pathname, return false);
    wchar_t* path = longpath(pathname);
    if (!win_isfile(path)) { return true; }
    BOOL result = DeleteFileW(path);
    winexpect(result, return false);
    return true;
}

size_t      file_read   (File* file, char* buf, size_t count) {
    nonnull(expect, file, return 0);
    nonnull(expect, buf, return 0);
    ensure(file_readable(file), return 0, ERROR_FILE_NOT_READABLE);
    if (!file->open || !count) { return 0; }
    DWORD readed = 0;
    BOOL result = ReadFile(file->handle, buf, count - 1, &readed, NULL);
    winexpect(result, return 0);
    buf[readed] = '\0';
    return (size_t)readed;
}
size_t      file_write  (File* file, const char* buf, size_t count) {
    nonnull(expect, file, return 0);
    nonnull(expect, buf, return 0);
    ensure(file_writable(file), return 0, ERROR_FILE_NOT_WRITABLE);
    if (!file->open || !count) { return 0; }
    DWORD written = 0;
    BOOL result = WriteFile(file->handle, buf, count, &written, NULL);
    winexpect(result, return 0);
    return (size_t)written;
}

size_t      file_size   (File* file) {
    nonnull(ensure, file, return 0);
    if (!file->open) { return 0; }
    LARGE_INTEGER LISize = {0};
    BOOL result = GetFileSizeEx(file->handle, &LISize);
    winexpect(result, return 0);
    return (size_t)LISize.QuadPart;
}
size_t      file_tell   (File* file) {
    nonnull(ensure, file, return 0);
    if (!file->open) { return 0; }
    LARGE_INTEGER LINew = {0};
    LARGE_INTEGER LIOffset = {0};
    BOOL result = SetFilePointerEx(file->handle, LIOffset, &LINew, SEEK_CUR);
    winexpect(result, return 0);
    return (size_t)LINew.QuadPart;
}
size_t      file_seek   (File* file, SeekOrigin origin, ptrdiff_t offset) {
    nonnull(ensure, file, return 0);
    if (!file->open) { return 0; }
    DWORD method = 0;
    switch (origin) {
        case ORIGIN_START:   method = FILE_BEGIN;     break;
        case ORIGIN_CURRENT: method = FILE_CURRENT;   break;
        case ORIGIN_END:     method = FILE_END;       break;
        default: ensure(false, return 0, "Invalid SeekOrigin value: %i", origin);
    }

    LARGE_INTEGER LINew = {0};
    LARGE_INTEGER LIOffset = {0};
    LIOffset.QuadPart = offset;
    BOOL result = SetFilePointerEx(file->handle, LIOffset, &LINew, method);
    winexpect(result, return 0);
    return (size_t)LINew.QuadPart;
}

bool        isdir       (char* pathname) {
    nonnull(expect, pathname, return false);
    return win_isdir(longpath(pathname));
}
bool        isfile      (char* pathname) {
    nonnull(expect, pathname, return false);
    return win_isfile(longpath(pathname));
}
void        makedir     (char* pathname) {
    nonnull(expect, pathname, return);

    wchar_t* path = longpath(pathname);
    if (win_isdir(path)) { return; }

    /* Skip "\\?\C:\"*/
    WCHAR* next = &path[7];
    while (*next) {
        if (*next != L'\\' && *next != L'/') { next++; continue; }
        *next = L'\0';
        if (!win_isdir(path)) {
            BOOL result = CreateDirectoryW(path, NULL);
            winexpect(result || GetLastError() == ERROR_ALREADY_EXISTS, return);
        }
        *next++ = L'\\';
    }
    BOOL result = CreateDirectoryW(path, NULL);
    winexpect(result || GetLastError() == ERROR_ALREADY_EXISTS, return);
}
bool        removedir   (char *pathname, bool force) {
    nonnull(expect, pathname, return false);
    wchar_t* path = longpath(pathname);
    return win_removedir(path, force);
}

#endif /* INFO_OS_WINDOWS */