#include <stdio.h>

union Data {
    int i;
    float f;
    char str[20];
};

int main() {
    union Data data;

    // Storing an integer
    data.i = 10;
    printf("data.i: %d\n", data.i);

    // Storing a float (overwrites the integer)
    data.f = 220.5;
    printf("data.f: %f\n", data.f);

    // Storing a string (overwrites the float)
    snprintf(data.str, sizeof(data.str), "Hello, World!");
    printf("data.str: %s\n", data.str);

    // Note: Accessing data.i or data.f now may result in undefined behavior
    // because the string has overwritten their values.

    return 0;
}
