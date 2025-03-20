#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <sstream>
#include <cctype>

// Basic variable class with inferred types
class Variable {
public:
    enum Type { INT, STRING };
    Variable(int val) : type(INT), intVal(val) {}
    Variable(const std::string& val) : type(STRING), strVal(val) {}

    Type getType() const { return type; }
    int getInt() const { return intVal; }
    const std::string& getString() const { return strVal; }

private:
    Type type;
    int intVal = 0;
    std::string strVal;
};

// Scope class to manage variables
class Scope {
public:
    Scope(bool main = false){
        main = main;

    }
    void addVariable(const std::string& name, std::shared_ptr<Variable> var) {
        variables[name] = var;
        printVariables();
    }

    std::shared_ptr<Variable> getVariable(const std::string& name) const {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second;
        }
        return nullptr;
    }

    void printVariables() const {
        for (const auto& pair : variables) {
            std::cout << "Variable " << pair.first << " = ";
            if (pair.second->getType() == Variable::INT) {
                std::cout << pair.second->getInt();
            } else {
                std::cout << pair.second->getString() << "\n";
            }
            std::cout << std::endl;
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Variable>> variables;
};

// Interpreter to handle scopes and the file input
class Interpreter {
public:
    void println(const std::string& content){
    std::cout << content;
    std::cout << "\n";
    }
    void enterScope() {
        scopes.push_back(std::make_shared<Scope>());
    }

    void exitScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    void addVariable(const std::string& name, std::shared_ptr<Variable> var) {
        if (!scopes.empty()) {
            scopes.back()->addVariable(name, var);
        }
    }

    std::shared_ptr<Variable> getVariable(const std::string& name) const {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            std::shared_ptr<Variable> var = (*it)->getVariable(name);
            if (var != nullptr) return var;
        }
        return nullptr;
    }

    void printMainScope() const {
        if (!scopes.empty()) {
            std::cout << "Main Scope Variables:\n";
            scopes.front()->printVariables();
        }
    }

    // Lexer to read tokens from the file
    void processLine(const std::string& line) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            // Skip comments
            //std::cout << token;
            if (token == "//") {
                break; // Ignore the rest of the line
            }

            // Enter or exit scope
            if (token == "{") {
                enterScope();
            } else if (token == "}") {
                exitScope();
            } else if (isalpha(token[0])) {
                
                // Process variable assignment (x = "value" or y = 1)
                std::string varName = token;
                std::string equals;
                iss >> equals; // Expect "="
                if (equals != "=") {
                    
                    println(equals);
                    std::cerr << "Syntax error: Expected '=' after variable name\n";
                    return;
                }

                // Determine if it's a string or an integer
                std::string value;
                iss >> value;
            if (value[0] == '"') {
                // String literal
                std::string start = value;

                // Continue appending until the final quote is encountered
                while (start[start.size() - 1] != '"') { // Ensure we get the complete string
                    std::string tempstart;
                    iss >> tempstart;
                    start += " " + tempstart;
                }

                // Now that we have the complete string, remove leading and trailing quotes
                if (start.size() > 1 && start[0] == '"' && start[start.size() - 1] == '"') {
                    start = start.substr(1, start.size() - 2); // Remove both leading and trailing quotes
                } else if (start[0] == '"') {
                    start = start.substr(1); // In case the trailing quote was missing
                }

                
                addVariable(varName, std::make_shared<Variable>(start));
        } else if (isdigit(value[0]) || value[0] == '-') {
                    // Integer literal (default to i32)
                    addVariable(varName, std::make_shared<Variable>(std::stoi(value)));
                } else {
                    std::cerr << "Syntax error: Invalid value assignment\n";
                }
            }
        }
    }

    // Function to read and interpret the file
    void interpretFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        enterScope(); // Start with the main scope
        std::string line;
        while (std::getline(file, line)) {
            processLine(line);
        }
        file.close();
        printMainScope(); // Print the final state of the main scope
        exitScope(); // Clean up
    }

private:
    std::vector<std::shared_ptr<Scope>> scopes; // Stack of scopes
};

// Main function to initialize the interpreter and load a file
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.mvn>\n";
        return 1;
    }

    std::string filename = argv[1];
    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".mvn") {
        Interpreter interpreter;
        interpreter.interpretFile(filename);
    } else {
        std::cerr << "Error: The file must have a .mvn extension.\n";
        return 1;
    }

    return 0;
}
