#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#define MAX_TYPE_NAME_LENGTH 32
#define MAX_CUSTOM_TYPES 64

// Function pointer type for custom print functions
typedef void (*custom_print_fn)(const void* data);

// Function prototypes
void print(const char* format, ...);
void register_custom_type(const char* type_name, custom_print_fn fn);

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

#ifdef PRINT_IMPLEMENTATION

static struct {
    char type_name[MAX_TYPE_NAME_LENGTH];
    custom_print_fn fn;
} custom_types[MAX_CUSTOM_TYPES];

static size_t num_custom_types = 0;

void register_custom_type(const char* type_name, custom_print_fn fn) {
    if (num_custom_types < MAX_CUSTOM_TYPES && strlen(type_name) < MAX_TYPE_NAME_LENGTH) {
        strncpy(custom_types[num_custom_types].type_name, type_name, MAX_TYPE_NAME_LENGTH - 1);
        custom_types[num_custom_types].type_name[MAX_TYPE_NAME_LENGTH - 1] = '\0';
        custom_types[num_custom_types].fn = fn;
        num_custom_types++;
    }
}

static custom_print_fn get_custom_print_fn(const char* type_name) {
    for (size_t i = 0; i < num_custom_types; i++) {
        if (strcmp(custom_types[i].type_name, type_name) == 0) {
            return custom_types[i].fn;
        }
    }
    return NULL;
}

void print(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '{' && *(format + 1) == ':') {
            format += 2; // Skip '{:'
            char type_name[MAX_TYPE_NAME_LENGTH] = {0};
            size_t i = 0;
            while (*format && *format != '}' && i < MAX_TYPE_NAME_LENGTH - 1) {
                type_name[i++] = *format++;
            }
            if (*format == '}') {
                custom_print_fn custom_fn = get_custom_print_fn(type_name);
                if (custom_fn) {
                    void* arg = va_arg(args, void*);
                    custom_fn(arg);
                } else {
                    print_string("{:");
                    print_string(type_name);
                    print_char('}');
                }
            } else {
                print_string("{:");
                print_string(type_name);
            }
        } else if (*format == '{' && *(format + 1) && *(format + 1) != '{') {
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

#endif // PRINT_IMPLEMENTATION

#endif // PRINT_H
