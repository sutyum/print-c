#define PRINT_IMPLEMENTATION
#include "print.h"
#include <unistd.h>

// Implement _putchar for your specific environment
void _putchar(char c) {
    write(STDOUT_FILENO, &c, 1);
}

// Define custom structs
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char name[20];
    int age;
} Person;

// Custom print function for Point
void print_point(const void* data) {
    const Point* p = (const Point*)data;
    print("({}, {})", p->x, p->y);
}

// Custom print function for Person
void print_person(const void* data) {
    const Person* person = (const Person*)data;
    print("{s}, {d} years old", person->name, person->age);
}

int main() {
    const char* name = "Alice";
    double age = 30.5678;
    int number = 42;
    unsigned int hex_number = 0xABCD;
    
    // Register custom types
    register_custom_type("Point", print_point);
    register_custom_type("Person", print_person);
    
    Point p = {10, 20};
    Person person = {"Bob", 25};
    
    print("Hello {s}, you are {f:.2} years old.\n", name, age);
    print("Decimal: {d}, Hexadecimal: {x}, Binary: {b}\n", number, hex_number, number);
    print("Character: {c}, String: {s}\n", 'A', "Hello, World!");
    print("NULL string: {s}\n", (char*)NULL);
    
    int* ptr = &number;
    print("Pointer to number: {p}\n", (void*)ptr);
    
    // Use custom types
    print("Point: {:Point}\n", &p);
    print("Person: {:Person}\n", &person);
    
    return 0;
}
