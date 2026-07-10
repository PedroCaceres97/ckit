#include <ckit/utils.h>
#include <ckit/config.h>
#include <ckit/symbols.h>

CKIT_BUFFERS(tostr, char, CKIT_TOSTR_BUFFER_COUNT, CKIT_TOSTR_BUFFER_SIZE)

static const float ckit_f32_rounders[8] = {
    0.5f,
    0.05f,
    0.005f,
    0.0005f,
    0.00005f,
    0.000005f,
    0.0000005f,
    0.00000005f
};
static const double ckit_f64_rounders[16] = {
    0.5,
    0.05,
    0.005,
    0.0005,
    0.00005,
    0.000005,
    0.0000005,
    0.00000005,
    0.000000005,
    0.0000000005,
    0.00000000005,
    0.000000000005,
    0.0000000000005,
    0.00000000000005,
    0.000000000000005,
    0.0000000000000005
};
static const uint64_t ckit_powers_10[19] = {
    1ULL, 
    10ULL, 
    100ULL, 
    1000ULL, 
    10000ULL,
    100000ULL, 
    1000000ULL, 
    10000000ULL,
    100000000ULL, 
    1000000000ULL,
    10000000000ULL, 
    100000000000ULL,
    1000000000000ULL, 
    10000000000000ULL,
    100000000000000ULL, 
    1000000000000000ULL,
    10000000000000000ULL, 
    100000000000000000ULL,
    1000000000000000000ULL
};

static const char ckit_digit_pair[201] =
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899";
static const char ckit_hex_pair[513] =
	"000102030405060708090A0B0C0D0E0F"
	"101112131415161718191A1B1C1D1E1F"
	"202122232425262728292A2B2C2D2E2F"
	"303132333435363738393A3B3C3D3E3F"
	"404142434445464748494A4B4C4D4E4F"
	"505152535455565758595A5B5C5D5E5F"
	"606162636465666768696A6B6C6D6E6F"
	"707172737475767778797A7B7C7D7E7F"
	"808182838485868788898A8B8C8D8E8F"
	"909192939495969798999A9B9C9D9E9F"
	"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
	"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
	"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
	"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
	"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
	"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

static char* ckit_u32str(char* buffer, uint32_t value) {
    char temp[CKIT_TOSTR_BUFFER_SIZE] = {0};
    char* cursor = PTRADD(temp, CKIT_TOSTR_BUFFER_SIZE - 1);

    while (value >= 100) {
        uint32_t quotient = value / 100;
        uint32_t module = value - quotient * 100;
        cursor -= 2;
        cursor[0] = ckit_digit_pair[module * 2 + 0];
        cursor[1] = ckit_digit_pair[module * 2 + 1];
        value = quotient;
    }

    if (value < 10) {
        *--cursor = (char)('0' + value);
    } else {
        cursor -= 2;
        cursor[0] = ckit_digit_pair[value * 2 + 0];
        cursor[1] = ckit_digit_pair[value * 2 + 1];
    }

    while(*cursor) { *buffer++ = *cursor++; }
    *buffer = '\0';
    return buffer;
}
static char* ckit_u64str(char* buffer, uint64_t value) {
    char temp[CKIT_TOSTR_BUFFER_SIZE] = {0};
    char* cursor = PTRADD(temp, CKIT_TOSTR_BUFFER_SIZE - 1);

    while (value >= 100) {
        uint64_t quotient = value / 100;
        uint32_t module = value - quotient * 100;
        cursor -= 2;
        cursor[0] = ckit_digit_pair[module * 2 + 0];
        cursor[1] = ckit_digit_pair[module * 2 + 1];
        value = quotient;
    }

    if (value < 10) {
        *--cursor = (char)('0' + value);
    } else {
        cursor -= 2;
        cursor[0] = ckit_digit_pair[value * 2 + 0];
        cursor[1] = ckit_digit_pair[value * 2 + 1];
    }

    while(*cursor) { *buffer++ = *cursor++; }
    *buffer = '\0';
    return buffer;
}

int i32str_out(char* buffer, int32_t value, bool plus, bool space) {
    int written = 0;
    uint32_t unsigned_v = (uint32_t)value;
    if (value < 0) { buffer[written++] = '-'; unsigned_v = (uint32_t)(-(int64_t)value); } 
    else if (plus) { buffer[written++] = '+'; }
    else if (space){ buffer[written++] = ' '; }
    char* end = ckit_u32str(&buffer[written], unsigned_v);
    written += PTRDST(buffer, end);
    return written;
}
int i64str_out(char* buffer, int64_t value, bool plus, bool space) {
    int written = 0;
    uint64_t unsigned_v = (uint64_t)value;
    if (value < 0) { buffer[written++] = '-'; unsigned_v = (uint64_t)(-(int64_t)value); } 
    else if (plus) { buffer[written++] = '+'; }
    else if (space){ buffer[written++] = ' '; }
    char* end = ckit_u64str(&buffer[written], unsigned_v);
    written += PTRDST(buffer, end);
    return written;
}
int u32str_out(char* buffer, uint32_t value, bool plus, bool space) {
    int written = 0;
    if (plus) { buffer[written++] = '+'; }
    else if (space){ buffer[written++] = ' '; }
    char* end = ckit_u32str(&buffer[written], value);
    written += PTRDST(buffer, end);
    return written;
}
int u64str_out(char* buffer, uint64_t value, bool plus, bool space) {
    int written = 0;
    if (plus) { buffer[written++] = '+'; }
    else if (space){ buffer[written++] = ' '; }
    char* end = ckit_u64str(&buffer[written], value);
    written += PTRDST(buffer, end);
    return written;
}
int x32str_out(char* buffer, uint32_t value, bool x) {
    int written = 0;
    if (x) {
        buffer[written++] = '0';
        buffer[written++] = 'x';
    }

    for (int i = 3; i >= 0; --i) {
        uint32_t byte = (value >> (i * 8)) & 0xFF;
        const char* pair = &ckit_hex_pair[byte * 2];
        buffer[written++] = pair[0];
        buffer[written++] = pair[1];
    }

    buffer[written] = '\0';
    return written;
}
int x64str_out(char* buffer, uint64_t value, bool x) {
    int written = 0;
    if (x) {
        buffer[written++] = '0';
        buffer[written++] = 'x';
    }

    for (int i = 3; i >= 0; --i) {
        uint32_t byte = (value >> (i * 8)) & 0xFF;
        const char* pair = &ckit_hex_pair[byte * 2];
        buffer[written++] = pair[0];
        buffer[written++] = pair[1];
    }

    buffer[written] = '\0';
    return written;
}
int f32str_out(char* buffer, float value, int precision, bool plus, bool space) {
    if (isnan(value)) { 
        strcpy(buffer, "nan");
        return 3;
    }
    if (isinf(value)) { 
        strcpy(buffer, TERNARY(signbit(value), "-inf", TERNARY(plus, "+inf", "inf"))); 
        return TERNARY(signbit(value), 4, TERNARY(plus, 4, 3));
    }

    int written = 0;
    if (value < 0.0f) { buffer[written++] = '-'; value = -value; }
    else if (plus) { buffer[written++] = '+'; }
    else if (space){ buffer[written++] = ' '; } 

    precision = CLAMP(precision, 0, 7);
    value += ckit_f32_rounders[precision];

    uint32_t integer = (uint32_t)value;
    buffer = ckit_u32str(buffer, integer);

    if (precision != 0) {
        buffer[written++] = '.';
    
        float decimals_f = value - (float)integer;
        while (precision > 0 && (int)(decimals_f * 10.0f) == 0) {
            buffer[written++] = '0';
            precision--;
            decimals_f *= 10.0f;
        }

        uint32_t decimals_u = (uint32_t)(decimals_f * ckit_powers_10[precision]);
        if (decimals_u != 0 && precision > 0) { buffer = ckit_u32str(buffer, decimals_u); }
    }

    buffer[written] = '\0';
    return written;
}
int f64str_out(char* buffer, double value, int precision, bool plus, bool space) {
    if (isnan(value)) { 
        strcpy(buffer, "nan");
        return 3;
    }
    if (isinf(value)) { 
        strcpy(buffer, TERNARY(signbit(value), "-inf", TERNARY(plus, "+inf", "inf"))); 
        return TERNARY(signbit(value), 4, TERNARY(plus, 4, 3));
    }

    int written = 0;
    if (value < 0.0f) { buffer[written++] = '-'; value = -value; }
    else if (plus) { buffer[written++] = '+'; }
    else if (space){ buffer[written++] = ' '; } 

    precision = CLAMP(precision, 0, 15);
    value += ckit_f64_rounders[precision];

    uint64_t integer = (uint64_t)value;
    buffer = ckit_u64str(buffer, integer);

    if (precision == 0) {
        buffer[written++] = '.';
        double decimals_f = value - (double)integer;
        while (precision > 0 && (int)(decimals_f * 10.0f) == 0) {
            buffer[written++] = '0';
            precision--;
            decimals_f *= 10.0f;
        }

        uint64_t decimals_u = (uint64_t)(decimals_f * ckit_powers_10[precision]); 
        if (decimals_u != 0 && precision > 0) { buffer = ckit_u64str(buffer, decimals_u); }
    }

    buffer[written] = '\0';
    return written;
}
int ptrstr_out(char* buffer, void* value) {
#if defined(INFO_PTR_32BIT)
    return x32str_out(buffer, (uint32_t)value, true);
#elif defined(INFO_PTR_64BIT)
    return x64str_out(buffer, (uint64_t)value, true);
#else
    #error CKIT_ERROR_UNSUPPORTED_PTR
#endif
}
int sizestr_out(char* buffer, size_t value) {
#if defined(INFO_SIZE_32BIT)
    return u32str_out(buffer, (uint32_t)value, false, false);
#elif defined(INFO_SIZE_64BIT)
    return u64str_out(buffer, (uint64_t)value, false, false);
#else
    #error CKIT_ERROR_UNSUPPORTED_SIZE
#endif
}
int diffstr_out(char* buffer, ptrdiff_t value) {
#if defined(INFO_PTRDIFF_32BIT)
    return i32str_out(buffer, (uint32_t)value, false, false);
#elif defined(INFO_PTRDIFF_64BIT)
    return i64str_out(buffer, (uint64_t)value, false, false);
#else
    #error CKIT_ERROR_UNSUPPORTED_DIFF
#endif
}
int bytesstr_out(char* buffer, size_t value) {
    static const char* units[] = {"B", "KB", "MB", "GB"};

    int unit = 0;
    int written = 0;
    int fraction = 0;

    while (value >= 1024 && unit < 3) {
        fraction = (value % 1024) * 10 / 1024;
        value /= 1024;
        unit++;
    }

    written += sizestr_out(buffer, value);
    if (unit == 0) {
        buffer[written++] = units[0][0];
        return written;
    }

    buffer[written++] = '.';
    written += u32str_out(&buffer[written], fraction, false, false);
    buffer[written++] = units[unit][0];
    buffer[written++] = units[unit][1];
    return written;
}

const char* i32str(int32_t value, bool plus, bool space) {
    char* buffer = ckit_tostr_next();
    i32str_out(buffer, value, plus, space);
    return buffer;
}
const char* i64str(int64_t value, bool plus, bool space) {
    char* buffer = ckit_tostr_next();
    i64str_out(buffer, value, plus, space);
    return buffer;
}
const char* u32str(uint32_t value, bool plus, bool space) {
    char* buffer = ckit_tostr_next();
    u32str_out(buffer, value, plus, space);
    return buffer;
}
const char* u64str(uint64_t value, bool plus, bool space) {
    char* buffer = ckit_tostr_next();
    u64str_out(buffer, value, plus, space);
    return buffer;
}
const char* x32str(uint32_t value, bool x) {
    char* buffer = ckit_tostr_next();
    x32str_out(buffer, value, x);
    return buffer;
}
const char* x64str(uint64_t value, bool x) {
    char* buffer = ckit_tostr_next();
    x64str_out(buffer, value, x);
    return buffer;
}
const char* f32str(float value, int precision, bool plus, bool space) {
    char* buffer = ckit_tostr_next();
    f32str_out(buffer, value, precision, plus, space);
    return buffer;
}
const char* f64str(double value, int precision, bool plus, bool space) {
    char* buffer = ckit_tostr_next();
    f64str_out(buffer, value, precision, plus, space);
    return buffer;
}
const char* ptrstr(void* value) {
    char* buffer = ckit_tostr_next();
    ptrstr_out(buffer, value);
    return buffer;
}
const char* sizestr(size_t value) {
    char* buffer = ckit_tostr_next();
    sizestr_out(buffer, value);
    return buffer;
}
const char* diffstr(ptrdiff_t value) {
    char* buffer = ckit_tostr_next();
    diffstr_out(buffer, value);
    return buffer;
}
const char* bytesstr(size_t value) {
    char* buffer = ckit_tostr_next();
    bytesstr_out(buffer, value);
    return buffer;
}