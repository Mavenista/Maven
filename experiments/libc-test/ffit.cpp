#include <ffi.h>
#include <iostream>
#include <dlfcn.h>

int main() {
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to open libc\n";
        return 1;
    }

    void* printf_func = dlsym(handle, "printf");
    if (!printf_func) {
        std::cerr << "Failed to find printf\n";
        return 1;
    }

    // Prepare ffi
    ffi_cif cif;
    ffi_type* args[1];
    void* values[1];
    const char* format = "Hello from libffi: %s\n";
    const char* str = "World";

    args[0] = &ffi_type_pointer;
    values[0] = (void*)&format;

    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1, &ffi_type_sint, args) != FFI_OK) {
        std::cerr << "Failed to prepare CIF\n";
        return 1;
    }

    ffi_call(&cif, FFI_FN(printf_func), nullptr, values);
    dlclose(handle);

    return 0;
}
