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
#include <iomanip>

class Transpiler {
    public:
    Transpiler(){
        compiled = "";
    }
    void addcontent(std::string con){
        line += con;
    }
    void inverseadd(std::string con){
            line = con + line;
        }
        void endline(){
            compiled += (line +";");
            line = "";
        }
        void finishcompile(){
            compiled += "}";
        }
        std::string getcompiled(){
            return compiled;
        }
        private:
        std::string line = "";
        std::string compiled = "";
    };
int main() {
    auto TP = Transpiler();
    TP.addcontent("1 + 1");
    TP.inverseadd("i32 x =");
    TP.endline();
    TP.finishcompile();
    std::cout << TP.getcompiled();
    std::shared_ptr<std::string> x = std::make_shared<std::string>(std::string("Hello World"));

    std::shared_ptr<std::string>* y = &x;
    **y = (*(*y)) + " & Me!";
    int32_t z = {1+1};

}





