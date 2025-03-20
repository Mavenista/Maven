#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <cstdint> // Include this header for fixed-width integer types
#include <cstddef> // For size_t

// Define custom types
using char_p = char*;
using w_char_p = wchar_t*;
using u_size = size_t;

// Define Typing template class
template <typename Wrapper>
class Typing {
public:
    Typing(const std::string& value, Wrapper wrapper) 
        : value(value), wrapper(wrapper) {}

    const std::string& getHeading() const { return value; }
    const Wrapper& getType() const { return wrapper; }

private:
    std::string value;
    Wrapper wrapper;
};

// Define a variant that can hold different types of Typing
using TypingVariant = std::variant<
    Typing<int8_t>, Typing<int16_t>, Typing<int32_t>, Typing<int64_t>,
    Typing<uint8_t>, Typing<uint16_t>, Typing<uint32_t>, Typing<uint64_t>,
    Typing<size_t>, Typing<u_size>,
    Typing<float>, Typing<double>,
    Typing<std::string>, Typing<char>, Typing<char_p>,
    Typing<wchar_t>, Typing<w_char_p>>;

int main() {
    // Create an unordered_map with a string key and TypingVariant as the value
    std::unordered_map<std::string, TypingVariant> typings;

    // Use in-place construction for Typing objects
    typings["i8"]  = Typing<int8_t>("i8", int8_t{});
    typings["i16"] = Typing<int16_t>("i16", int16_t{});
    typings["i32"] = Typing<int32_t>("i32", int32_t{});
    typings["i64"] = Typing<int64_t>("i64", int64_t{});
    

    typings["u8"]  = Typing<uint8_t>("u8", uint8_t{});
    typings["u16"] = Typing<uint16_t>("u16", uint16_t{});
    typings["u32"] = Typing<uint32_t>("u32", uint32_t{});
    /*typings["u64"] = Typing<uint64_t>("u64", uint64_t{});

    // Add Typing objects for size_t and custom u_size
    typings["size_t"] = Typing<size_t>("size_t", size_t{});
    typings["u_size"] = Typing<u_size>("u_size", u_size{});*/

    // Add Typing objects for floating-point types
    typings["float"]  = Typing<float>("float", float{});
    typings["double"] = Typing<double>("double", double{});

    // Add Typing objects for string type
    typings["string"] = Typing<std::string>("string", std::string{});

    // Add Typing objects for char, char pointers, and wide char types
    typings["char"]   = Typing<char>("char", char{});
    typings["char_p"] = Typing<char_p>("char_p", nullptr);
    typings["wchar"]  = Typing<wchar_t>("wchar", wchar_t{});
    typings["w_char_p"] = Typing<w_char_p>("w_char_p", nullptr);

    // Example usage: accessing and using a specific typing (char in this case)
    if (auto* charTyping = std::get_if<Typing<char>>(&typings["char"])) {
        char wrappedValue = (*charTyping).getType(); // Wrap a char value
        std::cout << "Wrapped char value: " << wrappedValue << std::endl;
    }

    // Example usage: accessing and using a wchar typing
    if (auto* wcharTyping = std::get_if<Typing<wchar_t>>(&typings["wchar"])) {
        wchar_t wrappedValue = (*wcharTyping).getType();
        std::wcout << L"Wrapped wchar value: " << wrappedValue << std::endl;
    }

    // Example usage: accessing and using a char pointer typing
    if (auto* charPTyping = std::get_if<Typing<char_p>>(&typings["char_p"])) {
        const char* wrappedValue = (*charPTyping).getType();
        std::cout << "Wrapped char* value: " << wrappedValue << std::endl;
    }

    return 0;
}
