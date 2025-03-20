#ifndef CPPHEADERPARSER_H
#define CPPHEADERPARSER_H

#include <string>
#include <vector>
#include <any>

// Enumerates the types of symbols we can detect.
enum class SymbolType {
    Macro,
    Constant,
    Assignment,
    Struct,
    Function
};

// Represents a parsed symbol from a header.
struct SymbolEntry {
    SymbolType type;
    std::string name;
    std::string value;   // For macros, constants, or assignments, this is the definition.
    std::string content; // For structs, this is the body.
    std::string dataType;
    std::vector<SymbolEntry> symbols;
    std::vector<std::string> args;
};

class CppHeaderParser {
public:
    // Parses the given header file and returns a vector of symbols.
    std::vector<SymbolEntry> parseFile(const std::string& filename,std::string Content);

private:
    // Utility: Reads an entire file into a string.
    std::string readFile(const std::string& filename);
};

#endif // CPPHEADERPARSER_H