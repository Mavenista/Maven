#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

// Helper function prototypes
VariableType get_type_from_string(const char *type_str);
const char *get_type_name(VariableType type);

// Implementation

// Create a new scope chain
ScopeChain *create_scope_chain() {
    ScopeChain *chain = malloc(sizeof(ScopeChain));
    chain->top = NULL;
    return chain;
}

// Create a new scope
Scope *create_scope() {
    Scope *scope = malloc(sizeof(Scope));
    scope->variables = NULL;
    scope->next = NULL;
    return scope;
}

// Push a new scope onto the chain
void push_scope(ScopeChain *chain) {
    Scope *new_scope = create_scope();
    new_scope->next = chain->top;
    chain->top = new_scope;
}

// Pop the topmost scope from the chain
void pop_scope(ScopeChain *chain) {
    if (chain->top == NULL) return;
    Scope *old_top = chain->top;
    chain->top = old_top->next;
    free_scope(old_top);
}

// Add a variable to a scope
void add_variable(Scope *scope, const char *name, const char *type_str, void *value) {
    VariableType type = get_type_from_string(type_str);
    Variable *var = malloc(sizeof(Variable));
    var->name = strdup(name);
    var->type = type;
    var->next = scope->variables;
    scope->variables = var;

    // Assign the value based on the type
    switch (type) {
        case TYPE_I8: var->value.i8 = *(int8_t *)value; break;
        case TYPE_I16: var->value.i16 = *(int16_t *)value; break;
        case TYPE_I32: var->value.i32 = *(int32_t *)value; break;
        case TYPE_I64: var->value.i64 = *(int64_t *)value; break;
        case TYPE_U8: var->value.u8 = *(uint8_t *)value; break;
        case TYPE_U16: var->value.u16 = *(uint16_t *)value; break;
        case TYPE_U32: var->value.u32 = *(uint32_t *)value; break;
        case TYPE_U64: var->value.u64 = *(uint64_t *)value; break;
        case TYPE_F32: var->value.f32 = *(float *)value; break;
        case TYPE_F64: var->value.f64 = *(double *)value; break;
        case TYPE_CHAR: var->value.c = *(char *)value; break;
        case TYPE_STRING: var->value.string = strdup((char *)value); break;
        default: break;
    }
}

// Remove a variable from a scope
void remove_variable(Scope *scope, const char *name) {
    Variable **current = &scope->variables;
    while (*current) {
        if (strcmp((*current)->name, name) == 0) {
            Variable *to_remove = *current;
            *current = to_remove->next;
            free_variable(to_remove);
            return;
        }
        current = &(*current)->next;
    }
}

// Find a variable in a specific scope
Variable *find_variable(Scope *scope, const char *name) {
    for (Variable *var = scope->variables; var != NULL; var = var->next) {
        if (strcmp(var->name, name) == 0) {
            return var;
        }
    }
    return NULL;
}

// Free a variable
void free_variable(Variable *var) {
    free(var->name);
    if (var->type == TYPE_STRING) {
        free(var->value.string);
    }
    free(var);
}

// Free a scope and all its variables
void free_scope(Scope *scope) {
    Variable *current = scope->variables;
    while (current) {
        Variable *next = current->next;
        free_variable(current);
        current = next;
    }
    free(scope);
}

// Free the entire scope chain
void free_scope_chain(ScopeChain *chain) {
    while (chain->top) {
        pop_scope(chain);
    }
    free(chain);
}

// Get the type from a string
VariableType get_type_from_string(const char *type_str) {
    if (strcmp(type_str, "i8") == 0) return TYPE_I8;
    if (strcmp(type_str, "i16") == 0) return TYPE_I16;
    if (strcmp(type_str, "i32") == 0) return TYPE_I32;
    if (strcmp(type_str, "i64") == 0) return TYPE_I64;
    if (strcmp(type_str, "u8") == 0) return TYPE_U8;
    if (strcmp(type_str, "u16") == 0) return TYPE_U16;
    if (strcmp(type_str, "u32") == 0) return TYPE_U32;
    if (strcmp(type_str, "u64") == 0) return TYPE_U64;
    if (strcmp(type_str, "f32") == 0) return TYPE_F32;
    if (strcmp(type_str, "f64") == 0) return TYPE_F64;
    if (strcmp(type_str, "char") == 0) return TYPE_CHAR;
    if (strcmp(type_str, "string") == 0) return TYPE_STRING;
    return -1; // Invalid type
}

// Get the string representation of a variable type
const char *get_type_name(VariableType type) {
    switch (type) {
        case TYPE_I8: return "i8";
        case TYPE_I16: return "i16";
        case TYPE_I32: return "i32";
        case TYPE_I64: return "i64";
        case TYPE_U8: return "u8";
        case TYPE_U16: return "u16";
        case TYPE_U32: return "u32";
        case TYPE_U64: return "u64";
        case TYPE_F32: return "f32";
        case TYPE_F64: return "f64";
        case TYPE_CHAR: return "char";
        case TYPE_STRING: return "string";
        default: return "unknown";
    }
}

// Get the value of a variable based on its type (from the scope)
void *get_variable(Scope *scope, const char *name) {
    Variable *var = find_variable(scope, name);
    if (var) {
        switch (var->type) {
            case TYPE_I8: return &(var->value.i8);
            case TYPE_I16: return &(var->value.i16);
            case TYPE_I32: return &(var->value.i32);
            case TYPE_I64: return &(var->value.i64);
            case TYPE_U8: return &(var->value.u8);
            case TYPE_U16: return &(var->value.u16);
            case TYPE_U32: return &(var->value.u32);
            case TYPE_U64: return &(var->value.u64);
            case TYPE_F32: return &(var->value.f32);
            case TYPE_F64: return &(var->value.f64);
            case TYPE_CHAR: return &(var->value.c);
            case TYPE_STRING: return var->value.string;
            default: return NULL;
        }
    }
    return NULL;
}

// Example usage
int main() {
    ScopeChain *chain = create_scope_chain();
    
    // Create a scope and push it onto the chain
    push_scope(chain);
    Scope *current_scope = chain->top;

    // Add variables with different types
    int8_t v_i8 = -10;
    add_variable(current_scope, "var_i8", "i8", &v_i8);
    
    int16_t v_i16 = 16;
    add_variable(current_scope, "var_i16", "i16", &v_i16);
    
    int32_t v_i32 = 32;
    add_variable(current_scope, "var_i32", "i32", &v_i32);
    
    int64_t v_i64 = 64;
    add_variable(current_scope, "var_i64", "i64", &v_i64);
    
    uint8_t v_u8 = 255;
    add_variable(current_scope, "var_u8", "u8", &v_u8);
    
    uint16_t v_u16 = 65535;
    add_variable(current_scope, "var_u16", "u16", &v_u16);
    
    uint32_t v_u32 = 4294967295;
    add_variable(current_scope, "var_u32", "u32", &v_u32);
    
    uint64_t v_u64 = 18446744073709551615U;
    add_variable(current_scope, "var_u64", "u64", &v_u64);

    float v_964 = 33.23f;
    add_variable(current_scope, "var_u64", "f32", &v_964);
    
    float v_f32 = 3.14f;
    add_variable(current_scope, "var_f32", "f32", &v_f32);
    
    double v_f64 = 3.14159;
    add_variable(current_scope, "var_f64", "f64", &v_f64);
    
    char v_c = 'A';
    add_variable(current_scope, "var_c", "char", &v_c);
    
    char *v_str = "Hello, World!";
    add_variable(current_scope, "var_str", "string", v_str);

    // Retrieve the variables from the scope
    printf("var_i8: %d\n", *(int8_t *)get_variable(current_scope, "var_i8"));
    printf("var_i16: %d\n", *(int16_t *)get_variable(current_scope, "var_i16"));
    printf("var_i32: %d\n", *(int32_t *)get_variable(current_scope, "var_i32"));
    printf("var_i64: %lld\n", *(int64_t *)get_variable(current_scope, "var_i64"));
    printf("var_u8: %d\n", *(uint8_t *)get_variable(current_scope, "var_u8"));
    printf("var_u16: %d\n", *(uint16_t *)get_variable(current_scope, "var_u16"));
    printf("var_u32: %d\n", *(uint32_t *)get_variable(current_scope, "var_u32"));
    printf("var_u64: %.2f\n", *(float *)get_variable(current_scope, "var_u64"));
    printf("var_f32: %.2f\n", *(float *)get_variable(current_scope, "var_f32"));
    printf("var_f64: %.5f\n", *(double *)get_variable(current_scope, "var_f64"));
    printf("var_c: %c\n", *(char *)get_variable(current_scope, "var_c"));
    printf("var_str: %s\n", (char *)get_variable(current_scope, "var_str"));
    
    // Pop the scope and free resources
    pop_scope(chain);
    free_scope_chain(chain);
    
    return 0;
}
