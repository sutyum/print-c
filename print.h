#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

// Function prototypes
void print(const char* format, ...);

// Internal helper functions
static void print_char(char c);
static void print_string(const char* str);
static void print_int(int value);
static void print_uint(unsigned int value);
static void print_hex(unsigned int value);
static void print_binary(unsigned int value);
static void print_pointer(const void* ptr);
static void print_double(double value, int precision);

// These functions should be implemented by the user
extern void _putchar(char c);

void print(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '{' && *(format + 1) && *(format + 1) != '{') {
            format++; // Skip '{'
            switch (*format) {
                case '}': // Just '{}'
                    print_int(va_arg(args, int));
                    break;
                case 'd':
                    print_int(va_arg(args, int));
                    format++; // Skip 'd'
                    break;
                case 'u':
                    print_uint(va_arg(args, unsigned int));
                    format++; // Skip 'u'
                    break;
                case 'x':
                    print_hex(va_arg(args, unsigned int));
                    format++; // Skip 'x'
                    break;
                case 'b':
                    print_binary(va_arg(args, unsigned int));
                    format++; // Skip 'b'
                    break;
                case 'f': {
                    double value = va_arg(args, double);
                    int precision = 6; // Default precision
                    format++; // Skip 'f'
                    if (*format == ':' && *(format + 1) == '.') {
                        format += 2; // Skip ':.'
                        precision = 0;
                        while (*format >= '0' && *format <= '9') {
                            precision = precision * 10 + (*format - '0');
                            format++;
                        }
                    }
                    print_double(value, precision);
                    break;
                }
                case 's':
                    print_string(va_arg(args, const char*));
                    format++; // Skip 's'
                    break;
                case 'c':
                    print_char((char)va_arg(args, int));
                    format++; // Skip 'c'
                    break;
                case 'p':
                    print_pointer(va_arg(args, const void*));
                    format++; // Skip 'p'
                    break;
                default:
                    print_char('{');
                    print_char(*format);
                    break;
            }
        } else if (*format == '{' && *(format + 1) == '{') {
            print_char('{');
            format++; // Skip one '{'
        } else {
            print_char(*format);
        }
        format++;
    }

    va_end(args);
}

static void print_char(char c) {
    _putchar(c);
}

static void print_string(const char* str) {
    if (str == NULL) {
        print_string("(null)");
    } else {
        while (*str) {
            print_char(*str++);
        }
    }
}

static void print_int(int value) {
    if (value < 0) {
        print_char('-');
        value = -value;
    }
    print_uint((unsigned int)value);
}

static void print_uint(unsigned int value) {
    if (value >= 10) {
        print_uint(value / 10);
    }
    print_char('0' + (value % 10));
}

static void print_hex(unsigned int value) {
    print_string("0x");
    for (int i = 28; i >= 0; i -= 4) {
        int digit = (value >> i) & 0xF;
        print_char(digit < 10 ? '0' + digit : 'a' + digit - 10);
    }
}

static void print_binary(unsigned int value) {
    print_string("0b");
    for (int i = 31; i >= 0; i--) {
        print_char('0' + ((value >> i) & 1));
    }
}

static void print_pointer(const void* ptr) {
    print_string("0x");
    uintptr_t value = (uintptr_t)ptr;
    for (int i = (sizeof(uintptr_t) * 8) - 4; i >= 0; i -= 4) {
        int digit = (value >> i) & 0xF;
        print_char(digit < 10 ? '0' + digit : 'a' + digit - 10);
    }
}

static void print_double(double value, int precision) {
    if (value < 0) {
        print_char('-');
        value = -value;
    }
    
    unsigned long long int_part = (unsigned long long)value;
    print_uint(int_part);
    
    if (precision > 0) {
        print_char('.');
        value -= int_part;
        
        for (int i = 0; i < precision; i++) {
            value *= 10;
            int digit = (int)value;
            print_char('0' + digit);
            value -= digit;
        }
    }
}

#endif // PRINT_H
