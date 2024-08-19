#include "../print.h"
#include <unistd.h>

// Implement _putchar for your specific environment
void _putchar(char c) {
    write(STDOUT_FILENO, &c, 1);
}

int main() {
    const char* name = "Alice";
    double age = 30.5678;
    int number = 42;
    unsigned int hex_number = 0xABCD;
    
    print("Hello {s}, you are {f:.2} years old.\n", name, age);
    print("Decimal: {d}, Hexadecimal: {x}, Binary: {b}\n", number, hex_number, number);
    print("Character: {c}, String: {s}\n", 'A', "Hello, World!");
    
    // Test NULL string
    print("NULL string: {s}\n", (char*)NULL);
    
    // Test pointer printing
    int* ptr = &number;
    print("Pointer to number: {p}\n", (void*)ptr);
    
    return 0;
}
