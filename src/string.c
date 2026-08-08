#include <ckit/string.h>
#include <ckit/config.h>
#include <ckit/symbols.h>
#include <ckit/error.h>

void string_destroy(String* str) {
    vthrownull(str);
    if (str->data) { ckit_free(str->data); }
    str->data = NULL;
    str->size = 0;
    str->capacity = 0;
    CKIT_DESTROY(str);
}
String* string_from(String* str, const char* src) {
    nthrownull(src);
    CKIT_CREATE(str, String);
    if (str->data) { ckit_free(str->data); }
    size_t len = strlen(src);
    str->capacity = MAX(len * 2, CKIT_STRING_MINIMUM_SIZE);
    str->data = ckit_malloc(str->capacity + 1);
    str->size = len;
    str->data[str->size] = '\0';
    memcpy(str->data, src, str->size);
    return str;
}
String* string_create(String* str) {
    CKIT_CREATE(str, String);
    if (str->data) { ckit_free(str->data); }
    str->capacity = CKIT_STRING_MINIMUM_SIZE;
    str->data = ckit_malloc(str->capacity + 1);
    str->data[0] = '\0'; // Just in case
    str->size = 0;
    return str;
}
String* string_duplicate(String* str, String* src) {
    nthrownull(src);
    CKIT_CREATE(str, String);
    if (str->data) { ckit_free(str->data); }
    str->capacity = MAX(src->size * 2, CKIT_STRING_MINIMUM_SIZE);
    str->data = ckit_malloc(str->capacity + 1);
    str->size = src->size;
    str->data[str->size] = '\0';
    memcpy(str->data, src->data, src->size);
    return str;
}

char* string_data(String* str) {
    nthrownull(str);
    return str->data;
}
size_t string_size(String* str) {
    zthrownull(str);
    return str->size;
}
size_t string_capacity(String* str) {
    zthrownull(str);
    return str->capacity;
}

char* string_at(String* str, size_t index) {
    nthrownull(str);
    nthrowbounds(index, str->size);
    return &str->data[index];
}
char string_get(String* str, size_t index) {
    zthrownull(str);
    zthrowbounds(index, str->size);
    return str->data[index];
}
void string_set(String* str, size_t index, char ch) {
    vthrownull(str);
    vthrowbounds(index, str->size);
    str->data[index] = ch;
}

void string_clear(String* str) {
    vthrownull(str);
    memset(str->data, 0, str->size);
    str->size = 0;
    str->data[0] = '\0'; // Just in case
}
void string_shrink(String* str) {
    vthrownull(str);
    if (str->capacity <= CKIT_STRING_MINIMUM_SIZE || str->size > str->capacity / 4) { return; }
    str->capacity = MAX(str->size * 2, CKIT_STRING_MINIMUM_SIZE);
    str->data = ckit_realloc(str->data, str->capacity + 1);
    str->data[str->size] = '\0';
}
void string_resize(String* str, size_t size) {
    vthrownull(str);
    if (!size) { string_clear(str); return; } 
    if (size > str->capacity || size < str->capacity / 4) {
        str->capacity = MAX(size * 2, CKIT_STRING_MINIMUM_SIZE);
        str->data = ckit_realloc(str->data, str->capacity + 1);
    }
    if (size > str->size) { memset(&str->data[str->size], 0, size - str->size); }
    str->size = size;
    str->data[str->size] = '\0';
}
void string_reserve(String* str, size_t capacity) {
    vthrownull(str);
    if (!capacity) { string_clear(str); return; } 
    if (capacity > str->capacity) {
        str->capacity = capacity;
        str->data = ckit_realloc(str->data, str->capacity + 1);
        memset(&str->data[str->size], 0, str->capacity + 1 - str->size);
    }
}

void string_erase(String* str, size_t index) {
    vthrownull(str);
    vthrowbounds(index, str->size);
    if (index != str->size - 1) { memmove(&str->data[index], &str->data[index + 1], str->size - index - 1); }
    str->size--;
    str->data[str->size] = '\0';
    string_shrink(str);
}
void string_popback(String* str) {
    vthrownull(str);
    if (!str->size) { return; }
    string_erase(str, str->size - 1);
}
void string_popfront(String* str) {
    vthrownull(str);
    if (!str->size) { return; }
    string_erase(str, 0);
}

void string_insert(String* str, size_t index, char ch) {
    vthrownull(str);
    vthrowbounds(index, str->size + 1); // Because you can insert at index str->size
    if (str->size == str->capacity) { string_reserve(str, str->size * 2); }
    if (index != str->size) { memmove(&str->data[index + 1], &str->data[index], str->size - index); }
    str->size++;
    str->data[index] = ch;
    str->data[str->size] = '\0';
}
void string_pushback(String* str, char ch) {
    vthrownull(str);
    string_insert(str, str->size, ch);
}
void string_pushfront(String* str, char ch) {
    vthrownull(str);
    string_insert(str, 0, ch);
}

void string_fill(String* str, char ch, size_t offset, size_t count) {
    vthrownull(str);
    if (!count) { return; }
    vthrowbounds(offset, str->size); // Checks if offset is smaller than str->size for better logging
    vthrowbounds(count, str->size - offset + 1); // Count must be smaller that str->size - offset + 1 as str->size is counting and offset is an index
    memset(&str->data[offset], ch, count);
    str->data[str->size] = '\0';
}
void string_copy(String* str, const char* src, size_t offset, size_t count) {
    vthrownull(str);
    if (!count) { return; }
    vthrownull(src);
    vthrowbounds(offset, str->size); // Checks if offset is smaller than str->size for better logging
    vthrowbounds(count, str->size - offset + 1); // Count must be smaller that str->size - offset, as we are dealing with indices we subtract one, this is to avoid integer overflow
    memmove(&str->data[offset], src, count); // To allow overlaping
    str->data[str->size] = '\0';
}
void string_append(String* str, const char* src) {
    vthrownull(str);
    vthrownull(src);
    size_t len = strlen(src);
    string_appendn(str, src, len);
}
void string_appendn(String* str, const char* src, size_t count) {
    vthrownull(str);
    if (!count) { return; }
    vthrownull(src);
    size_t idxaliasing = PTRPTR(src, str->data);
    bool aliasing = src >= str->data && idxaliasing < str->size;
    size_t size = str->size;
    string_resize(str, str->size + count);
    if (aliasing) { src = &str->data[aliasing]; }
    string_copy(str, src, size, count);
}
void string_appendch(String* str, char ch, size_t count) {
    vthrownull(str);
    if (!count) { return; }
    size_t size = str->size;
    string_resize(str, str->size + count);
    string_fill(str, ch, size, count);
}