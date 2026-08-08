#ifndef __CKIT_STRING_H__
#define __CKIT_STRING_H__

#include <ckit/info.h>
#include <ckit/utils.h>

typedef struct String {
    char* data;
    size_t size;
    size_t capacity;
    bool allocated;
} String;

void string_destroy(String* str);
String* string_from(String* str, const char* src);
String* string_create(String* str);
String* string_duplicate(String* str, String* src);

char* string_data(String* str);
size_t string_size(String* str);
size_t string_capacity(String* str);

char* string_at(String* str, size_t index);
char string_get(String* str, size_t index);
void string_set(String* str, size_t index, char ch);

void string_clear(String* str);
void string_shrink(String* str);
void string_resize(String* str, size_t size);
void string_reserve(String* str, size_t capacity);

void string_erase(String* str, size_t index);
void string_popback(String* str);
void string_popfront(String* str);

void string_insert(String* str, size_t index, char ch);
void string_pushback(String* str, char ch);
void string_pushfront(String* str, char ch);

void string_fill(String* str, char ch, size_t offset, size_t count);
void string_copy(String* str, const char* src, size_t offset, size_t count);
void string_append(String* str, const char* src);
void string_appendn(String* str, const char* src, size_t count);
void string_appendch(String* str, char ch, size_t coutn);

#endif /* __CKIT_STRING_H__ */