#include "Hparser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string, std::string> resolver = {
    {"int","i32"},
    {"unsigned int","u32"},
    {"int8_t","i8"},
    {"uint8_t","u8"},
    {"int16_t","i16"},
    {"uint16_t","u16"},
    {"int32_t","i32"},
    {"uint32_t","i32"},
    {"int64_t","i64"},
    {"uint8_t","u64"},
    {"int8_t","i8"},
    {"float","f32"},
    {"double","f64"},
    {"char*","RawString"},
    {"void","nil"}

};
// Utility function to read a file's entire contents into a string.
std::string CppHeaderParser::readFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::ostringstream ss;
    ss << inFile.rdbuf();
    return ss.str();
}

// Helper to deduce the type of a macro value based on its contents.
std::string deduceTypeForMacro(const std::string& rawValue) {
    std::string value = rawValue;
    // Trim whitespace.
    value.erase(0, value.find_first_not_of(" \t\n\r"));
    value.erase(value.find_last_not_of(" \t\n\r") + 1);

    // If it starts and ends with a quote, assume String.
    if (!value.empty() && value.front() == '"' && value.back() == '"') {
        return "String";
    }

    // Try to detect an integer.
    try {
        size_t pos;
        std::stoi(value, &pos);
        if (pos == value.size())
            return "i32";  // You can adjust this tag as needed.
    } catch (...) {}

    // Try to detect a floating-point number.
    try {
        size_t pos;
        std::stod(value, &pos);
        if (pos == value.size())
            return "f64";  // Or similar floating-point tag.
    } catch (...) {}

    // Fallback type.
    return "unknown";
}
std::string resolve(std::string ty){
    if (resolver.find(ty) == resolver.end()){
        return "unknown";
    } else {
        return resolver[ty];
    }
}
std::vector<SymbolEntry> CppHeaderParser::parseFile(const std::string& filename,std::string Content) {
    std::vector<SymbolEntry> symbols = {};

    std::string content = Content;
    if (Content == ""){
        content = readFile(filename);
    }
    //std::cout << "parsing "  << filename << std::endl;


    // Regex patterns:
    // Macro:   #define SYMBOL value...
    std::regex macroRegex(R"(^\s*#define\s+(\w+)\s+(.*)$)");
    // Constant: const <type> NAME = value;
    std::regex constantRegex(R"(^\s*const\s+([\w:<>]+)\s+(\w+)\s*=\s*(.*);)");
    // Assignment: type NAME = value; (excluding const declarations)
    std::regex assignmentRegex(R"(^\s*(?!const)([\w:<>]+)\s+(\w+)\s*=\s*(.*);)");
    std::regex declareregex(R"(^\s*([\w:<>]+(\s+[\w:<>]+)*)\s+(\w+)\s*;)");
    std::regex typedefRegex(R"(^\s*typedef\s+([\w:<>]+)\s+(\w+)\s*;)");
    // Struct:  struct NAME { ... };
    std::regex structRegex(R"(\bstruct\s+(\w+)\s*\{([^}]*)\}\s*;)");
    std::regex functionRegex(R"(^\s*([\w:<>*&\s]+?)\s+(\w+)\s*\(\s*([^)]*?)\s*\)\s*;)");

    std::regex externFunctionRegex(R"(^\s*extern\s+([\w:<>*&\s]+?)\s+(\w+)\s*\(\s*([^)]*?)\s*\)\s*;)");
    std::regex commentRegex(R"(//.*?$|/\*.*?\*/)");

    //std::cout << "parsing " << std::endl;


    // Process file line-by-line for macros, constants, and assignments.
    auto CleanedContent = std::regex_replace(content, structRegex, "");
    CleanedContent = std::regex_replace(CleanedContent,commentRegex,"");
    //std::cout << "parsing " << std::endl;
    std::istringstream iss(CleanedContent);
    std::string line;
    while (std::getline(iss, line)) {
        //std::cout << "parsing "  << line << std::endl;

        std::smatch match;
        // Check for macros.
        std::regex_search(line, match, macroRegex);
        //std::cout << "parsing still? " << match.size() << std::endl;
        if (std::regex_search(line, match, macroRegex)) {
            //std::cout << "parsing still? " << match.size() << std::endl;
            if (match.size() >= 3) {
                SymbolEntry entry;
                entry.type = SymbolType::Macro;
                entry.name = match[1].str();
                entry.value = match[2].str();
                entry.dataType = deduceTypeForMacro(entry.value);
                if (entry.dataType == "unknown"){
                    continue;
                }
                symbols.push_back(entry);
            }
        }
        else if (std::regex_search(line, match, typedefRegex)) {
            //std::cout << "parsing still tdef" << match.size() << std::endl;
            continue;
            if (match.size() >= 3) {
                SymbolEntry entry;
                entry.type = SymbolType::Assignment;
                // Group 1: declared type, Group 2: variable name, Group 3: value.
                //std::cout << "parsing still tdef" << match.size() << std::endl;
                auto ty = match[1].str();
                while (ty[0] == '_'){
                    ty = ty.substr(1,ty.size()-1);
                }
                //std::cout << "parsing still tdef " << ty << std::endl;
                entry.dataType = resolve(match[1].str());
                if (entry.dataType == "unknown"){
                    continue;
                }
                //std::cout << "parsing still tdef " << entry.dataType << std::endl;
                entry.name = match[2].str();
                //std::cout << "parsing still tdef " << ty << std::endl;
                entry.value = entry.dataType;
                resolver[entry.name] = entry.dataType;
                symbols.push_back(entry);
            }
        }
        
        // Check for constant declarations.
        else if (std::regex_search(line, match, constantRegex)) {
            if (match.size() >= 4) {
                SymbolEntry entry;
                entry.type = SymbolType::Constant;
                // Group 1: declared type, Group 2: name, Group 3: value.
                entry.dataType = resolve(match[1].str());
                if (entry.dataType == "unknown"){
                    continue;
                }
                entry.name = match[2].str();
                entry.value = match[3].str();

                symbols.push_back(entry);
            }
        }
        // Check for normal assignments.
        else if (std::regex_search(line, match, assignmentRegex)) {
            if (match.size() >= 4) {
                SymbolEntry entry;
                entry.type = SymbolType::Assignment;
                // Group 1: declared type, Group 2: variable name, Group 3: value.
                entry.dataType = resolve(match[1].str());
                if (entry.dataType == "unknown"){
                    continue;
                }
                entry.name = match[2].str();
                entry.value = match[3].str();
                symbols.push_back(entry);
            }
        }
        else if (std::regex_search(line, match, declareregex)) {
            
            if (match.size() >= 3) {
                SymbolEntry entry;
                entry.type = SymbolType::Assignment;
                // Group 1: declared type, Group 2: variable name, Group 3: value.
                //std::cout << "decl: " << match[1].str() << (resolver.find(match[1].str()) != resolver.end()) << std::endl;
                //std::cout << match[2].str() << std::endl;
                entry.dataType = resolve(match[1].str());
                if (entry.dataType == "unknown"){
                    continue;
                }
                entry.name = match[2].str();
                entry.value = entry.dataType;
                symbols.push_back(entry);
            }
        }
        else if (std::regex_search(line, match, externFunctionRegex)) {
            
            if (match.size() >= 3) {
                SymbolEntry entry;
                entry.type = SymbolType::Function;
                // Group 1: declared type, Group 2: variable name, Group 3: value.
                //std::cout << "decl: " << match[1].str() << (resolver.find(match[1].str()) != resolver.end()) << std::endl;
                //std::cout << match[2].str() << std::endl;
                std::vector<std::string> args;
                auto res = resolve(match[1].str());
                if (res == "unknown"){
                    continue;
                }
                args.push_back(res); // return type
                entry.name = match[2].str(); // fn name
                int a = 2;
                int argcount = 0;
                std::string resolved;
                while (a != match.size()-1){
                    a++;
                    if (a >= match.size()-1){
                        break;
                    }
                    resolved = resolve(match[a].str());
                    if (resolved == "unknown"){
                        break;
                    }
                    if (a+1 >= match.size()-1){
                        args.push_back("a" + std::to_string(argcount));
                        args.push_back(resolved); // typename?
                    } else {
                        args.push_back(match[a+1].str()); // argname?
                        args.push_back(resolved); // typename?
                    }
                    argcount++;

                }
                if (resolved == "unknown"){
                    continue;
                }
                entry.dataType = "function";

                entry.value = entry.dataType;
                entry.args = args;
                symbols.push_back(entry);
            }
        }
        else if (std::regex_search(line, match, functionRegex)) {
            
            if (match.size() >= 3) {
                SymbolEntry entry;
                entry.type = SymbolType::Function;
                std::vector<std::string> args;
                auto res = resolve(match[1].str());
                if (res == "unknown"){
                    continue;
                }
                args.push_back(res); // return type
                entry.name = match[2].str(); // fn name
                int a = 2;
                int argcount = 0;
                std::string resolved;
                while (a != match.size()-1){
                    a++;
                    std::cout << "a" <<a << std::endl;
                    if (a >= match.size()-1){
                        break;
                    }
                    resolved = resolve(match[a].str());
                    if (resolved == "unknown"){
                        break;
                    }
                    if (a+1 >= match.size()-1){
                        args.push_back("a" + std::to_string(argcount));
                        args.push_back(resolved); // typename?
                    } else {
                        args.push_back(match[a+1].str()); // argname?
                        args.push_back(resolved); // typename?
                    }
                    argcount++;

                }
                if (resolved == "unknown"){
                    continue;
                }
                entry.dataType = "function";

                entry.value = entry.dataType;
                entry.args = args;

                //exit(0);
                // Group 1: declared type, Group 2: variable name, Group 3: value.
                //std::cout << "decl: " << match[1].str() << (resolver.find(match[1].str()) != resolver.end()) << std::endl;
                //std::cout << match[2].str() << std::endl;
                entry.value = entry.dataType;
                symbols.push_back(entry);
            }
        }

        //std::cout << "parsing nothing? " << match.size() << std::endl;
    }

    // For structs, use regex on the entire content.
    //std::cout << "parsing structs" << std::endl;

    auto structsBegin = std::sregex_iterator(content.begin(), content.end(), structRegex);
    auto structsEnd = std::sregex_iterator();
    for (std::sregex_iterator i = structsBegin; i != structsEnd; ++i) {
        std::smatch match = *i;
        if (match.size() >= 3) {
            SymbolEntry entry;
            entry.type = SymbolType::Struct;
            entry.name = match[1].str();
            entry.content = match[2].str();
            entry.symbols = parseFile("",match[2].str());
            // Struct definitions are not given a dataType.
            entry.dataType = "";
            symbols.push_back(entry);
        }
    }
    //std::cout << "returned/parsed " << symbols.size() << " symbols" << std::endl;
    return symbols;
}


std::string symbolTypeToString(SymbolType type) {
    switch (type) {
        case SymbolType::Macro: return "Macro";
        case SymbolType::Constant: return "Constant";
        case SymbolType::Assignment: return "Assignment";
        case SymbolType::Struct: return "Struct";
        default: return "Unknown";
    }
}

int Example() {
    try {
        // Replace "example.h" with the path to your C++ header file.
        auto symbolicator = CppHeaderParser();auto symbols = symbolicator.parseFile("example.h","");

        // Output the parsed symbols.
        for (const auto& symbol : symbols) {
            //std::cout << symbolTypeToString(symbol.type) << " - " << symbol.name;
            if (!symbol.dataType.empty())
                //std::cout << " (Type: " << symbol.dataType << ")";
            if (symbol.type == SymbolType::Struct) {
                //std::cout << "\nStruct Body: "<< "\n";
                for (auto& s: symbol.symbols){
                //std::cout << s.name << "(" << s.dataType << ")\n";
                }
            } else {
                //std::cout << "=" << symbol.value << "\n";
                
            }
            //std::cout << "--------------------------\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}