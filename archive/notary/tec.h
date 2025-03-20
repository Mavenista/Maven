#ifndef ENUM_TEST_H
#define ENUM_TEST_H

#include <inttypes.h>  // For fixed-width integer types

// Define the structure matching entryenumT in the LLVM IR

struct entryenumT {
    int32_t field1;  // i32
    const char* field2;  // ptr (assumed to be a string)
    int32_t field3;  // i32
    int32_t field4;  // i32
};

 
void EnumTest(struct entryenumT* argone,const char* input);  


#endif  // ENUM_TEST_H
