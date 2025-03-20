#ifndef ENUM_TEST_H
#define ENUM_TEST_H

#include <cstdint>  // For fixed-width integer types

// Define the structure matching entryenumT in the LLVM IR
extern "C" {
struct entryenumT {
    int32_t field1;  // i32
    const char* field2;  // ptr (assumed to be a string)
    int32_t field3;  // i32
    int32_t field4;  // i32
};

// Declare the EnumTest function, which takes a const char* and returns an entryenumT

    entryenumT EnumTest(const char* input);  // C-style linkage to prevent name mangling
}

#endif  // ENUM_TEST_H
