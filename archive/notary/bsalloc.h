#ifndef BSALLOC_H
#define BSALLOC_H
#include<inttypes.h>

typedef enum {
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_F32,
    TYPE_F64,
    TYPE_CHAR,
    TYPE_STRING,
} VariableType;

typedef struct Variable {
    char *name; // Variable name
    VariableType type; // Type of the variable
    union {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        float f32;
        double f64;
        char c;
        char *string; // Dynamically allocated for strings
    } value;
    struct Variable *next; // Pointer to the next variable in the scope
} Variable;

typedef struct Scope {
    Variable *variables; // Linked list of variables in the scope
    struct Scope *next; // Pointer to the next scope in the chain
} Scope;

typedef struct ScopeChain {
    Scope *top; // The topmost scope in the chain
} ScopeChain;

// Function prototypes
ScopeChain *create_scope_chain();
Scope *create_scope();
void push_scope(ScopeChain *chain);
void pop_scope(ScopeChain *chain);
void add_variable(Scope *scope, const char *name, const char *type_str, void *value);
void remove_variable(Scope *scope, const char *name);
Variable *find_variable(Scope *scope, const char *name);
void free_variable(Variable *var);
void free_scope(Scope *scope);
void free_scope_chain(ScopeChain *chain);
void *get_variable(Scope *scope, const char *name);

#endif