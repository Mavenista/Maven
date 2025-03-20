#include <iostream>
#include <any>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cxxabi.h>
#include <memory>
#include <vector>
#include <sstream>
#include <cctype>
#include <variant>
#include <array>
#include <typeinfo>
#include <chrono>
#include <stdexcept>
#include <cstddef>
#include <type_traits>
#include <cstring>
#include <functional>

std::string demangle(const char* mangledName) {
    int status = -1;
    std::unique_ptr<char, void(*)(void*)> result{
        abi::__cxa_demangle(mangledName, nullptr, nullptr, &status),
        std::free
    };
    if (status != 0) {
        return "Error demangling name";
    }
    return result.get();
}


std::string convertToString(std::any input) {
    try {
        if (input.type() == typeid(std::string)) {
            return std::any_cast<std::string>(input);
        }
        else if (input.type() == typeid(bool)) {
            return (std::any_cast<bool>(input) ? "true": "false");
        }
        else if (input.type() == typeid(const char*)) {
            return std::string(std::any_cast<const char*>(input));
        }
        else if (input.type() == typeid(char)) {
            return std::string(1, std::any_cast<char>(input));
        }
        // Handling various integer types
        else if (input.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(input));
        }
        else if (input.type() == typeid(long)) {
            return std::to_string(std::any_cast<long>(input));
        }
        else if (input.type() == typeid(long long)) {
            return std::to_string(std::any_cast<long long>(input));
        }
        else if (input.type() == typeid(short)) {
            return std::to_string(std::any_cast<short>(input));
        }
        else if (input.type() == typeid(unsigned int)) {
            return std::to_string(std::any_cast<unsigned int>(input));
        }
        else if (input.type() == typeid(unsigned long)) {
            return std::to_string(std::any_cast<unsigned long>(input));
        }
        else if (input.type() == typeid(unsigned long long)) {
            return std::to_string(std::any_cast<unsigned long long>(input));
        }
        else if (input.type() == typeid(unsigned short)) {
            return std::to_string(std::any_cast<unsigned short>(input));
        }   else if (input.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(input));
        } else {
            std::string ret = "Unsupported type!";
            ret = ret + " " + demangle(input.type().name());
            
            return ret;
        }
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Bad any cast: " << e.what() << std::endl;
        return "Bad any cast!";
    }
}

class PtrVal {
    public:
    PtrVal(std::any value):value(value){
        
    }
    std::any getValue(){
        
        return value;}
    void setValue(std::any v){
        if (v.type() != value.type()){
            throw std::runtime_error("Type mismatch expected: " + demangle(value.type().name()) + "\ngot: " + demangle(v.type().name()));
        }
        this->value=v;
    }
    
    private:
    std::any value;
};

class Pointer {
    public:
    Pointer(PtrVal value): ptrvalue(value), isfreed(false){
        this->ptr = &ptrvalue;
        
    }
    std::any getValue() {
        if (isfreed) {
            std::cerr << "Pointer is freed." << std::endl;
            exit(0);
            return nullptr;
        }
        return ptr->getValue();
    }
    PtrVal** getptr(){
        return &ptr;
    }
    Pointer clone(){
        return Pointer(this->getValue());
    }
    std::string printable(){
        std::string v = "Pointer @ <";
        return v + type + ">";
        
    }
    void editValue(std::any value){
        (*this->ptr).setValue(value);
    }
    void free() {
        
        if (!isfreed) {
            isfreed = true;  // Mark as freed
            this->ptr = nullptr;   // Nullify the pointer
            this->ptrvalue=PtrVal(0);
        } else {
            std::cerr << "Pointer is already freed." << std::endl;
        }
    }
    
    private:
    PtrVal ptrvalue;
    PtrVal* ptr;
    std::string type;
    bool isfreed;

};

class BorrowedPointer{
    public:
    BorrowedPointer(PtrVal** ptr,bool mut=false): ptr(ptr),mut(mut){}
    PtrVal** getptr(){return ptr;}
    void editValue(std::any value){
        if (mut == false){
            throw std::runtime_error("Cannot edit immutable borrow of pointers");
        }
        (**ptr).setValue(value);
    }
    
    private:
    PtrVal** ptr;
    bool mut;
};

std::string func(BorrowedPointer obj){
    auto v = (**obj.getptr()).getValue();
    std::string sv = std::any_cast<std::string>(v);
    return sv + " and hello from func!";

}

void funct(BorrowedPointer obj){
    auto v = (**obj.getptr()).getValue();
    std::string sv = std::any_cast<std::string>(v);
    obj.editValue(sv + " and hello from le pointer itself.");
}

Pointer trade(BorrowedPointer obj){
    auto v = (**obj.getptr()).getValue();
    std::string sv = std::any_cast<std::string>(v);
    obj.editValue(std::string("nullptr"));
    return Pointer(PtrVal(sv));
    


}

int main(){
    auto x = PtrVal(std::string("hello world"));
    Pointer ptr = Pointer(x);
    
    auto deref = ptr.getptr();
    std::cout << deref << std::endl;
    std::cout << convertToString((**deref).getValue()) << std::endl;
    std::cout << func(BorrowedPointer(deref)) << std::endl;
    std::cout << convertToString((**deref).getValue()) << std::endl;
    funct(BorrowedPointer(deref,true));
    std::cout << convertToString((**deref).getValue()) << std::endl;
    Pointer traded = trade(BorrowedPointer(deref,true));
    std::cout << convertToString((**deref).getValue()) << std::endl;
    std::cout << convertToString((**(traded.getptr())).getValue()) << std::endl;

    
    return 0;


}