#ifndef __CKIT_STRING_H__
#define __CKIT_STRING_H__

#include <ckit/info.h>

int i32str_out(char* dest, int32_t value, bool plus, bool space);
int i64str_out(char* dest, int64_t value, bool plus, bool space);
int u32str_out(char* dest, uint32_t value, bool plus, bool space);
int u64str_out(char* dest, uint64_t value, bool plus, bool space);
int x32str_out(char* dest, uint32_t value, bool x);
int x64str_out(char* dest, uint64_t value, bool x);
int f32str_out(char* dest, float value, int precision, bool plus, bool space);
int f64str_out(char* dest, double value, int precision, bool plus, bool space);
int ptrstr_out(char* dest, void* value);
int sizestr_out(char* dest, size_t value);
int diffstr_out(char* dest, ptrdiff_t value);
int bytesstr_out(char* dest, size_t value);
#define boolstr_out(dest, value) strcpy(dest, TERNARY(!!(value), "Yes", "No"))

const char* i32str(int32_t value, bool plus, bool space);
const char* i64str(int64_t value, bool plus, bool space);
const char* u32str(uint32_t value, bool plus, bool space);
const char* u64str(uint64_t value, bool plus, bool space);
const char* x32str(uint32_t value, bool x);
const char* x64str(uint64_t value, bool x);
const char* f32str(float value, int precision, bool plus, bool space);
const char* f64str(double value, int precision, bool plus, bool space);
const char* ptrstr(void* value);
const char* sizestr(size_t value);
const char* diffstr(ptrdiff_t value);
const char* bytesstr(size_t value);
#define boolstr(value) TERNARY(!!(value), "Yes", "No")

#endif /* __CKIT_STRING_H__ */