# Single-Header Print Library

This is a lightweight, format-string based printing library implemented as a single header file. It provides printf-like functionality without depending on the standard C library, making it suitable for use in embedded systems or other environments where the full C standard library is not available.

## Features

- Format string based printing similar to printf
- Support for basic types: integers, unsigned integers, hexadecimal, binary, floats, characters, and strings
- Custom type support allowing users to define their own print functions for complex types
- Single header file implementation for easy integration
- No dependencies on the standard C library (except for va_list and basic types)

## Usage

1. Copy `print.h` into your project directory.

2. In **one** C file in your project, define `PRINT_IMPLEMENTATION` before including `print.h`:

   ```c
   #define PRINT_IMPLEMENTATION
   #include "print.h"
   ```

3. In all other C files that use the library, just include `print.h`:

   ```c
   #include "print.h"
   ```

4. Implement the `_putchar` function for your specific environment:

   ```c
   void _putchar(char c) {
       // Implementation depends on your system
       // For example, on Unix-like systems:
       write(STDOUT_FILENO, &c, 1);
   }
   ```

5. Use the `print` function in your code:

   ```c
   print("Hello, {s}! The answer is {d}.\n", "world", 42);
   ```

## Custom Types

You can register custom print functions for your own types:

```c
typedef struct {
    int x;
    int y;
} Point;

void print_point(const void* data) {
    const Point* p = (const Point*)data;
    print("({}, {})", p->x, p->y);
}

int main() {
    register_custom_type("Point", print_point);
    Point p = {10, 20};
    print("Point: {:Point}\n", &p);
    return 0;
}
```

## Format Specifiers

- `{d}` or `{}`: int
- `{u}`: unsigned int
- `{x}`: hexadecimal
- `{b}`: binary
- `{f}`: float or double
- `{s}`: string
- `{c}`: char
- `{p}`: pointer
- `{:typename}`: custom type

## License

This library is released under the MIT License. See the LICENSE file for details.
