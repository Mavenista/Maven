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
#include <thread>
#include <stdexcept>
#include <cstddef>
#include <type_traits>
#include <cstring>
#include <functional>
#include <iomanip>
#include <polaroid.h>
#include <map>
#include <sjson.hpp>
#include<ordered_map.h>
#include <typeindex>
#include <Hparser.h>

const std::string red_block = "\033[31m";    // Red blk
const std::string green_block = "\033[32m";  // Green blk
const std::string yellow_block = "\x1b[33m";
const std::string reset = "\033[0m";
const std::string fcircle = "●";
const std::string circle = "◯";
std::string result = "";
int warns = 0;
std::vector<int> vidids;
auto debug = true;
tsl::ordered_map<std::string,std::vector<char>> objects;
tsl::ordered_map<int,bool> isunsafedecl;
tsl::ordered_map<int,bool> issafedecl;
tsl::ordered_map<std::string,std::string> ptrnowptsto;
std::vector<int> hasbeenwarned;
auto aassign = true;
bool compile = true;
bool compileflag = true;
auto autosavedebug =true;
std::string logcon;
tsl::ordered_map<std::string,int> ids;
tsl::ordered_map<std::string,std::vector<std::string>> notes;
tsl::ordered_map<int,std::shared_ptr<type_instruction>> forwards;
tsl::ordered_map<int,std::any> forwardscplex;
tsl::ordered_map<int,std::any> forwardscplex2;
std::unordered_map<std::string,std::string> exports;
std::vector<int> isused;

// Cantor Pairing function
// (x+y) (x+y+1)
// ------------- + y
//        2
uint64_t CPf( uint64_t x,uint64_t y){
    if (x>y){std::swap(x,y);}
    return (((x+y) * (x+y+1))/2) + y;

}

void savelog(){
    if (debug){

    std::ofstream LogFile("maven.log");
    LogFile << logcon + "Saved log!";
    LogFile.close();
    }
}

void logat(std::string con, std::string from){
    if (debug){
    logcon += ("@" + from +" -> " + con + "\n");
    if (autosavedebug){
        savelog();
    }
    }
}

// debug/helper functions
std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;


    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    
    return result;
}


std::vector<int> isbadnode;
std::vector<int> issig;

bool in(std::string inp, std::vector<std::string> op){
    for (int i = 0; i < op.size() ; i ++){
    if (inp == op[i]){
        return true;
            //Do Stuff
    }}
    return false;
}
template <typename T>
bool inT(T inp, std::vector<T> op){
    for (int i = 0; i < op.size() ; i ++){
    if (inp == op[i]){
        return true;
            //Do Stuff
    }}
    return false;
}
template <typename T, typename K>
const T* reversefind(tsl::ordered_map<T,K> pool, K reversevalue,bool* found=nullptr){
    for (auto& p : pool){
        if (p.second == reversevalue){
            if (found){
                if (typeid(K) == typeid(std::string)){
                }
                *found = true;
            }
            const T* v = &p.first;
        
            return v;

        }
    }
    return nullptr;
    
}

template <typename T>
std::string debugvectorToString(const std::vector<T>& vec) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) {
            oss << ", "; 
        }
    }
    return oss.str();
}
// ANSI escape codes for styling
const std::string RED = "\033[31m";
const std::string LIGHT_BLUE = "\033[94m";
const std::string BOLD = "\033[1m";
const std::string RESET = "\033[0m";

bool isdeclunsafe = false;
bool isdeclsafe = false;
std::vector<std::string> HashTagOptions;

 

std::string replace(std::string s, std::string from, std::string to)
{
    if(!from.empty())
        for(std::string::size_type pos = 0; (pos = s.find(from, pos) + 1); pos += to.size())
            s.replace(--pos, from.size(), to);
    return s;
}


uint64_t hexStringToNumber(const std::string& hexString) {
    uint64_t number = 0;
    
    
    std::stringstream ss;

    
    ss << std::hex << hexString;

    
    ss >> number;

    return number;
}

uint64_t binaryStringToNumber(const std::string& binString){
    std::string stream = replace(binString,"0b","");
    uint64_t bin = 0;
    for (int i=0;i!=stream.size();i++){
        bin = (bin*2) + std::stoi(std::string(1,stream[i]));
    }
    return bin;

}

uint64_t octalStringToNumber(const std::string& binString){
    std::string stream = replace(binString,"0o","");
    uint64_t bin = 0;
    for (int i=0;i!=stream.size();i++){
        bin = (bin*8) + std::stoi(std::string(1,stream[i]));
    }
    return bin;

}

int64_t safeStringToInt64(const std::string& str) {
    try {
        size_t pos;
        int64_t value = std::stoll(str, &pos);


        if (pos != str.length()) {
            throw std::invalid_argument("Input contains non-numeric characters");
        }

        return value;
    } catch (const std::invalid_argument& e) {
        //println("FAIL");
        
    } catch (const std::out_of_range& e) {
        //println("FAIL2");
        exit(0);
    }

    return NULL; 
}

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



std::string removeTrailingZeros(const std::string& input) {

    
    double value = std::stod(input);
    

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value;
    std::string result = oss.str();
    

    size_t pos = result.find_last_not_of('0');
    if (pos != std::string::npos && result[pos] == '.') {
        result = result.substr(0, pos); 
    } else if (pos != std::string::npos) {
        result = result.substr(0, pos + 1); 
    }
    
    return result;
}

class ArgParser {
    public:
    ArgParser(char** argv,int argc): argv(argv), argc(argc){
    
    }
    bool has(std::string value){
        for (int i = 0; i != argc; i++){
            if (value == std::string(argv[i])){
                return true;
            }
        }
        return false;
    }
    void parse(){
        for (int i = 0; i != argc; i++){
            auto key = std::string(argv[i]);
            if(in(key,sflags)){

                std::string val;
                if (argv[i+1][0] == '\''){
                    while (argv[i+1][std::string(argv[i+1]).size()-1] != '\'' ){
                        val += std::string(argv[i+1]) + " ";
                        i++;

                    }
                    val += std::string(argv[i+1]);
                    val = replace(val,"'","\"");
                } else {
                    val = argv[i+1];
                }
                if (has("-sflags") || has("-flags")){
                    std::cout << key << " : " << val << std::endl;
                }
                if (values.find(key) == values.end()){
                    values[key] = std::vector<std::string>{std::string(val)};
                } else {
                    values[key].push_back(val);
                }

            i++;
            } else {
                if (has("-flags")){
                    std::cout << std::string(argv[i]) << std::endl;
                }
            }
            
        }

    }

    tsl::ordered_map<std::string,std::vector<std::string>> values;
    std::vector<std::string> sflags{"linklib","link","emit","linkpath","output","compilerflags","target","compile","ccflags","typing","toolchain","cc","headerpath","deckpath"};
    char** argv;
    int argc;
    private:
};
char** chardummy = new char*[3];
auto AP = ArgParser(chardummy,9);





// Error helper function
std::string readLine(std::string filepath, int lineNumber,bool isnew=true) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + filepath);
    }
    
    std::string line;
    for (int i = 1; i <= lineNumber && std::getline(file, line); ++i) {
        if (i == lineNumber) {
            file.close();
            return line;
        }
    }
    file.close();
    /*if (isnew){
        return readLine(filepath,lineNumber-1,false);
    }*/
    //throw std::runtime_error("Unable to get line " + std::to_string(lineNumber));
    return "";
}

tsl::ordered_map<std::string, std::string> gerr(std::vector<std::string> g){
    tsl::ordered_map<std::string, std::string> x;
    for (size_t i = 0; i < g.size(); ++i) {
        x[g[i]] = g[i+1];
        ++i;
    }
    return x;
}
// Error displayer function
void displayError(std::string error,int errorLine, int startPos, int endPos,
                  tsl::ordered_map<std::string, std::string> notes,bool warning=false) {
        logat(error,"dE");
    try {
        if (warning && AP.has("-nowarns")){
            warns += 1;
            return;
        }

        std::string filepath = AP.values["compile"][0];
        std::string errorLineContent = readLine(filepath, errorLine);
        std::string beforeLine = errorLine > 1 ? readLine(filepath, errorLine - 1) : "...";
        std::string afterLine = readLine(filepath, errorLine + 1).empty() ? "..." : readLine(filepath, errorLine + 1);
        std::string idr = " ERROR: ";
        if (warning){
            idr = " WARNING: ";
            
        }

        std::cout << BOLD << (!warning ? (red_block + fcircle + reset) : (yellow_block + circle + reset)) <<" " << RESET << filepath << " [" + std::to_string(errorLine)  + ":" + std::to_string(startPos) + ".." + std::to_string(endPos) + "]\n";


        if (!beforeLine.empty() && errorLine-1 > 0) {
            std::cout << LIGHT_BLUE << errorLine - 1 << RESET << std::string(4-std::to_string(errorLine-1).size(), ' ') + "|     " << beforeLine << "\n";
        }
        std::cout << LIGHT_BLUE << errorLine << RESET << std::string(4-std::to_string(errorLine).size(), ' ') + "|     " << errorLineContent << "\n";

        
        if (startPos > endPos){
            endPos = errorLineContent.size();
        }
        std::cout << std::string(4, ' ') + "|     " << std::string(startPos, ' ') << RED << std::string(endPos - startPos, '^') << RESET<< idr << error << "\n";

        if (!afterLine.empty()) {
            std::cout << LIGHT_BLUE << errorLine + 1 << RESET << std::string(4-std::to_string(errorLine+1).size(), ' ') + + "|     " << afterLine << "\n";
        }
        if (notes.find("reason") != notes.end()){
            std::cout << BOLD << "reason" << ": " << RESET << notes["reason"] << "\n";
            notes.erase("reason");
        }
        if (notes.find("note") != notes.end()){
            std::cout << BOLD << "note" << ": " << RESET << notes["note"] << "\n";
            notes.erase("note");
        }
        if (notes.find("[X]") != notes.end()){
            std::cout << BOLD << "Example" << ": " << RESET << "\n";
            std::cout << BOLD << "[X]" << ": " << RESET << notes["[X]"] << "\n";
            notes.erase("[X]");
            std::cout << BOLD << "[O]" << ": " << RESET << notes["[O]"] << "\n";
            notes.erase("[O]");
        }

        for (const auto& [key, value] : notes) {
            std::cout << BOLD << key << ": " << RESET << value << "\n";
        }
        if (!warning){
            std::cout <<  red_block + fcircle + reset + " Build failed." << std::endl;
            exit(0);
        }

    } catch (const std::exception& e) {
        std::cout << RED << "Failed to display error: '" << error << "' " << e.what() << RESET << "\n";
        exit(0);
    }
}



bool compareAnyTy(std::any a, std::any b);






tsl::ordered_map<std::string,std::string> ruleset;
std::vector<std::string> allowances;





// Helper function to print a string (and spit it out to maven.log)
void println(std::string content) {
    if (content == "my_str2"){
        int* dat = nullptr;
        *dat = 10;
    }
    std::cout << content  << "\n";
    logat(content,"println");
}



// converter functions for interpreter
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
        else if (input.type() == typeid(signed char)) {
            return std::to_string(std::any_cast<signed char>(input));
        }
        else if (input.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(input));
        }
        else if (input.type() == typeid(unsigned char)) {
            return std::to_string(static_cast<int>(std::any_cast<unsigned char>(input)));
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
            return removeTrailingZeros(std::to_string(std::any_cast<double>(input)));
        }    else if (input.type() == typeid(float)) {
            return removeTrailingZeros(std::to_string(std::any_cast<float>(input)));
        
        }  else if (demangle(input.type().name()) == "nullification") {
            return "none";
        
        } else {
            std::string ret = "Unsupported type!";
            ret = ret + " Type:: " + demangle(input.type().name());
            return ret;
        }
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Bad any cast: " << e.what() << std::endl;
        return "Bad any cast!";
    }
}





class ObjectBundler {
public:
    // Bundles the library.
    // The format is:
    // [4 bytes magic "DECK"]
    // [4 bytes uint32_t: metadata blob size]
    // [metadata blob: number of pairs, then for each: key length, key, value length, value]
    // [object file content]
    bool bundle(const std::string &objFilePath,
                const std::unordered_map<std::string, std::string> &metadata,
                const std::string &outputFilePath)
    {

        std::ifstream objFile(objFilePath, std::ios::binary);
        if (!objFile) {
            std::cerr << "Error opening object file: " << objFilePath << "\n";
            return false;
        }
        std::vector<char> objContent((std::istreambuf_iterator<char>(objFile)),
                                      std::istreambuf_iterator<char>());
        objFile.close();

        std::stringstream metaStream(std::ios::in | std::ios::out | std::ios::binary);
        // no. of keyvalue pairs
        uint32_t pairCount = static_cast<uint32_t>(metadata.size());
        writeUint32(metaStream, pairCount);

        for (const auto &pair : metadata) {
            writeString(metaStream, pair.first);
            writeString(metaStream, pair.second);
        }
        std::string metaDataBlob = metaStream.str();
        uint32_t metaBlobSize = static_cast<uint32_t>(metaDataBlob.size());



        std::ofstream outFile(outputFilePath, std::ios::binary);
        if (!outFile) {
            std::cerr << "Error opening output file: " << outputFilePath << "\n";
            return false;
        }
        // Magic bytes here.
        outFile.write("DECK", 4);

        writeUint32(outFile, metaBlobSize);

        outFile.write(metaDataBlob.data(), metaBlobSize);

        if (!objContent.empty()) {
            outFile.write(objContent.data(), objContent.size());
        }
        outFile.close();
        return true;
    }

    // Unbundler
    // It reads the header, extracts the metadata blob (parsing the key-value pairs)
    // and then reads the remaining bytes as the object file content.
    bool unbundle(const std::string &bundleFilePath,
                  std::unordered_map<std::string, std::string>* pmetadata,
                  std::vector<char> *pobjContent)
    {   
        std::unordered_map<std::string, std::string> metadata;
        std::vector<char> objContent;
        std::ifstream inFile(bundleFilePath, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error opening bundle file: " << bundleFilePath << "\n";
            return false;
        }
        char magic[4];
        inFile.read(magic, 4);
        if (std::string(magic, 4) != "DECK") {
            std::cerr << "Invalid bundle file format\n";
            return false;
        }
        // Read blob size.
        uint32_t metaBlobSize = readUint32(inFile);

        std::vector<char> metaBuffer(metaBlobSize);
        inFile.read(metaBuffer.data(), metaBlobSize);
        std::stringstream metaStream(std::string(metaBuffer.begin(), metaBuffer.end()),
                                     std::ios::in | std::ios::binary);

        uint32_t pairCount = readUint32(metaStream);
        for (uint32_t i = 0; i < pairCount; i++) {
            std::string key = readString(metaStream);
            std::string value = readString(metaStream);
            metadata[key] = value;
        }

        objContent.assign((std::istreambuf_iterator<char>(inFile)),
                           std::istreambuf_iterator<char>());
        inFile.close();
        *pmetadata = metadata;
        *pobjContent = objContent;
        return true;
    }

private:
    // Helpers
    void writeUint32(std::ostream &os, uint32_t value) {
        os.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
    uint32_t readUint32(std::istream &is) {
        uint32_t value;
        is.read(reinterpret_cast<char *>(&value), sizeof(value));
        return value;
    }

    void writeString(std::ostream &os, const std::string &str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        writeUint32(os, len);
        os.write(str.data(), len);
    }

    std::string readString(std::istream &is) {
        uint32_t len = readUint32(is);
        std::string str(len, '\0');
        is.read(&str[0], len);
        return str;
    }
};

// more converters


std::any convertToInt8(const std::any& value) {
    
    if (value.type() == typeid(int32_t)) {
        return static_cast<int8_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<int8_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<int8_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<int8_t>(std::any_cast<double>(value));
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToInt16(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<int16_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<int16_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<int16_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<int16_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<int16_t>(std::any_cast<double>(value));
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToInt32(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<int32_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<int32_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<int32_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<int32_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<int32_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<int32_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<int32_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<int32_t>(std::any_cast<double>(value));
    } else {
        //println(value.type().name());
        throw std::bad_any_cast();
    }
}

std::any convertToInt64(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<int64_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<int64_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<int64_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<int64_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<int64_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<int64_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<int64_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<int64_t>(std::any_cast<double>(value));
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToUInt8(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<uint8_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<uint8_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<uint8_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<uint8_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<uint8_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<uint8_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<uint8_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<uint8_t>(std::any_cast<double>(value));
    } else {
        //println(demangle(value.type().name()));
        throw std::bad_any_cast();
    }
}

std::any convertToUInt16(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<uint16_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<uint16_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<uint16_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<uint16_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<uint16_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<uint16_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<uint16_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<uint16_t>(std::any_cast<double>(value));
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToUInt32(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<uint32_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<uint32_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<uint32_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<uint32_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<uint32_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<uint32_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<uint32_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<uint32_t>(std::any_cast<double>(value));
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToUInt64(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<uint64_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<uint64_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<uint64_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<uint64_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<uint64_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<uint64_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<uint64_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<uint64_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<uint64_t>(std::any_cast<double>(value));
    } else {
        
        throw std::bad_any_cast();
    }
}

std::any convertToFloat(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<float>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<float>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<float>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<float>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<float>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<float>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<float>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<float>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(float)) {
        return std::any_cast<float>(value);
    } else if (value.type() == typeid(double)) {
        return static_cast<float>(std::any_cast<double>(value));
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToDouble(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<double>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<double>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<double>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<double>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<double>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<double>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<double>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<double>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(float)) {
        return static_cast<double>(std::any_cast<float>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<double>(std::any_cast<double>(value));
    } else if (value.type() == typeid(ssize_t)) {
        return static_cast<double>(std::any_cast<float>(value));
    } else if (value.type() == typeid(size_t)) {
        return static_cast<double>(std::any_cast<double>(value));
    } else if (value.type() == typeid(std::string)) {
        return std::stod(std::any_cast<std::string>(value));
    } else {
        //println(demangle(value.type().name()));
        throw std::bad_any_cast();
    }
}

std::any convertToStr(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return std::to_string(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return std::to_string(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return std::to_string(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return std::to_string(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return std::to_string(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return std::to_string(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return std::to_string(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return std::to_string(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(float)) {
        return std::to_string(std::any_cast<float>(value));
    } else if (value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(value));
    } else if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToChar(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<char>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<char>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<char>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<char>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<char>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<char>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<char>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<char>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(float)) {
        return static_cast<char>(std::any_cast<float>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<char>(static_cast<int>(std::any_cast<double>(value)));
    } else if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value)[0];
    } else {
        throw std::bad_any_cast();
    }
}

std::any convertToWChar(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<wchar_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<wchar_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<wchar_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<wchar_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<wchar_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<wchar_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<wchar_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<wchar_t>(std::any_cast<uint64_t>(value));
    } else if (value.type() == typeid(float)) {
        return static_cast<wchar_t>(std::any_cast<float>(value));
    } else if (value.type() == typeid(double)) {
        return static_cast<wchar_t>(std::any_cast<double>(value));
    } else if (value.type() == typeid(std::string)) {
        return static_cast<wchar_t>(std::any_cast<std::string>(value)[0]);
    } else {
        throw std::bad_any_cast();
    }
}

ssize_t convertToSSize(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        return static_cast<ssize_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        return static_cast<ssize_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        return static_cast<ssize_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        return static_cast<ssize_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<ssize_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<ssize_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<ssize_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        // Prevent casting large unsigned values to signed ssize_t
        if (std::any_cast<uint64_t>(value) > static_cast<uint64_t>(std::numeric_limits<ssize_t>::max())) {
            throw std::overflow_error("Value too large to fit into ssize_t");
        }
        return static_cast<ssize_t>(std::any_cast<uint64_t>(value));
    } else {
        throw std::bad_any_cast();
    }
}

// Helper function for size_t conversion (unsigned size type)
size_t convertToSize(const std::any& value) {
    if (value.type() == typeid(int8_t)) {
        if (std::any_cast<int8_t>(value) < 0) throw std::overflow_error("Negative value cannot fit in unsigned size_t");
        return static_cast<size_t>(std::any_cast<int8_t>(value));
    } else if (value.type() == typeid(int16_t)) {
        if (std::any_cast<int16_t>(value) < 0) throw std::overflow_error("Negative value cannot fit in unsigned size_t");
        return static_cast<size_t>(std::any_cast<int16_t>(value));
    } else if (value.type() == typeid(int32_t)) {
        if (std::any_cast<int32_t>(value) < 0) throw std::overflow_error("Negative value cannot fit in unsigned size_t");
        return static_cast<size_t>(std::any_cast<int32_t>(value));
    } else if (value.type() == typeid(int64_t)) {
        if (std::any_cast<int64_t>(value) < 0) throw std::overflow_error("Negative value cannot fit in unsigned size_t");
        return static_cast<size_t>(std::any_cast<int64_t>(value));
    } else if (value.type() == typeid(uint8_t)) {
        return static_cast<size_t>(std::any_cast<uint8_t>(value));
    } else if (value.type() == typeid(uint16_t)) {
        return static_cast<size_t>(std::any_cast<uint16_t>(value));
    } else if (value.type() == typeid(uint32_t)) {
        return static_cast<size_t>(std::any_cast<uint32_t>(value));
    } else if (value.type() == typeid(uint64_t)) {
        return static_cast<size_t>(std::any_cast<uint64_t>(value));
    } else {
        throw std::bad_any_cast();
    }
}

// Map of converters
tsl::ordered_map<std::string, std::function<std::any(const std::any&)>> converters = {
    {"i8", convertToInt8},
    {"i16", convertToInt16},
    {"i32", convertToInt32},
    {"i64", convertToInt64},
    {"u8", convertToUInt8},
    {"u16", convertToUInt16},
    {"u32", convertToUInt32},
    {"u64", convertToUInt64},
    {"f32", convertToFloat},
    {"f64", convertToDouble},
    {"RawString", convertToString},
    {"Character", convertToChar},
    {"wchar", convertToWChar}, // needs patching
    {"isize", convertToSSize},
    {"usize", convertToSize}
};
// various value structs
struct nullification {

        bool operator==(const nullification& other) const {
        return (*this == other);
    }

    // Overloading the != operator as a member function
    bool operator!=(const nullification& other) const {
        return !(*this == other);  // reuse the == operator
    }
};

struct nullificationptr {

        bool operator==(const nullification& other) const {
        return (*this == other);
    }

    // Overloading the != operator as a member function
    bool operator!=(const nullification& other) const {
        return !(*this == other);  // reuse the == operator
    }
};
struct NoAssign {

        bool operator==(const nullification& other) const {
        return (*this == other);
    }

    // Overloading the != operator as a member function
    bool operator!=(const nullification& other) const {
        return !(*this == other);  // reuse the == operator
    }
};

struct nullificationv {

        bool operator==(const nullification& other) const {
        return (*this == other);
    }

    // Overloading the != operator as a member function
    bool operator!=(const nullification& other) const {
        return !(*this == other);  // reuse the == operator
    }
};

tsl::ordered_map<std::string,std::shared_ptr<Instruction>> dummyinstructions = {{"none",std::make_shared<resource_instruction>("none",nullptr)},{"null",std::make_shared<resource_instruction>("Null",nullptr)}};
 // primitive types
tsl::ordered_map<std::string, std::any> dummyValues = {
        {"i8", static_cast<std::any>(static_cast<int8_t>(0))}, // i8
        {"i16", static_cast<std::any>(static_cast<int16_t>(0))}, // i16
        {"i32", static_cast<std::any>(static_cast<int32_t>(0))}, // i32
        {"i64", static_cast<std::any>(static_cast<int64_t>(0))}, // i64
        {"u8", static_cast<std::any>(static_cast<uint8_t>(0))}, // u8
        {"u16", static_cast<std::any>(static_cast<uint16_t>(0))}, // u16
        {"u32", static_cast<std::any>(static_cast<uint32_t>(0))}, // u32
        {"u64", static_cast<std::any>(static_cast<uint64_t>(0))}, // u64
        {"f32", static_cast<std::any>(0.0f)}, // f32
        {"f64", static_cast<std::any>(0.0)} , // f64
        {"RawString", std::any(std::string("example string"))}, // String (C style)
        
        {"Character", std::any('A')}, // char
        {"Bool",std::any(true)},
        {"nil",std::any(nullificationv{})}
        //{"wchar", std::any(L'あ')} // wchar (Japanese character lol)
};





struct Object;
class MappedFunction;
class Tag;
class UnMappedFunction;
struct List;
struct TypeSafeList;
struct Tuple;
class StructDecl;
class StructInstance;
class Enum;
class EnumKey;
class Pointer;
class BorrowedPointer;
class Class;
class Module;


std::string getTypeName(const std::any& value,bool cb = false,bool tp=false);





// Template class for specific types

class Typing {
public:
    Typing(const std::string& name)
        : name(name){
    }
    std::shared_ptr<Typing> clone() {
        return std::make_shared<Typing>(*this);
    }

    std::string getName()  { return name; }





    std::any convert(const std::any& v,std::string typekey) {

    
    logat("Typing convert for std::any called for type: " + getTypeName(v) + " to " + typekey,"Typing.c");
    auto conv = converters.find(typekey);
    if (conv == converters.end()) {
        logat("Converter for type " + typekey + " not found!","Typing.c");
        return {};  // Return empty if no converter found
    }
    if (getTypeName(v) == typekey){
        return v;
    }
    
    
    if (v.type() == typeid(int64_t)){
        //println("int test payload");
        std::any test;
        if (typekey != "i32"){
        test = conv->second(1);
        } else {
            test = conv->second(int16_t(1));
        }
        if (v.type() == test.type()){
            //println("true");
            return v;
        }
    }
    return conv->second(v);
}

private:
    std::string name;
};


std::shared_ptr<Typing> getTyping(const std::string& typeName) {
    static tsl::ordered_map<std::string, std::shared_ptr<Typing>> typings;


    if (typings.empty()) {
        typings["i8"]  = std::make_shared<Typing>("i8");
        typings["i16"] = std::make_shared<Typing>("i16");
        typings["i32"] = std::make_shared<Typing>("i32");
        typings["i64"] = std::make_shared<Typing>("i64");
        typings["u8"]  = std::make_shared<Typing>("u8");
        typings["u16"] = std::make_shared<Typing>("u16");
        typings["u32"] = std::make_shared<Typing>("u32");
        typings["u64"] = std::make_shared<Typing>("u64");
        typings["f32"] = std::make_shared<Typing>("f32");
        typings["f64"] = std::make_shared<Typing>("f64");
        typings["RawString"] = std::make_shared<Typing>("RawString");
        typings["Number"] = std::make_shared<Typing>("Number");
        typings["char"] = std::make_shared<Typing>("char");
        typings["wchar"] = std::make_shared<Typing>("wchar");
    }

    auto it = typings.find(typeName);
    if (it != typings.end()) {
        return it->second->clone();
    }
    return nullptr;
}


int idctr = 0;
tsl::ordered_map<int,std::vector<int>> lpos; // line & pos for errorisms
class ASTNode {
public:
    ASTNode(): id(++idctr){}
    std::string getPointsTO(){return pointsTO;}
    void setPointsTO(std::string x){this->pointsTO = x;}
    virtual ~ASTNode() = default;
    int id;
    std::string pointsTO;
};


// Further value structs to be used in the interpreter (can be struct or class)


struct Object {
public:
    Object(std::any value, std::string type,bool negative=false) : value(value), type(type), nve(negative) {
        if (value.type() == typeid(Object)){
            this->value = std::any_cast<Object>(value).GetStore();
        }
    }

    bool isString(){
        try {
            std::any_cast<std::string>(value);
            return true;
        } catch (const std::bad_any_cast&) {
            return false;
        }        
    }

    std::string getString() {
        try {
            return std::any_cast<std::string>(value);
        } catch (const std::bad_any_cast&) {
            std::string str = "Stored value is not a string.";
            str = str+ " Value is: " + value.type().name();
            throw std::runtime_error(str);
        }
    }
    std::string getType() const { return type;}
    std::any GetStore() {
        if (convertedvalue.has_value() == true){
            logat("free return","Object.GS");
            

            return convertedvalue;
        }
        logat("Beginning obj conversion","Object.GS");
        std::any ret = GetStoreFn();
        
        this->convertedvalue = ret;

        logat("Successful conversion","Object.GS");
        return ret;
    }
    double max(std::string type){
        if (type[0] == 'f'){
            
        }
        if (type[0] == 'u'){
            return static_cast<double>((1ULL  << std::stoi(type.substr(1))-1)*2 - 1);
        } else {
            return static_cast<double>((1ULL  << std::stoi(type.substr(1))-1) - 1);
        }
    } // better to just make a constants list instead of constantly calling
    std::any GetStoreFn() {
        std::vector<std::string> inttypeNames = {
        "i8", "i16", "i32", "i64",    // Signed integer types
        "u8", "u16", "u32", "u64",    // Unsigned integer types
        "f32", "f64",                 // Floating-point types
        "isize","usize"
        };
        auto typer = getTyping(type);  // Get the typing for the current type
        std::string valueStr;
        
        // Convert the value to a string if it's a string or using the convertToString function
        logat("isString: " + convertToString(isString()),"Object.GSF");
        auto iss = isString();
        if (iss) {
            valueStr = getString();
        } else {
            valueStr = convertToString(value);
        }
        logat("PD -> " +getTypeName(value)  + " -> " + type ,"Object.GSF");
        
        if (in(getTypeName(value),inttypeNames) == false){
            iss = true;
            
        }
        logat("isString: " + convertToString(iss) + "\n" + getTypeName(value),"Object.GSF");
        double converted;
        if (iss == false){
        try {



        // Try to convert the string to an integer
        converted = std::any_cast<double>(convertToDouble(valueStr));  // Attempt conversion to int
        } catch(...){
            logat("F","Object.GSF");
            converted= NULL;
        }
        logat("FD","Object.GSF");
        
        //std::cout << "max: " << type << ": " << max(type) << std::endl;
        if (in(type,inttypeNames) == true && converted > max(type)){
            throw std::runtime_error("Value exceeds the maximum of " + type + "\nMax of: " + convertToString(max(type)));
        }
        logat("FM","Object.GSF");
        

        if (nve == true){
            //println("NVEEE!");
            converted = -1*converted;
        }
        } else {
            converted = NULL;
        }
        //std::cout << nve << std::endl;
        std::any ret;
        //println("Just about to convert!");

        // Check if conversion to integer was successful
        if (converted == NULL && type != "f64" && type != "f32" && iss == true) {
            // If `*p` is non-null, conversion failed, treat it as a string
            //println("String detected, performing string conversion");
            logat("RawString detected, performing string conversion to " + type + " iss:" + std::to_string(iss),"Object.GSF");
            ret = typer->convert(std::any(valueStr),type);  // Convert the string using the appropriate typing
        } else if (type == "f64"){
            if (nve == true){
            
            return static_cast<double>(-1)*std::any_cast<double>(typer->convert(value,"f64"));
            } else {
                return typer->convert(value,"f64");
            }
        } else if (type == "f32"){
            if (nve == true){
            
            return static_cast<float>(-1)*std::any_cast<float>(typer->convert(value,"f32"));
            } else {
                return typer->convert(value,"f32");
            }
        } else {
            // Conversion succeeded, treat it as an integer
            logat("Integer detected, performing integer conversion to " + type,"Object.GSF");
            //println("Integer detected, performing integer conversion!");
            ret = typer->convert(std::any(converted),type);  // Convert the integer using the appropriate typing
        }
        

        //println("Conversion done!");

        return ret;
    }
    std::string printable(){
        std::string valueStr;
        if (isString() == true){
        valueStr = getString();
        } else {
            valueStr = convertToString(this->GetStore());
        }
        //println("RETURNED");
        return valueStr;
    }
    void setNVE(bool n){
        //println("NVE SET");
        
        this->nve = n;
    }
    void setType(std::string newtype){
        type = newtype;
    }
    std::any value;

private:
    
    std::string type;
    std::string strval;
    std::any convertedvalue;
    bool nve;
};




class StructDecl {
public:
    StructDecl( tsl::ordered_map<std::string,std::any> value,tsl::ordered_map<std::string,std::any> cons,tsl::ordered_map<std::string,std::any> fns={})
        : value(value), constants(cons), fns(fns){}
     tsl::ordered_map<std::string,std::any> getValue() { return value; }
     tsl::ordered_map<std::string,std::any> getConsts()  { return constants; }
     tsl::ordered_map<std::string,std::any> getFns()  { return fns; }
    void addstruct(std::string ls){
        this->listed_structs.push_back(ls);
    }
    std::vector<std::string> getls(){return listed_structs;}
    void setls(std::vector<std::string> sls){
        listed_structs = sls;
    }
private:
    tsl::ordered_map<std::string,std::any> value;
    tsl::ordered_map<std::string,std::any> constants;
    tsl::ordered_map<std::string,std::any> fns;
    std::vector<std::string> listed_structs = {};
};

class Class {
  public:
      Class(const tsl::ordered_map<std::string,std::any> value,tsl::ordered_map<std::string,std::any> cons)
          : value(value), constants(cons){}
      const tsl::ordered_map<std::string,std::any> getValue() const { return value; }
      const tsl::ordered_map<std::string,std::any> getConsts() const { return constants; }
  private:
      const tsl::ordered_map<std::string,std::any> value;
      tsl::ordered_map<std::string,std::any> constants;
  };

class ClassInstance {
  public:
      ClassInstance(const tsl::ordered_map<std::string,std::any> value,tsl::ordered_map<std::string,std::any> cons)
          : value(value), constants(cons){}
      const tsl::ordered_map<std::string,std::any> getValue() const { return value; }
      const tsl::ordered_map<std::string,std::any> getConsts() const { return constants; }
  private:
      const tsl::ordered_map<std::string,std::any> value;
      tsl::ordered_map<std::string,std::any> constants;
  };

class StructInstance {
    public:
    StructInstance( tsl::ordered_map<std::string,std::any> value, tsl::ordered_map<std::string,std::any> cons,tsl::ordered_map<std::string,std::any> fns={} )
        : value(value),cons(cons), Fns(fns){}
     tsl::ordered_map<std::string,std::any> getValue()  { return value; }
     tsl::ordered_map<std::string,std::any> getCons()  { return cons; }
    tsl::ordered_map<std::string,std::any> getFns()  { return Fns; }
    void setSN(std::string inp){
        SN = inp;
    }
    std::string getSN(){
        return SN;
    }
    private:
    tsl::ordered_map<std::string,std::any> value;
    tsl::ordered_map<std::string,std::any> cons;
    tsl::ordered_map<std::string,std::any> Fns;
    std::string SN;
};

struct TypeSafeList {
    public:
    TypeSafeList(tsl::ordered_map<int,std::any> list,std::any  type): list(list),type(type){}
    std::any get(int index){
        if (index < 0){
            index = list.size() + index;
        }
        std::any found = list.find(index);
        auto f = std::any_cast<tsl::ordered_map<int, std::any>::iterator>(found);
        if (f == list.end()){
            throw std::runtime_error("Index " + convertToString(index) + " not found in list." );
        }
        return f->second;
    }
    void add(std::any value){
        if (compareAnyTy(type,value) == false){
            throw std::runtime_error("Type of value added to TypeSafe list should be: " + getTypeName(type) + ".\nType got: " + getTypeName(value));
        }
        this->list[list.size()] = value;}
    void remove(){this->list.erase(list.size()-1);}
    int len() {return list.size();}
    tsl::ordered_map<int,std::any> getlist(){return list;}
    bool getSZU() const {
        return SZU;
    }
    void setSZU(bool b){
        SZU = b;
    }
    std::any getType()const{return type;}
    void set(int index, std::any v){
        if (compareAnyTy(type,v) == false){
            throw std::runtime_error("Type of value added to TypeSafe list should be: " + getTypeName(type) + ".\nType got: " + getTypeName(v));
        }
        this->list[index] = v;
    }
    private:
    tsl::ordered_map<int,std::any> list;
    std::any  type;
    bool SZU = false;
};


struct Tuple {
    public:
    Tuple(tsl::ordered_map<int,std::any> list,std::string type): list(list),type(type){}
    std::any get(int index){
        if (index < 0){
            index = list.size() + index;
        }
        std::any found = list.find(index);
        auto f = std::any_cast<tsl::ordered_map<int, std::any>::iterator>(found);
        if (f == list.end()){
            throw std::runtime_error("Index " + convertToString(index) + " not found in tuple." );
        }
        return f->second;
    }
    void set(int index,std::any value){
        if (list.find(index) == list.end()){
            throw std::runtime_error("Tuple index sizes are immutable.");
        } else if (getTypeName(value) != type && type!=""){
            throw std::runtime_error("Tuple is typed.");
        }
        this->list[index] = value;}
    int len() {return list.size();}
    std::string getType()const{return type;}
    tsl::ordered_map<int,std::any> getcomparer() const {
       return list;
    }
    private:
    tsl::ordered_map<int,std::any> list;
    std::string type;
};

struct List {
    public:
    List(tsl::ordered_map<int,std::any> list): list(list){}
    std::any get(int index){
        if (index < 0){
            index = list.size() + index;
        }
        std::any found = list.find(index);
        auto f = std::any_cast<tsl::ordered_map<int, std::any>::iterator>(found);
        if (f == list.end()){
            throw std::runtime_error("Index " + convertToString(index) + " not found in list." );
        }
        return f->second;
    }
    void add(std::any value){this->list[list.size()] = value;}
    void remove(){this->list.erase(list.size()-1);}
    int len() {return list.size();}
    std::vector<std::string> StringRep(){
        std::vector<std::string> SR;
        for (auto& v : list){
            SR.push_back(convertToString(v.second));
        }
    }
    private:
    tsl::ordered_map<int,std::any> list;
};

struct Break {
    public:
    Break(std::any value=std::any()): value(value) {}
    std::any getValue(){return value;}
    private:
    std::any value;
};

struct Return {
    public:
    Return(std::any value=std::any()): value(value) {}
    std::any getValue(){return value;}
    private:
    std::any value;
};



class TypeEnumKey {
    public:
    TypeEnumKey(std::string id): id(id){}
    std::string getId() const {return id;}
    private:
    std::string id;
};
// EnumVariant - baseenum: my_data, variant: rect, values: (1,2)
class EnumVariant{
    public:
    EnumVariant(tsl::ordered_map<int,std::string> data): data(data){}
    tsl::ordered_map<int,std::string> getData() const {return data;}
    void addStore(int index,std::any v){
        this->store[index] = v;
    }
    void dumptemp(tsl::ordered_map<std::string, std::string> temps){
        tsl::ordered_map<int,std::string> tdata;
        for (auto&k:data){
            //println("1: " + k.second + "\n2. " + temps.find(k.second)->second);
            tdata[k.first] = temps.find(k.second)->second;
        }
        this->data = tdata;
    }
    std::vector<std::string> checktypes() {
        std::vector<std::string> ct;
        for (auto& v: data){
            ct.push_back(getTypeName(v.second));
        }
        return ct;
    }
    private:
    tsl::ordered_map<int,std::string> data;
    tsl::ordered_map<int,std::any> store;
};
 // need a node to interpret this
class FixedDataEnumKey{
    public:
    FixedDataEnumKey(tsl::ordered_map<int,std::string> data): data(data){}
    tsl::ordered_map<int,std::string> getData() const {return data;}
    void addStore(int index,std::any v){
        this->store[index] = v;
    }
    private:
    tsl::ordered_map<int,std::string> data;
    tsl::ordered_map<int,std::any> store;
};




class Enum {
    public:

    Enum(std::unordered_map<std::string,EnumKey> fields): fields(std::move(fields)){}
    std::unordered_map<std::string,EnumKey> getValue() const {return fields;}

    // Copy constructor (shallow copy, or deep copy if needed)
    Enum(const Enum& other) : fields(other.fields) {}

    // Copy assignment operator
    Enum& operator=(const Enum& other) {
        if (this != &other) {
            fields = other.fields;  // Assign the unordered_map
        }
        return *this;
    }

    // Move constructor (optional, for efficiency)
    Enum(Enum&& other) noexcept : fields(std::move(other.fields)) {}

    // Move assignment operator (optional, for efficiency)
    Enum& operator=(Enum&& other) noexcept {
        if (this != &other) {
            fields = std::move(other.fields);
        }
        return *this;
    }

    private:
    std::unordered_map<std::string,EnumKey> fields;

};

class EnumKey {
    public:
    EnumKey(std::string type, std::vector<std::any> value,Enum base=Enum({}),std::string name = ""): type(type), value(value),base(base),name(name) {}
    // Copy constructor
    EnumKey(const EnumKey& other)
        : type(other.type), value(other.value), base(other.base), name(other.name) {}

    // Copy assignment operator
    EnumKey& operator=(const EnumKey& other) {
        if (this != &other) {
            type = other.type;
            value = other.value;
            base = other.base; // Requires the copy assignment of Enum
            name = other.name;
        }
        return *this;
    }
    void setBase(Enum& e){
        this->base = e;
    }

    std::string getName(){return name;}
    std::vector<std::any> getValue() {return value;}
    Enum getBase() const {return base;}
    const std::string getType() const {return type;}
    private:
    std::string name;
    std::string type;
    std::vector<std::any> value;
    Enum base;



};

class PtrVal {
public:
    PtrVal(std::any value) : value(value) {}

    std::any getValue() const {
        
        return value;
    }

    void setValue(std::any v) {
        if (v.type() != value.type() && v.type() != typeid(nullptr)) {
            throw std::runtime_error("Type mismatch expected:: " + demangle(value.type().name()) + "\ngot: " + demangle(v.type().name()));
        }
        this->value = v;
    }
    void CastsetValue(std::any v) {
        this->value = v;
    }

private:
    std::any value;
};


class Pointer {
public:
    // Constructor
    Pointer(PtrVal* ptr) : ptr(ptr) {}
    

    bool getMut() const {
        return mut;
    }

    void setMut(bool t) {
        this->mut = t;
    }

    // Return the pointer to PtrVal
    PtrVal* getptr() const {
        if (freed == true){
            throw std::runtime_error("POINTER IS ALREADY FREED.");
        }
        return ptr;
    }

    // Free the allocated memory
    void freemem() {
        if (freed == false){
        if (ptr){
        if (ptr != nullptr){
        //delete ptr;
        freed = true;
        ptr = nullptr;
        }
        }
        }
    }
    void setFree(){
        this->freed = true;
    }

    // Clone the Pointer (shallow copy of PtrVal)
    Pointer clone() {
        return Pointer(new PtrVal(ptr->getValue()));  // This creates a shallow copy of the pointer
    }

    // Edit the PtrVal
    void editValue(std::any v) {
        ptr->setValue(v);
    }
    void pointsto(std::string v){
        //println("POINTING TO: " + v);
        
        this->pointfrom = v;
        //exit(0);
    }
    std::string ptrType(){
        return getTypeName(ptr->getValue());
    }
    std::string getpt(){return pointfrom;}

private:
    PtrVal* ptr;
    bool mut = false;
    bool freed = false;
    std::string pointfrom;
};

// BorrowedPointer class
class BorrowedPointer {
public:
    // Constructor takes a reference to PtrVal and a flag for mutability
    BorrowedPointer(PtrVal* ref, bool isMutable) : ptr(ref), mutableFlag(isMutable) {}

    // Return the value stored in PtrVal
    std::any getValue() const {
        return ptr->getValue();
    }

    std::string ptrType(){
        return getTypeName(ptr->getValue());
    }    

    // Return the reference to PtrVal
    PtrVal* getptr() const {
        return ptr;
    }

    // Edit the value if the pointer is mutable
    void editValue(std::any v) {
        if (!mutableFlag) {
            throw std::runtime_error("Cannot edit: Pointer is immutable.");
        }
        ptr->setValue(v);
    }
    bool isMut()const{return mutableFlag;}

private:
    PtrVal* ptr;  // Raw pointer to PtrVal
    bool mutableFlag;

};

class UnMappedFunction {
public:
    UnMappedFunction(const std::shared_ptr<ASTNode> value,tsl::ordered_map<int,std::string> args)
        : value(value), args(args){}
    const std::shared_ptr<ASTNode> getBody() const { return value; }
    const tsl::ordered_map<int,std::string> getargs() const { return args; }
private:
    const std::shared_ptr<ASTNode> value;
    tsl::ordered_map<int,std::string> args;
};

class Searchable{
    public:
    Searchable(std::string type, tsl::ordered_map<int,std::any> tochecks): type(type), tochecks(tochecks) {}
    std::string getType(){return type;}
    tsl::ordered_map<int,std::any> getToChecks(){return tochecks;}
    private:
    std::string type;
    tsl::ordered_map<int,std::any> tochecks;

};

class Future;

class MappedFunction {
public:
    MappedFunction(const std::shared_ptr<ASTNode> value, tsl::ordered_map<std::string,std::any> args,tsl::ordered_map<int,std::string> tick,std::any retty,tsl::ordered_map<std::string,std::string> internals,bool isselfptr=false)
        : value(value), args(args), tick(tick),retty(retty), internals(internals),isselfptr(isselfptr){}
    const std::shared_ptr<ASTNode> getBody() const { return value; }
    tsl::ordered_map<std::string,std::any> getargs() { return args; }
    tsl::ordered_map<int,std::string> gettick() {
        return tick;
    }
    tsl::ordered_map<std::string,std::string> getinternals() {
        return internals;
    }
    std::any getretty() const {
        return retty;
    }
    void setselfupdate(std::string s){
        this->updateself = s;
    }
    void setself(std::any v){
        args["self"] = v;
    }
    void removeself(){
        args.erase("self");
    }
    std::string getselfupdate(){
        return updateself;
    }
    void setselforigin(std::shared_ptr<ASTNode> slf){
        selforigin = slf;
    }
    std::shared_ptr<ASTNode> getselforigin(){
        return selforigin;
    }
    bool issignature = false;
    void setselfptr(bool s){
        this->isselfptr = s;
    }
    bool isselfptr = false;
private:
    const std::shared_ptr<ASTNode> value;
    std::shared_ptr<ASTNode> selforigin=nullptr;
    tsl::ordered_map<std::string,std::any> args;
    tsl::ordered_map<int,std::string> tick;
    tsl::ordered_map<std::string,std::string> internals;
    std::any retty;
    std::string updateself;
};


class Tag {
    public:
    Tag(tsl::ordered_map<int,std::any> tagtypes):tagtypes(tagtypes){}
    
    tsl::ordered_map<int,std::any> tagtypes;
    private:
};

class CFunction {
public:
    // Change constructor to take std::function by value
    CFunction(std::function<void(std::string)> value)
        : value(std::move(value)) {}

    const std::function<void(std::string)>& getFN() const { return value; }

    void call(std::string arg) {
        value(arg);  // Call the stored function
    }

private:
    std::function<void(std::string)> value;  // Store by value
};

class CEXTFunction {
public:
    // Change constructor to take std::function by value
    CEXTFunction(std::any value,MappedFunction MFNlink)
        : value(value), MFNlink(MFNlink) {
            if (!value.has_value()){
                throw std::runtime_error("Invalid value set");
            }
        }

    std::any getDV() { return value; }
    MappedFunction MFNlink;



private:
    std::any value;  // Store by value
};

class TagFunction {
public:
    // Change constructor to take std::function by value
    TagFunction(MappedFunction MFNlink,std::shared_ptr<ASTNode> AN)
        : MFNlink(MFNlink), AN(AN) {
        }

    MappedFunction MFNlink;
    std::shared_ptr<ASTNode> AN;



private:
    std::any value;  // Store by value
};

class Module {
    public:
    Module(std::string path){
    path += ".deck";
    if (AP.has("deckpath")){
        for (auto& dp : AP.values["deckpath"]){
            //std::cout << "searching " << dp + "/" + path<< std::endl;
            if (std::filesystem::exists(dp + "/" + path)){
                path = std::filesystem::absolute(dp + "/" + path).string();
                break;
            }
        }
    } else {
        path = std::filesystem::absolute(path).string();
    }
    //std::cout << "unbundling " << path << std::endl;
    logat("unbundling " + path,"Module");
    bundler.unbundle(path,&imports,&ocontent);
    hash = imports["DECKhash"];
    objects[hash] = ocontent;
    imports.erase("DECKhash");
    }
    void addMethod(std::string name, std::any method){
        methods[name] = method;
    }

    void addNode(std::string name, std::vector<std::shared_ptr<ASTNode>> nodes){
        MNodes[name] = nodes;
    }
   
    ObjectBundler bundler = ObjectBundler();
    std::string hash;
    std::vector<std::string> from;
    std::unordered_map<std::string,std::string> imports;
    std::unordered_map<std::string, std::any> methods;
    std::unordered_map<std::string,std::vector<std::shared_ptr<ASTNode>>> MNodes;
    std::vector<char> ocontent;
    private:

};

class ModuleMethod {
     public:
        ModuleMethod(std::string hash, std::string vname, std::any mfn) : hash(hash), vname(vname), mfn(mfn){}
        std::string hash;
        std::string vname;
        std::any mfn;
    private:
        
};


int vidct = 0;

// Variable struct that is used in a scope, ALL additions of types need to be here

struct Variable {
    std::any value;  
    int valueid;
    

    Variable() = default;
    Variable(std::any value,bool mut=false) : value(std::move(value)), mut(mut),valueid(vidct++) {}
    Variable(const Variable& other) : value(other.value), mut(other.mut), valueid(other.valueid) {}
    Variable(Variable&& other) noexcept : value(std::move(other.value)), mut(std::move(other.mut)),valueid(std::move(other.valueid)) {}
    void setVID(int id){
        this->valueid = id;
    }
    std::string selfup;

    void setselfup(std::string s){
        this->selfup = s;
    }

    // Assignment operators
    Variable& operator=(const Variable& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }

    Variable& operator=(Variable&& other) noexcept {
        if (this != &other) {
            value = std::move(other.value);
        }
        return *this;
    }
    void update(std::any v){
        this->value = v;
    }

    // Type checking methods
    bool isTagF() const { return value.type() == typeid(TagFunction); }
    bool isTag() const { return value.type() == typeid(Tag); }
    bool isCEFN() const { return value.type() == typeid(CEXTFunction); }
    bool isModule() const { return value.type() == typeid(Module); }
    bool isModuleM() const { return value.type() == typeid(ModuleMethod); }
    bool isCFN() const { return value.type() == typeid(CFunction); }
    bool isClassI() const { return value.type() == typeid(ClassInstance); }
    bool isClass() const { return value.type() == typeid(Class); }
    bool isMFN() const { return value.type() == typeid(MappedFunction); }
    bool isUFN() const { return value.type() == typeid(UnMappedFunction); }
    bool isNull() const { return value.type() == typeid(nullification); }
    bool isNullv() const { return value.type() == typeid(nullificationv); }
    bool isInt() const { return value.type() == typeid(int32_t); }
    bool isString() const { return value.type() == typeid(std::string); }
    bool isBool() const { return value.type() == typeid(bool); }
    bool isObj() const { return value.type() == typeid(Object); }
    bool isList() const { return value.type() == typeid(List); }
    bool isTSL() const { return value.type() == typeid(TypeSafeList); }
    bool isTuple() const { return value.type() == typeid(Tuple); }
    bool isStruct() const { return value.type() == typeid(StructDecl); }
    bool isStructI() const { return value.type() == typeid(StructInstance); }
    bool isEnum() const { return value.type() == typeid(Enum); }
    bool isEK() const { return value.type() == typeid(EnumKey); }
    bool isPTR() const { return value.type() == typeid(Pointer); }
    bool isBPTR() const { return value.type() == typeid(BorrowedPointer); }
    bool isMut() {return mut;}
    void setMut() {this->mut=true;}
    CFunction getCFN() const {
        if (isCFN()) {
            return std::any_cast<CFunction>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    TagFunction getTagF() const {
        if (isTagF()) {
            return std::any_cast<TagFunction>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    Tag getTag() const {
        if (isTag()) {
            return std::any_cast<Tag>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    Module getModule() const {
        if (isModule()) {
            return std::any_cast<Module>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    ModuleMethod getModuleM() const {
        if (isModuleM()) {
            return std::any_cast<ModuleMethod>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    CEXTFunction getCEFN() const {
        if (isCEFN()) {
            return std::any_cast<CEXTFunction>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    Class getClass() const {
        if (isClass()) {
            return std::any_cast<Class>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    ClassInstance getClassI() const {
        if (isClassI()) {
            return std::any_cast<ClassInstance>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    int getInt() const {
        if (isInt()) {
            return std::any_cast<int32_t>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    nullification getNull() const {
        if (isNull()) {
            return std::any_cast<nullification>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    std::string getString() const {
        if (isString()) {
            return std::any_cast<std::string>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    bool getBool() const {
        if (isBool()) {
            return std::any_cast<bool>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    Object getObj() const {
        if (isObj()) {
            
            return std::any_cast<Object>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    List getList() const {
        if (isList()) {
            
            return std::any_cast<List>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    Tuple getTuple() const {
        if (isTuple()) {
            
            return std::any_cast<Tuple>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    TypeSafeList getTSL() const {
        if (isTSL()) {
            
            return std::any_cast<TypeSafeList>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    StructDecl getStruct() const {
        if (isStruct()) {
            return std::any_cast<StructDecl>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    StructInstance getStructI() const {
        if (isStructI()) {
            return std::any_cast<StructInstance>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    Enum getEnum() const {
        if (isEnum()) {
            return std::any_cast<Enum>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    EnumKey getEK() const {
        if (isEK()) {
            return std::any_cast<EnumKey>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    Pointer getPTR() const {
        if (isPTR()) {
            return std::any_cast<Pointer>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    BorrowedPointer getBPTR() const {
        if (isBPTR()) {
            return std::any_cast<BorrowedPointer>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    UnMappedFunction getUFN() const {
        if (isUFN()) {
            return std::any_cast<UnMappedFunction>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    MappedFunction getMFN() const {
        if (isMFN()) {
            return std::any_cast<MappedFunction>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }

    nullificationv getnullv() const {
        if (isNullv()) {
            return std::any_cast<nullificationv>(value);
        }
        throw std::bad_any_cast();  // Handle the error as needed
    }
    
    // Get the type as a string
    std::string getType() const {
        if (isInt()) return "int";
        if (isString()) return "RawString";
        if (isBool()) return "bool";
        if (isNull()) return "none";
        if (isNullv()) return "nil";
        if (isObj()) return "obj";
        if (isList()) return "list";
        if (isTuple()) return "tuple";
        if (isStructI()) return "struct";
        if (isEK()) return "ek";
        if (isEnum()) return "enum";
        if (isPTR()) return "ptr";
        if (isBPTR()) return "bptr";
        if (isStruct()) return "_struct";
        if (isTSL()) return "TSL";
        if (isUFN()) return "UFN";
        if (isCFN()) return "CFN";
        if (isCEFN()) return "CEFN";
        if (isMFN()) return "MFN";
        if (isClass()) return "Class";
        if (isClassI()) return "ClassI";
        if (isModule()) return "MLE";
        if (isModuleM()) return "MODM"; // the urge to just call this as (your) MOM
        if (isTag()) return "tag";
        if (isTagF()) return "tagF";
        return "unknown";
    }

    std::any getValue(){
        return value;
    }

private:
bool mut;
};

std::string getTypeName(const std::any& value,bool cb,bool tp){
    if (value.type() == typeid(MappedFunction)){
        auto mfn = std::any_cast<MappedFunction>(value);
        std::string v = "@sig(";
        for (auto& arg: mfn.getargs()){
            v += getTypeName(arg.second) + ",";

        }
        v += ") -> ";
        v += getTypeName(mfn.getretty());
        return v;
    };
    if (value.type() == typeid(StructInstance)) return "Struct";
    if (value.type() == typeid(StructDecl)) return "_struct";
    if (value.type() == typeid(Object)) return std::any_cast<Object>(value).getType();
    if (value.type() == typeid(List)) return "list";
    if (value.type() == typeid(TypeSafeList)) return "[" + getTypeName(std::any_cast<TypeSafeList>(value).getType()) + ":" + std::to_string(std::any_cast<TypeSafeList>(value).len()) + "]";
    if (value.type() == typeid(Tuple)) return "tup";
    if (value.type() == typeid(int8_t)) return "i8";
    if (value.type() == typeid(int16_t)) return "i16";
    if (value.type() == typeid(int32_t)) return "i32";
    if (value.type() == typeid(int64_t)) return "i64";
    if (value.type() == typeid(uint8_t)) return "u8";
    if (value.type() == typeid(uint16_t)) return "u16";
    if (value.type() == typeid(uint32_t)) return "u32";
    if (value.type() == typeid(uint64_t)) return "u64";
    if (value.type() == typeid(float)) return "f32";
    if (value.type() == typeid(double)) return "f64";
    if (value.type() == typeid(std::string)) return "RawString";
    if (value.type() == typeid(CEXTFunction)) return "CEXTFN";
    if (value.type() == typeid(Pointer)) return "&" + getTypeName(std::any_cast<Pointer>(value).getptr()->getValue());
    if (value.type() == typeid(BorrowedPointer)){
        if (tp){
            return "&" + getTypeName(std::any_cast<BorrowedPointer>(value).getptr()->getValue());
        }
        std::string mstr = std::any_cast<BorrowedPointer>(value).isMut() ? " mut" : "";
        return "&b" + (mstr) + " " + getTypeName(std::any_cast<BorrowedPointer>(value).getptr()->getValue());
    } 
    if (value.type() == typeid(int)) return "Number";
    if (value.type() == typeid(char)) return "Character";
    if (value.type() == typeid(std::wstring)) return "wchar";
    if (value.type() == typeid(nullificationv)) return "nil";
    if (value.type() == typeid(nullification)) return "none";
    if (value.type() == typeid(bool)) return "Bool";
    if (value.type() == typeid(ModuleMethod)) return std::any_cast<ModuleMethod>(value).hash + std::any_cast<ModuleMethod>(value).vname;
    if (value.type() == typeid(TagFunction)){return getTypeName(std::any_cast<TagFunction>(value).MFNlink);}
    println("ukt:" + demangle(value.type().name()));

    return "unknown";
}

template <typename K,typename T>
bool compareMap(const tsl::ordered_map<K, T>& map1, const tsl::ordered_map<K, T>& map2);





// Scope class to manage variables
class Scope {
public:
    Scope(bool isMain = false,std::shared_ptr<Scope> parentScope = nullptr) : isMain(isMain), parentScope(parentScope) {}

    void addVariable(const std::string& name, std::shared_ptr<Variable> var) {
        auto check = variables.find(name); // mut tracelog time.
        if (check != variables.end()){
            if (check->second->isMut() == false){
                std::string err = "Attempting to modify immutable variable: ";
                err = err + name;
                throw std::runtime_error(err);
            } else {
                //println("MUT: TRUE");
                //println(name);
                try {
                if (/*var->value.type() != typeid(nullification) &&*/ (var->value.type() != check->second->value.type() && ruleset["typing"] == "static")){
                    throw std::runtime_error("Attempting to modify static typed variable with different type than inferred");
                }} catch (std::bad_any_cast){
                    throw std::runtime_error("Ruleset.typing can only have a string value\nEither 'static' or 'dynamic'");
                }
                var->setMut();
                var->setVID( check->second->valueid);
                variables[name] = var;
            }
        } else {
        //println("MUT: x");
        //println(name);

            //println(name);
            //println("check: " + convertToString(check != variables.end()));
            ////println(convertToString(var->getInt()));

            
        
        variables[name] = var;
        }
        logat("Successfully set var","Scope.aV");
    }

    std::shared_ptr<Variable> getVariable(const std::string& name)  {
        auto it = variables.find(name);
        
        if (it != variables.end()) {
            
            return it->second;
        }
        
        if (parentScope) {
            auto it =  parentScope->getVariable(name);
            if (it->getType() == "ptr"){
                
                return std::make_shared<Variable>(BorrowedPointer(it->getPTR().getptr(),false));
            }
            return it;
        }
        throw std::runtime_error("Variable '" + name + "' does not exist within this scope or previous scopes");
        return nullptr;
    }
    void setLoop(){this->isLoop=true;}
    bool getLoop(){return isLoop;}
    void setSign(){this->sig=true;}
    bool getSign(){return sig;}
    void removeVariable(const std::string& name,bool panic=true) {
        if (variables.find(name)  == variables.end()){
            if (panic == true){
                throw std::runtime_error("Deletion of non-existent variable within scope: " + name);
            }
            return;
        }
        //println("Deleted " + name);
        variables.erase(name);
    }

    /*void printVariables()  { //legacy when we didn't have printf or even __compileprint__
    for (const auto& pair : variables) {
        if (dummyValues.find(pair.first) == dummyValues.end()){
        //std::cout << "Variable " << pair.first << " = ";
        std::string type = pair.second->getType();
        

        if (type == "int") {
            //std::cout << pair.second->getInt();
        } else if (type == "RawString") {
            //std::cout << pair.second->getString();
        } else if (type == "bool") {
            pair.second->visit();
        } else if (type=="obj"){
            Object obj = pair.second->getObj();
            std::string out = obj.printable();
            //std::cout << out;
        } else {
            //std::cout << "Type: " << type;
        }
        //std::cout << std::endl;
    }
    //println("DONE WITH VARS");
    }
    }*/

    bool isptr(std::string vcheck){
        if (pointers.find(vcheck) != pointers.end()){
            return true;
        }
        return false;
    }
    bool isMainScope() const { return isMain; }
    bool isVar(std::string n){
        auto b1 = variables.find(n) != variables.end();
        if (b1){
            return b1;
        }
        if (parentScope){
            return parentScope->isVar(n);
        }
        return false;
    }
    void freeall(){
        
        for (const auto& v:variables){
            std::string type = v.second->getType();
            if (type == "ptr"){
                v.second->getPTR().freemem();
                //println("Deleted ptr: " + v.first);
            }
            
        }
        
    }

private:
    bool sig;
    bool isMain;
    bool isLoop;
    std::shared_ptr<Scope> parentScope;
    tsl::ordered_map<std::string, std::shared_ptr<Variable>> variables;
    tsl::ordered_map<std::string, int> pointers;
};

enum class TokenType {
    Identifier,
    Assign,   // '='
    Colon, // :
    Walrus, // :=
    Semicolon, // ;
    Integer,
    String,
    DOT, // .
    OP, // == and etc
    COMMA, // ,
    LBracket, // [
    RBracket, // ]
    LParen, // (
    RParen, // )
    LBrace,   // '{'
    RBrace,   // '}'
    KeyWord,
    Ampersand, // &
    AmpersandB, // &b
    AmpersandG, // &g
    AmpersandEq, // &=
    Bang, // !
    Unarary, // ?
    NUnarary, // ??
    ReturnTy, // ->
    Anon, // ||
    Decorator,
    Hashtag, // #
    DoubleDot, // ..
    LSearchable, // [|
    RSearchable, // |]
    Deconstruct, // %| x, y, |%
    EqArrow, // =>
    Variadic, // ...
    Comment, // //
    LExpr, // |
    RExpr, // |
    xadecimal, // 0x/0b/0o
    EndOfFile,
    Invalid
};

// not strictly neccesary but will be useful for improved error handling later
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "Identifier";
        case TokenType::Assign: return "Assign";
        case TokenType::Integer: return "Integer";
        case TokenType::String: return "RawString";
        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::OP: return "Op";
        case TokenType::COMMA: return "COMMA";
        case TokenType::LBracket: return "LBracket";
        case TokenType::RBracket: return "RBracket";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::KeyWord: return "KeyWord";
        case TokenType::EndOfFile: return "EndOfFile";
        case TokenType::Invalid: return "Invalid";
        case TokenType::Colon: return "Colon";
        case TokenType::ReturnTy: return "ReturnTy";
        case TokenType::Semicolon: return "SemiColon";
        default: return "Unknown";
    }
}

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), currentPos(0) {}
    int line = 1;
    tsl::ordered_map<int,int> linemaxpos;
    int poswline = 0;
    int cdiff = 0;
    bool lookAheadForTemp(){
        //std::cout<<"Looking rn";
        //std::cout<<input.substr(currentPos,5);
        

        
        int c = currentPos;
        std::string id;
        if (input.substr(c,1) == "<"){
            c++;
        }
        while (input.substr(c,1) != ">"){
            if (input.substr(c,1) == ""){
                
                return false;
            }
            if (input.substr(c,1) == "," ){
            if (converters.find(id) != converters.end()){
                return true;
            } else {
                
                
                return false;
            }
                id = "";
            } else {
                id += input.substr(c,1);
            }
            //std::cout <<std::endl<< id<<std::endl;
            
            c++;
        }
        if (converters.find(id) != converters.end()){
            return true;
            } else {
                
                
            return false;
        }
    }
    int getCpos(){
        return currentPos;
    }
    void rollback(int setto){
        currentPos = setto;
    }
    Token getNextToken() {
        // += /= etc

        logat("Getting Next Token at pos(" + convertToString(currentPos) + ")","Lexer.gNT");
        std::vector<std::string> op{"==", "!=", "<<",">>",">=","<=","&&","||","+","-","/","*",">","<","+=","-=","*="};
        std::vector<std::string> keywords{"if","elif","else","mut","loop","while","break","struct","enum","fn","return","assert","mod","todo","drop","let","match","future","class","extern","as","pub","in","out","inout","asm","inline","import","from","ref","tag"};
        //println("pwl: " + std::to_string(poswline) + "\ndiff: " + std::to_string((currentPos - cdiff)) + "\ncPOS: " + std::to_string((currentPos)) + " cDIFF: " + std::to_string((cdiff)));
        
        skipWhitespaceAndComments();


        
        if (currentPos >= input.size()) {
            return { TokenType::EndOfFile, "" };
        }

        char ch = input[currentPos];
        //std::cout << ch;
        if (input.substr(currentPos,3) == "..."){
            ladvance();
            ladvance();
            ladvance();
            return {TokenType::Variadic,"..."};
        }
        if (input.substr(currentPos,1) == "|"){
            ladvance();

            return {TokenType::LExpr,"|"};
        }
        if (input.substr(currentPos,2) == "//"){
            ladvance();
            
            return getNextToken();
        }
        if (input.substr(currentPos,2) == ".."){
            ladvance();
            ladvance();
            return {TokenType::DoubleDot,".."};
        }
        if (ch == '.') {
            ladvance();
            return { TokenType::DOT, "." };
        }
        if (ch == ';') {
            ladvance();
            return { TokenType::Semicolon, ";" };
        }
        if (input.substr(currentPos,2) == "@|"){
            ladvance();
            ladvance();
            return {TokenType::Anon,"@|"};
        }
        if (input.substr(currentPos,2) == "[|"){
            ladvance();
            ladvance();
            return {TokenType::LSearchable,"[|"};
        }
        if (input.substr(currentPos,2) == "|]"){
            ladvance();
            ladvance();
            return {TokenType::RSearchable,"|]"};
        }

        if (input.substr(currentPos,2) == "=>"){
            ladvance();
            ladvance();
            return {TokenType::EqArrow,"=>"};
        }
        if (ch == '@') {
            ladvance();
            return { TokenType::Decorator, "@" };
        }

        if (input.substr(currentPos,2) == "->"){
            ladvance();
            ladvance();
            return {TokenType::ReturnTy,"->"};            
        }

        if (input.substr(currentPos,2) == ":="){
            ladvance();
            ladvance();
            return {TokenType::Walrus,":="};            
        }

        if (input.substr(currentPos,2) == "??"){
            ladvance();
            ladvance();
            return {TokenType::NUnarary,"??"};            
        }


        if (ch == '?') {
            ladvance();
            return { TokenType::Unarary, "?" };
        }


        
        if (input.substr(currentPos,2) == "&b" && std::isalpha(input[currentPos+2]) == false) {
            
            ladvance();
            ladvance();
            //println("AB");
            //println(input.substr(currentPos,1));
            return { TokenType::AmpersandB, "&b" };
        }
        if (input.substr(currentPos,2) == "&g" && std::isalpha(input[currentPos+2]) == false) {
            
            ladvance();
            ladvance();
            return { TokenType::AmpersandG, "&g" };
        }
        if (input.substr(currentPos,2) == "&=") {
            ladvance();
            ladvance();
            return { TokenType::AmpersandEq, "&=" };
        }
        if (in(input.substr(currentPos,2),op) == true || in(input.substr(currentPos,1),op) == true || (input.substr(currentPos,1) == "b" && in(input.substr(currentPos+1,1),{"|","&","^"}))) {
            return {TokenType::OP, parseoperators()};
        }
        if (ch == '&') {
            ladvance();
            return { TokenType::Ampersand, "&" };
        }

        if (input.substr(currentPos,2) == "#\""){
            return { TokenType::String, parseTagStringLiteral() };
        }
        if (ch == '#') {
            ladvance();
            return { TokenType::Hashtag, "#" };
        }
        if (ch == '{') {
            ladvance();
            return { TokenType::LBrace, "{" };
        } else if (ch == '}') {
            ladvance();
            return { TokenType::RBrace, "}" };
        } 
        if (ch == '(') {
            ladvance();
            return { TokenType::LParen, "(" };
        } 
        if (ch == ')'){
            ladvance();
            return {TokenType::RParen, ")"};

        }
        if (ch == '[') {
            //println("LB");
            ladvance();
            return { TokenType::LBracket, "[" };
        } 
        if (ch == ']'){
            ladvance();
            return {TokenType::RBracket, "]"};

        }
        if (ch == ','){
            ladvance();
            return {TokenType::COMMA, ","};

        }
        
        if (ch == '=') {
            ladvance();
            return { TokenType::Assign, "=" };
        }
        if (ch == ':'){
            ladvance();
            return {TokenType::Colon,":"};
        }
        if (std::isalpha(ch) || ch == '_') {
            std::string pi = parseIdentifier();
            if (in(pi,keywords) == true){
                //std::cout << "keyword " << pi;
                return {TokenType::KeyWord, pi};
            } else if (pi == "var"){
                return {TokenType::KeyWord, "mut"};
            } else if (pi == "extend"){
                return {TokenType::KeyWord, "mod"};
            } else if (pi == "and"){
                return {TokenType::OP, "&&"};
            } else if (pi == "or"){
                return {TokenType::OP, "||"};
            } else if (pi == "is"){
                return {TokenType::OP, "=="};
            } // maybe is not in the future
            return { TokenType::Identifier, pi};
        }

        if (ch == '"' || ch == '\'') {
            return { TokenType::String, parseStringLiteral() };
        }
        if (input.substr(currentPos,2) == "0x"){
            ladvance();
            ladvance();
            auto StartPos = poswline;
            auto start = currentPos;
            while (currentPos < input.size() && (input[currentPos] != ' ' && input[currentPos] != '\n' && (isalpha(input[currentPos]) || isdigit(input[currentPos])) == true)) {
            if (!inT<char>(input[currentPos],{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'})){
                
                displayError("Hexadecimal representations eg. 0o736 may only contain numbers from 0 to 9 and letters A to F",line,StartPos,poswline+ (poswline == StartPos ? 1 : 0),gerr({}));
            }
            ladvance();
            }

            return {TokenType::xadecimal,convertToString(hexStringToNumber(input.substr(start,currentPos-start)))};
        }
        if (input.substr(currentPos,2) == "0b"){
            ladvance();
            ladvance();
            auto StartPos = poswline;
            auto start = currentPos;
            while (currentPos < input.size() && (input[currentPos] != ' ' && input[currentPos] != '\n' && (isalpha(input[currentPos]) || isdigit(input[currentPos])) == true)) {
            //println("Whaddafak: " + std::to_string(input[currentPos] != '0' && input[currentPos] != '1'));
            if (input[currentPos] != '0' && input[currentPos] != '1'){
                //std::cout << "Line: " << line << "Start: " << StartPos << "end: " << poswline << std::endl;
                displayError("Binary representations eg. 0b101 may only contain 0 or 1",line,StartPos,poswline+ (poswline == StartPos ? 1 : 0),gerr({"note","Aside from Octal, a Hexadecimal representation is possible as well."}));
            }
            ladvance();
            }

            return {TokenType::xadecimal,convertToString(binaryStringToNumber(input.substr(start,currentPos-start)))};
        }

        if (input.substr(currentPos,2) == "0o"){
            ladvance();
            ladvance();
            auto StartPos = poswline;
            auto start = currentPos;
            while (currentPos < input.size() && (input[currentPos] != ' ' && input[currentPos] != '\n' && (isalpha(input[currentPos]) || isdigit(input[currentPos])) == true)) {
            if (!inT<char>(input[currentPos],{'0','1','2','3','4','5','6','7'})){
                
                displayError("Octal representations eg. 0o736 may only contain numbers from 0 to 7",line,StartPos,poswline+ (poswline == StartPos ? 1 : 0),gerr({"note","Aside from Binary, Octal and Hexadecimal representations are possible as well."}));
            }
            ladvance();
            }

            return {TokenType::xadecimal,convertToString(octalStringToNumber(input.substr(start,currentPos-start)))};
        }

        
        if (std::isdigit(ch) || ch == '-') {
            return { TokenType::Integer, parseNumberLiteral() };
        }
        if (ch == '!') {
            ladvance();
            return { TokenType::Bang, "!" };
        }
        if (input.substr(currentPos,2) == "%|") {
            ladvance();
            ladvance();
            return { TokenType::Deconstruct, "%|" };
        }
        if (input.substr(currentPos,2) == "|%") {
            ladvance();
            ladvance();
            return { TokenType::Deconstruct, "|%" };
        }


        return { TokenType::Invalid, "" };
    }

private:
    std::string input;
    size_t currentPos;

    void ladvance() {
        if (currentPos < input.size()) {
            char ch = input[currentPos];
            if (input.substr(currentPos,2) == "//"){
                
                while (ch != '\n'){
                    
                    currentPos++;
                    ch = input[currentPos];
                }
            }
            

            if (ch == '\n') {
                
                line++;
                std::string strch(1,ch);
                poswline = 0;
            } else {
                poswline++;
            }
            currentPos++;
        }
    }


    void skipWhitespaceAndComments() {
    while (currentPos < input.size()) {
        char ch = input[currentPos];

        if (std::isspace(ch)) {
            ladvance();
            continue;
        }

        /*if (input.substr(currentPos, 2) == "//") {
            while (currentPos < input.size() && input[currentPos] != '\n') {
                ladvance();
            }
            // Consume the newline character
            if (currentPos < input.size() && input[currentPos] == '\n') {
                println("consumed comm: " + std::to_string(line));
                ladvance();
            }
            continue;
        }*/

        // Break if neither whitespace nor comment
        break;
    }
}

    std::string parseoperators(){
        // == != > < >> << >= <= && ||
        std::vector<std::string> op{"==", "!=", "<<",">>",">=","<=","&&","||","+=","-=","/=","b|","b&","b^"};
        std::string inp;
        inp = input.substr(currentPos, 2);
        
        for (int i = 0; i < op.size() ; i ++){
        if (inp == op[i]){
            ladvance();
            ladvance();
            return inp;
               //Do Stuff
        }}
        inp = input[currentPos];
        ladvance();
        return inp;


    }
    std::string parseIdentifier() {
        size_t start = currentPos;
        while (currentPos < input.size() && (std::isalnum(input[currentPos]) || input[currentPos] == '_')) {
            ladvance();
        }
        return input.substr(start, currentPos - start);

    }
    std::string encodeUTF8(char32_t codePoint) {
        std::string utf8;
        if (codePoint <= 0x7F) {
            utf8.push_back(static_cast<char>(codePoint));
        } else if (codePoint <= 0x7FF) {
            utf8.push_back(static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F)));
            utf8.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
        } else if (codePoint <= 0xFFFF) {
            utf8.push_back(static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F)));
            utf8.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
        } else if (codePoint <= 0x10FFFF) {
            utf8.push_back(static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07)));
            utf8.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
        }
        return utf8;
    }
    std::string StringRules(){
        if (input.substr(currentPos,2) == "\\u"){
            ladvance();
            ladvance();
            if (input[currentPos] != '{'){
                throw std::runtime_error("Unicode: \\u{}");
            }
            ladvance();
            size_t start = currentPos;
            while (input[currentPos] != '}'){
                ladvance();
            }
            
            std::string result = input.substr(start, currentPos - start);
            ladvance();

            return encodeUTF8(static_cast<char32_t>(hexStringToNumber("0x" + result)));
        } else if (input.substr(currentPos,2) == "\\n"){
            ladvance();
            ladvance();
            return "\n";
        }  else if (input.substr(currentPos,2) == "\\r"){
            ladvance();
            ladvance();
            return "\r";
        }
        auto retstr = input[currentPos];
        ladvance();
        return std::string(1,retstr);
    }
    std::string parseStringLiteral() {
        char ch;
        ch = input[currentPos];
        size_t start = ++currentPos;
        std::string result;
        while (currentPos < input.size() && input[currentPos] != ch) {
            result += StringRules();
        }
        ladvance(); // Skip the closing quote
        return result;
    }
    std::string parseTagStringLiteral() {
        auto ch = input.substr(currentPos,2);
        ladvance();
        ladvance();
        size_t start = currentPos;
        while (currentPos < input.size() && input.substr(currentPos,2) != "\"#") {
            ladvance();
        }
        std::string result = input.substr(start, currentPos - start);
        ladvance(); // Skip the closing quote
        ladvance(); // Skip the closing quote
        return result;
    }

    std::string parseNumberLiteral() {
        size_t start = currentPos;

        while (currentPos < input.size() && (std::isdigit(input[currentPos]) || input[currentPos] == '-' || input[currentPos] == '_')) {
            ladvance();
        }
        return replace(input.substr(start, currentPos - start),"_","");
    }
};

// AST NODES

class MappedFunctionNode : public ASTNode {
public:
    MappedFunctionNode(tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> value,std::shared_ptr<ASTNode> body, tsl::ordered_map<int, std::string>tick,tsl::ordered_map<std::string, std::string> internals,bool isselfptr=false) : value(value),body(body),tick(tick),internals(internals),isselfptr(isselfptr) {}
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> getValue() { return value; }
    std::shared_ptr<ASTNode> getBody() const { return body; }
    tsl::ordered_map<int, std::string> gettick(){
        return tick;
    }
    tsl::ordered_map<std::string, std::string> getinternals(){
        return internals;
    }
    void setself(std::shared_ptr<ASTNode> v){
        this->value["self"] = v;
    }
    void setNoneRet(bool b){
        this->isnoneret = b;
    }
    bool isselfptr= false;
    bool isnoneret=false;
private:
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> value;
    std::shared_ptr<ASTNode>body;
    tsl::ordered_map<int, std::string> tick;
    tsl::ordered_map<std::string, std::string> internals;

};

class UnMappedFunctionNode : public ASTNode {
public:
    UnMappedFunctionNode(tsl::ordered_map<int,std::string> value,std::shared_ptr<ASTNode> body) : value(value),body(body) {}
    tsl::ordered_map<int,std::string> getValue() const { return value; }
    std::shared_ptr<ASTNode> getBody() const { return body; }

private:
    tsl::ordered_map<int,std::string> value;
    std::shared_ptr<ASTNode>body;

};

class IntLiteralNode : public ASTNode {
public:
    IntLiteralNode(int32_t value) : value(value) {}

    IntLiteralNode(const std::string& str_value) {
        try {
            value = std::stoi(str_value);  
        } catch (const std::invalid_argument& e) {

            throw std::runtime_error("Invalid integer literal: " + str_value);
        } catch (const std::out_of_range& e) {

            throw std::runtime_error("Integer literal out of range: " + str_value);
        }
    }
    int getValue() const { return value; }

    void setValue(int value){
        this->value = value;
    }

private:
    int value;
};

class MemAccNode : public ASTNode {
public:
    MemAccNode(std::shared_ptr<ASTNode> value,std::shared_ptr<ASTNode> nxt,bool assign= false, std::shared_ptr<ASTNode> assignv = std::make_shared<ASTNode>()) : value(value), nxt(nxt),assign(assign),assignv(assignv) {}
    std::shared_ptr<ASTNode> getValue() { return value; }
    std::shared_ptr<ASTNode> getNxt() { return nxt; }
    bool getAssign(){return assign;}
    void setAssign(bool b){assign = b;}
    std::shared_ptr<ASTNode> getAssignv() { return assignv; }

private:
    std::shared_ptr<ASTNode> value;
    std::shared_ptr<ASTNode> nxt;
    bool assign = false;
    std::shared_ptr<ASTNode> assignv;
};

class ChainNode : public ASTNode {
    public:
    ChainNode(std::vector<std::shared_ptr<ASTNode>> vec):vec(vec){}
    std::vector<std::shared_ptr<ASTNode>> getChain(){
        return vec;
    }
    private:
    std::vector<std::shared_ptr<ASTNode>> vec;
};

class ImportNode : public ASTNode {
    public:
    ImportNode(std::string name,std::vector<std::string> from):name(name), from(from){}
    std::string getName(){
        return name;
    }
    std::vector<std::string> getFrom(){
        return from;
    }
    void setFrom(std::vector<std::string> f){
        this->from = f;
    }
    private:
    std::string name;
    std::vector<std::string> from;
};

class StringLiteralNode : public ASTNode {
public:
    StringLiteralNode(const std::string& value) : value(value) {}
    const std::string& getValue() const { return value; }

private:
    std::string value;
};

class ONode : public ASTNode {
public:
    ONode(const Object& value) : value(value) {}
    Object getValue() { return value; }
    void setNVE() {value.setNVE(true);}

private:
    Object value;
};

class LoopNode : public ASTNode {
public:
    LoopNode(const std::shared_ptr<ASTNode>& value,const std::shared_ptr<ASTNode> times=std::make_shared<ASTNode>()) : body(value),times(times) {}
    const std::shared_ptr<ASTNode> getValue() const { return body; }
    const std::shared_ptr<ASTNode> getTimes() const {return times;}

private:
    std::shared_ptr<ASTNode> body;
    std::shared_ptr<ASTNode> times;
};

class GiveOwnershipNode: public ASTNode {
    public:
    GiveOwnershipNode(std::shared_ptr<ASTNode> expr): expr(expr){}
    std::shared_ptr<ASTNode> getExpr() {return expr;}
    
    private:
    std::shared_ptr<ASTNode> expr;
};

class ModifyPtrNode: public ASTNode {
    public:
    ModifyPtrNode(std::shared_ptr<ASTNode> lhs,std::shared_ptr<ASTNode> expr): lhs(lhs),expr(expr){}
    std::shared_ptr<ASTNode> getExpr() {return expr;}
    std::shared_ptr<ASTNode> getLHS() {return lhs;}
    
    private:
    std::shared_ptr<ASTNode> lhs;
    std::shared_ptr<ASTNode> expr;
};

class PointerNode : public ASTNode {
    public:
    PointerNode(std::shared_ptr<ASTNode> expr,bool mut=false,bool isborrow=false): expr(expr),mut(mut),isborrow(isborrow){}
    std::shared_ptr<ASTNode> getExpr(){return expr;}
    bool getMut(){return mut;}
    bool getBorrow(){return isborrow;}
    private:
    std::shared_ptr<ASTNode> expr;
    bool mut;
    bool isborrow;
};

class DerefNode : public ASTNode {
    public:
    DerefNode(std::shared_ptr<ASTNode>expr): expr(expr){}
    std::shared_ptr<ASTNode> getExpr(){return expr;}
    private:
    std::shared_ptr<ASTNode> expr;
};

class TupleNode : public ASTNode {
public:
    TupleNode(const tsl::ordered_map<int,std::shared_ptr<ASTNode>>& value,std::string type = "",std::shared_ptr<ASTNode> size=std::make_shared<ASTNode>()) : value(value),type(type),size(size) {}
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> getValue() { return value; }
    std::string getType() {return type;}
    std::shared_ptr<ASTNode> getSize() {return size;}
    
private:
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> value;
    std::string type;
    std::shared_ptr<ASTNode> size;
};

class ListNode : public ASTNode {
public:
    ListNode(const tsl::ordered_map<int,std::shared_ptr<ASTNode>>& value) : value(value) {}
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> getValue() { return value; }
    

private:
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> value;
};

class TypeSafeListNode : public ASTNode {
public:
    TypeSafeListNode(const tsl::ordered_map<int,std::shared_ptr<ASTNode>>& value,std::shared_ptr<ASTNode> type) : value(value),type(type) {}
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> getValue() { return value; }
    std::shared_ptr<ASTNode>  getType() { return type; }
    
private:
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> value;
    std::shared_ptr<ASTNode>  type;
};

class TSLInitNode : public ASTNode {
public:
    TSLInitNode(const std::shared_ptr<ASTNode>& value,const std::shared_ptr<ASTNode>& body)
        : value(value),body(body) {}
    const std::shared_ptr<ASTNode>& getExpr() const { 
        
        return value; }
    const std::shared_ptr<ASTNode>& getBody() const { return body; }

private:
    const std::shared_ptr<ASTNode> value;
    const std::shared_ptr<ASTNode> body;
};

class IndexNode : public ASTNode {
public:
    IndexNode(const std::shared_ptr<ASTNode>& value,std::shared_ptr<ASTNode> index) : value(value), index(index){}
    std::shared_ptr<ASTNode> getValue() { return value; }
    std::shared_ptr<ASTNode> getIndex() { return index; }

private:
    std::shared_ptr<ASTNode> value;
    std::shared_ptr<ASTNode> index;
};

class BinOP : public ASTNode {
public:
    BinOP(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right, const std::string& exp) : exp(exp),right(right),left(left) {}
    const std::string& getValue() const { return exp; }
    const std::shared_ptr<ASTNode> getleft() const {return left;}
    const std::shared_ptr<ASTNode> getright() const {return right;}
private:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    std::string exp;
};

class AssignNode : public ASTNode {
public:
    AssignNode(const std::string& varName, std::shared_ptr<ASTNode> value,bool mut=false)
        : varName(varName), value(value), mut(mut) {}

    const std::string& getVarName() const { return varName; }
    std::shared_ptr<ASTNode> getValue() const { return value; }
    bool getMut() const {return mut;}
    void setMut(bool v) {this->mut=v;}

private:
    std::string varName;
    std::shared_ptr<ASTNode> value;
    bool mut;
};

class ExprAssignNode : public ASTNode {
public:
    ExprAssignNode(std::shared_ptr<ASTNode> varName, std::shared_ptr<ASTNode> value,bool mut=false)
        : varName(varName), value(value), mut(mut) {}

    std::shared_ptr<ASTNode> getVarName() const { return varName; }
    std::shared_ptr<ASTNode> getValue() const { return value; }
    bool getMut() const {return mut;}
    void setMut(bool v) {this->mut=v;}

private:
    std::shared_ptr<ASTNode> varName;
    std::shared_ptr<ASTNode> value;
    bool mut;
};

class StrongAssignNode : public ASTNode {
public:
    StrongAssignNode(const std::string& varName, std::shared_ptr<ASTNode> value,std::shared_ptr<ASTNode> strongtype,bool mut=false)
        : varName(varName), value(value), strongtype(strongtype), mut(mut) {}

    const std::string& getVarName() const { return varName; }
    const std::shared_ptr<ASTNode> getType() const { return strongtype; }
    std::shared_ptr<ASTNode> getValue() const { return value; }
    bool getMut() const {return mut;}
    void setMut(bool v) {this->mut = v;}

private:
    std::string varName;
    std::shared_ptr<ASTNode> strongtype;
    std::shared_ptr<ASTNode> value;
    bool mut;
};


class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string value)
        : value(value) {}
    std::string getValue() const { return value; }

private:
    std::string value;
};

class RefNode : public ASTNode {
public:
    RefNode(std::shared_ptr<ASTNode>  value)
        : value(value) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }

private:
    std::shared_ptr<ASTNode> value;
};

class TagNode : public ASTNode {
public:
    TagNode(std::shared_ptr<ASTNode>  value)
        : value(value) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }

private:
    std::shared_ptr<ASTNode> value;
};

class SizeOfNode : public ASTNode {
public:
    SizeOfNode(std::shared_ptr<ASTNode>  value)
        : value(value) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }

private:
    std::shared_ptr<ASTNode> value;
};

class StackSizeNode : public ASTNode {
public:
    StackSizeNode(){}
    

private:
};

class PtrtointNode : public ASTNode {
public:
    PtrtointNode(std::shared_ptr<ASTNode>  value)
        : value(value) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }

private:
    std::shared_ptr<ASTNode> value;
};

// tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>
class ASMNode : public ASTNode {
public:
    ASMNode(std::string ASMStr,std::vector<std::string> regs,std::unordered_map<std::string,std::shared_ptr<ASTNode>> in,std::unordered_map<std::string,std::shared_ptr<ASTNode>>out,std::unordered_map<std::string,std::shared_ptr<ASTNode>>inout,std::unordered_map<std::string,std::shared_ptr<ASTNode>>assignables)
        : ASMStr(ASMStr),regs(regs), in(in), out(out),inout(inout),assignables(assignables) {}


    std::string ASMStr;
    std::vector<std::string> regs;
    std::unordered_map<std::string,std::shared_ptr<ASTNode>> in;
    std::unordered_map<std::string,std::shared_ptr<ASTNode>> out;
    std::unordered_map<std::string,std::shared_ptr<ASTNode>> inout;
    std::unordered_map<std::string,std::shared_ptr<ASTNode>> assignables;
private:
};
class TypeIDNode : public ASTNode {
public:
    TypeIDNode(std::shared_ptr<ASTNode>  value)
        : value(value) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }

private:
    std::shared_ptr<ASTNode> value;
};

class CastNode : public ASTNode {
public:
    CastNode(std::shared_ptr<ASTNode> value,std::shared_ptr<ASTNode> dest)
        : value(value), dest(dest) {}
    std::shared_ptr<ASTNode> getValue() { return value; }
    std::shared_ptr<ASTNode> getDest() { return dest; }

private:
    std::shared_ptr<ASTNode> value;
    std::shared_ptr<ASTNode> dest;
};

class PubNode : public ASTNode {
public:
    PubNode(std::shared_ptr<ASTNode> value)
        : value(value){}
    std::shared_ptr<ASTNode> getValue() { return value; }


private:
    std::shared_ptr<ASTNode> value;

};



class ExternNode : public ASTNode {
public:
    ExternNode(std::vector<std::shared_ptr<ASTNode>> value,std::vector<std::string> fnnames,std::vector<std::string> isvariadic)
        : value(value), fnnames(fnnames), isvariadic(isvariadic) {}
    std::vector<std::shared_ptr<ASTNode>> getValue() { return value; }
    std::vector<std::string> getfnnames() {return fnnames;}
    std::vector<std::string> getvdic() {return isvariadic;}

private:
    std::vector<std::shared_ptr<ASTNode>> value;
    std::vector<std::string> fnnames;
    std::vector<std::string> isvariadic;
};

class Fakepass : public ASTNode {
public:
    Fakepass(std::any value)
        : value(value) {}
    std::any getValue() const { return value; }

private:
    std::any value;
};

class FutureNode : public ASTNode {
  public:
      FutureNode(const std::string& value)
          : value(value) {}
      std::string getValue() const { return value; }

  private:
      std::string value;
  };

class InternalsNode : public ASTNode {
  public:
      InternalsNode(const std::string& value,const std::string& pub)
          : value(value),pub(pub){}
      std::string getValue() const { return value; }
      std::string getPub() const { return pub; }


  private:
      std::string value;

      std::string pub;
  };

class SearchableNode : public ASTNode {
public:
    SearchableNode(const std::string& value,tsl::ordered_map<int,std::shared_ptr<ASTNode>> tochecks)
        : value(value), tochecks(tochecks) {}
    std::string getValue() const { return value; }
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> getChecks() const {return tochecks;}

private:
    std::string value;
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> tochecks;
};

class MatchNode : public ASTNode {
public:
    MatchNode(const std::shared_ptr<ASTNode> value,tsl::ordered_map<std::shared_ptr<ASTNode>,std::shared_ptr<ASTNode>> ns,std::shared_ptr<ASTNode> elser=std::make_shared<ASTNode>())
        : value(value), ns(ns), elser(elser) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }
    std::shared_ptr<ASTNode> getElse() const { return elser; }
    tsl::ordered_map<std::shared_ptr<ASTNode>,std::shared_ptr<ASTNode>> getns(){return ns;}

private:
    std::shared_ptr<ASTNode> elser;
    std::shared_ptr<ASTNode> value;
    tsl::ordered_map<std::shared_ptr<ASTNode>,std::shared_ptr<ASTNode>>ns;
};

class DeconsNode : public ASTNode {
public:
    DeconsNode(std::shared_ptr<ASTNode>value, std::vector<std::string> idents)
        : value(value), idents(idents) {}
    std::shared_ptr<ASTNode> getValue() { return value; }

    std::vector<std::string> getIdents() const {return idents;}

    void setValue(std::shared_ptr<ASTNode> val){
        this->value = val;
    }

private:
    std::shared_ptr<ASTNode> value;
    std::vector<std::string> idents;
};

class NUnararyNode : public ASTNode {
public:
    NUnararyNode(const std::shared_ptr<ASTNode> value, const std::shared_ptr<ASTNode> elser)
        : value(value), elser(elser) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }
    std::shared_ptr<ASTNode> getElse() const { return elser; }

private:
    std::shared_ptr<ASTNode> value;
    std::shared_ptr<ASTNode> elser;
};

class IfLetNode : public ASTNode {
public:
    IfLetNode(const std::shared_ptr<ASTNode> value, const std::shared_ptr<ASTNode> body,const std::shared_ptr<ASTNode> elser)
        : value(value), body(body), elser(elser) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }
    std::shared_ptr<ASTNode> getBody() const { return body; }
    std::shared_ptr<ASTNode> getElse() const { return elser; }

private:
    std::shared_ptr<ASTNode> value;
    std::shared_ptr<ASTNode> body;
    std::shared_ptr<ASTNode> elser;
};

class EnumNode : public ASTNode {
public:
    EnumNode(const tsl::ordered_map<std::string,std::vector<std::shared_ptr<ASTNode>>> value)
        : value(value) {}
    const tsl::ordered_map<std::string,std::vector<std::shared_ptr<ASTNode>>> getValue() const { return value; }

private:
    const tsl::ordered_map<std::string,std::vector<std::shared_ptr<ASTNode>>> value;
};

class DropNode : public ASTNode {
public:
    DropNode(std::shared_ptr<ASTNode> value)
        : value(value) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }

private:
    std::shared_ptr<ASTNode> value;
};

class ModNode : public ASTNode {
public:
    ModNode(std::shared_ptr<ASTNode> value,std::vector<std::shared_ptr<ASTNode>> states)
        : value(value), states(states) {}
    std::shared_ptr<ASTNode> getValue() const { return value; }
    std::vector<std::shared_ptr<ASTNode>> getStates() const {return states;}

private:
    std::shared_ptr<ASTNode> value;
    std::vector<std::shared_ptr<ASTNode>> states;
};



class ExpressionNode : public ASTNode {
public:
    ExpressionNode(const std::shared_ptr<ASTNode>& value,const std::shared_ptr<ASTNode>& body)
        : value(value),body(body) {}
    const std::shared_ptr<ASTNode>& getExpr() const { 
        
        return value; }
    const std::shared_ptr<ASTNode>& getBody() const { return body; }

private:
    const std::shared_ptr<ASTNode> value;
    const std::shared_ptr<ASTNode> body;
};

class DecoratorNode : public ASTNode {
public:
    DecoratorNode(const std::shared_ptr<ASTNode>& value,const std::shared_ptr<ASTNode>& body)
        : value(value),body(body) {}
    const std::shared_ptr<ASTNode>& getClr() const { 
        
        return value; }
    const std::shared_ptr<ASTNode>& getFn() const { return body; }

private:
    const std::shared_ptr<ASTNode> value;
    const std::shared_ptr<ASTNode> body;
};

class AssertionNode : public ASTNode {
public:
    AssertionNode(std::shared_ptr<ASTNode> value)
        : value(value){}
    std::shared_ptr<ASTNode> getExpr() const { 
        
        return value; }


private:
    std::shared_ptr<ASTNode> value;

};

class TONode : public ASTNode {
public:
    TONode(const std::shared_ptr<ASTNode>& value)
        : value(value) {}
    const std::shared_ptr<ASTNode>& getExpr() const { 
        
        return value; }
    

private:
    const std::shared_ptr<ASTNode> value;
    
};

class CallNode : public ASTNode {
public:
    CallNode(tsl::ordered_map<int,std::shared_ptr<ASTNode>> value, std::shared_ptr<ASTNode> body,tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> inserts)
        : value(value),body(body),inserts(inserts) {}
     tsl::ordered_map<int,std::shared_ptr<ASTNode>> getExpr() { 
        //println(typeid(value).name());
        return value; }
     std::shared_ptr<ASTNode> getBody()  { return body; }
     void setBody(std::shared_ptr<ASTNode> b) {
        this->body = b;
    }
     void setExpr( tsl::ordered_map<int,std::shared_ptr<ASTNode>> v){
        this->value = v;
    }

    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> getinserts(){
        return inserts;
    }

private:
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> value;
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> inserts;
    std::shared_ptr<ASTNode> body;
    
};


class StructDeclNode : public ASTNode {
    public:
    StructDeclNode( tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> value,tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  constants) : value(value), cons(constants) {}
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  getValue() {return value;}
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> getCons(){return cons;}
    private:
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  value;
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  cons;
};

class ClassDeclNode : public ASTNode {
    public:
    ClassDeclNode(const tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> value,tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  constants) : value(value), cons(constants) {}
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  getValue() {return value;}
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> getCons(){return cons;}
    private:
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  value;
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>>  cons;
};


class BreakNode : public ASTNode {
public:
    BreakNode(const std::shared_ptr<ASTNode> value)
        : value(value){}
    const std::shared_ptr<ASTNode> getValue() const { return value; }
private:
    const std::shared_ptr<ASTNode> value;
};

class RetNode : public ASTNode {
public:
    RetNode(const std::shared_ptr<ASTNode> value)
        : value(value){}
    const std::shared_ptr<ASTNode> getValue() const { return value; }
private:
    const std::shared_ptr<ASTNode> value;
};

class StructInstanceNode: public ASTNode {
    public:
    StructInstanceNode( tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> value,std::shared_ptr<ASTNode> base)
        : value(value), base(base) {}
     tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> getValue() { return value; }
    const std::shared_ptr<ASTNode> getBase() const {return base;}
    private:
     tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> value;
    std::shared_ptr<ASTNode> base;
};

class IFNode : public ASTNode {
public:
    IFNode(const std::shared_ptr<ExpressionNode>& value,const tsl::ordered_map<int,std::shared_ptr<ExpressionNode>> elses,const std::shared_ptr<ExpressionNode>& nott)
        : value(value), elses(elses), nott(nott) {}
    tsl::ordered_map<int,std::shared_ptr<ExpressionNode>> getElses() const { return elses; }
    std::shared_ptr<ExpressionNode> getMain() const { return value; }
    std::shared_ptr<ExpressionNode> getNot() const { return nott; }

private:
    std::shared_ptr<ExpressionNode> value;
    std::shared_ptr<ExpressionNode> nott;
    tsl::ordered_map<int,std::shared_ptr<ExpressionNode>> elses;
};

class WhileNode : public ASTNode {
public:
    WhileNode(const std::shared_ptr<ExpressionNode>& expr) : expr(expr) {}
    std::shared_ptr<ExpressionNode> getExpr() { return expr;}

private:
std::shared_ptr<ExpressionNode> expr;

};

// need to make a growable vector
class BlockNode : public ASTNode {
public:
    void addStatement(std::shared_ptr<ASTNode> statement) {
        statements.push_back(statement);
    }
    int StatementsLen() {
        return statements.size();
    }

    const std::vector<std::shared_ptr<ASTNode>>& getStatements() const {
        return statements;
    }

private:
    std::vector<std::shared_ptr<ASTNode>> statements;
};

class Future{
    public:
      Future(std::string ref,std::shared_ptr<Scope> cs): ref(ref),cs(cs) {}
      std::any get() {
          if (value.has_value()){
              return value;
          } else {
              if (justincase){
                  throw std::runtime_error("Corrupted future.");
              }
              this->value = cs->getVariable(ref)->getValue();
              this->justincase = true;
              return value;


          }
      }
    private:
      bool justincase = false;
      std::string ref;
      std::shared_ptr<Scope> cs;
      std::any value;
};

// Type checkers
template <typename K,typename T>
bool compareOMap(const tsl::ordered_map<K, T>& map1, const tsl::ordered_map<K, T>& map2);
template <typename K,typename T>
bool compareUMap(const std::unordered_map<K, T>& map1, const std::unordered_map<K, T>& map2);
bool compareAnyTy(std::any a, std::any b) {
    logat(demangle(a.type().name()) + " == " + demangle(b.type().name()),"cAT");
    bool checker = false;
    if (a.type() == typeid(std::shared_ptr<Future>) && b.type() == typeid(std::shared_ptr<Future>)){
        return true;
    }
    if (a.type() == typeid(std::shared_ptr<Future>)){
        return compareAnyTy((std::any_cast<std::shared_ptr<Future>>(a))->get(),b);
    }
    if (a.type() == typeid(Enum) && b.type() == typeid(EnumKey)){
        const auto& EnumA = std::any_cast<Enum>(a);
        const auto& EKB = std::any_cast<EnumKey>(b);
        return compareAnyTy(EnumA,EKB.getBase());
    }
    if (a.type() == typeid(StructDecl) && b.type() == typeid(StructInstance)){
        auto& sstructA = std::any_cast<StructDecl&>(a);
        auto& sinstanceB = std::any_cast<StructInstance&>(b);
        auto fields = sstructA.getValue();
        auto ifields = sinstanceB.getValue();
        for (const auto& f : fields){
            if (ifields.find(f.first) == ifields.end()){
                return false;
            }
            if (compareAnyTy(fields[f.first],ifields[f.first])  == false){
                return false;
            }
        }
        auto cons = sstructA.getConsts();
        auto cons2 = sinstanceB.getCons();
        if (!compareMap(cons,cons)) return false;
        auto fns = sstructA.getFns();
        auto fns2 = sinstanceB.getFns();
        if (!compareMap(fns,fns2)) return false;
        return true;
    }
    if (b.type() == typeid(StructDecl) && a.type() == typeid(StructInstance)){
        auto& sstructA = std::any_cast<StructDecl&>(b);
        auto& sinstanceB = std::any_cast<StructInstance&>(a);
        auto fields = sstructA.getValue();
        auto ifields = sinstanceB.getValue();
        for (const auto& f : fields){
            if (ifields.find(f.first) == ifields.end()){
                return false;
            }
            if (compareAnyTy(fields[f.first],ifields[f.first])  == false){
                return false;
            }
        }
        auto cons = sstructA.getConsts();
        auto cons2 = sinstanceB.getCons();
        if (!compareMap(cons,cons)) return false;
        auto fns = sstructA.getFns();
        auto fns2 = sinstanceB.getFns();
        if (!compareMap(fns,fns2)) return false;
        return true;
    }
    if (a.type() == typeid(Class) && b.type() == typeid(ClassInstance)){
        const auto& c = std::any_cast<Class>(a);
        const auto& ci = std::any_cast<ClassInstance>(b);
        return compareMap(c.getConsts(),ci.getConsts()) && compareMap(c.getValue(),ci.getValue());
    }
    if (a.type() == typeid(Object) && b.type() != typeid(Object)){
        return compareAnyTy(std::any_cast<Object>(a).GetStore(),b);
    }
    if (b.type() == typeid(Object) && a.type() != typeid(Object)){
        return compareAnyTy(std::any_cast<Object>(b).GetStore(),a);
    }

    if (a.type() != b.type()) return false;
    // change to getT
    if (a.type() == typeid(StructDecl)) {
        auto& structA = std::any_cast<StructDecl&>(a);
        auto& structB = std::any_cast<StructDecl&>(b);
        return compareOMap<std::string>(structA.getValue(), structB.getValue()) && compareMap(structA.getConsts(),structB.getConsts()) && compareMap(structA.getFns(),structB.getFns());
    } else if (a.type() == typeid(StructInstance)) {
        auto& instanceA = std::any_cast<StructInstance&>(a);
        auto& instanceB = std::any_cast<StructInstance&>(b);
        return compareOMap<std::string>(instanceA.getValue(),instanceB.getValue()) && compareMap(instanceA.getCons(),instanceB.getCons()) && compareMap(instanceA.getFns(),instanceB.getFns());
    } else if (a.type() == typeid(EnumKey)) {


        auto& enumkA = std::any_cast<EnumKey&>(a);
        auto& enumkB = std::any_cast<EnumKey&>(b);

        if (enumkA.getValue().size() != enumkB.getValue().size()){
            return false;
        }
        if (enumkA.getName() != enumkB.getName()){
            return false;
        }
        for(int i = 0;i != enumkA.getValue().size();i++) {
            if (compareAnyTy(enumkA.getValue()[i],enumkB.getValue()[i]) == false){
                return false;
            }
        }
        return true;
    } else if (a.type() == typeid(Enum)) {
        const auto& enumA = std::any_cast<const Enum&>(a);
        const auto& enumB = std::any_cast<const Enum&>(b);
        return compareUMap<std::string,EnumKey>(enumA.getValue(),enumB.getValue());
    } else if (a.type() == typeid(TypeSafeList)) {
         auto& listA = std::any_cast< TypeSafeList&>(a);
         auto& listB = std::any_cast< TypeSafeList&>(b);
        if (listA.getSZU()){
            return compareAnyTy(listA.getType(),listB.getType());
        }
        return listA.len() == listB.len() && compareAnyTy(listA.getType(),listB.getType());
    } else if (a.type() == typeid(Tuple)) {
         auto& tupleA = std::any_cast< Tuple&>(a);
         auto& tupleB = std::any_cast< Tuple&>(b);
        return tupleA.len() == tupleB.len() && compareMap(tupleA.getcomparer(),tupleB.getcomparer());
    } else if (a.type() == typeid(List)) {
        auto& listA = std::any_cast<List&>(a);
        auto& listB = std::any_cast<List&>(b);
        return listA.len() == listB.len();
    } else if (a.type() == typeid(PtrVal)) {
        const auto& ptrA = std::any_cast<const PtrVal&>(a);
        const auto& ptrB = std::any_cast<const PtrVal&>(b);
        return compareAnyTy(ptrA.getValue(), ptrB.getValue());
    } else if (a.type() == typeid(PtrVal*)) {
    const auto* ptrA = std::any_cast<const PtrVal*>(a);
    const auto* ptrB = std::any_cast<const PtrVal*>(b);

    // Compare the pointers directly
    if (ptrA == ptrB) {
        return true; // Same reference
    }} else if (a.type() == typeid(Pointer)) {
        const auto& ptrA = std::any_cast<const Pointer&>(a);
        const auto& ptrB = std::any_cast<const Pointer&>(b);
        return compareAnyTy(ptrA.getptr()->getValue(),ptrB.getptr()->getValue()) && ptrA.getMut() == ptrA.getMut();
    } else if (a.type() == typeid(BorrowedPointer)) {
        const auto& borrowA = std::any_cast<const BorrowedPointer&>(a);
        const auto& borrowB = std::any_cast<const BorrowedPointer&>(b);
        return borrowA.isMut() == borrowB.isMut() && getTypeName(borrowA.getptr()->getValue()) == getTypeName(borrowB.getptr()->getValue());
    } else if (a.type() == typeid(Class)){
        const auto& c = std::any_cast<Class>(a);
        const auto& cb = std::any_cast<Class>(b);
        return compareMap(c.getConsts(),cb.getConsts()) && compareMap(c.getValue(),cb.getValue());
    } else if (a.type() == typeid(ClassInstance)){
        const auto& c = std::any_cast<ClassInstance>(a);
        const auto& cb = std::any_cast<ClassInstance>(b);
        return compareMap(c.getConsts(),cb.getConsts()) && compareMap(c.getValue(),cb.getValue());
    } else if (a.type() == typeid(Object)) {
        const auto& objA = std::any_cast<const Object&>(a);
        const auto& objB = std::any_cast<const Object&>(b);
        return objA.getType() == objB.getType(); // make this better
    } else if (a.type() == typeid(MappedFunction)) {
        auto& MFNA = std::any_cast<MappedFunction&>(a);
        auto& MFNB = std::any_cast<MappedFunction&>(b);
        auto MFNAT = MFNA.gettick();

        auto MFNBT = MFNB.gettick();
        if (MFNAT.size() != MFNBT.size()){
            return false;
        }
        auto MFNAA = MFNA.getargs();
        auto MFNBA = MFNB.getargs();
        //println("argcheck");
        for (const auto& k: MFNAT){

            if ((k.second != "self" && k.first != 0) || k.second != "self"){

            if (compareAnyTy(MFNAA[k.second],MFNBA[MFNBT[k.first]]) == false){
               
                return false;
            }} else if (MFNBT[0] != "self"){
                return false;
            } else {
                return true;
            }


        }


        if (compareAnyTy(MFNA.getretty(),MFNB.getretty()) == false){
            return false;
        }

        return true;
    } else if (a.type() == b.type()){
        return true;
    }


    return false; 
}

template <typename K,typename T>
bool compareMap(const tsl::ordered_map<K, T>& map1, const tsl::ordered_map<K, T>& map2) {

    if (map1.size() != map2.size()) return false;
    for (const auto& [key, value1] : map1) {
        logat(convertToString(key),"cM");
        const auto it = map2.find(key);
        if (it == map2.end()) return false;
        const T& value2 = it->second;
        if (compareAnyTy(value1, value2) == false) return false;
    }
    return true;
}
template <typename K,typename T>
bool compareOMap(const tsl::ordered_map<K, T>& map1, const tsl::ordered_map<K, T>& map2) {

    if (map1.size() != map2.size()) return false;
    for (const auto& [key, value1] : map1) {
        const auto it = map2.find(key);
        if (it == map2.end()) return false;
        const T& value2 = it->second;
        if (compareAnyTy(value1, value2) == false) return false;
    }
    return true;
}

template <typename K,typename T>
bool compareUMap(const std::unordered_map<K, T>& map1, const std::unordered_map<K, T>& map2) {

    if (map1.size() != map2.size()) return false;
    for (const auto& [key, value1] : map1) {
        const auto it = map2.find(key);
        if (it == map2.end()) return false;
        const T& value2 = it->second;
        if (compareAnyTy(value1, value2) == false) return false;
    }
    return true;
}


// Pretty self explanatory to be a Parser
class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer) {
        
        ////println(currentToken.value);
        currentToken = lexer.getNextToken();  // Get the first token
    }

    // Parse the entire input and return the root AST node
    std::shared_ptr<BlockNode> parse() {
        return parseBlock();
    }

private:
    Lexer& lexer;
    Token currentToken;
    tsl::ordered_map<std::string,int> structs;


    void advance() {
        currentToken = lexer.getNextToken();
    }
    void rb(int pos){
        
        lexer.rollback(pos-1);
        currentToken = lexer.getNextToken();
        logat("Rolldback to " + currentToken.value,"P.r");

    }


    bool match(TokenType expected) {
        if (currentToken.type == expected) {
            advance();
            return true;
        }
        displayError("Expected '" + tokenTypeToString(expected) + "'\nGot: '" + currentToken.value + "'",lexer.line,lexer.poswline,lexer.poswline + currentToken.value.size(),gerr({"note","Please refer to the documentation for how to implement your program"}) );
        
        return false;
    }


    void error(const std::string& msg) {
        throw std::runtime_error("Parsing error: " + msg);
    }

    // Parse a block (i.e., {...})
    std::shared_ptr<BlockNode> parseBlock() {
        if (!match(TokenType::LBrace)) {
            
        }

        auto block = std::make_shared<BlockNode>();
        while (currentToken.type != TokenType::RBrace && currentToken.type != TokenType::EndOfFile) {
            block->addStatement(parseStatement());
        }

        if (!match(TokenType::RBrace)) {
            
        }
        return block;
    }
    std::shared_ptr<ASTNode> parseExtern(){
        advance(); // extern
        auto apos = lexer.poswline;
        auto lang = consume(TokenType::Identifier);
        if (lang == "C"){
            std::vector<std::shared_ptr<ASTNode>> fns;
            std::vector<std::string> fnnames;
            std::vector<std::string> isvariadic;
            match(TokenType::LBrace);
            while (currentToken.type != TokenType::RBrace){
                auto s = currentToken.value;
                if (s != "fn"){
                    displayError("Expected 'fn' keyword in extern decleration",lexer.line,lexer.poswline,lexer.poswline+ s.size(),{});
                }
                match(TokenType::KeyWord); // fn

                auto fnname= currentToken.value;
                fnnames.push_back(fnname);
                advance(); // foo
                match(TokenType::LParen); // (
                tsl::ordered_map<std::string, std::shared_ptr<ASTNode>> args;
                tsl::ordered_map<int,std::string> tick;
                tsl::ordered_map<std::string,std::string> internals;
                int a = 0;
                while (currentToken.type != TokenType::RParen){
                    if (currentToken.type != TokenType::Variadic){

                    auto aid = consume(TokenType::Identifier);
                    match(TokenType::Colon);
                    
                    args[aid] = parseExpression();
                    tick[a] = aid;
                    a += 1;
                    } else {
                        advance(); // ...
                        isvariadic.push_back(fnname);
                        if (currentToken.type != TokenType::RParen){
                            auto bpos = lexer.poswline;
                            auto ctoken = currentToken.value;
                            advance();
                            tsl::ordered_map<std::string,std::string> notes;
                            notes["note"] = "A variadic operator (...) signifies the end of the function decleration";
                            displayError("Following the variadic operator (...) must be ')'\nGot: '" + ctoken + "'" ,lexer.line,bpos,lexer.poswline,notes);
                        }

                    }
                    if (currentToken.type != TokenType::RParen){
                        match(TokenType::COMMA);
                    }

                }
                match(TokenType::RParen);
                if (currentToken.type != TokenType::ReturnTy){
                    tsl::ordered_map<std::string,std::string> notes;
                    notes["note"] = "Maven does not allow 'void' types to be returned therefore a specified return type is required";
                    displayError("Expected '->' in extern function decleration",lexer.line,lexer.poswline,lexer.poswline+2,notes);
                }
                match(TokenType::ReturnTy);
                args["-!retty"] = parseExpression();

                fns.push_back(std::make_shared<MappedFunctionNode>(args,std::make_shared<ASTNode>(),tick,internals));


            }
            advance(); // }
            return std::make_shared<ExternNode>(fns,fnnames,isvariadic);
        } else {
            displayError("Unsupported extern, currently supported externs are: C",lexer.line,apos,lexer.poswline,{});
        }
    }
    std::shared_ptr<ASTNode> parseFunction(){
        bool anon;
        if (currentToken.type == TokenType::Anon){
            anon = true;
        }
        advance(); // fn/||
        std::string fnname;
        if (!anon){
        fnname = consume(TokenType::Identifier);
        }
        consume(TokenType::LParen); // (
        bool map = false;
        bool isselfptr = false;
        std::shared_ptr<ASTNode> retty;
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> args;
        tsl::ordered_map<int, std::string> tick;
        tsl::ordered_map<std::string, std::string> internals;
        tsl::ordered_map<int,std::string> argus;
        int a = 0;
        while (currentToken.type != TokenType::RParen){
            std::string name;
            if (currentToken.value == "@|" || currentToken.value == "@"){
                auto res = parseExpression();
                if (auto inte = std::dynamic_pointer_cast<InternalsNode>(res)){
                    name = inte->getPub();
                    internals[name] = inte->getValue();
                } else {
                    throw std::runtime_error("Unsupported Decree.");
                }
            } else {
                if (currentToken.type == TokenType::Ampersand && a == 0){
                    advance();
                    auto apos = lexer.poswline;
                    name = consume(TokenType::Identifier);
                    if (name != "self"){
                        displayError("Only self may start with an ampersand(&), no other identifier may.",lexer.line,apos,lexer.poswline,gerr({"note","Did you mean to put self? If not:","[O]","fn foo(arg:&type)...","[X]","fn foo(&arg)..."}));

                    } else {
                        isselfptr = true;
                    }
                } else {
                    name = consume(TokenType::Identifier); // arg
                }
            }
            if ((currentToken.type == TokenType::Colon || ruleset["typing"] == "static") && a == 0){
                if (name != "self"){

                
                match(TokenType::Colon); // :
                map = true;
                args[name] = parseExpression();
                tick[a] = name;
                } else {
                    map = true;
                    args[name] = std::shared_ptr<ASTNode>();
                    tick[a] = name;
                }


            }
            if (map == true && a != 0){
                consume(TokenType::Colon);
                args[name]= parseExpression();
                tick[a] = name;
            } else if (map == false){
                argus[a] = name; // unmapped
            }
            if (currentToken.type != TokenType::RParen){
                
                consume(TokenType::COMMA);
            }

            
            a += 1;
        }
        advance(); // )
        if (ruleset["typing"] == "static"){
            map = true;
        }
        // args r done now
        if (map == true){
            if (currentToken.type != TokenType::ReturnTy){
                retty = std::make_shared<IdentifierNode>("none");
            } else {
            consume(TokenType::ReturnTy);
            retty = parseExpression();
            }
            args["-!retty"] = retty;
            if (currentToken.type == TokenType::LBrace){
            if (anon){
                return std::make_shared<MappedFunctionNode>(args,parseBlock(),tick,internals,isselfptr);
            }
            return std::make_shared<AssignNode>(fnname,std::make_shared<MappedFunctionNode>(args,parseBlock(),tick,internals,isselfptr));
            } else {
                if (anon){
                    return std::make_shared<MappedFunctionNode>(args,std::make_shared<BlockNode>(),tick,internals,isselfptr);
                }
                return std::make_shared<AssignNode>(fnname,std::make_shared<MappedFunctionNode>(args,std::make_shared<BlockNode>(),tick,internals,isselfptr));
            }
        }
        // all that's left is the noding then interpretations
        if (anon){
            return std::make_shared<UnMappedFunctionNode>(argus,parseBlock());
        }

        return std::make_shared<AssignNode>(fnname,std::make_shared<UnMappedFunctionNode>(argus,parseBlock()));
        
    }
    std::shared_ptr<ASTNode> parseSizeOf(){
        advance(); // szof/sizeof
        return std::make_shared<SizeOfNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parseStackSize(){
        advance(); // StackSize
        return std::make_shared<StackSizeNode>();
    }
    std::shared_ptr<ASTNode> parseP2I(){
        advance(); // pti/ptrtoint
        return std::make_shared<PtrtointNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parseTypeid(){
        advance(); // szof/sizeof
        return std::make_shared<TypeIDNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parseDecorator(){
        advance(); // @
        if (currentToken.value == "sig" || currentToken.value == "signature" ){
            return parseSignature();
        }
        if (currentToken.value == "unsafe" || currentToken.value == "usafe" ){
            return parseUnsafe();
        }
        if (currentToken.value == "safe"){
            return parseSafe();
        }
        if (currentToken.value == "i" || currentToken.value == "internal" ){
            return parseInternal();
        }
        if (currentToken.value == "szof" || currentToken.value == "sizeof" ){
            return parseSizeOf(); // uint64_t TypeSize = DL.getTypeAllocSize(LLVMType); // llvm::DataLayout DL(M);
        }
        if (currentToken.value == "tyid" || currentToken.value == "typeid" ){
            return parseTypeid();
        }
        if (currentToken.value == "pti" || currentToken.value == "ptrtoint" ){
            return parseP2I();
        }
        if (currentToken.value == "StackSize"){
            return parseStackSize();
        }
        auto clr = parseExpression();
        auto fn = parseStatement();
        return std::make_shared<DecoratorNode>(clr,fn);
        // node time!
    }
    std::shared_ptr<ASTNode> ParseRet(){
        int l = lexer.line;
        advance(); // ret
        std::shared_ptr<ASTNode> expr;
        
        if (lexer.line == l){
            
            expr = parseExpression();
        }
        return std::make_shared<RetNode>(expr);
    }
    // Parse a statement, which can be an expression or a block
    std::shared_ptr<ASTNode> parseHashTag(){
        match(TokenType::Hashtag); // #
        auto idpos = lexer.poswline;
        std::string id;
        if (currentToken.type == TokenType::LParen){
            advance(); // (
            id = consume(TokenType::Identifier);
            consume(TokenType::RParen);

        } else {
            id = consume(TokenType::Identifier);
        }
        
        if (id == "ruleset"){
            match(TokenType::LParen);
            auto rid = consume(TokenType::Identifier);
            match(TokenType::Assign);
            int apos = lexer.poswline;
            auto exp = parseExpression();
            if (auto expn = std::dynamic_pointer_cast<StringLiteralNode>(exp)){
                ruleset[rid] = expn->getValue();
                
            } else {
                displayError("Ruleset expression must be a string",lexer.line,apos,lexer.poswline,gerr({"note","ruleset(typing=...) can only be declared as a string","[X]","ruleset(typing=...)","[O]","ruleset(typing=\"static\") or ruleset(typing=\"dynamic\")"}) );
            }
            
            
            
            match(TokenType::RParen);
            

        } else if (id == "link"){
            match(TokenType::LParen);
            auto rid = consume(TokenType::Identifier);
            match(TokenType::Assign);
            int apos = lexer.poswline;
            auto exp = parseExpression();
            if (auto expn = std::dynamic_pointer_cast<StringLiteralNode>(exp)){
                if (AP.values.find("link") == AP.values.end()){
                    AP.values["link"] = {};
                }
                AP.values["link"].push_back(expn->getValue());
                
            } else {
                displayError("Link expression must be a string",lexer.line,apos,lexer.poswline,gerr({"note","link(path=...) can only be declared as a string","[X]","link(path=...)","[O]","link(path=\"/Users/username/Documents/x.lib\")"}) );
            }
            
            
            
            match(TokenType::RParen);
            

        } else if (id == "linklib"){
            match(TokenType::LParen);
            auto rid = consume(TokenType::Identifier);
            match(TokenType::Assign);
            int apos = lexer.poswline;
            auto exp = parseExpression();
            if (auto expn = std::dynamic_pointer_cast<StringLiteralNode>(exp)){
                if (AP.values.find("linklib") == AP.values.end()){
                    AP.values["linklib"] = {};
                }
                AP.values["linklib"].push_back(expn->getValue());
                
            } else {
                displayError("Linklib expression must be a string",lexer.line,apos,lexer.poswline,gerr({"note","linklib(name=...) can only be declared as a string","[X]","link(name=...)","[O]","link(path=\"libm\") or link(name=\"m\")"}) );
            }
            
            
            
            match(TokenType::RParen);
            

        } else if (id == "result"){
            match(TokenType::LParen);
            auto rid = consume(TokenType::Identifier);
            result = rid;
            
            
            
            match(TokenType::RParen);
            

        } else if (id == "no_builtins"){
            // just for the check so no error
        } else if (id == "allow"){
            consume(TokenType::LParen);
            while (currentToken.type != TokenType::RParen){
                allowances.push_back(consume(TokenType::Identifier));
                if (currentToken.type != TokenType::RParen){
                    consume(TokenType::COMMA);
                }
            }
            consume(TokenType::RParen);
        }else {
            displayError("Invalid identifier for '#'\nGot: " + id,lexer.line,idpos,lexer.poswline-2,gerr({"note","Structure of '#' is #<id>(<key>=<type>) or #(<id>)","supported <id>s","\n- ruleset"}) );
        }
        
        auto austin = std::make_shared<ASTNode>(); isbadnode.push_back(austin->id); return austin;
    }
    std::shared_ptr<ASTNode> parseAssert(){
        advance(); // assert
        return std::make_shared<AssertionNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parseMod(){
        advance(); // mod
        auto id = parseIdentifier(); // must be id
        match(TokenType::LBrace);
        std::vector<std::shared_ptr<ASTNode>> states;
        while (currentToken.type != TokenType::RBrace){
            states.push_back(parseStatement());
        }
        match(TokenType::RBrace);
        return std::make_shared<ModNode>(id,states);
    }
    std::shared_ptr<ASTNode> parseTodo(){
        advance(); // todo
        match(TokenType::LBrace);
        int a = 0;
        while (currentToken.type != TokenType::RBrace || a != 0){
            if (currentToken.type == TokenType::LBrace){
                a += 1;
            } else if (currentToken.type == TokenType::RBrace){
                a -= 1;
            }
            advance();
        }
        match(TokenType::RBrace);
        auto austin = std::make_shared<ASTNode>(); isbadnode.push_back(austin->id); return austin;
    }
    std::shared_ptr<ASTNode> parseDrop(){
        advance(); // drop
        return std::make_shared<DropNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parseEnum(){
        tsl::ordered_map<std::string,std::vector<std::shared_ptr<ASTNode>>> ret;
        advance(); // enum
        auto enname = consume(TokenType::Identifier);
        match(TokenType::LBrace);
        while (currentToken.type != TokenType::RBrace){
            auto id = consume(TokenType::Identifier);
            ret[id] = {};
            if (currentToken.type == TokenType::LParen){
                match(TokenType::LParen);
                while (currentToken.type != TokenType::RParen){
                ret[id].push_back(parseExpression());
                if (currentToken.type != TokenType::RParen){
                    match(TokenType::COMMA);
                }
                }
                match(TokenType::RParen);
                
            }
            else {
                
            }
            if (currentToken.type != TokenType::RBrace){
                match(TokenType::COMMA);
            }
        }
        match(TokenType::RBrace);
        return std::make_shared<AssignNode>(enname,std::make_shared<EnumNode>(ret));
    }
    std::shared_ptr<ASTNode> parseMatch(){

        advance(); // match
        auto expr = parseExpression();
        tsl::ordered_map<std::shared_ptr<ASTNode>,std::shared_ptr<ASTNode>> nodesbodies;
        std::shared_ptr<ASTNode> elser;
        match(TokenType::LBrace);
        while (currentToken.type != TokenType::RBrace){
            if (currentToken.value == "_"){
                advance(); // _
                match(TokenType::EqArrow);
                elser = parseExpression();
            } else {
            auto expr1 = parseExpression();
            match(TokenType::EqArrow);
            nodesbodies[expr1] = parseBlock();
            }
        }
        match(TokenType::RBrace);
        return std::make_shared<MatchNode>(expr,nodesbodies,elser);
    }
    std::shared_ptr<ASTNode> parseClass(){
        advance(); // class
        auto n = consume(TokenType::Identifier); // x
        match(TokenType::LBrace);
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> cons;
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> fns;
        while (currentToken.type != TokenType::RBrace){
            auto stat = parseStatement();
            if (auto aa = std::dynamic_pointer_cast<AssignNode>(stat)){
                if (auto fn = std::dynamic_pointer_cast<MappedFunctionNode>(aa->getValue())){
                    fns[aa->getVarName()] = fn;
                }
                cons[aa->getVarName()] = aa->getValue();
            } else {
                throw std::runtime_error("smth");
            }

        }
        match(TokenType::RBrace);
        return std::make_shared<AssignNode>(n,(std::make_shared<ClassDeclNode>(fns,cons)));

    }
    std::shared_ptr<ASTNode> parsePub(){
        advance(); // pub/public
        auto exp = parseStatement();
        return std::make_shared<PubNode>(exp);

    }
    std::shared_ptr<ASTNode> parseInline(){
        advance(); // inline
        if (currentToken.value == "asm"){
            advance(); // asm
            consume(TokenType::LParen);
            std::string ASMStr = std::dynamic_pointer_cast<StringLiteralNode>(parseStringLiteral())->getValue();
            if (currentToken.type == TokenType::COMMA){
                advance(); // ,
            }
            std::vector<std::string> regs;
            std::unordered_map<std::string,std::shared_ptr<ASTNode>> in;
            std::unordered_map<std::string,std::shared_ptr<ASTNode>> out;
            std::unordered_map<std::string,std::shared_ptr<ASTNode>> inout;
            std::unordered_map<std::string,std::shared_ptr<ASTNode>> assigns;
            while (currentToken.type != TokenType::RParen){
                if (currentToken.value == "in"){
                    advance(); // in
                    consume(TokenType::LParen);
                    auto id = consume(TokenType::Identifier);
                    in[id] = std::make_shared<IdentifierNode>(id);
                    regs.push_back(id);
                    consume(TokenType::RParen);
                } else if (currentToken.value == "out"){
                    advance(); // out
                    consume(TokenType::LParen);
                    std::string id = consume(TokenType::Identifier);
                    out[id] = std::make_shared<StringLiteralNode>("outasm");
                    regs.push_back(id);
                    consume(TokenType::RParen);
                    consume(TokenType::KeyWord); // as (verify this better)
                    assigns[id] = parseExpression();
                } else if (currentToken.value == "inout"){
                    advance(); // inout
                    consume(TokenType::LParen);
                    std::string id = consume(TokenType::Identifier);

                    consume(TokenType::RParen);
                    if (currentToken.type != TokenType::EqArrow){
                    inout[id] = std::make_shared<IdentifierNode>(id);
                    regs.push_back(id);
                    assigns[id] = std::make_shared<IdentifierNode>(id);
                    } else {
                    advance(); // =>
                    auto nid = consume(TokenType::Identifier);
                    in[id] = std::make_shared<IdentifierNode>(id);
                    out[nid] = std::make_shared<IdentifierNode>(id);
                    regs.push_back(id);
                    regs.push_back(nid);
                    assigns[id] = std::make_shared<IdentifierNode>(id);
                    assigns[nid] = std::make_shared<IdentifierNode>(id);
                    }
                }
                if (currentToken.type != TokenType::RParen){
                    consume(TokenType::COMMA);
                }
            }
            consume(TokenType::RParen);
            return std::make_shared<ASMNode>(ASMStr,regs,in,out,inout,assigns);
        }
    }
    std::shared_ptr<ASTNode> symbolize(SymbolEntry symbol){
        try {
        if (symbol.dataType == "unknown"){
            return nullptr;
        }
        if (symbol.dataType == "RawString"){
            return std::make_shared<StringLiteralNode>(symbol.value);
        } else if (symbol.dataType == "i32"){
            return std::make_shared<IntLiteralNode>(symbol.value);
        } else if (converters.find(symbol.dataType) != converters.end()) {
            return std::make_shared<ONode>(Object(convertToDouble(symbol.value),symbol.dataType));
        } else if (dummyValues.find(symbol.dataType) != dummyValues.end()){
            return std::make_shared<Fakepass>(dummyValues[symbol.dataType]);
        } else {
            return std::make_shared<IdentifierNode>(symbol.dataType);
        }
        } catch  (...) {
            return nullptr;
        }
    }
    std::shared_ptr<ASTNode> parseImport(){
        advance(); // import
        if (currentToken.value == "header"){
        advance(); // header
        std::string fp = std::dynamic_pointer_cast<StringLiteralNode>(parseStringLiteral())->getValue();
        std::vector<std::shared_ptr<ASTNode>> vec;
        std::string dp;
        if (AP.has("headerpath")){
        for (auto& p : AP.values["headerpath"] ){
            if (std::filesystem::exists(p + "/" + fp)){
                dp = p;
                break;
            }
        }
        } else {
            dp = "./externals";
        }
        auto parser = CppHeaderParser();
        auto parsed = parser.parseFile(dp + "/" + fp,"");
        std::vector<std::shared_ptr<ASTNode>> externs;
        std::vector<std::string> fnnames;
        for (auto& sym: parsed){
            if (sym.type == SymbolType::Assignment){
                auto s = symbolize(sym);
                if (s){
                vec.push_back(std::make_shared<AssignNode>(sym.name,s,true)); 
                }
            } else if (sym.type == SymbolType::Macro) {
                auto s = symbolize(sym);
                if (s){
                vec.push_back(std::make_shared<AssignNode>(sym.name,s,true)); 
                }
            }  else if (sym.type == SymbolType::Function) {
                tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> argur;
                tsl::ordered_map<int,std::string> tick;
                argur["-!retty"] = std::make_shared<IdentifierNode>(sym.args[0]);
                for (int i = 1; i != sym.args.size();i++){

                    std::string idn = sym.args[i+1];
                    std::shared_ptr<ASTNode> an = std::make_shared<IdentifierNode>(idn);
                    argur[sym.args[i]] = an;
                    tick[i] = sym.args[i];
                    i++;
                }
                tsl::ordered_map<std::string, std::string> internals;
                externs.push_back(std::make_shared<MappedFunctionNode>(argur,std::make_shared<BlockNode>(),tick,internals,false));
                fnnames.push_back(sym.name);
            } else if (sym.type == SymbolType::Struct) {
                tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> fields;
                tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> cons;
                for (auto& s : sym.symbols){
                    auto sy = symbolize(sym);
                    if (sy){
                    fields[sym.name] = sy;
                    }
                }
                vec.push_back(std::make_shared<AssignNode>(sym.name,std::make_shared<StructDeclNode>(fields,cons),true));
            }
        }
        std::vector<std::string> variadic{};
        vec.push_back(std::make_shared<ExternNode>(externs,fnnames,variadic));
        return std::make_shared<ChainNode>(vec);
        } else {
            std::string imp;
            std::vector<std::string> from = {};
            if (currentToken.type == TokenType::LBrace){
                advance(); // {
                while (currentToken.type != TokenType::RBrace){
                    if (currentToken.type == TokenType::Variadic){
                        advance();
                        from.push_back("*");
                        break;

                    }
                    from.push_back(consume(TokenType::Identifier)); // x
                    if (currentToken.type!= TokenType::RBrace){
                        consume(TokenType::COMMA); // ,
                    }
                }
                consume(TokenType::RBrace); // }
                if (from.size() == 0){
                    from.push_back("*");
                }
                if (currentToken.value != "from"){
                    // error here throw std::runtime_error
                }
                advance(); // from
            }
            if (currentToken.type == TokenType::Identifier){
                imp = std::dynamic_pointer_cast<IdentifierNode>(parseIdentifier())->getValue();
            } else if (currentToken.type == TokenType::String){
                imp = std::dynamic_pointer_cast<StringLiteralNode>(parseStringLiteral())->getValue();
            } 
            auto n = imp;
            auto aas = split(n,'/');
            auto iname = aas[aas.size()-1];
            return std::make_shared<AssignNode>(n,std::make_shared<ImportNode>(imp,from));
        }
    }
    std::shared_ptr<ASTNode> parseTag(){
        auto pv = parseFunction();
        auto mfn = std::dynamic_pointer_cast<AssignNode>(pv);
        return std::make_shared<AssignNode>(mfn->getVarName(),std::make_shared<TagNode>(mfn->getValue()),mfn->getMut());
        
    }
    std::shared_ptr<ASTNode> parseStatement() {
        std::shared_ptr<ASTNode> state;
        logat("Parsing next statement","Parser.pS");
        auto apos = lexer.poswline;
        auto aline = lexer.line;
        if (currentToken.type == TokenType::KeyWord){
            logat("Next statement is keyword","Parser.pS");
            if (currentToken.value == "if"){
                state = parseIf();
            } else if (currentToken.value == "tag"){
                state = parseTag();
            } else if (currentToken.value == "drop"){
                state = parseDrop();
            } else if (currentToken.value == "class"){
                state = parseClass();
            } else if (currentToken.value == "match"){
                state = parseMatch();
            } else if (currentToken.value == "mod"){
                state = parseMod();
            } else if (currentToken.value == "todo"){
                state = parseTodo();
            } else if (currentToken.value == "assert"){
                state = parseAssert();
            } else if (currentToken.value == "fn"){
                state = parseFunction();
            } else if (currentToken.value == "struct"){
                state = parseStruct();
            } else if (currentToken.value == "enum"){
                return parseEnum();
            } else if (currentToken.value == "break"){
                state = parseBreak();
            } else if (currentToken.value == "loop"){
                state = ParseLoop();
            } else if (currentToken.value == "while"){
                state = ParseWhile();
            } else if (currentToken.value == "return"){
                state = ParseRet();
            } else if (currentToken.value == "extern"){
                state = parseExtern();
            } else if (currentToken.value == "pub"){
                state = parsePub();
            } else if (currentToken.value == "inline"){
                state = parseInline();
            } else if (currentToken.value == "import"){
                state = parseImport();
            } else if (currentToken.value == "mut"){
                advance(); //mut
                auto lline = lexer.line;
                auto mutpos = lexer.poswline;
                std::shared_ptr<ASTNode> masn = parseStatement();

                if (auto anode = std::dynamic_pointer_cast<AssignNode>(masn)){
                    //println(anode->getVarName());
                    anode->setMut(true);
                    state = anode;

                } else if (auto sanode = std::dynamic_pointer_cast<StrongAssignNode>(masn)){
                    sanode->setMut(true);
                    state = sanode;
                } else {
                    displayError("mut/var keyword is not supported for this type",lline,mutpos,lexer.poswline,gerr({"reason","the 'mut'/'var' keyword can only be used for where something is being assigned, this occurs in assigning directly, defining a function, etc","note","the mut keyword and var keyword are aliases for eachother","[X]","a = mut 7","[O]","var a = 7"}) );
                    state = masn;
                }

            }

        }

        if (state){
            if (currentToken.type == TokenType::Semicolon){

                advance();
            }
            return state;
        }



        return parseExpression();
    }
    std::string consume(TokenType T){
        std::string v = currentToken.value;
        match(T);
        return v;
    }
    std::shared_ptr<AssignNode> parseStruct(){
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> keys;
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> cons;
        advance(); // struct
        std::string vn = consume(TokenType::Identifier);
        match(TokenType::LBrace);
        while (currentToken.type != TokenType::RBrace){
            std::string typer = consume(TokenType::Identifier);
            if (currentToken.type == TokenType::Walrus){
                advance();
                cons[typer] = parseExpression();
            } else {
            consume(TokenType::Colon);
            keys[typer] = parseExpression();
            }
            if (currentToken.type != TokenType::RBrace){
                consume(TokenType::COMMA);
            }

        }
        advance(); // }
        structs[vn] = 0;
        return std::make_shared<AssignNode>(vn,std::make_shared<StructDeclNode>(keys,cons));
    }


    std::shared_ptr<BreakNode> parseBreak(){
        int l = lexer.line;
        advance(); // break
        std::shared_ptr<ASTNode> expr;
        if (lexer.line == l){
            
            expr = parseExpression();
        }
        return std::make_shared<BreakNode>(expr);
    }

    std::shared_ptr<WhileNode> ParseWhile(){
        advance(); // while
        auto expr = parseExpression();
        auto body = parseBlock();
        return std::make_shared<WhileNode>(std::make_shared<ExpressionNode>(expr,body));
    }
    std::shared_ptr<ASTNode> parseIfLet(){
        advance(); // let
        auto assignexpr = parseExpression();
        auto body = parseExpression();
        if (currentToken.value != "else"){
            throw std::runtime_error("Else is require for iflet");
        }
        match(TokenType::KeyWord);
        auto elser = parseExpression();
        return std::make_shared<IfLetNode>(assignexpr,body,elser);
    }
    // Parse any expression with precedence handling
    std::shared_ptr<ASTNode> parseIf(){
        advance(); // if
        if (currentToken.value == "let"){
            
            return parseIfLet();
        }
        std::shared_ptr<ASTNode> main = std::dynamic_pointer_cast<ASTNode>(parseExpression()); // 1 == 1
        std::shared_ptr<ASTNode> mainbody = std::dynamic_pointer_cast<ASTNode>(parseExpression());; // {}
        std::shared_ptr<ExpressionNode> ifmain = std::make_shared<ExpressionNode>(main,mainbody);
        tsl::ordered_map<int, std::shared_ptr<ExpressionNode>> elifs;
        int a = 0;

        while (currentToken.value == "elif") {
            advance(); // elif
            std::shared_ptr<ASTNode> expr = std::dynamic_pointer_cast<ASTNode>(parseExpression());
            std::shared_ptr<ASTNode> body = std::dynamic_pointer_cast<ASTNode>(parseExpression());
            elifs[a] = std::make_shared<ExpressionNode>(expr, body);
            a += 1;
        }

        std::shared_ptr<ExpressionNode> elsemain;
        if (currentToken.value == "else") {
            advance(); // else

            std::shared_ptr<ASTNode> elsebody = std::dynamic_pointer_cast<ASTNode>(parseExpression());
            elsemain = std::make_shared<ExpressionNode>(nullptr,elsebody);
        }

        return std::make_shared<IFNode>(ifmain, elifs, elsemain);


    }

    std::shared_ptr<ASTNode> ParseIndexSlice(std::shared_ptr<ASTNode> lhs){
        int posi = lexer.getCpos();
        advance(); // [
        auto v = parseExpression(); // 1 / 1+1 / adapt for slice later
        if (currentToken.type == TokenType::Colon){
            rb(posi);
            return lhs;
        }
        if (currentToken.type == TokenType::COMMA){
            rb(posi);
            return lhs;
        }
        match(TokenType::RBracket); // ]
        return std::make_shared<IndexNode>(lhs,v);
    }
    std::shared_ptr<ASTNode> ParseLoop(){
        advance(); // loop
        std::shared_ptr<ASTNode> times;
        if (currentToken.type == TokenType::Integer){
            //println(currentToken.value);
            times = parseIntLiteral();
            //println(convertToString(std::dynamic_pointer_cast<IntLiteralNode>(times)->getValue()));
        } else {
            times = std::make_shared<ASTNode>();
        }
        auto v = parseBlock(); // {}
        
        return std::make_shared<LoopNode>(v,times);
    }
    std::shared_ptr<ASTNode> parsemember(std::shared_ptr<ASTNode> lhs){
        if (auto ltest = std::dynamic_pointer_cast<IntLiteralNode>(lhs)){
        advance();
        

        auto rhs = parseIntLiteral();
        std::string l_str = std::to_string(std::dynamic_pointer_cast<IntLiteralNode>(lhs)->getValue());
        std::string r_str = std::to_string(std::dynamic_pointer_cast<IntLiteralNode>(rhs)->getValue());


        bool isNegative = (l_str[0] == '-');
        

        if (isNegative) {
            l_str = l_str.substr(1);
        }


        std::string combined = l_str + "." + r_str;


        double result = std::stod(combined);

        

        return std::make_shared<ONode>(Object(result, "f64", isNegative));
        }

        std::shared_ptr<ASTNode> mem = lhs;
        if (auto lh = std::dynamic_pointer_cast<IdentifierNode>(lhs)){
            mem->setPointsTO(lh->getValue());
        }

        
        
        
        if (currentToken.type == TokenType::DOT){
            while (currentToken.type == TokenType::DOT){
                advance(); //.
                std::string p = currentToken.value; // access pt eg lhs.rhs.rec
                advance();
                if (currentToken.type == TokenType::Assign){
                    advance();
                    mem = std::make_shared<MemAccNode>(mem, std::make_shared<StringLiteralNode>(p),true,parseExpression());
                } else {
                    mem = std::make_shared<MemAccNode>(mem, std::make_shared<StringLiteralNode>(p));
                }

            }

        }
        
        return mem;
        
    }
    std::shared_ptr<ASTNode> parseCall(std::shared_ptr<ASTNode> lhs){
        std::string ptsto;
        if (auto id = std::dynamic_pointer_cast<IdentifierNode>(lhs)){
            ptsto = id->getValue();
        }
        tsl::ordered_map<int,std::shared_ptr<ASTNode>> dat;
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> inserts;
        match(TokenType::LParen); // (
        int a = 0;
        while (currentToken.type != TokenType::RParen){
            auto expr = parseExpression();
            if (currentToken.type == TokenType::Colon){
                if (auto id = std::dynamic_pointer_cast<IdentifierNode>(expr)){
                    advance(); // :
                    inserts[id->getValue()] = parseExpression();
                }
            } else {
                dat[a] = expr;
            }
            a += 1;
            if (currentToken.type != TokenType::RParen){
                match(TokenType::COMMA);
            }
        }
        match(TokenType::RParen); // )
        auto retnode = std::make_shared<CallNode>(dat,lhs,inserts);
        //retnode->setPointsTO(ptsto);
        return retnode;
    }

        

    std::shared_ptr<ASTNode> ParseUnarary(std::shared_ptr<ASTNode> lhs){
        match(TokenType::Unarary);
        auto iftrue = parseExpression();
        auto BN = std::make_shared<BlockNode>();
        BN->addStatement(iftrue);
        std::shared_ptr<ExpressionNode> ifmain = std::make_shared<ExpressionNode>(lhs,BN);
        match(TokenType::Bang);
        auto elser = parseExpression();
        auto BN2 = std::make_shared<BlockNode>();
        BN2->addStatement(elser);
        auto elsemain = std::make_shared<ExpressionNode>(std::make_shared<ASTNode>(),BN2);
        tsl::ordered_map<int, std::shared_ptr<ExpressionNode>> elses = {};
        auto ifn = std::make_shared<IFNode>(ifmain,elses,elsemain);
        isused.push_back(ifn->id);
        return ifn;
        // IFNode(const std::shared_ptr<ExpressionNode>& value,const tsl::ordered_map<int,std::shared_ptr<ExpressionNode>> elses,const std::shared_ptr<ExpressionNode>& nott)

    }
    std::shared_ptr<ASTNode> ParseNUnarary(std::shared_ptr<ASTNode> lhs){
        match(TokenType::NUnarary);
        
        auto elser = parseExpression();
        return std::make_shared<NUnararyNode>(lhs,elser);

    }
    std::shared_ptr<ASTNode> parseassign(std::shared_ptr<ASTNode> lhs){
        advance(); // =
        return std::make_shared<ExprAssignNode>(lhs,parseExpression());
    }
    bool lookAheadForIrreg(std::shared_ptr<ASTNode> lhs,tsl::ordered_map<std::string,int> structs){
        if (auto SD = std::dynamic_pointer_cast<StructDeclNode>(lhs) ){
            return true;
        } else if (auto ID = std::dynamic_pointer_cast<IdentifierNode>(lhs)){
            if (structs.find(ID->getValue()) != structs.end()){
                return true;
            }
        }
        return false;
    }
    std::shared_ptr<ASTNode> ParseDecon(std::shared_ptr<ASTNode> lhs = NULL){
            advance(); // |
            std::vector<std::string> idents;
            while (currentToken.type != TokenType::Deconstruct){
                idents.push_back(consume(TokenType::Identifier));
                if (currentToken.type != TokenType::Deconstruct){
                    match(TokenType::COMMA);
                }
            }
            match(TokenType::Deconstruct);
            return std::make_shared<DeconsNode>(lhs,idents);

    }
    std::shared_ptr<ASTNode> ParseCast(std::shared_ptr<ASTNode> lhs){
        advance(); // as
        auto castto = parseExpression();
        return std::make_shared<CastNode>(lhs,castto);
    }
    std::shared_ptr<ASTNode> parseExpression(int precedence = 0,std::shared_ptr<ASTNode> glhs=NULL,bool cb = false) {
        // Parse the left-hand side (LHS) of the expression first
        if (currentToken.type == TokenType::Semicolon){
            //advance();
            //println("skipped semicolon");
        }
        auto bpos = lexer.poswline;
        auto l1 = lexer.line;
        std::shared_ptr<ASTNode> lhs;
        if (glhs != NULL){
            logat("GLHS: !NULL","Parser.pE");
            lhs = glhs;
        } else {
            logat("Parsing primary for " + currentToken.value,"Parser.pE");
            lhs = parsePrimary();
        }
        auto l2 = lexer.line;
        auto apos = lexer.poswline;
        if (!cb){
        if (l1 == l2){
        std::vector<int> tl{lexer.line,bpos,apos};
        lpos[lhs->id] = tl;
        } else {

        std::vector<int> tl{l1,bpos,lexer.linemaxpos[l1]};
        lpos[lhs->id] = tl;
        }
        }
        
        

        auto clhs = lhs;
        //println("CUR: " + currentToken.value);
        //std::cout<<"CUR: " + currentToken.value;
        //a = lexer.poswline;
        auto bline = lexer.line;
        if (l1 != l2){
            return lhs;
        }
        
        logat("Parsing expression for " + currentToken.value,"Parser.pE");
        if (currentToken.type == TokenType::Semicolon){

            advance();
            return lhs;
        }
        if (currentToken.type == TokenType::KeyWord){
            if (currentToken.value == "as"){
                return ParseCast(lhs);
            }
        }
        if (currentToken.type == TokenType::LBrace && lookAheadForIrreg(lhs,structs) == true){

            lhs = parseStructInstance(lhs);
        }
        
        if (currentToken.value == "<" && lexer.lookAheadForTemp() == true){
            
            //std::cout<< "template looking";
            //lhs = parseTemplateO(lhs);
        }
        if (currentToken.type == TokenType::Deconstruct){
            lhs = ParseDecon(lhs);
        }
        if (currentToken.type == TokenType::NUnarary){
            lhs = ParseNUnarary(lhs);
        }



        if (currentToken.type== TokenType::AmpersandEq){
            lhs = parsePointerEq(lhs);
        } 


        if (currentToken.type == TokenType::LBracket){
            lhs = ParseIndexSlice(lhs);
            if (currentToken.type == TokenType::Assign){
                lhs = parseassign(lhs);
            }
        }
        if (currentToken.type == TokenType::LParen){
            
            lhs = parseCall(lhs);
        }
        if (currentToken.type == TokenType::DOT){
            lhs = parsemember(lhs);
        }        

        //println(tokenTypeToString(currentToken.type));
        while (((currentToken.type == TokenType::OP) && (getPrecedence(currentToken) >= precedence))) {
            Token op = currentToken;
            advance();  
            if (op.value == "+=" || op.value == "-=" || op.value == "*="){
                std::string accop = op.value.substr(0,1);
                
                Token Taop = Token{value: accop};
                int nextPrecedence = getPrecedence(Taop) + (isRightAssociative(Taop) ? 0 : 1);
                auto rhs = parseExpression();
                std::shared_ptr<IdentifierNode> id = std::dynamic_pointer_cast<IdentifierNode>(lhs);
                return std::make_shared<AssignNode>(id->getValue(),std::make_shared<BinOP>(id,rhs,accop));
            }

            int nextPrecedence = getPrecedence(op) + (isRightAssociative(op) ? 0 : 1);
            auto rhs = parseExpression(nextPrecedence);  
            
            //println("**");
            //println(op.value);
            

            lhs = std::make_shared<BinOP>(lhs, rhs, op.value); 
        }
        if (currentToken.type == TokenType::Unarary){
            lhs = ParseUnarary(lhs);
        }
        int cpos = lexer.poswline;
        if (clhs != lhs){
            logat("Fail clhs check","parser.pE");
            lhs = parseExpression(0,lhs,false);
            if (bline == lexer.line){
                lpos[lhs->id] = {bline,apos,cpos};
            } else if (cpos != lexer.poswline){

                lpos[lhs->id] = {lexer.line,cpos,lexer.poswline};
            } else {
                lpos[lhs->id] = {lexer.line,0,cpos};
            }

        } 
        

        
        return lhs;
    }
    
    std::shared_ptr<ASTNode> parsePointer(){
        bool isborrow=false;
        
        if (currentToken.type == TokenType::AmpersandB){
            isborrow=true;
        }
        advance(); // &/&b
        if (currentToken.type == TokenType::KeyWord && currentToken.value == "mut"){
            advance();
            return std::make_shared<PointerNode>(parseExpression(),true,isborrow);
        }
        return std::make_shared<PointerNode>(parseExpression(),false,isborrow);
    }
    std::shared_ptr<ASTNode> parseDeref(){
        advance(); // *
        return std::make_shared<DerefNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parseOwner(){
        advance(); // &g
        return std::make_shared<GiveOwnershipNode>(parseExpression());
    }
    std::shared_ptr<ASTNode> parsePointerEq(std::shared_ptr<ASTNode> lhs){
        advance(); // &=
        return std::make_shared<ModifyPtrNode>(lhs,parseExpression());
    }

    std::shared_ptr<ASTNode> parseSearchable(){
        advance(); // [|
        std::string type;
        int a = 0;
        int index = 0;
        tsl::ordered_map<int,std::shared_ptr<ASTNode>> checkers;
        while (currentToken.type != TokenType::RSearchable){
            if (currentToken.type == TokenType::DoubleDot && a == 0){
                advance();
                type =  "lend";
            }
            else if (currentToken.type == TokenType::DoubleDot && a != 0){
                if (type == "rend"){
                    throw std::runtime_error("Middle declerations are currently not supported");
                }
                advance();
                type = "rend";
            } else {
                checkers[index] = parseExpression();

                index += 1;
            }
            if (currentToken.type != TokenType::RSearchable){
                match(TokenType::COMMA);
            }
            a += 1;
        }
        match(TokenType::RSearchable);
        return std::make_shared<SearchableNode>(type,checkers);
    }
    std::shared_ptr<ASTNode> parseSignature(){
        advance(); // sig/signature

        match(TokenType::LParen); // (
        tsl::ordered_map<int, std::string> tick;
        tsl::ordered_map<std::string, std::string> internals;
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> args;
        int id = 0;
        while (currentToken.type != TokenType::RParen){
            args[std::string("a") + std::to_string(id)] = parseExpression();
            if (currentToken.type != TokenType::RParen){
                advance(); // ,
            }
            tick[id] =std::string("a") + std::to_string(id);
            id += 1;
        }
        std::shared_ptr<ASTNode> retty;
        match(TokenType::RParen); // )
        if (currentToken.type == TokenType::ReturnTy){
            advance(); // ->
            retty = parseExpression();
        } else {
            retty = std::make_shared<IdentifierNode>("none");
        }
        args["-!retty"] = retty;
        auto BN = std::make_shared<BlockNode>();
        BN->addStatement(std::make_shared<RetNode>(retty));
        auto node = std::make_shared<MappedFunctionNode>(args,BN,tick,internals);
        issig.push_back(node->id);
        return node;

    }
    std::shared_ptr<ASTNode> parseInternal(){
        advance(); // i/internal
        auto id = consume(TokenType::Identifier);
        auto pub = consume(TokenType::Identifier);
        return std::make_shared<InternalsNode>(id,pub);


    }
    std::shared_ptr<ASTNode> parseUnsafe(){

        advance(); // unsafe/usafe
        int apos = lexer.poswline;
        int aline = lexer.line; 

        auto stat = parseStatement();
        if (auto AN = std::dynamic_pointer_cast<AssignNode>(stat)){
            
            if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
            isunsafedecl[MFN->getBody()->id] = true;
            } else {
                displayError("Unsupported unsafe decree with the below statement",aline,apos,apos+7,gerr({"note","Currently unsafe decrees are only supported for functions"}));
            }
        } 
        else if (auto Pub = std::dynamic_pointer_cast<PubNode>(stat)){
            if (auto AAN = std::dynamic_pointer_cast<AssignNode>(Pub->getValue())){
            if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(AAN->getValue())){
            isunsafedecl[MFN->getBody()->id] = true;
            } else {
                displayError("Unsupported unsafe decree with the below statement",aline,apos,apos+7,gerr({"note","Currently unsafe decrees are only supported for functions"}));
            }
            } else {
                displayError("Unsupported unsafe decree with the below statement",aline,apos,apos+7,gerr({"note","Currently unsafe decrees are only supported for functions"}));
            }
        }
        else {
            displayError("Unsupported unsafe decree with the below statement",aline,apos,apos+7,gerr({"note","Currently unsafe decrees are only supported for functions"}));
        }
        
        return stat;
    }
    std::shared_ptr<ASTNode> parseSafe(){

        advance(); // safe
        int apos = lexer.poswline;
        int aline = lexer.line; 

        auto stat = parseStatement();
        if (auto AN = std::dynamic_pointer_cast<AssignNode>(stat)){

            if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
            issafedecl[MFN->getBody()->id] = true;
            } else {
                displayError("Unsupported safe decree with the below statement",aline,apos,apos+6,gerr({"note","Currently safe decrees are only supported for functions"}));
            }
        }
        else if (auto Pub = std::dynamic_pointer_cast<PubNode>(stat)){
            if (auto AAN = std::dynamic_pointer_cast<AssignNode>(Pub->getValue())){
            if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(AAN->getValue())){
            issafedecl[MFN->getBody()->id] = true;
            } else {
                displayError("Unsupported safe decree with the below statement",aline,apos,apos+7,gerr({"note","Currently safe decrees are only supported for functions"}));
            }
            } else {
                displayError("Unsupported safe decree with the below statement",aline,apos,apos+7,gerr({"note","Currently safe decrees are only supported for functions"}));
            }
            }
        else {
            displayError("Unsupported safe decree with the below statement",aline,apos,apos+6,gerr({"note","Currently safe decrees are only supported for functions"}));
        }
        
        return stat;
    }
    std::shared_ptr<ASTNode> ParseAnon(){
        int ipos = lexer.getCpos();
        return parseFunction();
    }

    std::shared_ptr<ASTNode> ParseFuture(){
        advance(); // future
        auto id = consume(TokenType::Identifier);
        return std::make_shared<FutureNode>(id);
    }

    std::shared_ptr<ASTNode> parseComment(){
        auto cline = lexer.line;
        while (lexer.line == cline){
            advance();
        }
        auto austin = std::make_shared<ASTNode>(); isbadnode.push_back(austin->id); return austin;
    }
    std::shared_ptr<ASTNode> ParseRef(){
        auto apos = lexer.poswline;
        auto aline = lexer.line;
        advance(); // ref
        if (!in("c_refs",allowances)){
        displayError("Unsafe use of C references",aline,apos-4,apos,gerr({"note","You may suppress this warning with the decleration: '#allow(c_refs)'"}),true);
        }
        return std::make_shared<RefNode>(parseExpression());
    }

    std::shared_ptr<ASTNode> parsePrimary() {
        logat("Parsing Primary for: " + tokenTypeToString(currentToken.type) + " " + currentToken.value,"Parser.pP");
        switch (currentToken.type) {
            case TokenType::Decorator:
                return parseDecorator();
            case TokenType::LSearchable:
                return parseSearchable();
            case TokenType::Ampersand:
                return parsePointer();
            case TokenType::AmpersandB:
                return parsePointer();
            case TokenType::AmpersandG:
                return parseOwner();
                
            case TokenType::Integer:
                return parseIntLiteral();
            case TokenType::String:
                return parseStringLiteral();
            case TokenType::Identifier:
                return parseIdentifier();
            case TokenType::LBrace:
                return parseBlock();
            case TokenType::LParen:
                return parseTuple();
            case TokenType::LBracket:
                return parseList();
            case TokenType::OP:
                if (currentToken.value == "-"){
                    advance();
                    std::shared_ptr<ASTNode> nint = parseIntLiteral();

                    if (auto rnint = std::dynamic_pointer_cast<IntLiteralNode>(nint)){
                    rnint->setValue(-1*(rnint->getValue()));
                    return rnint;
                    } else if (auto onint = std::dynamic_pointer_cast<ONode>(nint)){
                        onint->setNVE();
                        return onint;
                    }
                } else if (currentToken.value == "*"){
                    
                    return parseDeref();
                }
            case TokenType::KeyWord: // assignable keywords
            if (currentToken.value == "if"){
                auto IFN = parseIf();
                isused.push_back(IFN->id);
                return IFN;
            } else if (currentToken.value == "loop"){
                return ParseLoop();
            } else if (currentToken.value == "future"){
                return ParseFuture();
            } else if (currentToken.value == "while"){
                auto WN = ParseWhile();
                isused.push_back(WN->id);
                return WN;
            } else if (currentToken.value == "ref"){
                auto ref = ParseRef();

                return ref;
            } else if (currentToken.value == "match"){
                return parseMatch();
            } else {
                displayError("Keyword '" + currentToken.value + "' cannot be used in an expression.",lexer.line,lexer.poswline,lexer.poswline +currentToken.value.size(),gerr({"reason","Certain keywords can *only* be used by themselves","[X]","func = fn foo(...){} is not allowed","[O]","fn foo(...){} is allowed"}) );
            } 
            case TokenType::Hashtag:
                return parseHashTag();
            case TokenType::Unarary:
                advance();
                return std::make_shared<IdentifierNode>("?");
            case TokenType::Anon:
                return ParseAnon();
            case TokenType::Comment:
                return parseComment();
            case TokenType::LExpr:
                return parseContainedExpr();
            case TokenType::xadecimal:
                return parseHex();

                

                //return std::make_shared<StructDeclNode>(std::vector<std::string>({"chain",";"}));
            default:
            
                //std::cout << "TokenType: " << tokenTypeToString(currentToken.type) << "\n";
                //advance();
                displayError("Unexpected token: '" +  currentToken.value + "'" ,lexer.line,0,lexer.poswline,{});
                error("Unexpected token: '" +  currentToken.value + "'" + "\n" + demangle(typeid(currentToken.type).name()));
        }
        return nullptr;
    }
    std::shared_ptr<ASTNode> parseHex(){
        auto hexv = consume(TokenType::xadecimal);
        return std::make_shared<ONode>(Object(std::stoull(hexv),"u64"));
    }
    std::shared_ptr<ASTNode> parseContainedExpr(){
        match(TokenType::LExpr); // #|
        
        auto exp = parseExpression();
        match(TokenType::LExpr);
        return exp;
    }

    std::shared_ptr<ASTNode> parseList(){

        advance(); // [
        tsl::ordered_map<int,std::shared_ptr<ASTNode>> list;
        int a = 0;
        while (currentToken.type != TokenType::RBracket){
            std::shared_ptr<ASTNode> exp = parseExpression(); // 1
            list[a] = exp;
            if (a == 0 && currentToken.type == TokenType::Colon){

                match(TokenType::Colon);
                auto toret = std::make_shared<TSLInitNode>(exp,parseExpression());
                match(TokenType::RBracket);

                return toret;

                // make type safe init node....
            }
            if (currentToken.type != TokenType::RBracket){
                match(TokenType::COMMA); // ,
            a += 1;
            }

        }
        advance(); // ]
        if (ruleset["typing"] == "static"){
        if (list.find(0) == list.end()){
            throw std::runtime_error("empty TSLs can be declared with [<type>:?] or [<type>:0]");
        }
        return std::make_shared<TypeSafeListNode>(list,list[0]);
        }
        return std::make_shared<ListNode>(list);
    }

    // Parse an integer literal
    std::shared_ptr<ASTNode> parseIntLiteral() {
        std::string ival = currentToken.value;
        match(TokenType::Integer);
        
        if (currentToken.type == TokenType::Identifier && converters.find(currentToken.value) != converters.end()){
            int64_t rval = safeStringToInt64(ival);
            std::string type = currentToken.value;
            match(TokenType::Identifier);
            return std::make_shared<ONode>(Object(rval,type));
        }
        std::shared_ptr<ASTNode> intNode;
        logat("String to int","pIL");
        try {
        
            intNode = std::make_shared<IntLiteralNode>(std::stoi(ival));
        } catch (std::out_of_range& e) {
            if (!AP.has("-noautomatives")){
                intNode = std::make_shared<ONode>(Object(safeStringToInt64(ival),"i64"));
            }
        }
        return intNode;
    }

    // Parse a string literal
    std::shared_ptr<ASTNode> parseStringLiteral() {
        auto stringNode = std::make_shared<StringLiteralNode>(currentToken.value);
        advance();
        return stringNode;
    }


    // Parse an identifier
    std::shared_ptr<ASTNode> parseIdentifier() {
        logat("Parsing Identifier","Parser.pI");
        auto varName = currentToken.value;
        advance();
        //println("IDENT");
        //println(currentToken.value);
        //println(tokenTypeToString(currentToken.type));

        // Handle variable assignments (e.g., `x = ...`)
        if (currentToken.type == TokenType::Assign && aassign == true) {
            advance();
            //println("ASSIGNING");
            auto value = parseExpression();
            isused.push_back(value->id);
            return std::make_shared<AssignNode>(varName, value);
        } else if (currentToken.type == TokenType::Colon){
            auto posi = lexer.getCpos();
            advance(); // :
            aassign = false;

            auto strongtype = parseExpression();
            
            aassign = true;

            //std::cout<<"untrue?" << strongtype;
            
            //println(strongtype);


            if (currentToken.type != TokenType::Assign){
                rb(posi);
                return std::make_shared<IdentifierNode>(varName);
            }
            match(TokenType::Assign);
            auto value = parseExpression();
            return std::make_shared<StrongAssignNode>(varName,value,strongtype);
        } 

        // If not assignment, it's just a variable reference
        return std::make_shared<IdentifierNode>(varName);
    }

    std::shared_ptr<ASTNode> parseStructInstance(std::shared_ptr<ASTNode> vn){
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> fields;
        //println("STRUCTISM");
        match(TokenType::LBrace);// {
        while (currentToken.type != TokenType::RBrace){
            std::string fn = consume(TokenType::Identifier);
            if (currentToken.type == TokenType::RBrace){
                fields[fn] = std::make_shared<IdentifierNode>(fn);
            }
            else if (currentToken.type == TokenType::COMMA ){
                advance(); // ,
                fields[fn] = std::make_shared<IdentifierNode>(fn);
            } else {

                match(TokenType::Colon);
                fields[fn] = parseExpression();
                if (currentToken.type == TokenType::RBrace){
                    break;
                }
                match(TokenType::COMMA);
            }
        }
        advance(); // }
        //println("Done Struct");
        return std::make_shared<StructInstanceNode>(fields,vn);
    }

    // Parse an expression inside parentheses (i.e., (...))
    std::shared_ptr<ASTNode> parseTuple() {
        advance(); // (
        tsl::ordered_map<int,std::shared_ptr<ASTNode>> tuple;
        int a = 0;
        while (currentToken.type != TokenType::RParen){
            std::shared_ptr<ASTNode> exp = parseExpression(); // 1
            tuple[a] = exp;
            if (currentToken.type != TokenType::RParen){
                match(TokenType::COMMA); // ,
            a += 1;
            }

        }
        advance(); // )

        return std::make_shared<TupleNode>(tuple);
    }

    // Get the precedence of a binary operator
    int getPrecedence(const Token& token) {
        if (token.value == "==" || token.value == "!=") {
            return 1;
        } else if (token.value == ">" || token.value == "<" || token.value == ">=" || token.value == "<=") {
            return 2;
        } else if (token.value == "+" || token.value == "-") {
            return 3;
        } else if (token.value == "*" || token.value == "/" || token.value == "%") {
            return 4;
        }
        return 0;  // Unknown operator has lowest precedence
    }

    bool isRightAssociative(const Token& token) {

        return false;
    }
};



void setscope(std::shared_ptr<Scope> scope);


// type_instructions to the compiler part
std::shared_ptr<type_instruction> intty(std::shared_ptr<Instruction> inst){
    std::vector<std::shared_ptr<Instruction>> insts{inst};
    return std::make_shared<type_instruction>(nullptr,nullptr,insts);
}


std::shared_ptr<type_instruction> nety(std::any v){
    //logat("Nety","nety");
    if (v.type() == typeid(int) || v.type() == typeid(int32_t)){
        return intty(std::make_shared<resource_instruction>("i32",v));
    } else if (v.type() == typeid(std::string)){
        return intty(std::make_shared<resource_instruction>("RawString",v));
    } else if (v.type() == typeid(Object)){
        auto obj = std::any_cast<Object>(v);
        return intty(std::make_shared<resource_instruction>(obj.getType(),obj.GetStore()));
    } else if (v.type() == typeid(char)){
        return intty(std::make_shared<resource_instruction>("Character",v));
    } else if (v.type() == typeid(signed char)){
        return intty(std::make_shared<resource_instruction>("Character",static_cast<char>(std::any_cast<signed char>(v))));
    } else if (v.type() == typeid(int64_t)){
        return intty(std::make_shared<resource_instruction>("i64",v));
    } else if (v.type() == typeid(uint64_t)){
        return intty(std::make_shared<resource_instruction>("u64",v));
    } else if (v.type() == typeid(double)){
        return intty(std::make_shared<resource_instruction>("f64",v));
    } else if (v.type() == typeid(short)){
        return intty(std::make_shared<resource_instruction>("i16",v));
    } else if (v.type() == typeid(unsigned short)){
        return intty(std::make_shared<resource_instruction>("u16",v));
    } else if (v.type() == typeid(unsigned int)){
        return intty(std::make_shared<resource_instruction>("u32",v));
    } else if (v.type() == typeid(unsigned char)){
        return intty(std::make_shared<resource_instruction>("u8",v));
    } else if (v.type() == typeid(bool)){
        return intty(std::make_shared<resource_instruction>("Bool",v));
    } else if (v.type() == typeid(nullificationv)){
        std::vector<std::shared_ptr<Instruction>> elems{intty(std::make_shared<resource_instruction>("Character",static_cast<char>(102)))};
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"pointer");
    } else if (v.type() == typeid(nullification)){
        
        return intty(std::make_shared<resource_instruction>("none",""));
    } else if (v.type() == typeid(nullificationptr)){
        
        return intty(std::make_shared<resource_instruction>("Null",""));
    } else if (v.type() == typeid(Pointer)){
        auto ptr = std::any_cast<Pointer>(v);
        std::vector<std::shared_ptr<Instruction>> elems{nety(ptr.getptr()->getValue())};
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"pointer");
    } else if (v.type() == typeid(BorrowedPointer)){
        auto bptr = std::any_cast<BorrowedPointer>(v);
        std::vector<std::shared_ptr<Instruction>> elems{nety(bptr.getptr()->getValue())};
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"pointer");
    } else if (v.type() == typeid(StructDecl)){
        auto StructD = std::any_cast<StructDecl>(v);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : StructD.getValue()){
            elems.push_back(nety(f.second));
        }
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"struct");
    } else if (v.type() == typeid(StructInstance)){
        auto StructD = std::any_cast<StructInstance>(v);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : StructD.getValue()){
            elems.push_back(nety(f.second));
        }
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"struct");
    } else if (v.type() == typeid(Tuple)){
        auto Tup = std::any_cast<Tuple>(v);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : Tup.getcomparer()){
            elems.push_back(nety(f.second));
        }
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"struct");
    } else if (v.type() == typeid(MappedFunction)){
        auto MFN = std::any_cast<MappedFunction>(v);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : MFN.getargs()){
            

            elems.push_back(nety(f.second));
        }
        elems.push_back(nety(MFN.getretty()));
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"fty");
    } else if (v.type() == typeid(ModuleMethod)){
        auto MFN = std::any_cast<MappedFunction>(std::any_cast<ModuleMethod>(v).mfn);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : MFN.getargs()){

            elems.push_back(nety(f.second));
        }
        elems.push_back(nety(MFN.getretty()));
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"fty");
    } else if (v.type() == typeid(TagFunction)){
        auto MFN = std::any_cast<MappedFunction>(std::any_cast<TagFunction>(v).MFNlink);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : MFN.getargs()){

            elems.push_back(nety(f.second));
        }
        elems.push_back(nety(MFN.getretty()));
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"fty");
    } else if (v.type() == typeid(TypeSafeList)){
        auto tsl = std::any_cast<TypeSafeList>(v);
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& f : tsl.getlist()){
            elems.push_back(nety(f.second));
        }
        return std::make_shared<type_instruction>(nullptr,nullptr,elems,"array");
    } else {
        println("Failed to get type: "+ demangle(v.type().name()));
        int* idk = nullptr;
        *idk = 10;
        throw std::runtime_error("Unknown type: " + demangle(v.type().name()));
    }

}


class Interpreter{
public:
    Interpreter() {
        scopes.push_back(std::make_shared<Scope>(true));  // Create the global (main) scope
    }
    std::shared_ptr<Scope> getCurrentScope() const {
        if (!scopes.empty()) {
            return scopes.back();
        }
        return nullptr;
    }
    void CleanScope(){
        std::shared_ptr<Scope> cur = getCurrentScope();
        if (cur->isVar("self") == true){
            auto v = cur->getVariable("self");
            cur->freeall();
            scopes.pop_back();
            if (v->selfup != ""){
                update(v->selfup,v->getValue());
            }
        } else {
            cur->freeall();
            scopes.pop_back();
        }
        
    }
    void interpret(std::shared_ptr<BlockNode> rootBlock) {
        interpretBlock(rootBlock, /* isMainBlock= */ true);
    }
    


    
    std::vector<std::shared_ptr<Scope>> scopes;

    std::string interpretString(std::shared_ptr<StringLiteralNode> node){
        node = std::dynamic_pointer_cast<StringLiteralNode>(node);
        return node->getValue();
    }

    std::any interpretInteger(std::shared_ptr<IntLiteralNode> node){
        node = std::dynamic_pointer_cast<IntLiteralNode>(node);
        //println(convertToString(node->getValue())); // is 4
        return std::any(node->getValue()); // is not 4 after any_cast
    }

    std::any interpretBlock(std::shared_ptr<BlockNode> block, bool isMainBlock,std::string loop="",tsl::ordered_map<std::string,std::any> inserts={}) {
        auto ParentScope = getCurrentScope();
        std::shared_ptr<Scope> localScope;
        if (loop == "loopinit" || loop == "" || (ParentScope->getLoop() == true && ParentScope->getSign() == false)){
            //println("INIT LOOP.");
        
        localScope = std::make_shared<Scope>(isMainBlock,ParentScope);
        if (loop == "loopinit" || ParentScope->getLoop() == true){
            localScope->setLoop();
            localScope->setSign();
        }
        scopes.push_back(localScope);
        } else {
            localScope = ParentScope;
        }
        int a = 0;
        int len = block->StatementsLen();
        //println("entering true");

        if (isMainBlock) {
            //std::cout << "isMainBlock: " << isMainBlock << std::endl;
            std::function<void(std::string)> pln = println;
            localScope->addVariable("__compileprint__", std::make_shared<Variable>(CFunction(pln)));
            localScope->addVariable("true", std::make_shared<Variable>(true));
            //localScope->addVariable("bool", std::make_shared<Variable>(true));
            localScope->addVariable("false", std::make_shared<Variable>(false));
            localScope->addVariable("none", std::make_shared<Variable>(nullification{}));
            localScope->addVariable("null", std::make_shared<Variable>(Pointer(new PtrVal(nullptr))));
            // various Compile-time OS-based variables
            llvm::Triple defaultOSakanative;
            if (!AP.has("target")){
                defaultOSakanative = llvm::Triple(llvm::sys::getDefaultTargetTriple());
            } else if (AP.values["target"][0] == "native"){
                defaultOSakanative = llvm::Triple(llvm::sys::getDefaultTargetTriple());
            } else {
                defaultOSakanative = llvm::Triple(AP.values["target"][0]);
            }
            std::string OSN = defaultOSakanative.getOSTypeName(defaultOSakanative.getOS()).str();
            localScope->addVariable("OperatingSystem", std::make_shared<Variable>(OSN));
            dummyinstructions["OperatingSystem"] = std::make_shared<resource_instruction>("RawString",OSN);
            std::string ARCHN = defaultOSakanative.getArchName().str();
            localScope->addVariable("Architecture", std::make_shared<Variable>(ARCHN));
            dummyinstructions["Architecture"] = std::make_shared<resource_instruction>("RawString",ARCHN);
            std::string SPOSN = defaultOSakanative.getOSName().str();
            localScope->addVariable("CannonicalOS", std::make_shared<Variable>(SPOSN));
            dummyinstructions["CannonicalOS"] = std::make_shared<resource_instruction>("RawString",SPOSN);
            std::string Vendor = defaultOSakanative.getVendorTypeName(defaultOSakanative.getVendor()).str();
            localScope->addVariable("Vendor", std::make_shared<Variable>(Vendor));
            dummyinstructions["Vendor"] = std::make_shared<resource_instruction>("RawString",Vendor);
            if (AP.has("-showOSinfo")){
                println("OperatingSystem: " + OSN);
                println("Architecture: " + ARCHN);
                println("CannonicalOS: " + SPOSN);
                println("Vendor: " + Vendor);
            }
            if (compileflag){
            compile = false;
            }
            if (ruleset.find("typing") == ruleset.end()){
                if (AP.has("typing")){
                    ruleset["typing"] = AP.values["typing"][0];
                } else {
                throw std::runtime_error("Typing must be declared through ruleset, under the main scope or specfied as an argument");
                }
            }
            if (compileflag){
            compile = true;
            }
            dummyinstructions["true"] = std::make_shared<resource_instruction>("Bool",true);
            dummyinstructions["false"] = std::make_shared<resource_instruction>("Bool",false);
            dummyValues["GenericPointer"] = Pointer(new PtrVal(Object("Character","f")));
            for (const auto&  v: dummyValues){
                //println("++++++");
                if (v.first != "nil" && v.first != "GenericPointer"){

                    dummyinstructions[v.first] = std::make_shared<resource_instruction>(v.first,v.second);
                } else if (v.first == "GenericPointer"){
                    dummyinstructions[v.first] = std::make_shared<resource_instruction>("RawString",std::string("GenericPointer"));
                }else {
                    dummyinstructions[v.first] = std::make_shared<resource_instruction>("Void",v.second);
                }
                

                if (v.first != "i32" && v.first != "RawString" && v.first != "Bool" && v.first != "nil" && v.first != "GenericPointer"){
                    localScope->addVariable(v.first, std::make_shared<Variable>(Object(v.second,v.first)));
                } else {
                    localScope->addVariable(v.first, std::make_shared<Variable>(v.second));
                }
            }

        }
        //println("passed true");
        for (auto &i: inserts ){
            //std::cout << "inserting!" << i.first;
            if (i.first == "self"){ // add @|mut

                auto vec = std::any_cast<std::vector<std::any>>(i.second);
                auto varr = std::make_shared<Variable>(vec[0],true);

                varr->setselfup(std::any_cast<std::string>(vec[1]));

                localScope->addVariable(i.first,varr);
            }

            else if (getTypeName(i.second) == "i32" || getTypeName(i.second) == "RawString"){
            localScope->addVariable(i.first,std::make_shared<Variable>(i.second));
            } else if (converters.find(getTypeName(i.second)) != converters.end()) {
                localScope->addVariable(i.first,std::make_shared<Variable>(Object(i.second,getTypeName(i.second))));
            } else {
                if (i.first == "self"){ // add @|mut

                    auto vec = std::any_cast<std::vector<std::any>>(i.second);
                    auto varr = std::make_shared<Variable>(vec[0],true);

                    varr->setselfup(std::any_cast<std::string>(vec[1]));

                    localScope->addVariable(i.first,varr);
                } else {
                    localScope->addVariable(i.first,std::make_shared<Variable>(i.second));
                }
            }
        }
        setscope(getCurrentScope());

        std::any ret;
        std::any con;
        for (const auto& statement : block->getStatements()) {
            std::any pcon = con;
            con = interpretStatement(statement);
            if (inT<int>(statement->id,isbadnode)){
                if (pcon.type() != typeid(void)){
                
                ret = pcon;
                continue;
                }
            }
            if (isMainBlock){
                if (AP.has("compile")){
                
                //add(statement); notary's legacy
                }
            }
            
            if (statement == block->getStatements()[block->getStatements().size()-1] && !inT<int>(statement->id,isbadnode)) {
                ret = con;
            }
            if (con.type() == typeid(Break)){
                if (localScope->getLoop() != true){
                    throw std::runtime_error("Break can only be used in loop/while/for.");
                }
                //std::cout << "RETURNING BREAK" << std::endl;
                ret = con;
                //setscope(getCurrentScope());
                return ret;
            }
            if (con.type() == typeid(Return)){
                //std::cout << "RETURNING RETURN" << std::endl;
                ret = con;
                //setscope(getCurrentScope());
                return ret;
            }
            a++;
        }
        setscope(getCurrentScope());

        if (!isMainBlock && loop != "loopinit" && loop != "loop") {
            //println("Killed loop");
            //println(loop);
            // Clean up local scope variables
            CleanScope();
            
        } else if (isMainBlock == true){
            getCurrentScope()->freeall();
        }
        
        return ret;
    }

    std::any interpretAST(std::shared_ptr<ASTNode> node) {
        //println("NODUS");
        //logat("Interpreting statement ^","Interpreter.iS");
        if (auto blockNode = std::dynamic_pointer_cast<BlockNode>(node)) {
            logat("Interpreting Block","Interpreter.iS");
            return interpretBlock(blockNode, /* isMainBlock= */ false);
        } else if (auto CN = std::dynamic_pointer_cast<CallNode>(node)) {
            logat("Interpreting Call","Interpreter.iS");
            return interpretCall(CN);
        } else if (auto SIN = std::dynamic_pointer_cast<StructInstanceNode>(node)) {
            logat("Interpreting StructInstance","Interpreter.iS");
            return interpretStructInstance(SIN);
        } else if (auto IFN = std::dynamic_pointer_cast<IFNode>(node)) {
            logat("Interpreting If","Interpreter.iS");
            return interpretIf(IFN);
        } else if (auto AN = std::dynamic_pointer_cast<AssertionNode>(node)) {
            logat("Interpreting Assert","Interpreter.iS");
            return interpretAssert(AN);
        } else if (auto BN = std::dynamic_pointer_cast<BreakNode>(node)) {
            logat("Interpreting Break","Interpreter.iS");
            return interpretBreak(BN);
        } else if (auto RN = std::dynamic_pointer_cast<RetNode>(node)) {
            logat("Interpreting Return","Interpreter.iS");
            return interpretRet(RN);
        } else if (auto WN = std::dynamic_pointer_cast<WhileNode>(node)) {
            logat("Interpreting While","Interpreter.iS");
            return interpretWhile(WN);
        } else if (auto lON = std::dynamic_pointer_cast<LoopNode>(node)) {
            logat("Interpreting Loop","Interpreter.iS");
            return interpretLoop(lON);
        } else if (auto ON = std::dynamic_pointer_cast<ONode>(node)) {
            logat("Interpreting Object","Interpreter.iS");
            return interpretONode(ON);
        } else if (auto IdentNode = std::dynamic_pointer_cast<IdentifierNode>(node)) {
            logat("Interpreting Identifier " + IdentNode->getValue(),"Interpreter.iS");
            return interpretIdent(IdentNode);
        } else if (auto MAN = std::dynamic_pointer_cast<MemAccNode>(node)) {
            logat("Interpreting MemberAccess","Interpreter.iS");
            return interpretMember(MAN);
        } else if (auto IN = std::dynamic_pointer_cast<IndexNode>(node)) {
            logat("Interpreting Index","Interpreter.iS");
            return interpretIndex(IN);
        } else if (auto TN = std::dynamic_pointer_cast<TupleNode>(node)) {
            logat("Interpreting Tuple","Interpreter.iS");
            return interpretTuple(TN);
        } else if (auto LN = std::dynamic_pointer_cast<ListNode>(node)) {
            logat("Interpreting List","Interpreter.iS");
            return interpretList(LN);
        } else if (auto TLN = std::dynamic_pointer_cast<TypeSafeListNode>(node)) {
            logat("Interpreting TSL","Interpreter.iS");
            return interpretTSL(TLN);
        } else if (auto TLIN = std::dynamic_pointer_cast<TSLInitNode>(node)) {
            logat("Interpreting TSLinit","Interpreter.iS");
            return interpretTSLI(TLIN);
        } else if (auto WN = std::dynamic_pointer_cast<StructDeclNode>(node)) {
            logat("Interpreting StructDecl","Interpreter.iS");
            return interpretStructDecl(WN);
        } else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(node)) {
            logat("Interpreting Assign","Interpreter.iS");
            return interpretAssign(assignNode);
        } else if (auto EAN = std::dynamic_pointer_cast<ExprAssignNode>(node)) {
            logat("Interpreting ExprAssign","Interpreter.iS");
            return interpretExprAssign(EAN);
        } else if (auto SassignNode = std::dynamic_pointer_cast<StrongAssignNode>(node)) {
            logat("Interpreting SA","Interpreter.iS");
            //println("BLokl");
            return interpretStrongAssign(SassignNode);
        } else if (auto StringNode = std::dynamic_pointer_cast<StringLiteralNode>(node)) {
            logat("Interpreting String","Interpreter.iS");
            //println("BLokz");
            return interpretString(StringNode);
        } else if (auto OpNode = std::dynamic_pointer_cast<BinOP>(node)) {
            logat("Interpreting OP","Interpreter.iS");
            //println("BLokx");
            return interpretBinOp(OpNode);
        } else if (auto IntNode = std::dynamic_pointer_cast<IntLiteralNode>(node)) {
            logat("Interpreting Int","Interpreter.iS");
            //println("BLoky");
            return interpretInteger(IntNode);
        } else if (auto PN = std::dynamic_pointer_cast<PointerNode>(node)) {
            logat("Interpreting Ptr","Interpreter.iS");
            //println("BLoky");
            return interpretPointer(PN);
        } else if (auto GON = std::dynamic_pointer_cast<GiveOwnershipNode>(node)) {
            logat("Interpreting gPtr","Interpreter.iS");
            //println("BLoky");
            return interpretOwner(GON);
        } else if (auto DN = std::dynamic_pointer_cast<DerefNode>(node)) {
            logat("Interpreting *Ptr","Interpreter.iS");
            //println("BLokXYZ");
            return interpretderef(DN);
        } else if (auto MPN = std::dynamic_pointer_cast<ModifyPtrNode>(node)) {
            logat("Interpreting mPtr","Interpreter.iS");
            //println("BLokXYZ");
            return interpretModPtr(MPN);
        } else if (auto DN = std::dynamic_pointer_cast<DropNode>(node)) {
            logat("Interpreting Drop","Interpreter.iS");
            //println("BLokXYZ");
            return interpretDrop(DN);
        } else if (auto UFN = std::dynamic_pointer_cast<UnMappedFunctionNode>(node)) {
            logat("Interpreting UFN","Interpreter.iS");
            //println("BLokXYZ");
            return interpretUFN(UFN);
        } else if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(node)) {
            logat("Interpreting MFN","Interpreter.iS");
            //println("BLokXYZ");
            return interpretMFN(MFN);
        } else if (auto DN = std::dynamic_pointer_cast<DecoratorNode>(node)) {
            logat("Interpreting Decorator (christmas!)","Interpreter.iS");
            return interpretDecor(DN);
        } else if (auto MN = std::dynamic_pointer_cast<ModNode>(node)) {
            logat("Interpreting Mod","Interpreter.iS");
            return interpretMod(MN);
        } else if (auto EN = std::dynamic_pointer_cast<EnumNode>(node)) {
            logat("Interpreting Enum","Interpreter.iS");
            return interpretEnum(EN);
        } else if (auto NUN = std::dynamic_pointer_cast<NUnararyNode>(node)) {
            logat("Interpreting NUnary","Interpreter.iS");
            return interpretNUnary(NUN);
        } else if (auto ILN = std::dynamic_pointer_cast<IfLetNode>(node)) {
            logat("Interpreting If Let","Interpreter.iS");
            return interpretIfLet(ILN);
        } else if (auto MN = std::dynamic_pointer_cast<MatchNode>(node)) {
            logat("Interpreting Match","Interpreter.iS");
            return interpretMatch(MN);
        } else if (auto DN = std::dynamic_pointer_cast<DeconsNode>(node)) {
            logat("Interpreting Match","Interpreter.iS");
            return interpretDecon(DN);
        } else if (auto SN = std::dynamic_pointer_cast<SearchableNode>(node)) {
            logat("Interpreting Searchable","Interpreter.iS");
            return interpretSearchable(SN);
        } else if (auto FN = std::dynamic_pointer_cast<FutureNode>(node)) {
            logat("Interpreting Future","Interpreter.iS");
            return interpretFuture(FN);
        } else if (auto CN = std::dynamic_pointer_cast<ClassDeclNode>(node)) {
            logat("Interpreting Class","Interpreter.iS");
            return interpretClass(CN);
        } else if (auto EN = std::dynamic_pointer_cast<ExternNode>(node)) {
            logat("Interpreting Extern","Interpreter.iS");
            return interpretExtern(EN);
        } else if (auto FP = std::dynamic_pointer_cast<Fakepass>(node)) {
            logat("Interpreting Fakepass","Interpreter.iS");
            return FP->getValue();
        } else if (auto CN = std::dynamic_pointer_cast<CastNode>(node)) {
            logat("Interpreting Cast","Interpreter.iS");
            return interpretcast(CN);
        } else if (auto PN = std::dynamic_pointer_cast<PubNode>(node)) {
            logat("Interpreting Pub","Interpreter.iS");
            return interpretPub(PN);
        } else if (auto SON = std::dynamic_pointer_cast<SizeOfNode>(node)) {
            logat("Interpreting SizeOf","Interpreter.iS");
            return interpretSizeOf(SON);
        } else if (auto TIDN = std::dynamic_pointer_cast<TypeIDNode>(node)) {
            logat("Interpreting typeid","Interpreter.iS");
            return interpretTypeid(TIDN);
        } else if (auto TIDN = std::dynamic_pointer_cast<TypeIDNode>(node)) {
            logat("Interpreting typeid","Interpreter.iS");
            return interpretTypeid(TIDN);
        } else if (auto ASMN = std::dynamic_pointer_cast<ASMNode>(node)) {
            logat("Interpreting asm","Interpreter.iS");
            return interpretASM(ASMN);
        } else if (auto PTIN = std::dynamic_pointer_cast<PtrtointNode>(node)) {
            logat("Interpreting PTIN","Interpreter.iS");
            return interpretPtr2Int(PTIN);
        } else if (auto ChN = std::dynamic_pointer_cast<ChainNode>(node)) {
            logat("Interpreting ChN","Interpreter.iS");
            return interpretChain(ChN);
        } else if (auto ImN = std::dynamic_pointer_cast<ImportNode>(node)) {
            logat("Interpreting ImN","Interpreter.iS");
            return interpretModule(ImN);
        } else if (auto SSN = std::dynamic_pointer_cast<StackSizeNode>(node)) {
            logat("Interpreting SSN","Interpreter.iS");
            return interpretStackSize(SSN);
        } else if (auto RN = std::dynamic_pointer_cast<RefNode>(node)) {
            logat("Interpreting Ref","Interpreter.iS");
            return interpretRef(RN);
        } else if (auto TN = std::dynamic_pointer_cast<TagNode>(node)) {
            logat("Interpreting Tag","Interpreter.iS");
            return interpretTag(TN);
        } else {
            
            if (typeid(node) == typeid(std::make_shared<ASTNode>())){
                //println("Warning: unknown node");
                return std::any();
            }
            throw std::runtime_error("Unknown node!");
            //println(demangle(typeid(node).name()));
            // disable with prod
            //println("Interpreter error: Unknown statement type\n");
            return std::any();
        }
    }
    std::any interpretStatement(std::shared_ptr<ASTNode> node){
        auto res = interpretAST(node);
        logat("Finished Interpreting Node","Interpreter.iS");
        return res;
    }
    std::any interpretTag(std::shared_ptr<TagNode> TN){
        auto mfn = std::any_cast<MappedFunction>(interpretStatement(TN->getValue()));
        auto tyid = std::any_cast<uint64_t>(std::any_cast<Object>(interpretStatement(std::make_shared<TypeIDNode>(instanceize(std::make_shared<Fakepass>(mfn.getargs()[mfn.gettick()[0]]),mfn.getargs()[mfn.gettick()[0]])))).GetStore());
        tsl::ordered_map<int,std::any> tts;
        tts[tyid] = mfn;
        return Tag(tts);
        
    }
    std::any interpretRef(std::shared_ptr<RefNode> Ref){
        return Pointer(new PtrVal(interpretStatement(Ref->getValue())));
    }
    std::any interpretStackSize(std::shared_ptr<StackSizeNode> SSN){
        return Object(static_cast<uint64_t>(0),"u64");
    }
    std::any interpretModule(std::shared_ptr<ImportNode> ImN){
        auto from = ImN->getFrom();
        auto m = Module(ImN->getName());
        for (auto& fm : m.imports){
            auto modname = split(fm.first,':');
            auto f = modname[1];
            if (modname[0] == "mfn"){
            tsl::ordered_map<std::string,std::any> argstack;
            tsl::ordered_map<int,std::string> tick;
            tsl::ordered_map<std::string,std::string> internals;
            std::vector<std::shared_ptr<ASTNode>> nodes;
            std::shared_ptr<ASTNode> retnode;
            auto args = split(fm.second,',');
            int ticker = 0;
            for (auto& arg : args){
                auto types = split(arg,':');
                std::string inte =  types[1];
                inte = replace(inte,m.hash + "COLON" + m.hash,":");
                inte = replace(inte,m.hash + "COMMA" + m.hash,",");
                auto lex = Lexer("{\n" + inte + "\n}");
                auto parsed = Parser(lex).parse();
                auto interpreter = Interpreter();
                if (types[0] != "-!retty"){
                    nodes.push_back(parsed->getStatements()[0]); // assume 0th instruction is it and with gettypeName it will be...
                } else {
                    retnode = parsed->getStatements()[0];
                }
                argstack[types[0]] = interpreter.interpretBlock(parsed,true);
                tick[ticker] = types[0];
                ticker += 1;
            }
            auto retty = argstack["-!retty"];
            argstack.erase("-!retty");
            nodes.push_back(retnode);
            auto blk = std::make_shared<BlockNode>();
            blk->addStatement(std::make_shared<RetNode>(retnode));

            m.addMethod(f,MappedFunction(blk,argstack,tick,retty,internals,false));
            if (from.size() != 0){
            if (from[0] == "*"){
                from.push_back(f);

            }
            }
            m.addNode(f,nodes);
            }

        }
        std::vector<std::string> temp;
        for (auto& f : from){
            if (f != "*"){
                temp.push_back(f);
            }
        }
        from = temp;
        //std::cout << "from " << debugvectorToString(from) << std::endl;
        ImN->setFrom(from);
        forwardscplex[ImN->id] = m;
        if (!from.empty()){
            std::vector<std::shared_ptr<ASTNode>> chain;

            for (auto& f : from){
                if (m.methods.find(f) != m.methods.end()){
                    chain.push_back(std::make_shared<AssignNode>(f,std::make_shared<Fakepass>(ModuleMethod(m.hash,f,m.methods[f])),false));
                } else {
                    throw std::runtime_error("Unknown import");
                }
            }
            interpretStatement(std::make_shared<ChainNode>(chain)); // this does our assigning for us
            return NoAssign{};
        }
        
        return m;
    }
    std::any interpretChain(std::shared_ptr<ChainNode> ChN){
        for (auto& c : ChN->getChain()){
            logat("Interpreting chain element","interpreter.iC");
            interpretStatement(c);
            logat("Interpreted chain element","interpreter.iC");
        }
        return std::any();
    }
    std::any interpretPtr2Int(std::shared_ptr<PtrtointNode> PTIN){
        return Object(static_cast<uintptr_t>(PTIN->getValue()->id),"i64"); // garbage
    }
    std::any interpretASM(std::shared_ptr<ASMNode> assembl){

        //displayError("This operation is inherently unsafe and can only remain safe in managed conditions",lpos[assembl->][0],lpos[assembl->id][1],lpos[assembl->id][2]+2,gerr({"note","mark the function using the inline assembly with the unsafe decree to suppress this warning or mark the inline assembly with the safe decree if you believe it is safe."}) ,true);


        
        
        for (auto& a : assembl->assignables){
            interpretStatement(std::make_shared<AssignNode>(a.first,a.second));
        }
        return std::any();
    }
    std::any interpretSizeOf(std::shared_ptr<SizeOfNode> szn){
        std::any res = interpretStatement(szn->getValue());
        return Object(static_cast<uint64_t>(sizeof res),"u64");
    }

    std::any interpretTypeid(std::shared_ptr<TypeIDNode> tyid){
        auto inte = interpretStatement(tyid->getValue());
        uint64_t res =  static_cast<uint64_t>(std::type_index(inte.type()).hash_code());
        if (inte.type() == typeid(BorrowedPointer)){
            forwards[tyid->id] = nety(std::any_cast<BorrowedPointer>(inte).getValue());
            res = CPf(res, static_cast<uint64_t>(std::type_index(std::any_cast<BorrowedPointer>(inte).getValue().type()).hash_code()));

        }
        else if (inte.type() == typeid(Pointer)){
            forwards[tyid->id] = nety(std::any_cast<Pointer>(inte).getptr()->getValue());
            res = CPf(res , static_cast<uint64_t>(std::type_index(std::any_cast<Pointer>(inte).getptr()->getValue().type()).hash_code()));
        } else if (inte.type() == typeid(std::string)){
            forwards[tyid->id] = nety(inte);
            res = CPf(res , static_cast<uint64_t>(std::type_index(inte.type()).hash_code()));
        } else if (inte.type() == typeid(MappedFunction)){
            forwards[tyid->id] = nety(inte);
            res = CPf(res , static_cast<uint64_t>(std::type_index(inte.type()).hash_code()));
        } else {
            res = CPf(res,0);
        }
        
        return Object(res,"u64");
    }
    std::any interpretPub(std::shared_ptr<PubNode> PN){
        if (auto aas = std::dynamic_pointer_cast<AssignNode>(PN->getValue())){
        if (auto mfn = std::dynamic_pointer_cast<MappedFunctionNode>(aas->getValue())){
            if (AP.has("-bundle") || result == "package"){
                std::hash<std::string> hashery;
                exports["DECKhash"] = std::to_string(hashery(AP.values["compile"][0]));
                std::string argStr = "";
                int i = 0;
                for (auto& arg: mfn->getValue()){
                    auto ty = getTypeName(interpretStatement(arg.second));
                    ty = replace(ty,":",exports["DECKhash"] + "COLON" + exports["DECKhash"]);
                    ty = replace(ty,",",exports["DECKhash"] + "COMMA" + exports["DECKhash"]);
                    argStr += ((i == 0 ? "" : ",") + arg.first + ":" + ty);
                    i++;
                }
                exports["mfn:" +  aas->getVarName()] = argStr;
            }
            return interpretStatement(PN->getValue());
        } else {
            displayError("Keyword pub/public only supports functions",lpos[PN->id][0],lpos[PN->id][1],lpos[PN->id][2],gerr({"[O]","pub fn foo(...){...}","[X]","pub foo =\"bar\""}));
        }
        }
    }
    std::any interpretcast(std::shared_ptr<CastNode> CN){
        auto value = interpretStatement(CN->getValue());

        auto dest = interpretStatement(CN->getDest());
        logat("Casting to: " + getTypeName(dest) , "Interpreter.ic");
        forwards[CN->id] = nety(dest);
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> inserts = {};
        if (value.type() == typeid(Pointer)){
            auto ptr = std::any_cast<Pointer>(value);
            PtrVal* pv = ptr.getptr();
            
            
            tsl::ordered_map<int,std::shared_ptr<ASTNode>> args;
            Pointer p(nullptr);
            if (converters.find(replace(getTypeName(dest),"&","")) == converters.end()){
                 
                if (getTypeName(dest)[0] == '&'){
                   
                    p = Pointer(new PtrVal(dest)); // this just tricks maven and is not a good idea
                } else {
                    return dest;
                }
            } else {
                //return dest;
                p = Pointer(new PtrVal(converters[replace(getTypeName(dest),"&","")](pv->getValue()))); // make a try {} catch ... so if invalid conversions are made....

            }
            
            p.pointsto(ptr.getpt());
            return p;
        } else if (value.type() == typeid(BorrowedPointer)){
            auto bptr = std::any_cast<BorrowedPointer>(value);
            tsl::ordered_map<int,std::shared_ptr<ASTNode>> args;
            args[0] = CN->getValue();
            bptr.editValue(interpretStatement(std::make_shared<CallNode>(args,std::make_shared<IdentifierNode>(replace(getTypeName(dest),"&","")),inserts)));
            return bptr;
        } else if (value.type() == typeid(CEXTFunction)){
            auto cxfn = std::any_cast<CEXTFunction>(value);
            
            
            return CEXTFunction(std::any_cast<MappedFunction>(dest).getretty(),std::any_cast<MappedFunction>(dest));
        } else if (dummyValues.find(getTypeName(value)) != dummyValues.end()){
            auto ty = getTypeName(dest);
            if (value.type() == typeid(Object)){
                if (ty == "i32"){
                    return convertToInt32(std::any_cast<Object>(value).value);
                } else if (ty == "RawString"){
                    return convertToString(std::any_cast<Object>(value).value);
                }
                auto obj =Object(std::any_cast<Object>(value).value,ty);
                obj.GetStore();

                return obj;
            } else {
                if (ty == "i32"){
                    return convertToInt32(value);
                } else if (ty == "RawString"){
                    return convertToString(value);
                }
                auto obj = Object(value,ty);
                obj.GetStore();
                return obj;
            }
        } else {
            
            displayError("Given type " + demangle(value.type().name()) +  " is unable to be casted as it is not a pointer, only pointer types may be casted to one another using the 'as' keyword",lpos[CN->id][0],lpos[CN->id][1],lpos[CN->id][2],gerr({"note","Pointers may be casted through: <ptr> as <dest>","[O]","my_ptr = &10; myi64ptr = my_ptr as &i64"}));
        }
        return std::any();
    }

    std::any interpretExtern(std::shared_ptr<ExternNode> EN){
        auto fns = EN->getValue();
        auto fnnames = EN->getfnnames();
        for (int i = 0; i != fns.size();i++){
            if (auto mfn = std::dynamic_pointer_cast<MappedFunctionNode>(fns[i])){
                auto retty = mfn->getValue()["-!retty"];
                auto res = interpretStatement(retty);

                if (dummyValues.find(getTypeName(res)) != dummyValues.end()){
                    if (getCurrentScope()->isVar(fnnames[i]) == false){
                        getCurrentScope()->addVariable(fnnames[i],std::make_shared<Variable>(CEXTFunction(res,std::any_cast<MappedFunction>(interpretStatement(mfn)))));
                    } else {
                        throw std::runtime_error("Extern declared functions may not overwrite another variable name even if mutable");
                    }
                

                } else {
                     if (getCurrentScope()->isVar(fnnames[i]) == false){
                        getCurrentScope()->addVariable(fnnames[i],std::make_shared<Variable>(CEXTFunction(res,std::any_cast<MappedFunction>(interpretStatement(mfn)))));
                     } else {
                        throw std::runtime_error("Extern declared functions may not overwrite another variable name even if mutable");
                    }
                }
            
            } else {
                throw std::runtime_error("Extern closures may only have functions defined within them.");
            }
        }
        return std::any();

    }
    std::any interpretClass(std::shared_ptr<ClassDeclNode> CN){
        auto fns = CN->getValue();
        auto cons = CN->getCons();
        tsl::ordered_map<std::string, std::any> fnsa;
        tsl::ordered_map<std::string, std::any> consa;
        for (const auto& k : fns){
            fnsa[k.first] = interpretStatement(k.second);
        }
        for (const auto& k2 : cons){
            consa[k2.first] = interpretStatement(k2.second);
        }
        return Class(fnsa,consa);

    }
    std::any interpretFuture(std::shared_ptr<FutureNode> FN){
        auto id = FN->getValue();
        return std::make_shared<Future>(id,getCurrentScope());
    }
    std::any interpretSearchable(std::shared_ptr<SearchableNode> SN){
        tsl::ordered_map<int,std::any> retmap;
        auto map = SN->getChecks();
        for (auto& k : map){
            retmap[k.first] = interpretStatement(k.second);
        }
        return Searchable(SN->getValue(),retmap);
    }
    std::any interpretDecon(std::shared_ptr<DeconsNode> DN){
        auto res = interpretStatement(DN->getValue());
        auto inodes = DN->getIdents();
        tsl::ordered_map<std::string, std::any>inserts;
        if (res.type() == typeid(EnumKey)){

            auto EK = std::any_cast<EnumKey>(res);
            if (EK.getValue().size() < inodes.size()){
                throw std::runtime_error("Size Mismatch!");
            }
            auto vec = EK.getValue();
            for(int i = 0; i != inodes.size();i++){
                inserts[inodes[i]] = vec[i];
            }

        } else if (res.type() == typeid(TypeSafeList)){
            auto tsl = std::any_cast<TypeSafeList>(res);
            if (tsl.getlist().size() < inodes.size()){
                throw std::runtime_error("Size Mismatch!");
            }
            auto vec = tsl.getlist();
            for(int i = 0; i != inodes.size();i++){
                inserts[inodes[i]] = vec[i];
            }
        } else {
            throw std::runtime_error("Un supported deconstruction");
        }
        return inserts;
    }
    std::any interpretMatch(std::shared_ptr<MatchNode> MN){

        auto checkto = interpretStatement(MN->getValue());

        for (const auto& k: MN->getns()){
            if (auto DN = std::dynamic_pointer_cast<DeconsNode>(k.first)){


                if (std::any_cast<bool>(interpretStatement(std::make_shared<BinOP>(MN->getValue(),DN->getValue(),"=="))) == true){
                    DN->setValue(MN->getValue());
                    return interpretBlock(std::dynamic_pointer_cast<BlockNode>(k.second),false,"",std::any_cast<tsl::ordered_map<std::string, std::any>>(interpretStatement(DN)));
                }
            } else {
            if (std::any_cast<bool>(interpretStatement(std::make_shared<BinOP>(MN->getValue(),k.first,"=="))) == true){
                return interpretStatement(k.second);
            }
            }
        }
        if (MN->getElse()){
            return interpretStatement(MN->getElse());
        }
        throw std::runtime_error("No conditions of match were met, please use _ to set a default");
    }
    std::any interpretIfLet(std::shared_ptr<IfLetNode> ILN){
        auto vn = ILN->getValue();
        if (auto an = std::dynamic_pointer_cast<AssignNode>(vn)){
        auto res = interpretStatement(an->getValue());

        if (res.type() == typeid(nullification)){
            return interpretStatement(ILN->getElse());
        } else if (res.type() == typeid(EnumKey) && std::any_cast<EnumKey>(res).getName() == "none"){
            return interpretStatement(ILN->getElse());
        } else if (res.type() == typeid(EnumKey) && std::any_cast<EnumKey>(res).getValue().size() == 1){
            tsl::ordered_map<std::string,std::any> ins;
            ins[an->getVarName()] = std::any_cast<EnumKey>(res).getValue()[0];
            return interpretBlock(std::dynamic_pointer_cast<BlockNode>(ILN->getBody()),false,"",ins);
        } else {
            tsl::ordered_map<std::string,std::any> ins;
            ins[an->getVarName()] = res;
            return interpretBlock(std::dynamic_pointer_cast<BlockNode>(ILN->getBody()),false,"",ins);            
        }
        } else {
            throw std::runtime_error("if let <assign> ...");
        }
    }
    std::any interpretNUnary(std::shared_ptr<NUnararyNode> NUN){
        std::any res = interpretStatement(NUN->getValue());
        if (res.type() == typeid(EnumKey) && std::any_cast<EnumKey>(res).getName() != "none" && std::any_cast<EnumKey>(res).getValue().size() == 1){
            return std::any_cast<EnumKey>(res).getValue()[0]; // fix this for better handling
        } else if (res.type() == typeid(EnumKey) && std::any_cast<EnumKey>(res).getName() == "none"){
            return interpretStatement(NUN->getElse());
        } else if (res.type() == typeid(nullification)){
            return interpretStatement(NUN->getElse());
        } else {
            return res;
        }
        return std::any();
    }
    std::any interpretEnum(std::shared_ptr<EnumNode> EN){
        auto v = EN->getValue();
        std::unordered_map<std::string,EnumKey> keys;
        for (const auto& k : v){
            std::vector<std::any> l = {};
            for (const auto& kv : k.second){
                auto expr = interpretStatement(kv);
                l.push_back(expr);
            }
            keys.emplace(k.first, EnumKey("", l,Enum({}),k.first));
        }
        std::unordered_map<std::string,EnumKey> based;
        auto ret = Enum(keys);
        for (auto& e : keys){
            std::string type;
            std::string ef = e.first;
            if (std::any_cast<EnumKey>(e.second).getValue().size() == 0){
                type = "fixed";
            } else {
                type = "variant";
            }
            based.emplace(ef, EnumKey(type, e.second.getValue(),ret,ef));

        }
        return Enum(based);
    }

    std::any interpretDrop(std::shared_ptr<DropNode> DN){
        auto res = DN->getValue();
        if (auto IN = std::dynamic_pointer_cast<IdentifierNode>(res)){
            getCurrentScope()->removeVariable(IN->getValue());
        }
        return std::any();
    }
    std::any interpretMod(std::shared_ptr<ModNode> MN){
        tsl::ordered_map<std::string,std::any> cplex;
        cplex["type"] = std::string("");
        forwardscplex[MN->id] = cplex;
        if (auto IN = std::dynamic_pointer_cast<IdentifierNode>(MN->getValue())){
            auto base = interpretStatement(IN);
            if (base.type() == typeid(StructInstance)){
                auto SI = std::any_cast<StructInstance>(base);
                auto checkers = SI.getValue();
                auto cons = SI.getCons();
                auto fns = SI.getFns();
                for (const auto& state: MN->getStates()){
                    if (auto AN = std::dynamic_pointer_cast<AssignNode>(state) ){
                        std::any i;
                        if (auto mfnn = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
                            auto args = mfnn->getValue();
                            if (args.find("self") != args.end()){
                                mfnn->setself(MN->getValue());
                            }
                            i = interpretStatement(mfnn);
                        } else {
                            i = interpretStatement(AN->getValue());
                        }


                        if (i.type() == typeid(MappedFunction)){
                            if (checkers.find(AN->getVarName()) != checkers.end()){
                                throw std::runtime_error("Overwriting existing fields within a struct is not allowed");
                            }
                            if (fns.find(AN->getVarName()) != fns.end()){
                                throw std::runtime_error("Overwriting existing methods within a struct is not allowed");
                            }
                            if (cons.find(AN->getVarName()) != cons.end()){
                                throw std::runtime_error("Overwriting constants within a struct is not allowed");
                            }


                            fns[AN->getVarName()] = i;

                            getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(StructInstance(SI.getValue(),cons,fns)));

                        } else {
                            if (checkers.find(AN->getVarName()) != checkers.end()){
                                throw std::runtime_error("Overwriting existing fields within a struct is not allowed");
                            }
                            if (fns.find(AN->getVarName()) != fns.end()){
                                throw std::runtime_error("Overwriting existing methods within a struct is not allowed");
                            }
                            if (cons.find(AN->getVarName()) != cons.end()){
                                throw std::runtime_error("Overwriting constants within a struct is not allowed");
                            }
                            cons[AN->getVarName()] = i;
                            getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(StructInstance(SI.getValue(),cons,fns)));
                        }
                        
                    } else {
                        throw std::runtime_error("Mod(ifications) for structs only support constants through assigning or methods through functions");
                    }
                }
                getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(StructInstance(SI.getValue(),cons,fns)));
            } else if (base.type() == typeid(StructDecl)){
                auto SD = std::any_cast<StructDecl>(base);
                auto checkers = SD.getValue();
                auto cons = SD.getConsts();
                auto fns = SD.getFns();
                for (const auto& state: MN->getStates()){
                    if (auto AN = std::dynamic_pointer_cast<AssignNode>(state) ){
                        std::any i;
                        if (auto mfnn = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
                            auto args = mfnn->getValue();
                            if (args.find("self") != args.end()){
                                mfnn->setself(MN->getValue());
                            }
                            i = interpretStatement(mfnn);
                        } else {
                            i = interpretStatement(AN->getValue());
                        }

                        if (i.type() == typeid(MappedFunction)){
                            if (checkers.find(AN->getVarName()) != checkers.end()){
                                throw std::runtime_error("Overwriting existing fields within a struct is not allowed");
                            }
                            if (fns.find(AN->getVarName()) != fns.end()){
                                throw std::runtime_error("Overwriting existing methods within a struct is not allowed");
                            }
                            if (cons.find(AN->getVarName()) != cons.end()){
                                throw std::runtime_error("Overwriting constants within a struct is not allowed");
                            }
                            auto mfn = std::any_cast<MappedFunction>(i);
                            auto args = mfn.getargs();
                            if (args.find("self") != args.end()){
                                args["self"] = SD;
                            }

                            fns[AN->getVarName()] = mfn;

                            getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(StructDecl(SD.getValue(),cons,fns)));
                        } else {
                            if (checkers.find(AN->getVarName()) != checkers.end()){
                                throw std::runtime_error("Overwriting existing fields within a struct is not allowed");
                            }
                            if (fns.find(AN->getVarName()) != fns.end()){
                                throw std::runtime_error("Overwriting existing methods within a struct is not allowed");
                            }
                            if (cons.find(AN->getVarName()) != cons.end()){
                                throw std::runtime_error("Overwriting constants within a struct is not allowed");
                            }
                            
                            cons[AN->getVarName()] = i;
                            getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(StructDecl(SD.getValue(),cons,fns)));
                        }
                    } else {
                        throw std::runtime_error("Mod(ifications) for structs only support constants through assigning or methods through functions");
                    }
                }
                getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(StructDecl(SD.getValue(),cons,fns)));

            } else if (base.type() == typeid(Tag)){
                
                cplex["type"] = std::string("tag");
                forwardscplex[MN->id] = cplex;
                Tag t = std::any_cast<Tag>(base);
                auto map = t.tagtypes;
                for (auto& state: MN->getStates()){
                    if (auto AN = std::dynamic_pointer_cast<AssignNode>(state)){
                    auto res = interpretStatement(AN->getValue());
                    if (res.type() == typeid(MappedFunction)){
                        auto mfn = std::any_cast<MappedFunction>(res);
                        auto tyid = std::any_cast<uint64_t>(std::any_cast<Object>(interpretStatement(std::make_shared<TypeIDNode>(instanceize(std::make_shared<Fakepass>(mfn.getargs()[mfn.gettick()[0]]),mfn.getargs()[mfn.gettick()[0]])))).GetStore());
                        map[tyid] = mfn;
                    } else {
                        throw std::runtime_error("Unextendable type for tags"); // make this better (syntax)
                    }
                    } else {
                        throw std::runtime_error("Unextendable type for tags");
                    }
                }
                getCurrentScope()->addVariable(IN->getValue(),std::make_shared<Variable>(Tag(map)));
            }   else {
                throw std::runtime_error("Type is currently unsupported for mod(ification)");
            }
        } else {
            throw std::runtime_error("Mod can only be used with an identifier");
        }
        return std::any();
    }

    std::any interpretExprAssign(std::shared_ptr<ExprAssignNode> EAN){
        if (auto IN = std::dynamic_pointer_cast<IndexNode>(EAN->getVarName())){
            tsl::ordered_map<std::string,std::any> cplex;
            cplex["ty"] = std::string("arr");

            auto base = interpretStatement(IN->getValue());
            if (base.type() == typeid(Pointer)){
                if ((!isdeclunsafe && !inT<int>(IN->id,hasbeenwarned)) && !isdeclsafe){
                if (auto idntowarn = std::dynamic_pointer_cast<IdentifierNode>(IN->getValue())){
                    displayError("Unsafe pointer index access of '" + idntowarn->getValue() + "'",lpos[IN->getValue()->id][0],lpos[IN->getValue()->id][1],lpos[IN->getValue()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
                } else {
                    displayError("Unsafe pointer index access ",lpos[IN->getValue()->id][0],lpos[IN->getValue()->id][1],lpos[IN->getValue()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
                }
                }
                auto ptr = std::any_cast<Pointer>(base);
                int ind = std::any_cast<int>(interpretStatement(IN->getIndex()));
                auto val = interpretStatement(EAN->getValue());
                //ptr.getptr()->setValue(val);
                std::vector<std::shared_ptr<type_instruction>> forward {nety(ptr.getptr()->getValue()),nety(ptr.getptr()->getValue())};
                cplex["ty"] = std::string("ptr");
                cplex["arrity"] = forward;
                forwardscplex[IN->id] = cplex;
                if (auto IDN = std::dynamic_pointer_cast<IdentifierNode>(IN->getValue())){
                    
                    getCurrentScope()->addVariable(IDN->getValue(),std::make_shared<Variable>(ptr));
                    
                }  else if (auto MEM = std::dynamic_pointer_cast<MemAccNode>(IN->getValue())){
                    
                    interpretStatement(std::make_shared<MemAccNode>(MEM->getValue(),MEM->getNxt(),true,std::make_shared<Fakepass>(ptr)));
                    
                } else {
                    throw std::runtime_error("List assigning can only be used with identifiers, not lists");
                }
            }
            else if (base.type() == typeid(TypeSafeList)){

                auto tsl = std::any_cast<TypeSafeList>(base);
                int ind = std::any_cast<int>(interpretStatement(IN->getIndex()));
                auto val = interpretStatement(EAN->getValue());
                tsl.set(ind,val);
                std::vector<std::shared_ptr<type_instruction>> forward {nety(tsl),nety(tsl.get(ind))};
                forwardscplex[IN->id] = forward;
                if (auto IDN = std::dynamic_pointer_cast<IdentifierNode>(IN->getValue())){
                    
                    getCurrentScope()->addVariable(IDN->getValue(),std::make_shared<Variable>(tsl));
                    
                }  else if (auto MEM = std::dynamic_pointer_cast<MemAccNode>(IN->getValue())){
                    
                    interpretStatement(std::make_shared<MemAccNode>(MEM->getValue(),MEM->getNxt(),true,std::make_shared<Fakepass>(tsl)));
                    
                } else {
                    throw std::runtime_error("List assigning can only be used with identifiers, not lists");
                }
            } else if (base.type() == typeid(List)){
                auto list = std::any_cast<List>(base);
                list.add(interpretStatement(EAN->getValue()));
                if (auto IDN = std::dynamic_pointer_cast<IdentifierNode>(IN->getValue())){
                    getCurrentScope()->addVariable(IDN->getValue(),std::make_shared<Variable>(list));
                } else {
                    throw std::runtime_error("List assigning can only be used with identifiers, not lists");
                }
            }
        } else {
            throw std::runtime_error("This type is not assignable");
        }
        return std::any();
    }

    std::any interpretDecor(std::shared_ptr<DecoratorNode> DN){
        if (auto CN = std::dynamic_pointer_cast<CallNode>(DN->getClr())){
            if (auto AN = std::dynamic_pointer_cast<AssignNode>(DN->getFn())){
                if (auto FN = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
                    auto v = CN->getExpr();
                    tsl::ordered_map<int, std::shared_ptr<ASTNode>> out;
                    tsl::ordered_map<int, std::shared_ptr<ASTNode>> output; // needed because the ordered map tracks order of insertion

                    if (v.size() == 0){
                        out[0] = FN;
                    } else {
                        for (const auto& k : v){
                            out[k.first+1] = k.second;
                        }
                        out[0] = FN;
                    }
                    for (int i = 0; i != out.size();i++){
                        output[i] = out[i];
                    }
                    CN->setExpr(output);
                    auto res = interpretStatement(CN);
                    bool switcher = true;
                    if (res.type() == typeid(MappedFunction)){
                        if (std::any_cast<MappedFunction>(res).issignature){
                            switcher = false;
                            issig.push_back(CN->id); // unofficial use
                        }
                    }
                    if (switcher){
                    
                    auto var = Variable(res,AN->getMut());
                    std::shared_ptr<Scope> c = getCurrentScope();
                    c->addVariable(AN->getVarName(),std::make_shared<Variable>(var));
                    }


                    
                } else {
                    throw std::runtime_error("Decorators must be above only functions");
                }
            }
        } else {
            throw std::runtime_error("Decorators can only be called");
        }
        return std::any();
    }

    std::any interpretTSLI(std::shared_ptr<TSLInitNode> TSLI){
        bool szu;
        int size;
        if (auto IN = std::dynamic_pointer_cast<IdentifierNode>(TSLI->getBody())){
            if (IN->getValue() == "?"){
                szu = true;
            }
        }
        std::any sz;
        if (!szu){
            sz = interpretStatement(TSLI->getBody());
        } else {
            sz = 1;
        }
        
        if (getTypeName(sz) != "i32"){
            throw std::runtime_error("Need i32 for tsl init size");
        }
        size = std::any_cast<int32_t>(sz);
        forwardscplex[TSLI->id] = size;

        if (size < 0){
            throw std::runtime_error("Invalid size for typesafelist");
        }
        
        auto fill = interpretStatement(instanceize(TSLI->getExpr(),interpretStatement(TSLI->getExpr())));
        auto tsl = TypeSafeList({},fill);
        
        while (size != 0){
            tsl.add(fill);
            size -= 1;
        }
        if (szu){
            tsl.setSZU(true);
        }
        return tsl;
    }
    std::any interpretAssert(std::shared_ptr<AssertionNode> AN){
        auto boola = interpretStatement(AN->getExpr());
        if (boola.type() != typeid(bool)){
            throw std::runtime_error("Assert requires a boolean result");
        }
        bool b = std::any_cast<bool>(boola);
        if (!b){
            throw std::runtime_error("Assertion error");
        }
        return nullification{};
    }
    std::shared_ptr<ASTNode> instanceize(std::shared_ptr<ASTNode> node, std::any res){
        logat("instanceizing for: " + getTypeName(res),"Interpreter.ie");
        if (res.type() == typeid(StructDecl)){
            auto SD = std::any_cast<StructDecl>(res);
            return std::make_shared<Fakepass>(StructInstance(SD.getValue(),SD.getConsts(),SD.getFns()));
        }
        if (res.type() == typeid(Pointer)){
            auto ptr = std::any_cast<Pointer>(res);
            return std::make_shared<Fakepass>(Pointer(new PtrVal(interpretStatement(instanceize(std::make_shared<Fakepass>(ptr.getptr()->getValue()),ptr.getptr()->getValue())))));
        }
        if (res.type() == typeid(BorrowedPointer)){
            auto ptr = std::any_cast<BorrowedPointer>(res);
            return std::make_shared<Fakepass>(BorrowedPointer(ptr.getptr(),ptr.isMut()));
        }

        if (res.type() == typeid(void)){
            if (auto idn = std::dynamic_pointer_cast<IdentifierNode>(node)){
            if (idn->getValue() == "self"){
                throw std::runtime_error("GOT SELF AS VOID");
            }
            throw std::runtime_error("BAD INSTANCE");
            }
        }
        return node;
    }
    std::any interpretMFN(std::shared_ptr<MappedFunctionNode> MFN,bool first=false){
    if (ruleset["typing"] == "dynamic"){
        throw std::runtime_error("Decleration of mapped functions can only be used with a static typing ruleset");
    }      
    auto args = MFN->getValue();
    tsl::ordered_map<std::string,std::any> argus;


    std::any retty = interpretStatement(args["-!retty"]);
    forwardscplex2[MFN->id] = retty.type() == typeid(nullification);
    args.erase("-!retty");
    for (const auto& k: args){


        argus[k.first] = interpretStatement(k.second);

    }
    auto mappedfn = MappedFunction(MFN->getBody(),argus,MFN->gettick(),retty,MFN->getinternals(),MFN->isselfptr);
    tsl::ordered_map<int,std::shared_ptr<ASTNode>> argstack;
    int i = 0;
    for (auto& t : mappedfn.gettick()){
        if (mappedfn.gettick()[t.first] != "self"){
        auto inst = instanceize(MFN->getValue()[t.second],interpretStatement(MFN->getValue()[t.second]));
        if (inst){
        argstack[i] = inst;
        }
        i++;
        } else {
            auto selfinst = instanceize(MFN->getValue()[t.second],interpretStatement(MFN->getValue()[t.second]));
            //if (mappedfn.isselfptr){
                //selfinst = std::make_shared<Fakepass>(Pointer(new PtrVal(selfinst)));
            //}
            mappedfn.setself(std::dynamic_pointer_cast<Fakepass>(selfinst)->getValue());
        }
        

        

    }
    tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> inserts{};
    auto BN = std::dynamic_pointer_cast<BlockNode>(MFN->getBody());
    if (BN && BN->StatementsLen() != 0 && !AP.has("-nofnchecks") && !inT<int>(MFN->id,issig)){


    
    interpretStatement(std::make_shared<CallNode>(argstack,std::make_shared<Fakepass>(mappedfn),inserts)); // improve this!!


    }
    forwardscplex[MFN->id] = retty;
    if (inT<int>(MFN->id,issig)){
        mappedfn.issignature = true;
    }
    
    return mappedfn;
    }
    std::any interpretWrapper(std::shared_ptr<Fakepass> FN){
        return FN->getValue();
    }

    std::any interpretUFN(std::shared_ptr<UnMappedFunctionNode> UFN){

        if (ruleset["typing"] == "static"){
            throw std::runtime_error("Decleration of unmapped functions can only be used with a dynamic typing ruleset");
        }
        auto args = UFN->getValue();
        return UnMappedFunction(UFN->getBody(),args);
    }
    std::any interpretModPtr(std::shared_ptr<ModifyPtrNode> MPN){
        std::any ptr = interpretStatement(MPN->getLHS());
        if (ptr.type() != typeid(Pointer) && ptr.type()!= typeid(BorrowedPointer)){
            throw std::runtime_error("Modification of pointers is allowed only for Pointers and Mutable BorrowedPointers");
        }
        if (ptr.type() == typeid(Pointer)){
            std::any_cast<Pointer>(ptr).editValue(interpretStatement(MPN->getExpr()));
        } else if (ptr.type() == typeid(BorrowedPointer)){
            std::any_cast<BorrowedPointer>(ptr).editValue(interpretStatement(MPN->getExpr()));
        }
        return nullification{};
    }
    std::any interpretOwner(std::shared_ptr<GiveOwnershipNode> GON){
        //println("IO");
        std::any res = interpretStatement(GON->getExpr());
        //println("EXP");
        if (res.type() != typeid(Pointer)){
            throw std::runtime_error("You can only give ownership of a pointer");
        }
        Pointer p = std::any_cast<Pointer>(res);
        if (p.getpt() != ""){
            getCurrentScope()->removeVariable(p.getpt());
            getCurrentScope()->addVariable(p.getpt(),std::make_shared<Variable>(BorrowedPointer(p.getptr(),false),false));

        }
        //println(p.getpt());
        p.pointsto("");
        
        return p;
        
    }
    std::any interpretPointer(std::shared_ptr<PointerNode> PN){
        if (PN->getBorrow() == true){
            std::any res = interpretStatement(PN->getExpr());
            if (res.type() != typeid(Pointer)){
                throw std::runtime_error("Borrowing of pointers can only be done for pointer objects");
            }

            
            
            return BorrowedPointer(std::any_cast<Pointer>(res).getptr(),PN->getMut());

            
        }
        
        auto ptr = Pointer(new PtrVal(interpretStatement(PN->getExpr())));
        ptr.setMut(PN->getMut());
        return ptr;

    }
    std::string gettype(std::string s){
        if (converters.find(s) == converters.end() && s != "_struct" && s != "Struct" && s != "tup" && s != "list" && s != "tsl"){
            return getTypeName(interpretStatement(std::make_shared<IdentifierNode>(s)));
        } else {
            return s;
        }
    }
    std::string keyfind(tsl::ordered_map<std::string,int> m, int key){
        for (auto& pair:m){
            if (pair.second == key){
                return pair.first;
            }
        }
    }

    std::any interpretCall(std::shared_ptr<CallNode> CN){
        //std::cout << "Callr";
        //std::cout.flush();
        //std::cout << demangle(typeid(CN->getBody()).name());

        
        tsl::ordered_map<std::string,std::any> cplex;
        if (auto ccid = std::dynamic_pointer_cast<IdentifierNode>(CN->getBody())){
            auto cid = ccid->getValue();
            if (converters.find(cid) != converters.end()){
            auto argstack = CN->getExpr();
            if (argstack.size() > 1){
                throw std::runtime_error("Converters can only take 1 argument.");
            }
            // << "converters" << std::endl;

            
            return Object(interpretStatement(argstack[0]),cid).GetStore();
            
            
        }
        }
        logat("Getting caller","Interpreter.iC");
        
        auto caller = interpretStatement(CN->getBody());

        logat("Caller: " + getTypeName(caller) + "(" + std::to_string(CN->id) + ")","Interpreter.iC");
        
        //std::cout << "goose";

        
        if (caller.type() == typeid(EnumKey)){
        try {

        auto clr = std::any_cast<EnumKey>(caller);
        auto argus = clr.getValue();
        auto argstack = CN->getExpr();
        if (clr.getType() == "fixed" ){
            if (argstack.size() != 0){
                throw std::runtime_error("Invalid no. of arguments, expected 0.");
            }
            return clr;
        } else {
        if (argstack.size() != argus.size()){
            throw std::runtime_error("Invalid no. of arguments, expected " + std::to_string(argus.size()) + ".");
        }
        std::vector<std::any> result;

        
        for (int i=0; i != argus.size(); i++){
            auto expr = interpretStatement(argstack[i]);
            if (compareAnyTy(argus[i],expr) == false){
                throw std::runtime_error("Typemismatched.");
            }
            result.push_back(expr);
        }
        auto EK = EnumKey(clr.getType(),result,clr.getBase(),clr.getName());
        return EK;
        } 
        } catch (std::bad_any_cast& e){
            throw std::runtime_error("Bad call: " + convertToString(e.what()));
        }
        } else if (caller.type() == typeid(Enum)){   
            auto clr = std::any_cast<Enum>(caller);
            auto argstack = CN->getExpr();
            auto keys = clr.getValue();

            if (keys.size() > 2){
                throw std::runtime_error("Implicit Enum Calls can only have 2 keys, one with arguments, one without");
            }
            EnumKey vk = EnumKey("",{});
            std::vector<std::any> result;
            int a = 0;
            bool fail = false;
            for (auto k : keys){
                if (k.second.getType()  == "variant" && !fail){
                    
                    auto vec = k.second.getValue();
                    for (int i=0; i != vec.size(); i++){
                        auto expr = interpretStatement(argstack[i]);
                        if (compareAnyTy(vec[i],expr) == true){
                            result.push_back(expr);
                        } else {
                            fail = true;
                            
                        }
                        if (!fail){
                            return EnumKey(k.second.getType(),result,k.second.getBase(),k.second.getName());
                        }
                        
                    }
                }
                if (fail && a != 0){
                    if (k.second.getType()  == "variant"){
                        throw std::runtime_error("If one key is a variant, the other one must be fixed");
                    }
                }
                if (k.second.getType() == "variant"){
                    a += 1;
                }
                
            }
            if (fail){
                for (auto k : keys){
                    if (k.second.getType() == "fixed"){
                        return k.second;
                    }
                }
            }
            return std::any();

            
        
        } else if (caller.type() == typeid(UnMappedFunction)){
        try {
        //std::cout << "Calling Unmapped!";
        auto clr = std::any_cast<UnMappedFunction>(caller);
        auto argstack = CN->getExpr();
        auto argus = clr.getargs();
        tsl::ordered_map<std::string,std::any> args;
        if (argstack.size() != argus.size()){

            throw std::runtime_error("Expected " + convertToString(argus.size()) + " arguments\nGot: " + convertToString(argstack.size()) + " arguments");
        }
        for (auto& v : argstack){
            
            args[argus[v.first]] = interpretStatement(v.second);
        }
        auto b = clr.getBody();
        
        std::any res = interpretBlock(std::dynamic_pointer_cast<BlockNode>(b),false,"",args);
        CleanScope();
        if (res.type() == typeid(Return)){
            res = std::any_cast<Return>(res).getValue();
        }
        return res;
        
        } catch (std::bad_any_cast& e){
            throw std::runtime_error("Bad call: " + convertToString(e.what()));
        }} else if (caller.type() == typeid(MappedFunction)){
        try {
        //std::cout << "Calling Mapped!";

        auto clr = std::any_cast<MappedFunction>(caller);
        cplex["type"] = std::string("mappedfunction");
        cplex["fty"] = nety(caller);
        cplex["mfn"] = clr;
        forwardscplex[CN->id] = cplex;
        auto argstack = CN->getExpr();
        auto argus = clr.getargs();
        auto tick = clr.gettick();
        auto intes = clr.getinternals();
        auto insts = CN->getinserts();
        int times = 0;

        tsl::ordered_map<std::string,std::any> args;
        tsl::ordered_map<int, std::shared_ptr<ASTNode>> saveforcompileastack = argstack;
        //auto regresstick = tick;
        auto saveinserts = CN->getinserts();

        
        for (auto& ins : saveinserts){
            int offset = argus.find("self") == argus.end() ? 0 : 1;
            bool insertcheck = false;
            auto check = *reversefind<int,std::string>(tick,ins.first,&insertcheck);
            argstack[*reversefind<int,std::string>(tick,ins.first)-offset] = ins.second;
            times += 1;
            
        }
        /*if (times > intes.size()){
            throw std::runtime_error("Unknown named argument");
        } what was my logic with this...*/

        if (argus.find("self") == argus.end()){
        if (argstack.size() != argus.size()){
            throw std::runtime_error("Expected " + convertToString(argus.size()) + " arguments\nGot: " + convertToString(argstack.size()) + " arguments");
        }
        } else {
            if (argstack.size() != (argus.size()-1)){
            throw std::runtime_error("Expected " + convertToString(argus.size()-1) + " arguments\nGot: " + convertToString(argstack.size()) + " arguments");
            }
            std::vector<std::any> vec = {argus["self"],clr.getselfupdate()};
            for (int i = 0; i != saveforcompileastack.size();i++){
                saveforcompileastack[i+1] = saveforcompileastack[i];
                saveforcompileastack.erase(i);

            }
            saveforcompileastack[0] = clr.getselforigin();



            //CN->setExpr(saveforcompileastack);
            args["self"] = vec;
            argus.erase("self");
            /*for (auto& t: tick){
                tick[t.first-1] = t.second;
                tick.erase(t.first);
            }*/
            //tick.erase(0);
            // alignment adjustments
            for (auto& a: argstack){
                argstack[a.first+1] = a.second;
                argstack.erase(a.first);
            }
        
            
        }

        const auto sz = argstack.size() + (args.find("self") != args.end() ? 1 : 0);


        for (int iid = 0; iid!= sz/**/;iid++){
            if (args.find("self") != args.end() && iid == 0){
            } else {





            args[tick[iid]] = interpretStatement(argstack[iid]);

            if (compareAnyTy(argus[tick[iid]],args[tick[iid]]) == false){
                throw std::runtime_error("Mismatched type for argument no. " + convertToString(iid+1) +  ": " + std::to_string(saveinserts.size()) + ": " + tick[iid]);
            } // from here
            }
            
            
            
        }
        auto b = clr.getBody();
        tsl::ordered_map<std::string,std::any> iargs;
        
        for (const auto& k : args){
            if (intes.find(k.first) != intes.end() && iargs.find(k.first) == iargs.end()){
                iargs[intes[k.first]] = k.second;
            } else {
                iargs[k.first] = k.second;
            }
            
        }



        auto S = getCurrentScope();
        bool prev=isdeclunsafe;
        bool safeprev=isdeclsafe;
        if (!std::dynamic_pointer_cast<Fakepass>(CN->getBody())){ // dont want it to trigger when ran from MFN
        if (issafedecl.find(b->id) != issafedecl.end()){
            safeprev = isdeclsafe;
            isdeclsafe = true;
        }

        if (isunsafedecl.find(b->id) != isunsafedecl.end()){

            prev = isdeclunsafe;

            if ((!prev && !isdeclsafe && (!inT<int>(CN->id,hasbeenwarned)))){
            if (auto idntowarn = std::dynamic_pointer_cast<IdentifierNode>(CN->getBody())){
                displayError("Unsafe reference to '" + idntowarn->getValue() + "'",lpos[CN->getBody()->id][0],lpos[CN->getBody()->id][1],lpos[CN->getBody()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
            } else {
                displayError("Unsafe reference",lpos[CN->getBody()->id][0],lpos[CN->getBody()->id][1],lpos[CN->getBody()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
            }
            }

            isdeclunsafe = true;
        }
        }


        if (!std::dynamic_pointer_cast<BlockNode>(b)){
            return std::any();
        }
        

        std::any res = interpretBlock(std::dynamic_pointer_cast<BlockNode>(b),false,"",iargs); // to here
        if (isunsafedecl.find(b->id) != isunsafedecl.end()){
            isdeclunsafe = prev;
        }
        if (issafedecl.find(b->id) != issafedecl.end()){
            isdeclsafe = safeprev;
        }
        if (getCurrentScope() != S){
            CleanScope();
        }
        if (res.type() == typeid(Return)){
            res = std::any_cast<Return>(res).getValue();
        }
        if (clr.getretty().type() == typeid(nullification)){
            res = nullification{};
        } else {

        if (compareAnyTy(clr.getretty(),res) == false){


            displayError("Type mismatch on return: Got " + demangle(res.type().name()),lpos[CN->id][0],lpos[CN->id][1],lpos[CN->id][2],gerr({"note","Please ensure your function returns the specified type"}));
            
        }
        }
        if (saveforcompileastack.size() != 0 && args.find("self") != args.end()){
            CN->setExpr(saveforcompileastack);
        }
        return res;
        
        } catch (std::bad_any_cast& e){
            if (auto idntolog = std::dynamic_pointer_cast<IdentifierNode>(CN->getBody())){
                logat("Failed on call for: " + idntolog->getValue(),"Interpreter.iC");
            }
            throw std::runtime_error("Bad call (MFN) : " + convertToString(e.what()));
        }} else if (caller.type() == typeid(Class)){
            auto clr = std::any_cast<Class>(caller);
            auto fns = clr.getValue();
            auto argstack = CN->getExpr();
            if (fns.find("new") == fns.end()){
                if (argstack.size() != 0){
                    throw std::runtime_error("Class expected 0 arguments");
                }
                return ClassInstance(fns,clr.getConsts());
            } else {
                auto ci = ClassInstance(fns,clr.getConsts());
                auto vptr = std::make_shared<Variable>(ci,false);
                
                

            }


        } else if (caller.type() == typeid(CEXTFunction)){
            auto clr = std::any_cast<CEXTFunction>(caller);
            
            if ((!isdeclunsafe && !inT<int>(CN->id,hasbeenwarned)) && !isdeclsafe){
            hasbeenwarned.push_back(CN->id);
            if (auto idntowarn = std::dynamic_pointer_cast<IdentifierNode>(CN->getBody())){
                //println(std::to_string(CN->id) + "::"+ debugvectorToString<int>(lpos[CN->getBody()->id])); <- fix bpos not being diff
                displayError("Unsafe reference to '" + idntowarn->getValue() + "'",lpos[CN->getBody()->id][0],lpos[CN->getBody()->id][1],lpos[CN->getBody()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
            } else {
                displayError("Unsafe reference",lpos[CN->getBody()->id][0],lpos[CN->getBody()->id][1],lpos[CN->getBody()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
            }
            }
            for (auto& arg: CN->getExpr()){
                interpretStatement(arg.second);
            }
            for (auto& arg: CN->getinserts()){
                interpretStatement(arg.second);
            }
            cplex["type"] = std::string("mappedfunction");
            cplex["fty"] = nety(clr.MFNlink); // make this better later
            cplex["mfn"] = clr.MFNlink;
            forwardscplex[CN->id] = cplex;

            

            
            return interpretStatement(instanceize(std::make_shared<Fakepass>(clr.getDV()),clr.getDV()));


    
        } else if (caller.type() == typeid(ModuleMethod)){ 
        try {
            
            
            auto clr = std::any_cast<ModuleMethod>(caller);
            for (auto& arg: CN->getExpr()){
                interpretStatement(arg.second);
            }
            for (auto& arg: CN->getinserts()){
                interpretStatement(arg.second);
            }
            cplex["type"] = std::string("mappedfunction");
            cplex["fty"] = nety(clr.mfn); 
            cplex["mfn"] = std::any_cast<MappedFunction>(clr.mfn);
            forwardscplex[CN->id] = cplex;
            
            
            return std::any_cast<MappedFunction>(clr.mfn).getretty(); // might just do an in-interpreter call
        } catch (std::bad_any_cast& e){
            throw std::runtime_error("Bad call MM: " + convertToString(e.what()) + "::" + demangle(caller.type().name()));
        }}  else if (caller.type() == typeid(TagFunction)){ 
        try {
            
            

            auto tf = std::any_cast<TagFunction>(caller);
            auto mfn = tf.MFNlink;
            cplex["type"] = std::string("tag");
            cplex["fty"] = nety(mfn); 
            cplex["mfn"] = std::any_cast<MappedFunction>(mfn);

            
            auto base = tf.AN;
            auto argstack = CN->getExpr();
            //auto specs = CN->get
            tsl::ordered_map<int, std::shared_ptr<ASTNode>> newas;
            for (auto& arg : argstack){
                newas[arg.first+1] = arg.second;
            }

            newas[0] = base;
            cplex["astack"] = newas; // gen unique-ids on interpret!!
            forwardscplex[CN->id] = cplex;
            

            
            return interpretStatement(std::make_shared<CallNode>(newas,std::make_shared<Fakepass>(mfn),CN->getinserts()));


        } catch (std::bad_any_cast& e){
            throw std::runtime_error("Bad call MM: " + convertToString(e.what()) + "::" + demangle(caller.type().name()));
        }} else if (caller.type() == typeid(CFunction)){ // legacy
        try {
            cplex["type"] = std::string("cfn");
            forwardscplex[CN->id] = cplex;
            
            auto clr = std::any_cast<CFunction>(caller);
            auto stat = CN->getExpr().find(0)->second;
            std::any res = interpretStatement(stat);
            
            clr.call(convertToString(res));
            return nullification{};
        } catch (std::bad_any_cast& e){
            throw std::runtime_error("Bad call C: " + convertToString(e.what()) + "::" + demangle(caller.type().name()));
        }} else {
            throw std::runtime_error("Unknown caller: " + demangle(caller.type().name()));
        }
        return std::any();
    
    }
    std::any interpretStructInstance(std::shared_ptr<StructInstanceNode> SIN){

        std::any bret = interpretStatement( SIN->getBase());
        if (bret.type() != typeid(StructDecl)){
            throw std::runtime_error("Expected struct, got: " + demangle(bret.type().name()));
        }
        auto base = std::any_cast<StructDecl>(bret);
        auto inpmap = SIN->getValue();
        auto outmap = base.getValue();
        tsl::ordered_map<std::string,std::any> fields;
        for (const auto& value: inpmap){
            auto out = interpretStatement(value.second);
            //println(convertToString(out));
            //println("_____");
            if (outmap.find(value.first) == outmap.end()){
                throw std::runtime_error("Unrecognised field: " + value.first);
            } else if (compareAnyTy(outmap[value.first],out) == false){
                throw std::runtime_error("Unexpected type: " + getTypeName(out) + "(" + demangle(out.type().name()) + ")" + "\nExpected: " + getTypeName(outmap[value.first]) +  "(" + demangle(outmap[value.first].type().name()) + ")");

            }
            fields[value.first] = out;
        }
        auto ret = StructInstance(fields,base.getConsts(),base.getFns());
        if (compileflag){
            ret.setSN(std::dynamic_pointer_cast<IdentifierNode>(SIN->getBase())->getValue());
        }
        forwardscplex[SIN->id] = 0;
        vidids.push_back(SIN->id);
        
        return ret;
        

    }

    std::any interpretStructDecl(std::shared_ptr<StructDeclNode> WN){
        auto x =  WN->getValue();
        auto y =  WN->getCons();
        tsl::ordered_map<std::string,std::any> keys;
        tsl::ordered_map<std::string,std::any> cons;
        for (const auto& key : x){
            keys[key.first] = interpretStatement(key.second);
        }
        for (const auto& kk : y){
            cons[kk.first] = interpretStatement(kk.second);
        }

        
            
        
        return StructDecl(keys,cons);
    }
    void update(std::string a,std::any res){
        if (getCurrentScope()->isVar(a)){
            
            if (compareAnyTy(getCurrentScope()->getVariable(a)->getValue(),res) == false){
                throw std::runtime_error("Mismatched member access edit type");
            }
        }
        if (a == "self"){
            auto varr = std::make_shared<Variable>(res);
            varr->setselfup(getCurrentScope()->getVariable("self")->selfup);
            getCurrentScope()->addVariable(a,varr);
        } else {
            getCurrentScope()->addVariable(a,std::make_shared<Variable>(res));
        }
    }
    std::any AccessMem(std::shared_ptr<MemAccNode> baseMAN,std::any base, std::string member,bool assign,std::shared_ptr<ASTNode> expr,std::string aas){
        logat("Accessing Member","Interpreter.AM");
        tsl::ordered_map<std::string,std::any> cplex;
        cplex["vid"] = 0;
        if (auto id = std::dynamic_pointer_cast<IdentifierNode>(baseMAN->getValue())){
            cplex["vid"] = getCurrentScope()->getVariable(id->getValue())->valueid;
        }
        if (base.type() == typeid(Tuple)){
            if (!isdigit(member[0])){
                cplex["type"] = std::string("tuple");
                cplex["inferred"] = std::make_shared<MemAccNode>(std::make_shared<MemAccNode>(baseMAN->getValue(),std::make_shared<StringLiteralNode>("0")),std::make_shared<StringLiteralNode>(member));
                forwardscplex[baseMAN->id] = cplex;
                return interpretStatement(std::any_cast<std::shared_ptr<MemAccNode>>(cplex["inferred"]));
            }
            cplex["type"] = std::string("tuple");
            cplex["index"] = std::stoi(member);
            cplex["tuple"] = nety(base);
            cplex["memtype"] = nety(std::any_cast<Tuple>(base).get(safeStringToInt64(member)));
            
            forwardscplex[baseMAN->id] = cplex;
            
            return std::any_cast<Tuple>(base).get(safeStringToInt64(member));
        } else if (base.type() == typeid(EnumKey)){
            return std::any_cast<EnumKey>(base).getValue()[(safeStringToInt64(member))];
        } else if (base.type() == typeid(StructInstance)){
            cplex["type"] = std::string("structI");
            auto s = std::any_cast<StructInstance>(base);
            cplex["structI"] = nety(s);
            auto m = s.getValue();
            
            if (m.find(member) == m.end()){
                auto c = std::any_cast<StructInstance>(base).getCons();
                if (c.find(member) == c.end()){
                auto f = std::any_cast<StructInstance>(base).getFns();
                if (f.find(member) == c.end()){
                    throw std::runtime_error("Field " + member + " not found in struct");
                } else {
                    auto fn = std::any_cast<MappedFunction>(f[member]);
                    auto args = fn.getargs();
                    if (assign){
                        f[member] = interpretStatement(expr);
                        update(aas, StructInstance(s.getValue(),s.getCons(),f));
                    }
                    if (args.find("self") == args.end()){
                        cplex["siaccesstype"] = std::string("method");
                        forwardscplex[baseMAN->id] = cplex;
                        return fn;
                    } else {
                        cplex["siaccesstype"] = std::string("method");
                        forwardscplex[baseMAN->id] = cplex;
                        args["self"] = std::any_cast<StructInstance>(base);
                        auto mfn = MappedFunction(fn.getBody(),args,fn.gettick(),fn.getretty(),fn.getinternals(),fn.isselfptr);
                        mfn.setselforigin(baseMAN->getValue());
                        mfn.setselfupdate(aas);

                        return mfn;
                    }
                }
                
                } else {
                    if (assign){
                        c[member] = interpretStatement(expr);
                        update(aas, StructInstance(s.getValue(),c,s.getFns()));
                    }
                    cplex["memtype"] = nety(c[member]);
                    cplex["siaccesstype"] = std::string("cons");
                    forwardscplex[baseMAN->id] = cplex;
                    

                    return c[member];
                }
            }
            if (assign){
                m[member] = interpretStatement(expr);
                update(aas, StructInstance(m,s.getCons(),s.getFns()));
            }
            cplex["siaccesstype"] = std::string("member");
            cplex["memtype"] = nety(m[member]);
            int id = 0;
            for (auto& mem : m){
                if (mem.first == member){
                    break;
                }
                id++;
            }
            cplex["id"] = id;

            forwardscplex[baseMAN->id] = cplex;
            return m[member];
        } else if (base.type() == typeid(ClassInstance)){
            auto ci = std::any_cast<ClassInstance>(base);
            auto m = ci.getValue();
            auto c = ci.getConsts();
            if (m.find(member) != m.end()){
                if (assign){
                m[member] = interpretStatement(expr);
                update(aas, ClassInstance(m,ci.getConsts()));
                }
                return m[member];
            }
            if (c.find(member) != c.end()){
                if (assign){
                c[member] = interpretStatement(expr);
                update(aas, ClassInstance(ci.getValue(),c));
                }
                return c[member];
            }
        } else if (base.type() == typeid(Enum)){
            auto m = std::any_cast<Enum>(base).getValue();
            if (m.find(member) == m.end()){
                throw std::runtime_error("Field " + member + " not found in enum");
            }

            return m.at(member);         
        } else if (base.type() == typeid(Module)){
            auto mod = std::any_cast<Module>(base);
            cplex["type"] = std::string("module");
            cplex["mod"] = mod;
            forwardscplex[baseMAN->id] = cplex;
            if (mod.methods.find(member) != mod.methods.end()){
                return mod.methods.find(member)->second;
            } else {
                throw std::runtime_error("Entry " + member + " not found in module");
            }
        }
        throw std::runtime_error("No Member Access found");
    }
    std::any interpretMember(std::shared_ptr<MemAccNode> MAN){
        //println("----");
        
        auto aas = MAN->getValue()->getPointsTO();
        std::any base = interpretStatement(MAN->getValue());
        std::string member = convertToString(interpretStatement(MAN->getNxt()));
        std::any aam;
        try {
        
            aam = AccessMem(MAN,base,member,MAN->getAssign(),MAN->getAssignv(),aas);
        } catch (std::runtime_error& e){
            
            if (getCurrentScope()->isVar(member) && getCurrentScope()->getVariable(member)->value.type() == typeid(Tag)){

                auto tyid = std::any_cast<uint64_t>(std::any_cast<Object>(interpretStatement(std::make_shared<TypeIDNode>(std::make_shared<Fakepass>(base)))).GetStore());
                Tag t = std::any_cast<Tag>(interpretStatement(std::make_shared<IdentifierNode>(member)));
                if (t.tagtypes.find(tyid) != t.tagtypes.end()){
                    tsl::ordered_map<std::string,std::any> cplex;
                    cplex["vid"] = 0;
                    if (auto id = std::dynamic_pointer_cast<IdentifierNode>(MAN->getValue())){
                        cplex["vid"] = getCurrentScope()->getVariable(id->getValue())->valueid;
                    }
                    cplex["type"] = std::string("tag");
                    
                    auto mfn = std::any_cast<MappedFunction>(t.tagtypes[tyid]);
                    cplex["callertype"] = nety(mfn.getargs()[mfn.gettick()[0]]); // first arg
                    //mfn.setself(base); // we'll use this functionality
                    aam = TagFunction(mfn,MAN->getValue());
                    forwardscplex[MAN->id] = cplex;
                } else {
                    throw std::runtime_error("Used Tag does not support type");
                }

            } else {
                throw std::runtime_error(e.what());
            }

        }
        //forwardscplex[MAN->getValue()->id] = forwardscplex[MAN->id];
        //std::cout << "transfered " << MAN->id << " to " << MAN->getValue()->id << ":"<<MAN->getNxt()->id<<std::endl;
        return aam;
    }
    std::any interpretRet(std::shared_ptr<RetNode> RN){
        std::any expr = interpretStatement(RN->getValue());
        //std::cout << "V: " << convertToString(expr) << std::endl;
        if (expr.type() == typeid(std::make_shared<ASTNode>())){
            // default return
            return Return(nullification{});
        }
        //std::cout << "EXPRESSO";
        return Return(expr);
    }
    std::any interpretBreak(std::shared_ptr<BreakNode> BN){
        //std::cout << "BROKEN!" << std::endl;
        std::any expr = interpretStatement(BN->getValue());
        //std::cout << "V: " << convertToString(expr) << std::endl;
        if (expr.type() == typeid(std::make_shared<ASTNode>())){
            // default break
            return Break(nullification{});
        }
        //std::cout << "EXPRESSO";
        return Break(expr);
    }
    std::any interpretWhile(std::shared_ptr<WhileNode> WN){
        auto expr = interpretStatement(WN->getExpr()->getExpr());
        if (expr.type() != typeid(bool)){
            throw std::runtime_error("While syntax is while <bool/condition> {//block of code to run}");
        }
        bool whilebool = std::any_cast<bool>(expr);
        std::any ret;
        auto body = std::dynamic_pointer_cast<BlockNode>(WN->getExpr()->getBody());
        if (whilebool == true){
            ret = interpretBlock(body,false,"loopinit");
            try {
            whilebool = std::any_cast<bool>(interpretStatement(WN->getExpr()->getExpr()));
            } catch (std::bad_any_cast){
                throw std::runtime_error("While syntax is while <bool/condition> {//block of code to run}");
            }

        }
        std::shared_ptr<Scope> current = getCurrentScope();
        while (whilebool == true){
            if (checkbreak(ret) == true){
                ret = std::any_cast<Break>(ret).getValue();
                break;
            }
            ret = interpretBlock(body,false,"loop");
            try {
            whilebool = std::any_cast<bool>(interpretStatement(WN->getExpr()->getExpr()));
            } catch (std::bad_any_cast){
                throw std::runtime_error("While syntax is while <bool/condition> {//block of code to run}\nThis includes editing of variables in the while loop.");
            }
            if (AP.has("compile")){
                break;
            }
        }
        while (getCurrentScope() != current) {
            //println("X");
            CleanScope();
            
            //println("Y");
        }
        //println("A");
        CleanScope();
         // need to patch4nested

        return ret;
    }
    bool checkbreak(std::any type){
        //std::cout << "CHECKER";
        if (type.type() == typeid(Break)){
            return true;
        }
        return false;

    }
    std::any interpretLoop(std::shared_ptr<LoopNode> lON){
        //println("lloping");
        
        auto times = interpretStatement(lON->getTimes());
        //println(typeid(times).name());

        if (times.type() != typeid(int32_t) && times.type() != typeid(std::make_shared<ASTNode>())){
            throw std::runtime_error("Raw integers must be used for loop times");
        }
        int time;
        if (times.type() == typeid(std::make_shared<ASTNode>())){
            time = -1;
        } else {
        time = std::any_cast<int32_t>(times);
        }
        
        if (time != -1 && time < 0){
            throw std::runtime_error("Negative amount of times to loop is disallowed");
        }
        std::any ret;

        auto body = std::dynamic_pointer_cast<BlockNode>(lON->getValue());
        //println("PRE.");
        ret = interpretBlock(body,false,"loopinit");
        if (time != -1){
        time -= 1;
        }
        std::shared_ptr<Scope> current = getCurrentScope();
        //println(convertToString(time));
        if (time != -1){
            while (time != 0){
                if (checkbreak(ret) == true){
                    ret = std::any_cast<Break>(ret).getValue();
                    break;
                }
                
                ret = interpretBlock(body,false,/*loop=*/"loop");
                time -= 1;
            }
        } else {
            while (true){
                if (checkbreak(ret) == true){
                    ret = std::any_cast<Break>(ret).getValue();
                    break;
                }
                ret = interpretBlock(body,false,"loop");
            }
        
        }
        // it's up to loop to clean the scope
        //println(".p");
        
        while (getCurrentScope() != current) {
            //println("X");
            CleanScope();
            
            //println("Y");
        }
        //println("A");
        CleanScope();
         // need to patch4nested
        //println("Done loop.");
        return ret;

    }

    std::any interpretList(std::shared_ptr<ListNode> LN){
        auto lmap = LN->getValue();
        int a = 0;
        tsl::ordered_map<int,std::any> tlmap;
        for (int i = 0; i < lmap.size(); ++i) {
            tlmap[a] = interpretStatement(lmap[a]);
            a += 1;

        }
        
        return List(tlmap);
    }
    std::any interpretTuple(std::shared_ptr<TupleNode> TN){
        //println("TN");
        std::string type = TN->getType();
        auto tumap = TN->getValue();
        int a = 0;
        int32_t sz;
        auto size = interpretStatement(TN->getSize());
        //println("ETN");


        sz = -1;
        
        tsl::ordered_map<int,std::any> tlmap;
        for (int i = 0; i < tumap.size(); ++i) {
            if (type != "none" && type != ""){
                std::any ta = interpretStatement(tumap[a]);
                
                if (getTypeName(ta) != type){
                    throw std::runtime_error("Expected type: " + type + "\nGot: " + getTypeName(ta));
                }
                tlmap[a] = ta;
            } else {
            tlmap[a] = interpretStatement(tumap[a]);
            }
            a += 1;

        }
        if (tlmap.size() > sz && sz != -1){
            throw std::runtime_error("Declared tuple size: " + convertToString(sz) + "\nTuple size: " + convertToString(tlmap.size()));
        }
        if (sz == -1){
            sz = tlmap.size();
        }
        // add dummy values for later
        return Tuple(tlmap,type);
    }
    std::any interpretIndex(std::shared_ptr<IndexNode> IN){
        // TypeSafeList + Tuple!!! DO TMRW.
        std::any l = interpretStatement(IN->getValue());
        tsl::ordered_map<std::string,std::any> cplex;
        cplex["ty"] = std::string("arr");

        if (l.type() == typeid(Pointer)){
            std::cout << "wrking P" << std::endl;
            std::cout << isdeclunsafe  << ":" << isdeclsafe << std::endl;
            if ((!isdeclunsafe && !inT<int>(IN->id,hasbeenwarned)) && !isdeclsafe){
            if (auto idntowarn = std::dynamic_pointer_cast<IdentifierNode>(IN->getValue())){
                displayError("Unsafe pointer index access of '" + idntowarn->getValue() + "'",lpos[IN->getValue()->id][0],lpos[IN->getValue()->id][1],lpos[IN->getValue()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
            } else {
                displayError("Unsafe pointer index access ",lpos[IN->getValue()->id][0],lpos[IN->getValue()->id][1],lpos[IN->getValue()->id][2]+2,gerr({"note","mark the function calling this with the unsafe decree to suppress this warning or mark the function with the safe decree if you believe it is safe."}) ,true);
            }
            }
            cplex["ty"] = std::string("ptr");
            hasbeenwarned.push_back(IN->id);
            std::vector<std::shared_ptr<type_instruction>> forward {nety(std::any_cast<Pointer>(l).getptr()->getValue()),nety(std::any_cast<Pointer>(l).getptr()->getValue())};
            cplex["arrity"] = forward;
            forwardscplex[IN->id] = cplex;
            return std::any_cast<Pointer>(l); // would return addr to index
        }
        if (l.type() != typeid(List) && l.type() != typeid(TypeSafeList)){
            throw std::runtime_error("For now, indexes can only be used for lists. Map support soon.\nGot: " + demangle(l.type().name()));
        } if (l.type() == typeid(List)){
        List lobj = std::any_cast<List>(l);
        std::any i = interpretStatement(IN->getIndex());
        if (i.type() == typeid(Object)){
            Object obj = std::any_cast<Object>(i);
            if (obj.getType() == "i32"){
                i = obj.GetStore();
            }
        }
        if (i.type() != typeid(int32_t)){
            std::string errstr = "Used type: " + demangle(i.type().name());
            //println(errstr);
            throw std::runtime_error("Please use default integers or i32 for list indexing.");
        }
        int32_t index = std::any_cast<int32_t>(i);
        
        return lobj.get(index);
        } else if (l.type() == typeid(TypeSafeList)){
        
        TypeSafeList lobj = std::any_cast<TypeSafeList>(l);
        std::any i = interpretStatement(IN->getIndex());
        if (i.type() == typeid(Object)){
            Object obj = std::any_cast<Object>(i);
            if (obj.getType() == "i32"){
                i = obj.GetStore();
            }
        }
        if (i.type() != typeid(int32_t)){
            std::string errstr = "Used type: " + demangle(i.type().name());
            //println(errstr);
            throw std::runtime_error("Please use default integers or i32 for list indexing.");
        }
        int32_t index = std::any_cast<int32_t>(i);  
        std::vector<std::shared_ptr<type_instruction>> forward {nety(lobj),nety(lobj.get(index))};
        cplex["arrity"] = forward;
        forwardscplex[IN->id] = cplex;
        return lobj.get(index);
        }
        return std::any();

    }

    std::any interpretONode(std::shared_ptr<ONode> ON){
        Object x = ON->getValue();
        return x.GetStore();
    }

    std::any interpretStrongAssign(std::shared_ptr<StrongAssignNode> SANode){
        std::string var = SANode->getVarName();
        std::any type = interpretStatement(SANode->getType());
        
        std::any value = interpretStatement(SANode->getValue());
        if (compareAnyTy(type,value) == false){
                throw std::runtime_error("Incorrectly annotated type");
        }
        std::shared_ptr<Variable> vari = std::make_shared<Variable>(value,SANode->getMut());
        if (vari) {
            if (auto currentScope = getCurrentScope()) {
                currentScope->addVariable(var, vari); // issue here
            } else {
                std::cerr << "Runtime error: No current scope found\n";
            }
        }

        return nullification{};


        
    }
    std::any interpretIf(std::shared_ptr<IFNode> IfNode){
        //println("IF!");
        auto main = interpretStatement(IfNode->getMain()->getExpr());
        //println(demangle(typeid(IfNode->getMain()->getBody()).name()));


        
        if (main.type() != typeid(bool)){
            
            throw std::runtime_error("Expressions can only evaluate to a boolean (true or false) for if statements");
        }
        std::any firsttrue;
        std::any iftype;
        bool mainbool = std::any_cast<bool>(main);
        if (mainbool == true){
            //println("RETTING TRUE");
            //println(convertToString(interpretStatement(IfNode->getMain()->getBody())));
            if (AP.has("compile")){
                firsttrue = interpretStatement(IfNode->getMain()->getBody());
                iftype = firsttrue;
            } else {
            return interpretStatement(IfNode->getMain()->getBody());
            }
        } else if (AP.has("compile")){
            iftype = interpretStatement(IfNode->getMain()->getBody());
        }
        auto umap = IfNode->getElses();
        int a = 0;
        for (int i = 0; i < umap.size(); ++i) {
        auto stat = interpretStatement(umap[a]->getExpr());
        if (stat.type() != typeid(bool)){
            throw std::runtime_error("Expressions can only evaluate to a boolean (true or false) for elif statements");
        }
        bool elifbool = std::any_cast<bool>(stat);
        if (elifbool == true){
            if (AP.has("compile")){
                if (!firsttrue.has_value()){
                    firsttrue = interpretStatement(umap[a]->getBody());
                    if (inT<int>(IfNode->id,isused)){
                    if (firsttrue.type() != iftype.type()){
                        displayError("Else if condition returns a value of different type than if condition",lpos[umap[a]->getExpr()->id][0],lpos[umap[a]->getExpr()->id][1],lpos[umap[a]->getExpr()->id][2],{});
                    }
                    } 
                } else {
                    auto res = interpretStatement(umap[a]->getBody());
                    if (inT<int>(IfNode->id,isused)){
                    if (res.type() != iftype.type()){
                        displayError("Else if condition returns a value of different type than if condition",lpos[umap[a]->getExpr()->id][0],lpos[umap[a]->getExpr()->id][1],lpos[umap[a]->getExpr()->id][2],{});
                    }
                    }
                }
            } else {
            return interpretStatement(umap[a]->getBody());
            }
        } else if (AP.has("compile")){
            auto res = interpretStatement(umap[a]->getBody());
            if (inT<int>(IfNode->id,isused)){
            if (res.type() != iftype.type()){
                displayError("Else if condition returns a value of different type than if condition",lpos[umap[a]->getExpr()->id][0],lpos[umap[a]->getExpr()->id][1],lpos[umap[a]->getExpr()->id][2],{});
            }
            }
        }
        a += 1;
        }
        if (IfNode->getNot()){
            if (AP.has("compile")){
                if (!firsttrue.has_value()){
                    firsttrue = interpretStatement(IfNode->getNot()->getBody());
                    if (inT<int>(IfNode->id,isused)){
                    if (firsttrue.type() != iftype.type()){
                        displayError("Else condition returns a value of different type than if condition",lpos[IfNode->getNot()->getBody()->id][0],lpos[IfNode->getNot()->getBody()->id][1],lpos[IfNode->getNot()->getBody()->id][2]+2,gerr({})); // MAKE_BETTER
                    }
                    }
                } else {
                    auto res = interpretStatement(IfNode->getNot()->getBody());
                    if (inT<int>(IfNode->id,isused)){
                    if (res.type() != iftype.type()){
                        displayError("Else condition returns a value of different type than if condition",lpos[IfNode->getNot()->getBody()->id][0],lpos[IfNode->getNot()->getBody()->id][1],lpos[IfNode->getNot()->getBody()->id][2]+2,gerr({})); // MAKE_BETTER
                    }
                    }
                }
            } else {
            return interpretStatement(IfNode->getNot()->getBody());
            }

        } // interpret everything then set true value
        if (firsttrue.has_value()){
            return firsttrue;
        }
        return nullification{};
    }
    std::any interpretTSL(std::shared_ptr<TypeSafeListNode> TSLN){
        auto tlmap = TSLN->getValue();
        int a = 0;
        tsl::ordered_map<int,std::any> ttlmap;
        for (int i = 0; i < tlmap.size(); ++i) {
            std::any tadd = interpretStatement(tlmap[a]);
            auto tsln = interpretStatement(TSLN->getType());
            
            if (compareAnyTy(tadd,tsln) == false){ // patch here.
                //println(TSLN)

                throw std::runtime_error("Type of value added to TypeSafe list should be: " + getTypeName(tsln) + ".\nType got: " + getTypeName(tadd));
                
            }
            ttlmap[a] = tadd;
            a += 1;

        }
        //println("DONE TSL!");
        return TypeSafeList(ttlmap,interpretStatement(TSLN->getType()));
    }

    std::any interpretIdent(std::shared_ptr<IdentifierNode> IdentNode) {
        std::string var = IdentNode->getValue();
        
        auto currentScope = getCurrentScope();
        //println("Pre");
        std::shared_ptr<Variable> varobj = currentScope->getVariable(var);
        logat("Ident: " + varobj->getType(),"Interpret.iI");
        if (varobj->getType() == "int") {
            return varobj->getInt();
        } else if (varobj->getType() == "RawString") {
            return varobj->getString();
        } else if (varobj->getType() == "bool") {
            return varobj->getBool();
        } else if (varobj->getType() == "none") {
            return varobj->getNull();
        } else if (varobj->getType() == "nil") {
            return varobj->getnullv();
        } else if (varobj->getType() == "list") {
            return varobj->getList();
        } else if (varobj->getType() == "tuple") {
            return varobj->getTuple();
        } else if (varobj->getType() == "TSL") {
            return varobj->getTSL();
        } else if (varobj->getType() == "_struct") {
            return varobj->getStruct();
        } else if (varobj->getType() == "struct") {
            return varobj->getStructI();
        } else if (varobj->getType() == "enum") {
            return varobj->getEnum();
        } else if (varobj->getType() == "ek") {
            return varobj->getEK();
        } else if (varobj->getType() == "ptr") {
            //println("REF'D PTR");
            //exit(0);
            Pointer p = varobj->getPTR();
            p.pointsto(var);
            return p;
        } else if (varobj->getType() == "bptr") {
            return varobj->getBPTR();
        } else if (varobj->getType() == "UFN") {
            return varobj->getUFN();
        } else if (varobj->getType() == "MFN") {
            return varobj->getMFN();
        } else if (varobj->getType() == "CFN") {
            return varobj->getCFN();
        } else if (varobj->getType() == "Class") {
            return varobj->getClass();
        } else if (varobj->getType() == "ClassI") {
            return varobj->getClassI();
        } else if (varobj->getType() == "CEFN") {
            return varobj->getCEFN();
        } else if (varobj->getType() == "MLE") {
            return varobj->getModule();
        } else if (varobj->getType() == "tag") {
            return varobj->getTag();
        } else if (varobj->getType() == "tagF") {
            return varobj->getTagF();
        } else if (varobj->getType() == "MODM") {
            forwardscplex[IdentNode->id] = std::string(varobj->getModuleM().hash + varobj->getModuleM().vname);
            return varobj->getModuleM();
        } else if (varobj->getType() == "obj") {
            Object X = varobj->getObj();
            
            
            auto v =  X.GetStore();
            return v;
        }
        

        return nullification{};
    }

    
    std::any interpretBinOp(std::shared_ptr<BinOP> binOpNode) {
        std::any left = interpretStatement(binOpNode->getleft());
        std::any right = interpretStatement(binOpNode->getright());
        std::string op = binOpNode->getValue();
        std::vector<std::string> eqop = {"==","!=",">=","<=","<",">"};
        std::vector<std::string> inttypeNames = {
        "i8", "i16", "i32", "i64",    // Signed integer types
        "u8", "u16", "u32", "u64",    // Unsigned integer types
        "f32", "f64",                 // Floating-point types
        "isize","usize"
        };

        if (left.type() == typeid(Object)){
            left = std::any_cast<Object>(left).GetStore();
        }
        if (right.type() == typeid(Object)){
            right = std::any_cast<Object>(right).GetStore();
        }
        if (in(op,eqop)){

        if (left.type() != right.type() && (dummyValues.find(getTypeName(left)) == dummyValues.end() || dummyValues.find(getTypeName(right)) == dummyValues.end())){
        if (op == "=="){
            if (right.type() == typeid(Searchable) && left.type() == typeid(TypeSafeList)){
                auto tsl = std::dynamic_pointer_cast<TypeSafeListNode>(binOpNode->getleft());
                auto se = std::dynamic_pointer_cast<SearchableNode>(binOpNode->getright());
                if (se->getChecks().size() > tsl->getValue().size()){
                    throw std::runtime_error("gTC too high");
                }
                auto map = se->getChecks();
                if (se->getValue() == "rend"){

                for (int i = 0;i != map.size();i++){
                    if (std::any_cast<bool>(interpretStatement(std::make_shared<BinOP>(tsl->getValue()[i],map[i],"=="))) == false){
                        return false;
                    }
                }
                return true;
                }
                if (se->getValue() == "lend"){

                for (int i = tsl->getValue().size()-1;i != ((tsl->getValue().size()-1)-map.size());i--){


                    if (std::any_cast<bool>(interpretStatement(std::make_shared<BinOP>(tsl->getValue()[i],map[(tsl->getValue().size()-1)-i],"=="))) == false){
                        return false;
                    }
                }
                return true;
                }


            }
            if (left.type() == typeid(Searchable) && right.type() == typeid(TypeSafeList)){
                auto tsl = std::dynamic_pointer_cast<TypeSafeListNode>(binOpNode->getright());
                auto se = std::dynamic_pointer_cast<SearchableNode>(binOpNode->getleft());
                if (se->getChecks().size() > tsl->getValue().size()){
                    throw std::runtime_error("gTC too high");
                }
                auto map = se->getChecks();
                if (se->getValue() == "rend"){

                for (int i = 0;i != map.size();i++){
                    if (std::any_cast<bool>(interpretStatement(std::make_shared<BinOP>(tsl->getValue()[i],map[i],"=="))) == false){
                        return false;
                    }
                }
                return true;
                }
                if (se->getValue() == "lend"){

                for (int i = tsl->getValue().size()-1;i != 0;i--){
                    if (std::any_cast<bool>(interpretStatement(std::make_shared<BinOP>(tsl->getValue()[i],map[(tsl->getValue().size()-1)-i],"=="))) == false){
                        return false;
                    }
                }
                return true;
                }


            }
            return false;
        } else if (op == "!="){
            return true;
        }
        throw std::runtime_error("Comparing of two different types for: " + op + "\n" + getTypeName(left) + op + getTypeName(right));
        }
        if (left.type() == right.type() && (dummyValues.find(getTypeName(left)) == dummyValues.end())){

        if (op == "=="){
            return compareAnyTy(left,right) || compareAnyTy(right,left);
        } else if (op == "!="){
            return !(compareAnyTy(left,right) || compareAnyTy(right,left));
        }
        
                
        }}
        

        

        // Helper function to convert std::any to double


        try {
            if (in(getTypeName(left),inttypeNames)){

                double leftVal = std::any_cast<double>(convertToDouble(left));
                double rightVal = std::any_cast<double>(convertToDouble(right));
                

                double result;
                if (op == "+") {
                    result = leftVal + rightVal;
                    
                } else if (op == "-") {
                    result = leftVal - rightVal;
                } else if (op == "*") {
                    result = leftVal * rightVal;
                } else if (op == "/") {
                    if (rightVal == 0) {
                        std::cerr << "Runtime error: Division by zero\n";
                        return nullification{};
                    }
                    result = leftVal / rightVal;
                    //return Object(result,"f64");
                } else if (op == "==") {
                    return leftVal == rightVal;
                } else if (op == "!=") {
                    return leftVal != rightVal;
                } else if (op == ">") {
                    return leftVal > rightVal;
                } else if (op == "<") {
                    return leftVal < rightVal;
                } else if (op == ">=") {
                    return leftVal >= rightVal;
                } else if (op == "<=") {
                    return leftVal <= rightVal;
                } else if (op == "<<"){
                    result = static_cast<int64_t>(leftVal) << static_cast<int64_t>(rightVal);
                } else if (op == ">>"){
                    result = static_cast<int64_t>(leftVal) << static_cast<int64_t>(rightVal);
                } else if (op == "b|"){
                    result = static_cast<int64_t>(leftVal) | static_cast<int64_t>(rightVal);
                } else if (op == "b&"){
                    result = static_cast<int64_t>(leftVal) & static_cast<int64_t>(rightVal);
                } else if (op == "b^"){
                    result = static_cast<int64_t>(leftVal) ^ static_cast<int64_t>(rightVal);
                } 


                // Wrap result in Object and return perhaps hierachy?
                if (getTypeName(left) == "f64" || getTypeName(right) == "f64"){
                    return Object(result,"f64").GetStore();
                }
                if (getTypeName(left) == "i32" && getTypeName(right) == "i32"){
                    return static_cast<int>(result);
                }
                
                return Object(double(result), getTypeName(left)).GetStore();
            }

            // Handle string and character types
            if (left.type() == typeid(std::string) || left.type() == typeid(const char*) ||
                left.type() == typeid(char) || left.type() == typeid(wchar_t) ||
                left.type() == typeid(std::wstring)) {

                std::string leftStr;
                std::string rightStr;


                // Convert left to std::string
                if (left.type() == typeid(std::string)) {
                    leftStr = std::any_cast<std::string>(left);
                } else if (left.type() == typeid(const char*)) {
                    leftStr = std::string(std::any_cast<const char*>(left));
                } else if (left.type() == typeid(char)) {
                    leftStr = std::string(1, std::any_cast<char>(left));
                } else if (left.type() == typeid(std::wstring)) {
                    std::wstring wstr = std::any_cast<std::wstring>(left);
                    leftStr = std::string(wstr.begin(), wstr.end());
                }

                // Convert right to std::string
                if (right.type() == typeid(std::string)) {
                    rightStr = std::any_cast<std::string>(right);
                } else if (right.type() == typeid(const char*)) {
                    rightStr = std::string(std::any_cast<const char*>(right));
                } else if (right.type() == typeid(char)) {
                    rightStr = std::string(1, std::any_cast<char>(right));
                } else if (right.type() == typeid(std::wstring)) {
                    std::wstring wstr = std::any_cast<std::wstring>(right);
                    rightStr = std::string(wstr.begin(), wstr.end());
                }

                std::any result;
                if (op == "==") {

                    result = leftStr == rightStr;
                } else if (op == "!=") {
                    result = leftStr != rightStr;
                } else if (op == "+"){
                    result = leftStr + rightStr;
                }
                // Wrap result in Object and return
                return result;
                //return Object(std::any_cast<bool>(result), "bool");
            }

            // Handle logical operations for bool
            if ((left.type() == typeid(bool) || right.type() == typeid(bool))) {
                bool leftBool = std::any_cast<bool>(left);
                bool rightBool = std::any_cast<bool>(right);

                std::any result;
                if (op == "&&") {
                    result = leftBool && rightBool;
                } else if (op == "||") {
                    result = leftBool || rightBool;
                } else if (op == "==") {
                    result = leftBool == rightBool;
                } else if (op == "!=") {
                    result = leftBool != rightBool;
                }


                // Wrap result in Object and return
                return result;
            }
            if (left.type() == typeid(nullification)){
                if (right.type() != typeid(nullification) && op == "=="){
                    return false;
                } else if (right.type() == typeid(nullification) && op == "=="){
                    return true;
                } else if (right.type() != typeid(nullification) && op == "!="){
                    return true;
                } else if (right.type() == typeid(nullification) && op == "!="){
                    return false;
                }
            }
            if (right.type() == typeid(nullification)){
                if (left.type() != typeid(nullification) && op == "=="){
                    return false;
                } else if (left.type() == typeid(nullification) && op == "=="){
                    return true;
                } else if (left.type() != typeid(nullification) && op == "!="){
                    return true;
                } else if (left.type() == typeid(nullification) && op == "!="){
                    return false;
                }
            }
            if (left.type() == right.type() && left.type() == typeid(TypeEnumKey)){
                if (op == "=="){
                    try {
                    return std::any_cast<TypeEnumKey>(left).getId() == std::any_cast<TypeEnumKey>(right).getId();
                } catch (std::bad_any_cast){
                    return false;
                }
                }

            }

        } catch (const std::bad_any_cast& e) {
            std::cerr << "Bad any cast: " << e.what() << std::endl;
        }

        
        std::cerr << "Interpreter error: Invalid binary operation\n" << getTypeName(left) << op << getTypeName(right);
        return nullification{};
    }

    std::any interpretderef(std::shared_ptr<DerefNode> DN){
        std::any pobptr = interpretStatement(DN->getExpr());
        if (pobptr.type() == typeid(Pointer)){
            
            
            std::any tr = (*std::any_cast<Pointer>(pobptr).getptr()).getValue();
            forwards[DN->id] = nety(std::any_cast<Pointer>(pobptr).getptr()->getValue());
            
            
            
            return tr;

        } else if (pobptr.type() == typeid(BorrowedPointer)){
           //println("PRE-DIS");
            ////println(convertToString((*std::any_cast<BorrowedPointer>(pobptr).getptr()).getValue()));
            //println("WORKS?");
            if(std::any_cast<BorrowedPointer>(pobptr).getptr() == nullptr){
                throw std::runtime_error("????");
            }
            forwards[DN->id] = nety(std::any_cast<BorrowedPointer>(pobptr).getptr()->getValue());
            return (*(std::any_cast<BorrowedPointer>(pobptr).getptr())).getValue();
        }
        else if (auto idn = std::dynamic_pointer_cast<IdentifierNode>(DN->getExpr())){
            if (idn->getValue() == "self"){
                println("Moving forward");
                forwards[DN->id] = nety(pobptr);
                println("Moved forward");
                return pobptr;
            } else {
                throw std::runtime_error("Dereferencing is only supported for Pointers or BorrowedPointers\nGot: " + demangle(pobptr.type().name()));
            }
        }
        throw std::runtime_error("Dereferencing is only supported for Pointers or BorrowedPointers\nGot: " + demangle(pobptr.type().name()));
    }


    std::any interpretAssign(std::shared_ptr<AssignNode> node) {
        //println("beginning assign!");

        auto varName = node->getVarName();
        auto valueNode = node->getValue();
        int vid = 0;
        //println("POST EXEC ASSIGN");
        if (auto id = std::dynamic_pointer_cast<IdentifierNode>(valueNode)){
            vid = getCurrentScope()->getVariable(id->getValue())->valueid;
        }

        std::any result = interpretStatement(valueNode);
        if (result.type() == typeid(NoAssign)){
            return nullification{};
        }


        //println("Beginning assign");
        //println("MUTSTATUS: ");
        //println(convertToString(node->getMut()));
        std::shared_ptr<Variable> var;
        if (result.type() == typeid(int32_t)) {
            var = std::make_shared<Variable>(std::any_cast<int32_t>(result),node->getMut());
        } else if (result.type() == typeid(std::string)) {
            var = std::make_shared<Variable>(std::any_cast<std::string>(result),node->getMut());
        } else if (result.type() == typeid(bool)) {
            var = std::make_shared<Variable>(std::any_cast<bool>(result),node->getMut());
        } else if (result.type() == typeid(nullification)) {
            var = std::make_shared<Variable>(nullification{},node->getMut());
        }/**/ else if (result.type() == typeid(List)) {
            var = std::make_shared<Variable>(std::any_cast<List>(result),node->getMut());
        } else if (result.type() == typeid(TypeSafeList)) {
            TypeSafeList tsl = std::any_cast<TypeSafeList>(result);
            var = std::make_shared<Variable>(tsl,node->getMut());
        } else if (result.type() == typeid(Tuple)) {
            var = std::make_shared<Variable>(std::any_cast<Tuple>(result),node->getMut());
        } else if (result.type() == typeid(StructDecl)) {
            var = std::make_shared<Variable>(std::any_cast<StructDecl>(result),node->getMut());
        } else if (result.type() == typeid(StructInstance)) {
            auto SI = std::any_cast<StructInstance>(result);
            var = std::make_shared<Variable>(SI,node->getMut());
        } else if (result.type() == typeid(Enum)) {
            var = std::make_shared<Variable>(std::any_cast<Enum>(result),node->getMut());
        } else if (result.type() == typeid(EnumKey)) {
            var = std::make_shared<Variable>(std::any_cast<EnumKey>(result),node->getMut());
        } else if (result.type() == typeid(Pointer)) {
            Pointer ptr = std::any_cast<Pointer>(result);
            if (ptr.getpt() != varName && ptr.getpt() != ""){

            
            
            ptrnowptsto[ptr.getpt()] = varName;
            getCurrentScope()->removeVariable(ptr.getpt(),false);
            }
            var = std::make_shared<Variable>(ptr,node->getMut());
        } else if (result.type() == typeid(BorrowedPointer)) {
            BorrowedPointer ptr = std::any_cast<BorrowedPointer>(result);
            var = std::make_shared<Variable>(ptr,node->getMut());
        } else if (result.type() == typeid(MappedFunction)) {
            var = std::make_shared<Variable>(std::any_cast<MappedFunction>(result),node->getMut());
        } else if (result.type() == typeid(Class)) {
            var = std::make_shared<Variable>(std::any_cast<Class>(result),node->getMut());
        } else if (result.type() == typeid(ClassInstance)) {
            var = std::make_shared<Variable>(std::any_cast<ClassInstance>(result),node->getMut());
        } else if (result.type() == typeid(std::shared_ptr<Future>)) {
            throw std::runtime_error("Future is not assignable!");
        } else if (result.type() == typeid(CEXTFunction)) {
            var = std::make_shared<Variable>(std::any_cast<CEXTFunction>(result),node->getMut());
        } else if (result.type() == typeid(Module)) {
            var = std::make_shared<Variable>(std::any_cast<Module>(result),node->getMut());
        } else if (result.type() == typeid(ModuleMethod)) {
            var = std::make_shared<Variable>(std::any_cast<ModuleMethod>(result),node->getMut());
        } else if (result.type() == typeid(Tag)) {
            var = std::make_shared<Variable>(std::any_cast<Tag>(result),node->getMut());
        } else if (result.type() == typeid(TagFunction)) {
            var = std::make_shared<Variable>(std::any_cast<TagFunction>(result),node->getMut());
        } else if (result.type() == typeid(Object)) {

            auto obj = std::any_cast<Object>(result);
             // try to access value
            var = std::make_shared<Variable>(obj,node->getMut());
        } else {
            //println("Unrecognised type: " + demangle(result.type().name()));
            Object ovar = Object(result,getTypeName(result));
            ovar.GetStore(); // try to access value
            var = std::make_shared<Variable>(std::any_cast<Object>(ovar),node->getMut());
        }

        if (var) {
            if (auto currentScope = getCurrentScope()) {
                if (vid != 0){
                    var->setVID( vid);
                }

                currentScope->addVariable(varName, var);
                for (auto& id:vidids){
                    forwardscplex[id] = var->valueid;
                    vidids.clear();
                }
            } else {
                std::cerr << "Runtime error: No current scope found\n";
            }
        }
        

        return nullification{};
    }
    void insertscope(std::shared_ptr<Scope> ins){
        swapscope.push_back(getCurrentScope());

    }
    std::vector<std::shared_ptr<Scope>> swapscope;
    private:


};


class Instruct {
    public:
    std::shared_ptr<function_instruction> mainfn;
    std::shared_ptr<BlockNode> rootblock;
    bool isassignflag=false;
    Instruct(std::shared_ptr<BlockNode> rootblock): rootblock(rootblock){

    }

    std::shared_ptr<Instruction> handleILN(std::shared_ptr<IntLiteralNode> in){
        return std::make_shared<resource_instruction>(std::string("i32"),in->getValue());
    }
    std::shared_ptr<Instruction> handleObject(std::shared_ptr<ONode> obj){
        return std::make_shared<resource_instruction>(std::string(obj->getValue().getType()),obj->getValue().GetStore());
    }

    std::shared_ptr<Instruction> handleSLN(std::shared_ptr<StringLiteralNode> sln){
        return std::make_shared<resource_instruction>(std::string("RawString"),sln->getValue());
    }

    std::shared_ptr<block_instruction> handleBlock(std::shared_ptr<BlockNode> bn,bool ismain=false){
        std::vector<std::shared_ptr<Instruction>> insts;

        for (auto& stat : bn->getStatements()){

            auto ptr = instruct(stat);
            if (ptr){
                insts.push_back(ptr);
            }
        }
        return std::make_shared<block_instruction>(ismain,insts);
    }

    std::shared_ptr<Instruction> handleTag(std::string vn,std::shared_ptr<TagNode> TN){
        return std::make_shared<tag_instruction>(std::dynamic_pointer_cast<function_instruction>(handleMFN(vn,std::dynamic_pointer_cast<MappedFunctionNode>(TN->getValue()))));
    }

    std::shared_ptr<Instruction> handleAssign(std::shared_ptr<AssignNode> AN){
        bool prev = isassignflag;
        
        if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
            return handleMFN(AN->getVarName(),MFN);
        }
        if (auto SDN = std::dynamic_pointer_cast<StructDeclNode>(AN->getValue())){
            return handleStructDecl(AN->getVarName(),SDN);
        }
        if (auto Mod = std::dynamic_pointer_cast<ImportNode>(AN->getValue())){
            return handleModule(Mod);
        }
        if (auto Tag = std::dynamic_pointer_cast<TagNode>(AN->getValue())){
            return handleTag(AN->getVarName(),Tag);
        }
        if (auto IFN = std::dynamic_pointer_cast<IFNode>(AN->getValue())){
            auto node = std::make_shared<assign_instruction>(AN->getVarName(),handleIf(IFN,true));
            isassignflag = prev;
            return node;
        }
        if (auto WN = std::dynamic_pointer_cast<WhileNode>(AN->getValue())){
            auto node = std::make_shared<assign_instruction>(AN->getVarName(),handleWhile(WN,true));
            isassignflag = prev;
            return node;
        }
        isassignflag = true;
        
        auto node = std::make_shared<assign_instruction>(AN->getVarName(),instruct(AN->getValue()),false);
        isassignflag = prev;
        return node;
    }
    // struct_instance_instruction


    std::shared_ptr<Instruction> handleIdentifier(std::shared_ptr<IdentifierNode> IN){
        logat("Receiving " + IN->getValue() ,"Ins.hI");
        if (dummyinstructions.find(IN->getValue()) != dummyinstructions.end()){
            return dummyinstructions[IN->getValue()];
        }
        std::shared_ptr<load_instruction> linst;
        if (forwardscplex.find(IN->id) != forwardscplex.end()){
            linst = std::make_shared<load_instruction>(std::any_cast<std::string>(forwardscplex[IN->id]),isassignflag);
        } else {
            linst = std::make_shared<load_instruction>(IN->getValue(),isassignflag);
        }
        
        logat("Received " + linst->getName() ,"Ins.hI");
        return linst;
    }
    std::shared_ptr<Instruction> handleOperator(std::shared_ptr<BinOP> ON){
        return std::make_shared<operation_instruction>(instruct(ON->getleft()),instruct(ON->getright()),ON->getValue());
    }

    std::shared_ptr<Instruction> handleExtern(std::shared_ptr<ExternNode> EN){
        auto vecnode = EN->getValue();
        auto fnname = EN->getfnnames();
        auto isvdic = EN->getvdic();
        std::vector<std::shared_ptr<Instruction>> fninsts;
        for (int i = 0; i != vecnode.size();i++){
            auto fn = std::dynamic_pointer_cast<MappedFunctionNode>(vecnode[i]);
            std::vector<std::shared_ptr<Instruction>> args;
            auto fnnargs = fn->getValue();
            std::shared_ptr<Instruction> returnty = instruct(fnnargs["-!retty"]);
            fnnargs.erase("-!retty");
            for (auto & arg : fnnargs){
                args.push_back(instruct(arg.second));
            }
            fninsts.push_back(std::make_shared<declare_function_instruction>(fnname[i],args,returnty,in(fnname[i],isvdic)));

        }
        return std::make_shared<block_instruction>(false,fninsts); // MAKE THIS BETTER INTO A CHAIN INSTRUCTION
    }

    std::shared_ptr<Instruction> handleMFN(std::string name="",std::shared_ptr<MappedFunctionNode> MFN=nullptr){
        bool anon = false;
        if (name == ""){
            name = "anonfn" + std::to_string(bc);
            bc++;
            anon = true;
        }
        bool isnoneret = std::any_cast<bool>(forwardscplex2[MFN->id]);
 
        std::vector<std::shared_ptr<Instruction>> args;
        auto fnnargs = MFN->getValue();
        auto retty = fnnargs["-!retty"];
        auto intes = MFN->getinternals();
        std::shared_ptr<Instruction> returnty = instruct(fnnargs["-!retty"]);
        fnnargs.erase("-!retty");
        for (auto & arg : fnnargs){
            if (arg.first == "self" && MFN->isselfptr){
                args.push_back(std::make_shared<pointer_instruction>(instruct(arg.second)));
            } else {
            args.push_back(instruct(arg.second));
            }
        }
        std::vector<std::string> argnames;
        for (auto& argn : MFN->gettick()){
            if (intes.find(argn.second) != intes.end()){
                argnames.push_back(intes[argn.second]);
            } else {
            
                argnames.push_back(argn.second);
            }
        }
        std::shared_ptr<BlockNode> blk;
        if (anon && std::dynamic_pointer_cast<BlockNode>(MFN->getBody())->StatementsLen() == 0){
            blk = std::dynamic_pointer_cast<BlockNode>(MFN->getBody());
            blk->addStatement(std::make_shared<RetNode>(std::make_shared<IdentifierNode>(getTypeName(forwardscplex[MFN->id]))));
        } else {
            blk = std::dynamic_pointer_cast<BlockNode>(MFN->getBody());
        }
        
        return std::make_shared<function_instruction>(name,returnty,args,std::dynamic_pointer_cast<block_instruction>(instruct(blk)),false,false,false,argnames,"",isnoneret);
    }
    std::shared_ptr<Instruction> handlePub(std::shared_ptr<PubNode> PN){
        if (auto aas = std::dynamic_pointer_cast<AssignNode>(PN->getValue())){
            if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(aas->getValue())){
                auto name = aas->getVarName();
                std::vector<std::shared_ptr<Instruction>> args;
                auto fnnargs = MFN->getValue();
                auto internals = MFN->getinternals();
                std::shared_ptr<Instruction> returnty = instruct(fnnargs["-!retty"]);
                fnnargs.erase("-!retty");
                for (auto & arg : fnnargs){
                    args.push_back(instruct(arg.second));
                }
                std::vector<std::string> argnames;
                for (auto& argn : MFN->gettick()){
                    if (internals.find(argn.second) == internals.end()){
                        argnames.push_back(argn.second);
                    } else {
                        argnames.push_back(internals[argn.second]);
                    }
                }
                bool isnoneret = std::any_cast<bool>(forwardscplex2[MFN->id]);
                if (!AP.has("-bundle") && result != "package"){
                    return std::make_shared<function_instruction>(name,returnty,args,std::dynamic_pointer_cast<block_instruction>(instruct(MFN->getBody())),false,true,false,argnames,"",isnoneret);
                } else {
                    return std::make_shared<function_instruction>(name,returnty,args,std::dynamic_pointer_cast<block_instruction>(instruct(MFN->getBody())),false,true,false,argnames,AP.values["compile"][0],isnoneret);
                }
            }
        }
    }

    std::shared_ptr<Instruction> handleCall(std::shared_ptr<CallNode> CN){


        auto db = std::any_cast<tsl::ordered_map<std::string,std::any>>(forwardscplex[CN->id]);
        auto clr = std::any_cast<std::string>(db["type"]);
        if (clr == "mappedfunction" || clr == "tag"){

        std::vector<std::shared_ptr<Instruction>> args; // need to forward the mapped function
        tsl::ordered_map<int,std::shared_ptr<ASTNode>> finalargs; // sincerely think about this before doing it lmao
        tsl::ordered_map<std::string,std::shared_ptr<ASTNode>> finalstrargs; 
        auto MFN = std::any_cast<MappedFunction>(db["mfn"]);
        auto CNargs = CN->getExpr();
        if (clr == "tag"){
            CNargs = std::any_cast<tsl::ordered_map<int, std::shared_ptr<ASTNode>>>(db["astack"]);
        }
        auto inserts = CN->getinserts();
        auto tick = MFN.gettick();
        auto strarg = MFN.getargs();
        for (int i = 0;i != CNargs.size();i++){
            if (tick.find(i) != tick.end()){
                finalstrargs[tick[i]] = CNargs[i];
            } else {
                finalstrargs["arg" + std::to_string(i)] = CNargs[i];
            }
        }
        for (auto& insert : inserts){
            finalstrargs[insert.first] = std::move(insert.second);
        }
        int i = 0;
        for (auto& strarg: finalstrargs){
            bool checker = false;
            reversefind<int,std::string>(tick,strarg.first,&checker);
            if (checker){
                auto v = *reversefind<int,std::string>(tick,strarg.first);

                finalargs[v] = std::move(strarg.second);
            } else {
                finalargs[i] = std::move(strarg.second);
            }
            i++;
        }
        for (int i = 0;i != finalargs.size();i++){
            if (tick[i] == "self" && MFN.isselfptr){
                auto inst = instruct(finalargs[i]);
                if (auto li = std::dynamic_pointer_cast<load_instruction>(inst)){
                    li->setGiveBackPtr(true);
                    args.push_back(li);
                } else {
                    args.push_back(inst);
                }

                
            } else {
                args.push_back(instruct(finalargs[i]));
            }
            
        }
        //if (MFN) push variadic back
        return std::make_shared<call_instruction>(instruct(CN->getBody()),args,std::any_cast<std::shared_ptr<type_instruction>>(db["fty"]));
        }
        return std::make_shared<nop_instruction>();
    }
    std::shared_ptr<Instruction> handleDecorator(std::shared_ptr<DecoratorNode> DN){
        if (auto CN = std::dynamic_pointer_cast<CallNode>(DN->getClr())){
            if (auto AN = std::dynamic_pointer_cast<AssignNode>(DN->getFn())){
                if (auto FN = std::dynamic_pointer_cast<MappedFunctionNode>(AN->getValue())){
                    auto res = handleCall(CN);
                    if (!inT<int>(CN->id,issig)){
                    return std::make_shared<assign_instruction>(AN->getVarName(),res);
                    } else {
                        return res;
                    }
                }
            }
        }
    }

    std::shared_ptr<Instruction> handleRet(std::shared_ptr<RetNode> RN){
        return std::make_shared<return_instruction>(instruct(RN->getValue()));
    }


    std::shared_ptr<Instruction> handleIf(std::shared_ptr<IFNode> IFN,bool isassign = false){
        auto elifs = IFN->getElses();
        std::vector<std::shared_ptr<if_instruction>> insts = {};
        for (auto& elif : elifs){
            insts.push_back(std::make_shared<if_instruction>(instruct(elif.second->getExpr()),std::dynamic_pointer_cast<block_instruction>(instruct(elif.second->getBody()))));
        }
        if (IFN->getNot()){
        return std::make_shared<if_instruction>(instruct(IFN->getMain()->getExpr()),std::dynamic_pointer_cast<block_instruction>(instruct(IFN->getMain()->getBody())),std::dynamic_pointer_cast<block_instruction>(instruct(IFN->getNot()->getBody())),insts,isassign || inT<int>(IFN->id,isused));
        } else {
            return std::make_shared<if_instruction>(instruct(IFN->getMain()->getExpr()),std::dynamic_pointer_cast<block_instruction>(instruct(IFN->getMain()->getBody())),nullptr,insts,isassign || inT<int>(IFN->id,isused));
        }
    }
     std::shared_ptr<Instruction> handleWhile(std::shared_ptr<WhileNode> WN,bool isassign=false){
        return std::make_shared<while_instruction>(instruct(WN->getExpr()->getExpr()),std::dynamic_pointer_cast<block_instruction>(instruct(WN->getExpr()->getBody())),isassign || inT<int>(WN->id,isused));
     }


    std::shared_ptr<Instruction> handlePointer(std::shared_ptr<PointerNode> PN){
        if (!PN->getBorrow()){
            return std::make_shared<pointer_instruction>(instruct(PN->getExpr()),false);
        } else {
            return std::make_shared<borrowed_pointer_instruction>(instruct(PN->getExpr()));
        }
    }
    std::shared_ptr<Instruction> handleDereference(std::shared_ptr<DerefNode> DN){
        return std::make_shared<dereference_instruction>(instruct(DN->getExpr()),forwards[DN->id]);
    }

    std::shared_ptr<Instruction> handlePointerStore(std::shared_ptr<ModifyPtrNode> MPN){
        return std::make_shared<pointer_store_instruction>(instruct(MPN->getLHS()),instruct(MPN->getExpr()));
    }
    std::shared_ptr<Instruction> handleStructDecl(std::string name,std::shared_ptr<StructDeclNode> SDN){
        std::vector<std::shared_ptr<Instruction>> fields;
        for (auto& field : SDN->getValue()){
            fields.push_back(instruct(field.second));
        }
        return std::make_shared<struct_decl_instruction>(name,fields);
    }

    std::shared_ptr<Instruction> handleStructInstance(std::shared_ptr<StructInstanceNode> SIN){
        std::vector<std::shared_ptr<Instruction>> fields;
        std::vector<std::string> sfields;
        for (auto& field : SIN->getValue()){
            fields.push_back(instruct(field.second));
            sfields.push_back(field.first);
        }
        return std::make_shared<struct_instance_instruction>(instruct(SIN->getBase()),fields,sfields,false,std::any_cast<int>(forwardscplex[SIN->id]));
    }
    std::shared_ptr<Instruction> handleExprAssign(std::shared_ptr<ExprAssignNode> EAN){
        return std::make_shared<alt_assign_instruction>(instruct(EAN->getVarName()),instruct(EAN->getValue()));
    }
    std::shared_ptr<Instruction> handleMemberAccess(std::shared_ptr<MemAccNode> MAN,bool privassign=false){
        auto db = std::any_cast<tsl::ordered_map<std::string,std::any>>(forwardscplex[MAN->id]);
        std::string accessty = std::any_cast<std::string>(db["type"]);
        int vid = std::any_cast<int>(db["vid"]);
        if (MAN->getAssign()){
            MAN->setAssign(false); // we're done with wrapping it
            return std::make_shared<alt_assign_instruction>(handleMemberAccess(MAN,true),instruct(MAN->getAssignv()));
        }
        if (accessty == "structI"){
        std::string sity = std::any_cast<std::string>(db["siaccesstype"]);
        if (sity == "member"){
        std::vector<std::shared_ptr<type_instruction>> vec{std::any_cast<std::shared_ptr<type_instruction>>(db["structI"]),std::any_cast<std::shared_ptr<type_instruction>>(db["memtype"])};
        if (auto sln = std::dynamic_pointer_cast<StringLiteralNode>(MAN->getNxt())){
            return std::make_shared<access_struct_member_instruction>(instruct(MAN->getValue()),sln->getValue(),std::any_cast<int>(db["id"]),vec,privassign);
        } else  if (auto M2 = std::dynamic_pointer_cast<MemAccNode>(MAN->getNxt())){
            return std::make_shared<access_struct_member_instruction>(handleMemberAccess(M2),std::dynamic_pointer_cast<StringLiteralNode>(M2->getNxt())->getValue(),std::any_cast<int>(db["id"]),vec,privassign);
        }
        } else { // cons or method
        
        if (auto sln = std::dynamic_pointer_cast<StringLiteralNode>(MAN->getNxt())){
            return std::make_shared<access_extend_instruction>(instruct(MAN->getValue()),sln->getValue(),sity == "method",sity == "method" ? 0 : vid);
        } else  if (auto M2 = std::dynamic_pointer_cast<MemAccNode>(MAN->getNxt())){
            return std::make_shared<access_extend_instruction>(handleMemberAccess(M2),std::dynamic_pointer_cast<StringLiteralNode>(M2->getNxt())->getValue(),sity == "method",sity == "method" ? 0 : vid);
        }

        }
        } else if (accessty == "tuple"){
        if (db.find("inferred") != db.end()){
            return instruct(std::any_cast<std::shared_ptr<MemAccNode>>(db["inferred"]));
        }
        std::vector<std::shared_ptr<type_instruction>> vec{std::any_cast<std::shared_ptr<type_instruction>>(db["tuple"]),std::any_cast<std::shared_ptr<type_instruction>>(db["memtype"])};
        if (auto sln = std::dynamic_pointer_cast<StringLiteralNode>(MAN->getNxt())){
            return std::make_shared<access_tuple_member_instruction>(instruct(MAN->getValue()),std::any_cast<int>(db["index"]),vec);
        } else  if (auto M2 = std::dynamic_pointer_cast<MemAccNode>(MAN->getNxt())){
            return std::make_shared<access_tuple_member_instruction>(handleMemberAccess(M2),std::any_cast<int>(db["id"]),vec);
        }
        } else if (accessty == "module"){
        auto mod = std::any_cast<Module>(db["mod"]);
        
        if (auto sln = std::dynamic_pointer_cast<StringLiteralNode>(MAN->getNxt())){
            return std::make_shared<load_instruction>(mod.hash + sln->getValue());
        } // module within a module is a nogo jose;
        
        } else if (accessty == "tag"){
            auto sln = std::dynamic_pointer_cast<StringLiteralNode>(MAN->getNxt());
            return std::make_shared<get_tag_instruction>(std::dynamic_pointer_cast<load_instruction>(handleIdentifier(std::make_shared<IdentifierNode>(sln->getValue()))),std::any_cast<std::shared_ptr<type_instruction>>(db["callertype"]));
        }
    }

    std::shared_ptr<Instruction> handleTuple(std::shared_ptr<TupleNode> TN){
        std::vector<std::shared_ptr<Instruction>> elements;
        for (auto& v : TN->getValue()){
            elements.push_back(instruct(v.second));
        }
        return std::make_shared<tuple_instruction>(elements);
    }

    std::shared_ptr<Instruction> handleCast(std::shared_ptr<CastNode> CN){
        auto tinst = forwards[CN->id];
        return std::make_shared<cast_instruction>(instruct(CN->getValue()),tinst);
    }

    std::shared_ptr<Instruction> handleTSL(std::shared_ptr<TypeSafeListNode> TSLN){
        std::vector<std::shared_ptr<Instruction>> elems;
        for (auto& i : TSLN->getValue()){
            elems.push_back(instruct(i.second));
        }
        return std::make_shared<array_instruction>(elems,elems.size());
    }
    std::shared_ptr<Instruction> handleTSLInit(std::shared_ptr<TSLInitNode> TSLIN){
        std::vector<std::shared_ptr<Instruction>> elems;
        auto sz = std::any_cast<int32_t>(forwardscplex[TSLIN->id]);
        auto size = sz;
        while (size != 0) {
            elems.push_back(instruct(TSLIN->getExpr()));
            size--;
        }
        return std::make_shared<array_instruction>(elems,sz);
    }
    //std::vector<std::shared_ptr<type_instruction>>
    std::shared_ptr<Instruction> handleIndex(std::shared_ptr<IndexNode> IN){
        auto db = std::any_cast<tsl::ordered_map<std::string,std::any>>(forwardscplex[IN->id]);
        auto vec = std::any_cast<std::vector<std::shared_ptr<type_instruction>>>(db["arrity"]);
        std::string ty = std::any_cast<std::string>(db["ty"]);
        if (ty == "arr"){
        return std::make_shared<access_array_instruction>(instruct(IN->getValue()),vec,instruct(IN->getIndex()));
        } else if (ty == "ptr"){
            return std::make_shared<access_pointer_instruction>(instruct(IN->getValue()),vec,instruct(IN->getIndex()));
        }
    }
    std::shared_ptr<Instruction> handleExtend(std::shared_ptr<ModNode> MN){
        auto db = std::any_cast<tsl::ordered_map<std::string,std::any>>(forwardscplex[MN->id]);
        auto extty = std::any_cast<std::string>(db["type"]);
        if (extty == ""){
        std::vector<std::shared_ptr<Instruction>> exts;
        for (auto& ext: MN->getStates()){
            exts.push_back(instruct(ext));
        }
        return std::make_shared<extend_instruction>(instruct(MN->getValue()),exts);
        } else if (extty == "tag"){
            std::vector<std::shared_ptr<Instruction>> exts;
            for (auto& ext: MN->getStates()){
                exts.push_back(std::make_shared<tag_instruction>(std::dynamic_pointer_cast<function_instruction>(instruct(ext))));
            }
            return std::make_shared<chain_instruction>(exts);
        }
    }
    std::shared_ptr<Instruction> handleSZ(std::shared_ptr<SizeOfNode> SON){
        return std::make_shared<size_of_instruction>(instruct(SON->getValue()));
    }
    std::shared_ptr<Instruction> handleStackSize(std::shared_ptr<StackSizeNode> SSN){
        return std::make_shared<stack_size_instruction>();
    }
    std::shared_ptr<Instruction> handleRef(std::shared_ptr<RefNode> RN){
        auto inst = instruct(RN->getValue());
        inst->setGiveBackPtr(true);
        return inst;
    }
    std::shared_ptr<Instruction> handleTY(std::shared_ptr<TypeIDNode> TIDN){
        std::shared_ptr<type_instruction> ty = nullptr;
        if (forwards.find(TIDN->id) != forwards.end()){
            ty = forwards[TIDN->id];
        }
        return std::make_shared<type_id_instruction>(instruct(TIDN->getValue()),ty);
    }
    std::unordered_map<std::string,std::shared_ptr<Instruction>> asmUTIL(std::unordered_map<std::string,std::shared_ptr<ASTNode>> v){
        std::unordered_map<std::string,std::shared_ptr<Instruction>> ret;
        for (auto& x : v){
            ret[x.first] = instruct(x.second);
        }
        return ret;

    }
    std::shared_ptr<Instruction> handleASM(std::shared_ptr<ASMNode> ASMN){
        return std::make_shared<asm_instruction>(std::make_shared<resource_instruction>("ASMSTR",ASMN->ASMStr),ASMN->regs,asmUTIL(ASMN->in),asmUTIL(ASMN->out),asmUTIL(ASMN->inout));
    }
    std::shared_ptr<Instruction> handleP2I(std::shared_ptr<PtrtointNode> PTIN){
        return std::make_shared<ptrtoint_instruction>(instruct(PTIN->getValue()));
    }

    std::shared_ptr<Instruction> handleChain(std::shared_ptr<ChainNode> ChN){
        std::vector<std::shared_ptr<Instruction>> insts;
        for (auto& ch : ChN->getChain()){
            insts.push_back(instruct(ch));
        }
        return std::make_shared<chain_instruction>(insts);
    }

    std::shared_ptr<Instruction> handleModule(std::shared_ptr<ImportNode> ImN){
        Module mod = std::any_cast<Module>(forwardscplex[ImN->id]);
        std::vector<std::shared_ptr<Instruction>> insts;
        auto from = ImN->getFrom();
        for (auto& method : mod.methods){
            //auto mfn = std::any_cast<MappedFunction>(method.second);
            auto vec = mod.MNodes[method.first];
            std::vector<std::shared_ptr<Instruction>> args;
            
            std::shared_ptr<Instruction> retty = instruct(vec[vec.size()-1]);
            vec.pop_back();

            for (auto& arg: vec){
                args.push_back(instruct(arg));
            }


            std::shared_ptr<Instruction> returnty;
            if (!from.empty()){
                if (in(method.first,from)){
                    insts.push_back(std::make_shared<declare_function_instruction>(mod.hash + method.first,args,retty,false));
                }
            } else {
                insts.push_back(std::make_shared<declare_function_instruction>(mod.hash + method.first,args,retty,false));
            }
        }
        return std::make_shared<chain_instruction>(insts);
    }
    std::shared_ptr<Instruction> instruct(std::shared_ptr<ASTNode> node){
        if (auto in = std::dynamic_pointer_cast<IntLiteralNode>(node)){
            return handleILN(in);
        } else if (auto Object = std::dynamic_pointer_cast<ONode>(node)){
            return handleObject(Object);
        } else if (auto StrLit = std::dynamic_pointer_cast<StringLiteralNode>(node)){
            return handleSLN(StrLit);
        } else if (auto AN = std::dynamic_pointer_cast<AssignNode>(node)){
            return handleAssign(AN);
        } else if (auto IN = std::dynamic_pointer_cast<IdentifierNode>(node)){
            return handleIdentifier(IN);
        } else if (auto BN = std::dynamic_pointer_cast<BlockNode>(node)){
            return handleBlock(BN);
        } else if (auto ON = std::dynamic_pointer_cast<BinOP>(node)){
            return handleOperator(ON);
        } else if (auto EN = std::dynamic_pointer_cast<ExternNode>(node)){
            return handleExtern(EN);
        } else if (auto PN = std::dynamic_pointer_cast<PubNode>(node)){
            return handlePub(PN);
        } else if (auto CN = std::dynamic_pointer_cast<CallNode>(node)){
            return handleCall(CN);
        } else if (auto RN = std::dynamic_pointer_cast<RetNode>(node)){
            return handleRet(RN);
        } else if (auto IFN = std::dynamic_pointer_cast<IFNode>(node)){
            return handleIf(IFN);
        } else if (auto WN = std::dynamic_pointer_cast<WhileNode>(node)){
            return handleWhile(WN);
        } else if (auto PN = std::dynamic_pointer_cast<PointerNode>(node)){
            return handlePointer(PN);
        } else if (auto DN = std::dynamic_pointer_cast<DerefNode>(node)){
            return handleDereference(DN);
        } else if (auto MPN = std::dynamic_pointer_cast<ModifyPtrNode>(node)){
            return handlePointerStore(MPN);
        } else if (auto SIN = std::dynamic_pointer_cast<StructInstanceNode>(node)){
            return handleStructInstance(SIN);
        } else if (auto MAN = std::dynamic_pointer_cast<MemAccNode>(node)){
            return handleMemberAccess(MAN);
        } else if (auto TN = std::dynamic_pointer_cast<TupleNode>(node)){
            return handleTuple(TN);
        } else if (auto MFN = std::dynamic_pointer_cast<MappedFunctionNode>(node)){
            return handleMFN("",MFN);
        } else if (auto CN = std::dynamic_pointer_cast<CastNode>(node)){
            return handleCast(CN);
        } else if (auto TSLN = std::dynamic_pointer_cast<TypeSafeListNode>(node)){
            return handleTSL(TSLN);
        } else if (auto TSLIN = std::dynamic_pointer_cast<TSLInitNode>(node)){
            return handleTSLInit(TSLIN);
        } else if (auto IN = std::dynamic_pointer_cast<IndexNode>(node)){
            return handleIndex(IN);
        } else if (auto DN = std::dynamic_pointer_cast<DecoratorNode>(node)){
            return handleDecorator(DN);
        } else if (auto MN = std::dynamic_pointer_cast<ModNode>(node)){
            return handleExtend(MN);
        } else if (auto SON = std::dynamic_pointer_cast<SizeOfNode>(node)){
            return handleSZ(SON);
        } else if (auto SSN = std::dynamic_pointer_cast<StackSizeNode>(node)){
            return handleStackSize(SSN);
        } else if (auto TIDN = std::dynamic_pointer_cast<TypeIDNode>(node)){
            return handleTY(TIDN);
        } else if (auto ASMN = std::dynamic_pointer_cast<ASMNode>(node)){
            return handleASM(ASMN);
        } else if (auto PTIN = std::dynamic_pointer_cast<PtrtointNode>(node)){
            return handleP2I(PTIN);
        } else if (auto ChN = std::dynamic_pointer_cast<ChainNode>(node)){
            return handleChain(ChN);
        } else if (auto EAN = std::dynamic_pointer_cast<ExprAssignNode>(node)){
            return handleExprAssign(EAN);
        } else if (auto RN = std::dynamic_pointer_cast<RefNode>(node)){
            return handleRef(RN);
        } else {
            if (typeid(node) != typeid(std::make_shared<ASTNode>())){
            std::cout << "Unknown Node" << std::endl;
            }
            return nullptr;
        }
    }

    void finish(){
        std::unordered_map<std::string,bool> options = {};
        std::vector <std::shared_ptr<Instruction>> args;

        auto mainblock = handleBlock(rootblock,true);
        mainblock->insts.push_back(std::make_shared<return_instruction>(std::make_shared<resource_instruction>(std::string("i32"),0)));
        std::string mainfnname = "main";
        if (AP.has("-bundle") || result == "package"){
            mainfnname = exports["DECKhash"] + "main";
        }
        mainfn = std::make_shared<function_instruction>(mainfnname,std::make_shared<resource_instruction>(std::string("i32"),0),args,mainblock,true,false,false);

        Instructor instr(AP.values["compile"][0]);
        std::string target = "native";
        if (AP.has("target")){
            target = AP.values["target"][0];
        }
        std::string reloc = "PIC";
        if (AP.has("reloc")){
            target = AP.values["reloc"][0];
        }
        std::string outas;
        std::string output = replace(AP.values["compile"][0],".mvn","");
        if (AP.has("output")){
            outas = AP.values["output"][0];
        }
        if (AP.has("emit")){
            for (auto& out : AP.values["emit"]){
                if (out == "llvm-ir"){
                    options["emit_llvm-ir"] = true;
                }
            }
        }
        std::string llvm_target = instr.inittarget(target,reloc);
        instr.executeInstruction(mainfn);


        instr.buildAndOutput(output,options);
        std::string cc = "clang";
        if (AP.has("cc")){
            cc = AP.values["cc"][0];
        }
        
        std::string structure_clang = cc + " " + output + ".o -o " + outas + " " + " -target " + llvm_target + " " ;

        if (AP.values.find("link") != AP.values.end()){
            for (const auto& k: AP.values["link"]){
                auto p = k.find_last_of("/");
                auto path = k.substr(0,p);
                path = replace(path,"\"","");
                auto lib = k.substr(p+1);
                lib = replace(lib,".lib","");
                lib = replace(lib,"\"","");
                structure_clang += " -L\"" + path + "\" -l" + lib;

            }
        }
        if (AP.values.find("linkpath") != AP.values.end()){
            for (const auto& k: AP.values["linkpath"]){
                structure_clang += " -L" + k;
            }
        }
        if (AP.values.find("toolchain") != AP.values.end()){
            
            structure_clang += " --sysroot=\"" + AP.values["toolchain"][0] + "\" ";
            
        }
        if (AP.values.find("ccflags") != AP.values.end()){
            for (const auto& k: AP.values["ccflags"]){
                structure_clang += " " + k;
            }
        }
        if (AP.values.find("linklib") != AP.values.end()){
            for (const auto& k: AP.values["linklib"]){
                auto l = replace(k,".lib","");
                l = replace(k,"lib","");
                structure_clang += " -l" + l;
            }
        }
        if (!AP.has("-nocc") && !AP.has("-bundle") && result != "package"){

        //std::vector<std::string> objectpaths;
        auto linkdir = std::filesystem::absolute(outas + ".imports");
        std::filesystem::create_directory(linkdir);
        for (auto& o : objects){
            logat("Objecting " + linkdir.string() + "/" + o.first + ".o" ,"Ins.f");
            std::ofstream ofile(linkdir.string() + "/" + o.first + ".o", std::ios::binary);
            structure_clang += std::string(" " + linkdir.string() + "/" + o.first + ".o");
            ofile.write(o.second.data(),o.second.size());
            ofile.close();
        }
        if (AP.has("-showcc")){
            println(structure_clang);
        }
        

        system(structure_clang.c_str());
        std::filesystem::remove_all(linkdir);
        }
        if (AP.has("-bundle") || result == "package"){
            auto bundler = ObjectBundler();
            bundler.bundle(std::filesystem::absolute(output + ".o"),exports,std::filesystem::absolute(outas + ".deck"));
        }

        if (AP.has("emit")){
            if (in("object",AP.values["emit"])){
                
                
            } else {
                std::filesystem::remove(std::filesystem::absolute(output + ".o"));
            }
        } else {
            std::filesystem::remove(std::filesystem::absolute(output + ".o"));
        }
  
        println(green_block + fcircle + reset + " Finished build" + (warns == 0 ? "." : (" with " + std::to_string(warns) + " warnings.")));
    }
    

    /*std::string add(std::shared_ptr<ASTNode> node){
        if (auto in = std::dynamic_pointer_cast<IntLiteralNode>(node)){
            return handleILN(in);
        } else if (auto Object = std::dynamic_pointer_cast<ONode>(node)){
            return handleObject(Object);
        } else if (auto StrLit = std::dynamic_pointer_cast<StringLiteralNode>(node)){
            return handleSLN(StrLit);
        } else if (auto AssignN = std::dynamic_pointer_cast<AssignNode>(node)){
            return handleAssign(AssignN);
        } else if (auto BN = std::dynamic_pointer_cast<BlockNode>(node)){
            return handleBlock(BN);
        } else if (auto EN = std::dynamic_pointer_cast<ExternNode>(node)){
            return handleExtern(EN);
        } else if (auto IN = std::dynamic_pointer_cast<IdentifierNode>(node)){
            return handleIdentifier(IN);
        } else if (auto PN = std::dynamic_pointer_cast<PointerNode>(node)){
            return handlePTR(PN);
        } else if (auto DN = std::dynamic_pointer_cast<DerefNode>(node)){
            return handlederef(DN);
        } else if (auto MPN = std::dynamic_pointer_cast<ModifyPtrNode>(node)){
            return handlePTRassign(MPN);
        } else if (auto OpNode = std::dynamic_pointer_cast<BinOP>(node)){
            return handleOps(OpNode);
        } else if (auto RN = std::dynamic_pointer_cast<RetNode>(node)){
            return handleRet(RN);
        } else if (auto PN = std::dynamic_pointer_cast<PubNode>(node)){
            return handlePub(PN);
        } else if (auto MAN = std::dynamic_pointer_cast<MemAccNode>(node)){
            return handleMember(MAN);
        } else if (auto SIN = std::dynamic_pointer_cast<StructInstanceNode>(node)){
            return handleStructI("",SIN);
        } else if (auto CN = std::dynamic_pointer_cast<CallNode>(node)){ // add type checking for CEXTFunction args

            return handleCall(CN);
        } else if (auto CN = std::dynamic_pointer_cast<CastNode>(node)){
            return handleCast(CN);
        } else {
            println("Unknown node");
            return "";
        }
    }*/





    void setscope(std::shared_ptr<Scope> it){
        this->CScope = it;

    }
    void setinte(std::shared_ptr<Interpreter> i){
        this->inte = i;
    }
    private:
    std::shared_ptr<Interpreter> inte;
    std::string  lastid;
    std::vector<std::string> fnargs;
    int bc = 0;
    std::shared_ptr<Scope> CScope = {};
    std::string line = "";
    std::string compiled = "";
};

Instruct It = Instruct(nullptr);

void setscope(std::shared_ptr<Scope> scope){
    It.setscope(scope);
}

int main(int argc, char* argv[]) {
    AP = ArgParser(argv,argc);
    AP.parse();
    if (argc < 2){
        std::cout << "Maven c(ompiler) [v Arcadia]\nhttps://github.com/Mavenista/Maven\n(Tip: run 'mavenc help' for help)" << std::endl;
        exit(0);
    }
    if (AP.has("version")){
        std::cout << "Maven c(ompiler): Anthology no. 2025b (Arcadia)" << std::endl;
        exit(0);
    }
    if (AP.has("-unbundle")){
        std::unordered_map<std::string,std::string> imports;
        std::vector<char> ObjContent;
        auto bundler = ObjectBundler();
        bundler.unbundle(argv[2],&imports,&ObjContent);
        for (auto& b : imports){
            std::cout << b.first << ":" << b.second << std::endl; 
        } 
        exit(0);
    }
    
    
    //std::cout << std::boolalpha;

    std::string inpfn(argv[1]);
    if (AP.has("compile")){
        inpfn = AP.values["compile"][0];
        println(green_block + circle + reset + " Beginning build.");
    }
    if (AP.has("typing")){
        ruleset["typing"] = AP.values["typing"][0];
    } 

    std::ifstream file(inpfn);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        file.close();
        return 1;
    }
    

    std::string input((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    std::string builtins_str = "import {} from Builtins;";
    if (input.find("#(no_builtins)") != std::string::npos){
        builtins_str = "";
    }
    input = "{" + builtins_str + input + "\n}";


    Lexer lexer(input);
    file.close();
    Parser parser(lexer);

    auto rootBlock = parser.parse();
    if (!rootBlock) {
        std::cerr << "Parsing error\n";
        return 1;
    }
    It.rootblock = rootBlock;
    

    std::shared_ptr<Interpreter> interpreter = std::make_shared<Interpreter>();
    It.setinte(interpreter);
    try {
    
    interpreter->interpret(rootBlock);
    } catch(std::runtime_error& e){
        savelog();
        throw e;
    }
    logat("Finished execution","m");
    savelog();
    if (AP.has("compile")){
    It.finish();
    }

    

    return 0;
}

