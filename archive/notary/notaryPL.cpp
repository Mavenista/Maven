#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <system_error>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <any>
#include <dlfcn.h>
#include <filesystem>


std::string replacer(std::string s, std::string from, std::string to)
{
    if(!from.empty())
        for(std::string::size_type pos = 0; (pos = s.find(from, pos) + 1); pos += to.size())
            s.replace(--pos, from.size(), to);
    return s;
}

bool in(std::string inp, std::vector<std::string> op){
    for (int i = 0; i < op.size() ; i ++){
    if (inp == op[i]){
        return true;
            //Do Stuff
    }}
    return false;
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
                    val = replacer(val,"'","\"");
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

    std::unordered_map<std::string,std::vector<std::string>> values;
    std::vector<std::string> sflags{"linklib","link","emit","linkpath","output","compilerflags","target"};
    char** argv;
    int argc;
    private:
};
char** chardummy = new char*[3];
auto AP = ArgParser(chardummy,9);

using namespace llvm;

class Var {
    public:
    AllocaInst* allocd;
    llvm::StoreInst* stored;
    
    Var(){}
    void setallocd(AllocaInst* a){
        allocd = a;
    }
    void setstore(llvm::StoreInst* s){
        stored = s;
    }
    private:
};


class lir {

    public:
    LLVMContext context;
    IRBuilder<> builder;
    
    

    lir(): builder(context) {
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
    types =  gettypings();
    }
    

    std::unordered_map<std::string,Function*> fns;
    std::unordered_map<std::string,BasicBlock*> blocks;
    std::unordered_map<std::string,Var> vars;
    std::string targetstr;
    std::unordered_map<std::string, llvm::Type*> gettypings(){
    std::unordered_map<std::string, llvm::Type*> typings;

    // Populate the map with basic integer types

    typings["i1"]  = builder.getInt1Ty();     // 1-bit unsigned integer
    typings["u8"]  = builder.getInt8Ty();     // 8-bit unsigned integer
    typings["i8"]  = builder.getInt8Ty();     // 8-bit signed integer
    typings["u16"] = builder.getInt16Ty();    // 16-bit unsigned integer
    typings["i16"] = builder.getInt16Ty();    // 16-bit signed integer
    typings["u32"] = builder.getInt32Ty();    // 32-bit unsigned integer
    typings["i32"] = builder.getInt32Ty();    // 32-bit signed integer
    typings["u64"] = builder.getInt64Ty();    // 64-bit unsigned integer
    typings["i64"] = builder.getInt64Ty();    // 64-bit signed integer

    // Floating-point types
    typings["f32"] = builder.getFloatTy();    // 32-bit floating point
    typings["f64"] = builder.getDoubleTy();   // 64-bit floating point

    // Pointer type
    typings["ptr"] = builder.getPtrTy();  // Generic pointer (i8*)

    // Size type (size_t)
    typings["size_t"] = builder.getInt64Ty(); // Assuming 64-bit system for size_t

    // Character type (ASCII)
    typings["Character"] = builder.getInt8Ty();    // Represents ASCII characters (i8)

    // String type
    typings["String"] = PointerType::get(Type::getInt8Ty(context),0); // String represented as i8*
    return typings;

 
    }
    void setTarget(std::string tar){
    
    std::string errorr;
    this->targetstr = tar;
    StringRef targetTripleRef(targetstr);
    
    const Target *target = TargetRegistry::lookupTarget(targetstr, errorr);
    if (!target) {
        errs() << "Error: Unable to find target: " << targetstr << "\n";
        exit(0);
    }
    this->tarobj = target;
    // Define target options
    TargetOptions options;

    // Create the target machine with the specified target triple
    std::string error;
    std::string reloc;
    if (AP.has("-relocstatic")){
        reloc = "static";
        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(target->createTargetMachine(tar, "generic", "", options, Reloc::Static));
    }
    else if (AP.has("-relocrwpi")){
        reloc = "rwpi";
        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(target->createTargetMachine(tar, "generic", "", options, Reloc::RWPI));
    }
    else if (AP.has("-relocropi")){
        reloc = "ropi";
        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(target->createTargetMachine(tar, "generic", "", options, Reloc::ROPI));
    }
    else if (AP.has("-relocropirwpi")){
        reloc = "ropi rwpi";
        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(target->createTargetMachine(tar, "generic", "", options, Reloc::ROPI_RWPI));
    }
    else if (AP.has("-relocdynnopic")){
        reloc = "dynamic no PIC (dynnopic)";
        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(target->createTargetMachine(tar, "generic", "", options, Reloc::DynamicNoPIC));
    } else {
    reloc = "PIC";
    this->targetMachine = std::unique_ptr<llvm::TargetMachine>(target->createTargetMachine(tar, "generic", "", options, Reloc::PIC_));
    }
    if (AP.has("-v")){
        std::cout << "Reloc: " << reloc << std::endl;
    }
    if (!targetMachine){
        std::cout << "Failed to attain target" << std::endl;
        exit(0);
    }

    // Create the LLVM module and context
    
    }
    Module* newModule(std::string name){
    if (!targetMachine) {
        std::cerr << "Error: Target machine is not initialized!" << std::endl;
        exit(0);
    }

    Module *module = new Module(name, context);
    if (!module) {
        std::cerr << "Error: Module creation failed!" << std::endl;
        exit(0);
    }
    module->setDataLayout(targetMachine->createDataLayout());
    module->setTargetTriple(targetstr);
    mods.push_back(module);
    return module;
    }
    
    
    void initmain(){
    // --- Assignments and Simple Types ---
    FunctionType *funcType = FunctionType::get(Type::getInt32Ty(context), false);
    Function *mainFunction = Function::Create(funcType, Function::ExternalLinkage, "main", mods[0]);
    fns["main"] = mainFunction;
    mainFunction->setDSOLocal(true);
    


    }
    void linkext(FunctionType* fnt ,std::string name,std::string ovsas = ""){
        Function *ExtFn = Function::Create(fnt, Function::ExternalLinkage, name, mods[0]);
        if (!ExtFn){
            std::cout << "Bad External Linking Fn" << std::endl;
            exit(0);
        }
        ExtFn->setDSOLocal(true);
        if (ovsas != ""){
            fns[ovsas] = ExtFn;
        } else {
            fns[name] = ExtFn;
        }
    }
    Value* getint(std::string ty,double amt){
        if (ty == "i1") {
            return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), static_cast<int>(amt), false); // signed 1-bit
        } 
        else if (ty == "u8") {
            return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<uint8_t>(amt), false); // Unsigned 8-bit
        } else if (ty == "i8") {
            return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<int8_t>(amt), true); // Signed 8-bit
        } else if (ty == "u16") {
            return llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), static_cast<uint16_t>(amt), false); // Unsigned 16-bit
        } else if (ty == "i16") {
            return llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), static_cast<int16_t>(amt), true); // Signed 16-bit
        } else if (ty == "u32") {
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), static_cast<uint32_t>(amt), false); // Unsigned 32-bit
        } else if (ty == "i32") {
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), static_cast<int32_t>(amt), true); // Signed 32-bit
        } else if (ty == "u64") {
            return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), static_cast<uint64_t>(amt), false); // Unsigned 64-bit
        } else if (ty == "i64") {
            return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), static_cast<int64_t>(amt), true); // Signed 64-bit
        } else if (ty == "size_t") {
            return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), static_cast<uint64_t>(amt), false); // Assume size_t is 64-bit
        } else if (ty == "f32") {
            return llvm::ConstantFP::get(llvm::Type::getFloatTy(context), static_cast<float>(amt)); // 32-bit float
        } else if (ty == "f64") {
            return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), static_cast<double>(amt)); // 64-bit double
        } else {
            // Handle unknown type: return nullptr or print an error
            return nullptr;
        }
        

    }
    void ctxcheck(){
        if (&context == nullptr) {
    std::cerr << "Error: LLVMContext is not properly initialized!" << std::endl;
}

if (&builder == nullptr) {
    std::cerr << "Error: IRBuilder is not properly initialized!" << std::endl;
}
    }

    Value* getstr(std::string ty,std::string content){
        if (ty == "Character"){
            llvm::Type *charType = llvm::Type::getInt8Ty(context);

            // Create a constant 'char' value (e.g., the ASCII value of 'A' is 65)
            llvm::Constant *charValue = llvm::ConstantInt::get(charType, std::stoi(content));
            return charValue;
        } else if (ty == "String"){
            StringRef CREF(content.c_str());
            Value* ret =  builder.CreateGlobalString(CREF);
            if (!ret){
                std::cout << "Bad str";
            }
            return ret;
        } else {
            return nullptr;
        }
    }

    Value* getptr(Value * v,std::string ov="",int len=0){
        if (ov == "nb"){
            //return PointerType::get(v->getType(),len);
        } else if (ov.substr(0,4) == "load"){
            return vars[ov.substr(5)].allocd;
        }
        return builder.CreateBitCast(v, PointerType::get(v->getType(),len));
    }

    Value* getop(std::string op, Value * lhs, Value * rhs){
        if (op == "+"){
            return builder.CreateAdd(lhs,rhs);
        } else if (op == "-"){
            return builder.CreateSub(lhs,rhs);
        } else if (op == "*"){
            return builder.CreateMul(lhs,rhs);
        } else if (op == "/"){
            return builder.CreateSDiv(lhs,rhs);
        } else if (op == "=="){
            return builder.CreateICmpEQ(lhs,rhs);
        } else if (op == "!="){
            return builder.CreateICmpNE(lhs,rhs);
        } else if (op == "&&"){
            return builder.CreateAnd(lhs,rhs);
        } else if (op == "||"){
            return builder.CreateOr(lhs,rhs);
        }
    }
    FunctionType* getfnt(Type* retty,std::vector<Type*> t,bool variadic=false){
        return FunctionType::get(retty,t,variadic);
    }
    std::string initfn(std::string name, FunctionType* fnt){
    Function *fn = Function::Create(fnt, Function::InternalLinkage, name, mods[0]);
    fn->setDSOLocal(true);
    fns[name] = fn;
    return name;
    }
    std::string initextfn(std::string name, FunctionType* fnt){
    Function *fn = Function::Create(fnt, Function::ExternalLinkage, name, mods[0]);
    fn->setDSOLocal(true);
    fns[name] = fn;
    return name;
    }
    void endfn(Value * v){
        builder.CreateRet(v);
    }
    std::string initblock(std::string name,std::string within,bool insert=true){
    BasicBlock *block1 = BasicBlock::Create(context, name, fns[within]);
    blocks[name] = block1;
    if (insert){

    builder.SetInsertPoint(block1);
    
    }
    return name;
    }
    void endblock(Value * v){
        builder.CreateRet(v);
    }
    void initif(Value* c,std::string t,std::string f){
        builder.CreateCondBr(c,blocks[t],blocks[f]);
    }
    Type* initStruct(std::string name,std::vector<Type*> pts){
        auto s = StructType::create(context,name);
        s->setBody(pts);
        structs[name] = s;
        if (AP.has("-deadcode") == false){
        AllocaInst *p = builder.CreateAlloca(s, nullptr, "structU" + name);
        
        }
        return s;


    }
    Value* initStructInstance(std::string name,std::string as,std::unordered_map<int,Value*> fv){
        auto v = builder.CreateAlloca(structs[name],nullptr,as);
        auto vc = Var();
        vc.setallocd(v);
        vars[as] = vc;
        for (int i =0; i != fv.size(); i++){
            Value* p = builder.CreateStructGEP(structs[name],v,i,"field" + std::to_string(i) + "of" + as);
            builder.CreateStore(fv[i],p);
        }
        return v;
    }
    void jumpto(std::string bstr){
        builder.CreateBr(blocks[bstr]);
    }
    void gotoblock(std::string bstr){
        builder.SetInsertPoint(blocks[bstr]);
    }

    ArrayType* initarray(Type* ty,int sz){
        ArrayType* at= ArrayType::get(ty,sz);
        
        return at;
    }

    llvm::AllocaInst* alloc(std::string name,std::string ty,Type* tyt,bool flag=false){
        llvm::AllocaInst *allocaInst;
        if (ty[0] == 'a'){
        allocaInst = builder.CreateAlloca(initarray(tyt,std::stoi(ty.substr(1))),nullptr,name);  
        } else {
        allocaInst = builder.CreateAlloca(tyt,nullptr,name);
        }
        vars[name] = Var();
        vars[name].setallocd(allocaInst);
        if (AP.has("-experimentAR")){
        if (!flag){
        alloc("flag" + name,"i1",gettypings()["i1"],true);
        store("flag" + name,getint("i1",0),"i1",gettypings()["i1"],0,true);
        }
        }

        return allocaInst;

    }
    llvm::StoreInst* store(std::string name,Value* v,std::string ty,Type* tyt=NULL,int pos=0,bool flag=false){
        if (vars.find(name) == vars.end()){
            std::cout << "Invalid Var" << std::endl;
        }
        llvm::StoreInst* SI;
        if (ty[0] == 'a'){
            SI = builder.CreateStore(v, builder.CreateGEP(initarray(tyt,std::stoi(ty.substr(1))),(vars[name].allocd), {getint("i32",0),getint("i32",pos)}));
        } else {
         SI = builder.CreateStore(v,vars[name].allocd);
        }
        vars[name].setstore(SI);
        return SI;
    }
    Var assign(std::string name,std::string ty,Type* tyt, Value* value,int arraypos=0,std::string arrayty="" ){
        if (vars.find(name) == vars.end()){
        
        alloc(name,ty,tyt);
        }
        
        store(name,value,ty,tyt,arraypos);

        
        return vars[name];
    }

    Value* load(std::string name, std::string ty, Type* TY,std::string twine=""){
        if (ty[0] == 'a'){
            return builder.CreateGEP(ArrayType::get(TY,std::stoi(ty.substr(1))),vars[name].allocd,{getint("i32",0),getint("i32",0)});
        } else if (twine != ""){
            return builder.CreateLoad(TY,vars[name].allocd,twine);
        }else {
            return builder.CreateLoad(TY,vars[name].allocd,name);
        }
    }

    Value* phiload(std::string name,std::string varto,std::string bname,std::string ty,Type* TY,int loadnum=1){
        PHINode *PhiNode = builder.CreatePHI(PointerType::get(TY,0), loadnum, name);
        //builder.CreateLoad(TY,vars[varto].allocd)
        PhiNode->addIncoming(vars[varto].allocd,blocks[bname]);
        alloc("phid" + name,ty,TY);
        

        return PhiNode;


    }

    Value* callfn(std::string name,std::vector<llvm::Value*> args){
        Function* fn = fns[name];
        if (!fn){
            std::cout << "Bad FN\nName: " << name << "\nFound: " << ((fns.find(name) != fns.end()) ? "true" : "false") <<std::endl;
            exit(0);
        }

        return builder.CreateCall(fn,args);
    }

    void endmain(){
        builder.CreateRet(builder.getInt32(0)); 
    }
    int build(std::string output){
    std::string errorMessage;
    if (!targetMachine || !mods[0]){
        std::cout << "Bad Build" << std::endl;
        exit(0);
    }
    
    std::error_code error;
    llvm::raw_fd_ostream file(output + ".ll", error, llvm::sys::fs::OF_None);
    mods[0]->print(file, nullptr);
    if (verifyModule(*mods[0], &errs())) {
        errs() << "Error in module verification!\n";
        exit(0);
    }

    
    std::error_code EC;
    llvm::raw_fd_ostream dest(output, EC,sys::fs::OF_None);
    llvm::legacy::PassManager pass;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        llvm::errs() << "Target machine can't emit a file of this type";
        return 1;
    }
    pass.run(*mods[0]);
    dest.flush();
    return 0;
    }
    private:
    
    const Target* tarobj;
    std::unordered_map<std::string, llvm::Type*> types;
    std::unordered_map<std::string,StructType*> structs;
    
    
    std::unique_ptr<TargetMachine> targetMachine;
    
    std::vector<Module*> mods;
};


#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
#include <sstream>

// Token types
enum class TokenType {
    LParen,      // (
    RParen,      // )
    Arrow,       // ->
    Comma,       // ,
    HashTag,     // #
    Ellipse,     // ...
    Dot,         // .
    String,      // "..."
    Identifier,  // [a-zA-Z_][a-zA-Z0-9_]*
    Int,
    EndOfFile,   // End of file
    Unknown      // For error handling
};

// Token structure
struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

// Lexer class
class Lexer {
public:
    Lexer(const std::string& input) : input(input), pos(0) {}

    // Function to tokenize the input
    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < input.size()) {
            char current = input[pos];
            //std::cout << "current: " << current << std::endl;

            if (isspace(current)) {
                ++pos;  // Skip whitespace
                
            } 
            else if (current == '\\' and peek() == 'n') {
                ++pos;  // Skip \n?
            } else if (current == '(') {
                tokens.emplace_back(TokenType::LParen, "(");
                ++pos;
            } else if (current == ')') {
                tokens.emplace_back(TokenType::RParen, ")");
                ++pos;
            } else if (current == ',') {
                tokens.emplace_back(TokenType::Comma, ",");
                ++pos;
            } else if (current == '#') {
                tokens.emplace_back(TokenType::HashTag, "#");
                ++pos;
            } else if (current == '.') {
                if (match("...")) {
                    tokens.emplace_back(TokenType::Ellipse, "...");
                } else {
                    tokens.emplace_back(TokenType::Dot, ".");
                    ++pos;
                }
            } else if (current == '-' && peek() == '>') {
                tokens.emplace_back(TokenType::Arrow, "->");
                pos += 2;
            } else if (current == '"') {
                tokens.emplace_back(TokenType::String, readString());
            } else if (isalpha(current) || current == '_' || current == '&') {
                tokens.emplace_back(TokenType::Identifier, readIdentifier());
            } else if ((current == '-' && isdigit(peek())) || isdigit(current)) {

                auto v = readInt();

                
                tokens.emplace_back(TokenType::Int, v);
            } else {
                std::cout << "Warning: Invalid Lexical Token: " << current << std::endl;
                //tokens.emplace_back(TokenType::Unknown, std::string(1, current));
                ++pos;
            }
        }
        tokens.emplace_back(TokenType::EndOfFile, "EOF");
        return tokens;
    }

private:
    std::string input;
    size_t pos;

    // Helper: Match specific string
    bool match(const std::string& str) {
        if (input.substr(pos, str.size()) == str) {
            pos += str.size();
            return true;
        }
        return false;
    }

    // Helper: Peek at next character
    char peek() {
        return (pos + 1 < input.size()) ? input[pos + 1] : '\0';
    }

    // Helper: Read string literals
    std::string readString() {
        ++pos; // Skip opening "
        std::ostringstream oss;
        while (pos < input.size() && input[pos] != '"') {
            oss << input[pos++];
        }
        ++pos; // Skip closing "
        return oss.str();
    }

    // Helper: Read identifiers
    std::string readIdentifier() {
        std::ostringstream oss;
        while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_' || input[pos] == '-' || input[pos] == '.') || input[pos] == '&') {
            oss << input[pos++];
        }
        return oss.str();
    }
    std::string readInt() {
        auto start = pos;
        int dots = 0;
        std::ostringstream oss;
        while (pos < input.size() && (isdigit((input[pos])) || (input[pos] == '-' && pos == start) || (input[pos] == '.' && dots == 0))) {
            if (input[pos] == '.'){
                dots += 1;
            }
            oss << input[pos++];
        }
        return oss.str();
    }
};

// Parser class
class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {
        
    }
    std::string parseinfo(){
        advance(); //info
        if (is("target")){
            advance();
            if (AP.has("target") == false){
            Lir.setTarget(consume().value);
            } else {
            Lir.setTarget(AP.values["target"][0]);
            }
        }
        else if (is("modname")){
            advance();
            Lir.newModule(consume().value);
            //Lir.initmain();
        }
        else if (is("output")){
            advance();
            out = consume().value;

        } 
        return "";
    }

    std::string parseinternal(){
        advance(); //internal
        if (is("build")){
            advance();
            if (AP.values.find("output") != AP.values.end()){
                out = AP.values["output"][0];
            }
            std::string structure_clang;
            if (AP.values.find("link") != AP.values.end()){
                for (const auto& k: AP.values["link"]){
                    auto p = k.find_last_of("/");
                    auto path = k.substr(0,p);
                    path = replacer(path,"\"","");
                    auto lib = k.substr(p+1);
                    lib = replacer(lib,".lib","");
                    lib = replacer(lib,"\"","");
                    structure_clang += " -L\"" + path + "\" -l" + lib;

                }
            }
            if (AP.values.find("linkpath") != AP.values.end()){
                for (const auto& k: AP.values["linkpath"]){
                    structure_clang += " -L" + k;
                }
            }
            if (AP.values.find("compilerflags") != AP.values.end()){
                for (const auto& k: AP.values["compilerflags"]){
                    structure_clang += " " + k;
                }
            }
            if (AP.values.find("linklib") != AP.values.end()){
                for (const auto& k: AP.values["linklib"]){
                    auto l = replacer(k,".lib","");
                    structure_clang += " -l" + l;
                }
            }
            Lir.build(out + ".o");
            if (AP.has("-v") || AP.has("--verbose")){
                std::cout << "clang " + out + ".o -o " + out + " " + structure_clang << std::endl;
            }
            system(("clang " + out + ".o -o " + out + " " + structure_clang).c_str());

        }
        if (is("clean")){
            if (AP.values.find("emit") != AP.values.end()){
                if (!in("object",AP.values["emit"])){
                    std::filesystem::remove(out + ".o");
                }
                if (!in("llvm-ir",AP.values["emit"])){
                    std::filesystem::remove(out + ".o.ll");
                }
            } else {
            std::filesystem::remove(out + ".o");
            std::filesystem::remove(out + ".o.ll");
            }
            std::cout << "Finished building" << std::endl;
            exit(0);
        }
        
        return "";
    }


    std::unordered_map<int,std::string> parseexp(){
        expect("(");
        std::unordered_map<int,std::string> args;
        int a = 0;
        while (currentToken().type != TokenType::RParen){
            std::string bstr = "";
            while (currentToken().type != TokenType::Comma && currentToken().type != TokenType::RParen){
                bstr += parsefactor();
            }
            if (currentToken().type != TokenType::RParen){
                expect(",");
            }
            args[a] = bstr;
            a += 1;

        }
        expect(")");
        return args;
    }
    std::string parsecall(){
        expect("call");
        auto fid = consume().value;
        auto expr = parseexp();
        std::vector<Value*> v{};
        for (int i = 0;i != expr.size(); i++){
            v.push_back(getvalue(expr[i]));
        }
        auto val = Lir.callfn(fid,v);
        auto sval = "retval" + std::to_string(valuects);
        valuects += 1;
        values[sval] = val;
        return sval;
    }

    std::string parseextlink(){
        //LIR.linkext(printfType,"printf");
        expect("extlink");
        
        if (is("fn")){
            advance();
            auto fid = consume().value;
            auto t = parsesig();
            Lir.linkext(getfnts(t),fid);
            return "";
        }

    }

    std::string parsesig(){
        advance(); // sig
        bool vdic = false;
        if (is("variadic")){
            advance();
            vdic = true;
        }
        expect("(");
        std::unordered_map<int,std::string> map;
        int a = 0;
        while (currentToken().type != TokenType::RParen){
            map[a] = consume().value;
            a += 1;
            if (currentToken().type != TokenType::RParen){
                expect(",");
            }
        }
        expect(")");
        expect("->");
        std::string retty = consume().value;
        std::vector<Type*> t;
        for (int i = 0; i != map.size(); i++){
            t.push_back(gettype(map[i]));
        }
        
        auto fnt = Lir.getfnt(gettype(retty),t,vdic);
        auto valstr = "sig" + std::to_string(fntcts);
        fnts[valstr] = fnt;
        fntcts += 1;
        return valstr;

    }

    std::string mangle(std::string v,std::string b="", std::string t= "var"){
        auto cfn = fnchain[fnchain.size()-1];
        if (AP.has("-nomangle")){
        if (manglechain.find(blockchain[blockchain.size()-1]) == manglechain.end()){
            std::unordered_map<std::string,std::string> insert;
            insert[v] = v;
            manglechain[blockchain[blockchain.size()-1]] = insert;
        } else {
            manglechain[blockchain[blockchain.size()-1]][v] = v;
        }
        return v;
        }

        if (b == ""){
        if (manglechain.find(blockchain[blockchain.size()-1]) == manglechain.end()){
            std::unordered_map<std::string,std::string> insert;
            insert[v] = cfn + blockchain[blockchain.size()-1] + t + v;
            manglechain[blockchain[blockchain.size()-1]] = insert;
        } else {
            manglechain[blockchain[blockchain.size()-1]][v] = cfn + blockchain[blockchain.size()-1] + t + v;
        }
        return blockchain[blockchain.size()-1] + t + v;
        } else {
            if (manglechain.find(blockchain[blockchain.size()-1]) == manglechain.end()){
                std::unordered_map<std::string,std::string> insert;
                insert[v] = cfn + b + t + v;
                manglechain[blockchain[blockchain.size()-1]] = insert;

            } else {
                manglechain[blockchain[blockchain.size()-1]][v] = cfn + b + t + v;
            }
            return b + t + v;
        }
    }

    void addtoillegals(std::string v){
        if (illegals.find(blockchain[blockchain.size()-1]) == illegals.end()){
            illegals[blockchain[blockchain.size()-1]] = {v};
        } else {
            illegals[blockchain[blockchain.size()-1]].push_back(v);
        }

    }

    void addtype(std::string t,Type* ty){
        typespush[t] = ty;
    }
    std::string lookup(std::string tolp,std::string ty="var"){
        if (in(tolp,illegals[blockchain[blockchain.size()-1]])){
            return mangle(tolp,"",ty);
        } else {
            auto b = inv(tolp);
            return mangle(tolp,b,ty);
        }

    }

    std::string parseassignable(){
        advance(); // assignable
        auto id = consume().value;
        expect("as");
        auto tid = consume().value;
        if (is("block")){
            advance(); // block
            auto bid = consume().value;
            auto b = Lir.initblock(bid,fnchain[fnchain.size()-1],false);
            Lir.jumpto(b);
            Lir.gotoblock(b);
            blockchain.push_back(b);
            while (!is("end")){
                parsefactor();
            }
            parsefactor(); // end block
            if (AP.has("-experimentAR")){
                auto cblock = blockchain[blockchain.size()-1];
                Lir.gotoblock("__allocreigon" + fnchain[fnchain.size()-1]);
                Lir.alloc(mangle(id,""),tid,gettype(tid));
                Lir.gotoblock(cblock);
                Lir.store(mangle(id,""),Lir.builder.CreateLoad(gettype(tid),Lir.vars[manglechain[b][id]].allocd,"laas" + cblock + manglechain[b][id]),tid,gettype(tid)); // fix vars and blocks dicts in Lir to be function specific
                return "";

            } else {
                Lir.assign(mangle(id,""),tid,gettype(tid),Lir.builder.CreateLoad(gettype(tid),Lir.vars[manglechain[b][id]].allocd,"laas" + blockchain[blockchain.size()-1] + manglechain[b][id]));
                return "";

            }
        }
    }

    std::string parsenew(){
        advance(); // new
        if (is("ptr")){
            expect("ptr");
            auto exp = parseexp();
            auto sptr = mangle("storedptr","","ptr") + std::to_string(valuects);
            valuects += 1;
            Value* v;
            if (AP.has("-experimentAR")){
            auto cblock = blockchain[blockchain.size()-1];
            Lir.gotoblock("__allocreigon" + fnchain[fnchain.size()-1]);
            v = Lir.alloc(sptr,exp[0],gettype(exp[0]));
            Lir.gotoblock(cblock);
            Lir.store(sptr,getvalue(exp[1]),exp[0],gettype(exp[0]));
            } else {
            v = Lir.alloc(sptr,exp[0],gettype(exp[0]));
            Lir.store(sptr,getvalue(exp[1]),exp[0],gettype(exp[0]));


            }
            values[sptr] = v;
            return sptr;

        }
        if (is("structInstance")){
            expect("structInstance");
            auto pid = consume().value; // add mangling support here
            pid = lookup(pid,"struct");
            expect("as");
            auto nomangleid = currentToken().value;
            auto id = mangle(consume().value);
            std::unordered_map<std::string,std::string> vals;
            structchain[id] = {};
            int fpos = 0;
            while (!is("endstructI")){
                expect("field");
                auto fid = consume().value;
                structchain[id][fid] = fpos;
                std::cout << id << " : " << fid << " : " << fpos << std::endl;
                fpos++;
                expect("as");
                vals[fid] = parsefactor();
            }
            
            std::unordered_map<int,Value*> fvs;
            auto ss = structs[pid];
            for (auto& k: ss){
                fvs[k.second] = getvalue(vals[k.first]);
            }
            auto val = Lir.initStructInstance(pid,id,fvs);
            
            illegals[blockchain[blockchain.size()-1]].push_back(nomangleid);
            values["structI" + std::to_string(valuects)] = val;
            auto svk = "structI" + std::to_string(valuects);
            valuects += 1;
            return svk;
        }
        if (is("struct")){
            expect("struct");
            auto id = consume().value;
            std::vector<Type*> fields;
            std::unordered_map<std::string,int> pos;
            int a = 0;
            while (!is("endstruct")){
                expect("field");
                auto fid = consume().value;
                expect("as");
                fields.push_back(gettype(consume().value));
                pos[fid] = a;
                a++;
            }
            expect("endstruct");
            illegals[blockchain[blockchain.size()-1]].push_back(id);
            auto sty = Lir.initStruct(mangle(id,"","struct"),fields);
            structs[mangle(id,"","struct")] = pos;
            addtype(mangle(id,"","struct"),sty);
            return "";

        }
        if (is("loop")){
            expect("loop");
            auto id = consume().value;
            auto sloop = mangle("loopertrooper" + std::to_string(blockcts),"","loop");
            blockcts += 1;
            auto slv = mangle("loopertrooper" + std::to_string(blockcts),"","loopvar");
            blockcts += 1;
            auto inte = Lir.initblock(sloop,fnchain[fnchain.size()-1],false);
            auto v = Lir.assign(slv,"i32",gettype("i32"),Lir.getint("i32",std::stod(id)));

            Lir.jumpto(inte);
            Lir.gotoblock(inte);
            blockchain.push_back(inte);
            while (!is("endloop")){
                parsefactor();
            }
            auto slv2 = mangle("loopertrooper" + std::to_string(blockcts),"","loopvar");
            blockcts += 1;
            Lir.assign(slv + std::to_string(blockcts),"i32",gettype("i32"),Lir.getop("-",Lir.load(slv,"i32",gettype("i32"),"loopvphidl"),Lir.getint("i32",1)));
            //Lir.assign(slv,"i32",gettype("i32"),Lir.load(slv + std::to_string(blockcts),"i32",gettype("i32"),"phidlupd8l"));
            Lir.store(slv,Lir.load(slv + std::to_string(blockcts),"i32",gettype("i32"),"phidlup"),"i32",gettype("i32"));
            blockcts += 1;
            auto v2 = Lir.assign(slv2,"i32",gettype("i32"),Lir.load(slv,"i32",gettype("i32"),"loopaphidl"));
            auto slv3 = mangle("loopermissionover" + std::to_string(blockcts),"","loopdone");
            blockcts += 1;
            auto m = Lir.initblock(slv3,fnchain[fnchain.size()-1],false);
            Lir.initif(Lir.getop("!=",Lir.load(slv2,"i32",gettype("i32"),"loopphidl"),Lir.getint("i32",static_cast<double>(0))),inte,m);
            expect("endloop");
            pop();
            expect("end");
            expect("block");
            Lir.gotoblock(m);
            blockchain.push_back(m);
            // end block
            return "";




            
        }
        if (is("while")){
            expect("while");
            expect("(");
            auto cond = parsefactor();
            expect(")");
            expect("new");
            expect("block");
            auto wid = consume().value + std::to_string(blockcts);
            blockcts += 1;
            auto whileb = Lir.initblock(mangle(wid,"","while"),fnchain[fnchain.size()-1],false);
            auto sinte = "intetowhile" + std::to_string(blockcts);
            blockcts += 1;
            auto inte = Lir.initblock(sinte,fnchain[fnchain.size()-1],false);
            auto sm = "merge" + std::to_string(blockcts);
            blockcts += 1;
            auto merge = Lir.initblock(mangle(sm,"","while"),fnchain[fnchain.size()-1],false);
            Lir.jumpto(inte);
            Lir.gotoblock(inte);
            blockchain.push_back(inte);
            Lir.initif(getvalue(cond),whileb,merge);
            pop();
            Lir.gotoblock(whileb);
            blockchain.push_back(whileb);
            while (!is("endwhile")){
                parsefactor();
            }
            expect("endwhile");
            expect("cond");
            expect("(");
            Lir.initif(getvalue(parsefactor()),whileb,merge);
            expect(")");
            expect("end");
            expect("block");
            pop();
            Lir.gotoblock(merge);
            blockchain.push_back(merge);
            return "";

        }
        if (is("if")){
            advance(); // if
            expect("(");
            auto ifcond = parsefactor();
            expect(")");
            std::unordered_map<std::string,std::string> elifs;
            while (!is("else")){
                auto id = consume().value; // elifx
                expect("(");
                auto econd = parsefactor();
                expect(")");
                elifs[id] = econd;
                

            }
            advance(); // else
            expect("if");
            expect("if");
            
            std::string f;
            expect("new");
            expect("block");
            auto fid = consume().value; // manually do block writes
            std::string merges = "merge" + std::to_string(blockcts);
            std::string merge = Lir.initblock(mangle(merges,"","if"),fnchain[fnchain.size()-1],false);
            blockcts += 1;
            std::string sinte = "intemediary" + std::to_string(blockcts);
            std::string inte = Lir.initblock(mangle(sinte,"","if"),fnchain[fnchain.size()-1],false);
            blockcts += 1;
            f = Lir.initblock(mangle(fid,"","if"),fnchain[fnchain.size()-1],false);
            Lir.jumpto(mangle(sinte,"","if"));
            Lir.gotoblock(f);
            blockchain.push_back(f);
            while (!is("endif")){
                parsefactor();
            }
            Lir.jumpto(merge);
            expect("endif");
            expect("end");
            expect("block");
            pop();
            
            
            auto cond1 = ifcond;
            auto block1 = f;
            std::string sinte2 = "intemediary" + std::to_string(blockcts);
            std::string inte2 = Lir.initblock(mangle(sinte2,"","if"),fnchain[fnchain.size()-1],false);
            blockcts += 1;
            


            
            
            int switcher = 0;
            std::string id;
            while (switcher != elifs.size()){
                
                if (switcher != elifs.size()){
                expect("if");
                id = consume().value;
                }
                Lir.gotoblock(inte);
                blockchain.push_back(inte);
                if (switcher != 0){

                    cond1 = elifs[id];
                }
                Lir.initif(getvalue(cond1),block1,inte2); // if <> goto if else goto inte2
                pop();
                switcher++;
                if (AP.has("-debugif")){std::cout << "finished initelif" << std::endl;}
                


                expect("new");
                expect("block");
                id = consume().value; // manually do block writes
                block1 = Lir.initblock(mangle(id,"","elif"),fnchain[fnchain.size()-1],false);
                Lir.gotoblock(block1);
                blockchain.push_back(block1);
                while (!is("endelif")){
                    parsefactor();
                }

                Lir.jumpto(merge);
                expect("endelif");
                expect("end");
                expect("block");
                pop();
                if (switcher != elifs.size()){
                auto cond1 =  elifs[id];
                inte = inte2;
                std::string sinte2 = "intemediary" + std::to_string(blockcts);
                std::string inte2 = Lir.initblock(mangle(sinte2,"","if"),fnchain[fnchain.size()-1],false);
                blockcts += 1;
                }
                
                
                
            }
            expect("if");
            expect("else");
            expect("new");
            expect("block");
            auto elseid = consume().value;
            std::string elseb = Lir.initblock(mangle(elseid,"","else"),fnchain[fnchain.size()-1],false);

            if (elifs.size() != 0){
            Lir.gotoblock(inte2);
            blockchain.push_back(inte2);
            Lir.initif(getvalue(elifs[id]),block1,elseb);
            } else {
            Lir.gotoblock(inte2);
            blockchain.push_back(inte2);
            Lir.jumpto(merge);
            pop();
            Lir.gotoblock(inte);
            blockchain.push_back(inte);
            Lir.initif(getvalue(cond1),block1,elseb);
            }
            if (AP.has("-debugif")){std::cout << "finished initelif2" << std::endl;}
            pop();
            Lir.gotoblock(elseb);
            blockchain.push_back(elseb);
            while(!is("endelse")){
                parsefactor();
            }
            expect("endelse");
            Lir.jumpto(merge);
            expect("end");
            expect("block");
            pop();
            Lir.gotoblock(merge);
            auto nxt = Lir.initblock(blockchain[blockchain.size()-1] + std::to_string(blockcts),fnchain[fnchain.size()-1],false);
            Lir.jumpto(nxt);
            Lir.gotoblock(nxt);
            blockchain.pop_back();
            blockchain.push_back(nxt);

            

            

            return "";


        }
        if (is("fn")){
            advance(); // fn
            bool ext = false;
            if (is("external")){
                advance();
                ext = true;
            }
            auto fnname = consume().value;
            if (fnname == "main"){
            Lir.linkext(getfnts(parsesig()),fnname,"main");
            fnchain.push_back("main");
            } else {
            if (!ext){
            Lir.initfn(fnname,getfnts(parsesig()));
            fnchain.push_back(fnname);
            } else {
            Lir.initextfn(fnname,getfnts(parsesig()));
            fnchain.push_back(fnname);
            }


            if (fnchain.size() > 1){
                fnbchain[fnchain[fnchain.size()-2]] = blockchain;
                fnmchain[fnchain[fnchain.size()-2]] = manglechain;
                blockchain = {};
                manglechain = {};
            }
            }
            
            if (AP.has("-experimentAR")){
            auto sar = "entry" + std::to_string(blockcts);
            blockcts += 1;
            Lir.initblock(sar,fnchain[fnchain.size()-1],true);
            Lir.initblock("__allocreigon" + fnchain[fnchain.size()-1],fnchain[fnchain.size()-1],false);
            Lir.jumpto("__allocreigon" + fnchain[fnchain.size()-1]);
            
            
            } 
            return "";
            
        }
        if (is("block")){

            advance();
            bool init = true;
            if (is("noinit")){
                advance(); // noinit
                init = false;
            }
            auto bname = consume().value;
            if (blockchain.size() == 0 && fblockfn.find(fnchain[fnchain.size()-1]) == fblockfn.end()){
                fblockfn[fnchain[fnchain.size()-1]] = bname;
            }            
            if (blockchain.size() != 0){
            auto bna = Lir.initblock(bname,fnchain[fnchain.size()-1],false);
            
            if (init){
            Lir.jumpto(bname);
            
            }
            Lir.gotoblock(bname);
            blockchain.push_back(bname);
            return bna;
            } else {
            auto bna = Lir.initblock(bname,fnchain[fnchain.size()-1],init);
            blockchain.push_back(bname);
            return bna;
            }
            return "";

        }
        if (is("op")){
            advance(); // op
            auto exp = parseexp();
            auto v = Lir.getop(exp[0],getvalue(exp[1]),getvalue(exp[2]));
            auto valstr = "op" + std::to_string(valuects);
            values[valstr] = v;
            valuects += 1;
            
            return valstr;

        }
        auto attrs = parseexp();
        std::string valstr = attrs[0] + std::to_string(valuects);
        valuects += 1;
        if (attrs[0] == "String" || attrs[0] == "Character"){
        Value* v;
        if (values.find(attrs[1]) != values.end()){
            v = Lir.getptr(getvalue(attrs[1]));
        } else {
        v = Lir.getstr(attrs[0],attrs[1]);
        }
        values[valstr] = v;
        } else if (attrs[0][0] == '&'){
            std::cout << attrs[1] << std::endl;
            auto v = Lir.getptr(getvalue(attrs[1]));
            values[valstr] = v;

        } else {
        
        values[valstr] = Lir.getint(attrs[0],std::stod(attrs[1]));
        }

        return valstr;


        
    }
    std::string parseString(){
        auto str = currentToken().value;
        str = replacer(str,"\\n","\n");
        advance();
        return str;
    }
    std::string parseReturn(){
        advance();
        Lir.endblock(getvalue(parsefactor()));
        return "";
    }
    std::string parseInt(){
        auto inte = currentToken().value;
        advance();
        return inte;
    }
    std::string parseIdent(){
        auto ident = currentToken().value;
        advance();
        return ident;
    }
    std::string parseassign(){
        advance(); // assign
        auto nmvarname = consume().value;
        auto varname = mangle(nmvarname);
        expect("as");
        //if (AP.has("-experimentAR")){
        //Lir.gotoblock("__allocreigon" + fnchain[fnchain.size()-1]);
        //}
        auto expr = parseexp();
        if (AP.has("-experimentAR")){
        
            auto cblock = blockchain[blockchain.size()-1];
            

            Lir.gotoblock("__allocreigon" + fnchain[fnchain.size()-1]);
            if (Lir.vars.find(varname) == Lir.vars.end()){
            Lir.alloc(varname,expr[0],gettype(expr[0]));
            }
            
            Lir.gotoblock(cblock);
            Lir.assign(varname,expr[0],gettype(expr[0]),getvalue(expr[1]));
            if (AP.has("-experimentAR")){
            Lir.store("flag" + varname,Lir.getint("i1",1),"i1",gettype("i1"),0,true);
            }
            
            
        
        } else {
        Lir.assign(varname,expr[0],gettype(expr[0]),getvalue(expr[1]));
        }
        addtoillegals(nmvarname);
        return "";
    };
    Type* gettype(std::string t){
        std::vector<std::string> x{"struct"};
        if (t[0] == '&'){
            std::cout << "Pointertyped: " << t.substr(1) << std::endl;
            return PointerType::get(gettype(t.substr(1)),0);
        } else {
            if (Lir.gettypings().find(t) == Lir.gettypings().end()){
                if (typespush.find(t) == typespush.end()){
                    for (const auto& k: x){
                        if (typespush.find(lookup(t,k)) != typespush.end()){
                            return typespush[lookup(t,k)];
                        }
                    }
                }
                return typespush[t];
            }
            return Lir.gettypings()[t];
        }
    }
    std::string inv(std::string inp){
        for (const auto & k : illegals){
            if (in(inp,k.second)){
                return k.first;
            }
        }
        return "";
    }
    std::string parseload(){
        advance(); // load
        bool half = false;
        if (is("half")){
            advance();
            half = true;
        }
        auto x = parseexp();
        auto vn = x[1];
        bool copy = false;
        if (is("copy")){ advance();copy = true;}; // do copy now
        if (in(vn,illegals[blockchain[blockchain.size()-1]])){
            vn = mangle(vn);
            Value* v;
            if (half){
                
                v = Lir.vars[lookup(x[1])].allocd;
            } else {
                v = Lir.load(vn,x[0],gettype(x[0]),"l" + vn);
            }
            auto valstr = x[0] + std::to_string(valuects);
            valuects += 1;
            values[valstr] = v;
            return valstr;
        } else {
            auto bn = inv(vn);
            auto v = mangle(vn,bn);
            Value* load;
            
            Var inblock;
            LoadInst* attemptload;
            LoadInst* flagload;
            if (AP.has("-experimentAR")){
            auto cblock = blockchain[blockchain.size()-1];
            Lir.gotoblock("__allocreigon" + fnchain[fnchain.size()-1]);
            load = Lir.load(v,x[0],gettype(x[0]),"phidl" + mangle(vn));
            inblock = Lir.assign(mangle(vn),x[0],gettype(x[0]),load);
            attemptload = Lir.builder.CreateLoad(gettype(x[0]),inblock.allocd,"l" + mangle(vn));
            Lir.gotoblock(cblock);
            flagload = Lir.builder.CreateLoad(gettype(x[0]),Lir.vars["flag" + mangle(vn)].allocd,"lflag" + mangle(vn));
            Value *isAllocated = Lir.builder.CreateICmpEQ(flagload, ConstantInt::get(Type::getInt32Ty(Lir.context), 0));
            auto selectv = Lir.builder.CreateSelect(isAllocated, Lir.vars[v].allocd, inblock.allocd);
            Value* castedv;
            if (half){
                castedv = selectv;
            } else {
                castedv = Lir.builder.CreateLoad(gettype(x[0]),selectv,"lcasted" + mangle(vn));
            }

            auto valstr = x[0] + std::to_string(valuects);
            valuects += 1;
            values[valstr] = castedv;
            return valstr;
            } else {
            load = Lir.load(v,x[0],gettype(x[0]),"phidl" + mangle(vn));
            inblock = Lir.assign(mangle(vn),x[0],gettype(x[0]),load);
            attemptload = Lir.builder.CreateLoad(gettype(x[0]),inblock.allocd,"l" + mangle(vn));

            auto valstr = x[0] + std::to_string(valuects);
            valuects += 1;
            values[valstr] = attemptload;
            return valstr;           
            }



        }
        

    }

    std::string parsefactor(){
        //std::cout << currentToken().value << "<- " <<  std::endl;

        if (is("new")){
            if (AP.has("-log")){std::cout << "parsing new: " << tokens[pos+1].value <<  tokens[pos+2].value << std::endl;}
            return parsenew();
        } else if (is("info")){
            if (AP.has("-log")){std::cout << "parsing info: " << tokens[pos+1].value << std::endl;}
            return parseinfo();
        } else if (is("end")){
            if (AP.has("-log")){std::cout << "parsing end: " << tokens[pos+1].value << std::endl;}
            return parseend();
        } else if (is("return")){
            if (AP.has("-log")){std::cout << "parsing return: " << tokens[pos+1].value << std::endl;}
            return parseReturn();
        } else if (is("internal")){
            if (AP.has("-log")){std::cout << "parsing internal: " << tokens[pos+1].value << std::endl;}
            return parseinternal();
        } else if (is("assign")){
            if (AP.has("-log")){std::cout << "parsing assign: " << tokens[pos+1].value << std::endl;}
            return parseassign();
        } else if (is("load")){
            if (AP.has("-log")){std::cout << "parsing load: " << tokens[pos+1].value << std::endl;}
            return parseload();
        } else if (is("call")){
            if (AP.has("-log")){std::cout << "parsing call: " << tokens[pos+1].value << std::endl;}
            return parsecall();
        } else if (is("argload")){
            if (AP.has("-log")){std::cout << "parsing argload: " << tokens[pos+1].value << std::endl;}
            return parseargload();
        } else if (is("extlink")){
            if (AP.has("-log")){std::cout << "parsing extlink: " << tokens[pos+1].value << std::endl;}
            return parseextlink();
        } else if (is("access")){
            if (AP.has("-log")){std::cout << "parsing access: " << tokens[pos+1].value << std::endl;}
            return parseaccess();
        } else if (is("bitcast")){
            if (AP.has("-log")){std::cout << "parsing bitcast: " << tokens[pos+1].value << std::endl;}
            return parsebitcast();
        } else if (is("assignable")){
            if (AP.has("-log")){std::cout << "parsing assignable: " << tokens[pos+1].value << std::endl;}
            return parseassignable();
        } else if (currentToken().type == TokenType::String){
            return parseString();
        } else if (currentToken().type == TokenType::Int){
            return parseInt();
        } else if (currentToken().type == TokenType::Identifier){
            return parseIdent();
        } else {
            std::cout << "Bad token: " + tokenTypeToString(currentToken().type);
            exit(0);
            return "";
        }
    }

    std::string parsebitcast(){
        expect("bitcast");
        auto expr = parseexp();
        auto sbc = "lbitcast" + mangle(expr[0],"","bt") + std::to_string(valuects);
        valuects += 1;
        Value* bc = Lir.builder.CreateBitCast(getvalue(expr[1]),gettype(expr[0]),sbc);
        values[sbc] = bc;
        return sbc;
    }

    std::string parseaccess(){
        expect("access");
        if (is("ptrindex")){
            advance();
            
            auto exp = parseexp();
            auto ptr = getvalue(exp[1]);
            auto index = getvalue(exp[2]);
            Value* PointerPlusOffset = Lir.builder.CreateInBoundsGEP(gettype(exp[0]),ptr, index,mangle(exp[1],"","ptroffset"));
            Value* loadedvalue = Lir.builder.CreateLoad(gettype(exp[0]),PointerPlusOffset,"lap" + mangle(exp[1],"","ptroffset"));
            auto svalue = "lptroffset" + std::to_string(valuects);
            valuects+=1;
            values[svalue] = loadedvalue;
            return svalue;
        }
        if (is("structfield")){
            advance();
            auto fid = consume().value;
            expect("as");
            auto ftype = consume().value;
            auto e = parseexp();
            auto sf = "sfload" + mangle(fid + e[1]) + std::to_string(valuects);
            valuects += 1;
            std::cout << "e: "<<  e[1] << std::endl;
            auto gep = Lir.builder.CreateStructGEP(gettype(e[0]),getvalue(e[1]),structs[lookup(e[0],"struct")][fid],sf);
            if (typespush.find(lookup(ftype)) == typespush.end() && typespush.find(lookup(ftype,"struct")) == typespush.end() && ftype != "struct"){
            auto loadv = Lir.builder.CreateLoad(gettype(ftype),gep,"lsf" + sf);
            values[sf] = loadv;
            } else {
                auto loadv2 = Lir.builder.CreateLoad(PointerType::get(gettype(ftype),0),gep,"Plsf" + sf);
                values[sf] = loadv2;
            }

            
            return sf;

        }
    }

    void parse() {
        std::cout << "Parsing tokens...\n";
        while (currentToken().type != TokenType::EndOfFile){
            parsefactor();
        }
        std::cout << "Parsing complete.\n";
    }
    Value* getvalue(std::string v ){
        if (values.find(v) == values.end()){
            std::cout << "bad getvalue: " << v;
            exit(0);
        }
        return values[v];
    }
    FunctionType* getfnts(std::string v ){
        if (fnts.find(v) == fnts.end()){
            std::cout << "bad getvalue";
            exit(0);
        }
        return fnts[v];
    }
    void pop(){
        
        std::string bn = blockchain[blockchain.size()-1];
        illegals.erase(bn);
        blockchain.pop_back();
    }
    std::string parseargload(){
        expect("argload");
        auto index = consume().value;
        std::cout << fnchain[fnchain.size()-1] << std::endl;
        if (index == "0"){
            Argument* args = Lir.fns[fnchain[fnchain.size()-1]]->arg_begin();
            auto si = "arg" + index + std::to_string(valuects);
            valuects += 1;
            //iargs->getType()->dump();
            values[si] = args;
            return si;
        } else {
            Value* args = std::next(Lir.fns[fnchain[fnchain.size()-1]]->arg_begin(),std::stoi(index));
            auto si = "arg" + index + std::to_string(valuects);
            valuects += 1;
            values[si] = args;
            return si;
        }
    }
    std::string parseend(){
        advance(); // end
        if (is("block")){
            if (blockchain.size() > 1){
                advance(); // block
                auto pbname = blockchain[blockchain.size()-2];
                auto nbname = pbname + std::to_string(blockcts);
                blockcts += 1;
                Lir.initblock(nbname,fnchain[fnchain.size()-1],false);
                Lir.jumpto(nbname);
                pop();
                blockchain.pop_back(); // needed still
                Lir.gotoblock(nbname);
                blockchain.push_back(nbname);
            } else {
            pop();
            
            }
            return "";
        } else if (is("fn")){
            if (AP.has("-experimentAR")){
            Lir.gotoblock("__allocreigon" + fnchain[fnchain.size()-1]);

            Lir.jumpto(fblockfn[fnchain[fnchain.size()-1]]);
            }
            if (fnchain.size() > 1){
                blockchain = fnbchain[fnchain[fnchain.size()-2]];
                manglechain = fnmchain[fnchain[fnchain.size()-2]];
                fnchain.pop_back();
                Lir.gotoblock(blockchain[blockchain.size()-1]);
                
            } else {

            fnchain.pop_back();
            }

            return "";
        }
        return "";
    }

private:
    std::unordered_map<std::string,std::string> fblockfn;
    std::unordered_map<std::string,std::unordered_map<std::string,int>> structs;
    std::unordered_map<std::string,std::string> types;
    std::unordered_map<std::string,std::string> futures;
    std::string cfn;
    std::vector<Token> tokens;
    std::string out;
    int valuects = 0;
    int fntcts = 0;
    int blockcts = 0;
    lir Lir = lir();
    std::unordered_map<std::string,Value*> values;
    std::unordered_map<std::string,FunctionType*> fnts;
    std::unordered_map<std::string, std::vector<std::string>> fnbchain;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string,std::string>>> fnmchain;
    std::unordered_map<std::string, std::unordered_map<std::string,std::string>> manglechain;
    
    std::vector<std::string> blockchain;
    std::unordered_map<std::string, std::unordered_map<std::string,int>> structchain;
    std::unordered_map<std::string,Type*> typespush;
    std::unordered_map<std::string,std::vector<std::string>> illegals;
    std::vector<std::string> fnchain;
    size_t pos;

    const Token currentToken() const {
        return tokens[pos];
    }

    Token consume(){
        auto c = currentToken();
        advance();
        return c;
    }

    bool is(std::string v){
        if (currentToken().value == v){
            return true;
        }
        return false;

    }

    Token expect(std::string exp){
        auto c = currentToken();
        if (c.value != exp){
            std::cout << "BadToken: " + exp + "\nG: " + c.value;
            exit(0);
        } else {
            advance();
            return c;
        }

    }

    void advance() {
        if (pos < tokens.size()) ++pos;
    }

    std::string tokenToString(const Token token) const {
        return "Type: " + tokenTypeToString(token.type) + ", Value: \"" + token.value + "\"";
    }

    std::string tokenTypeToString(TokenType type) const {
        switch (type) {
            case TokenType::LParen: return "LParen";
            case TokenType::RParen: return "RParen";
            case TokenType::Arrow: return "Arrow";
            case TokenType::Comma: return "Comma";
            case TokenType::HashTag: return "HashTag";
            case TokenType::Ellipse: return "Ellipse";
            case TokenType::Dot: return "Dot";
            case TokenType::String: return "String";
            case TokenType::Identifier: return "Identifier";
            case TokenType::EndOfFile: return "EndOfFile";
            default: return "Unknown";
        }
    }
};

// Main function
int load(std::string filen) {


    // Read file
    std::ifstream file(filen);
    if (!file) {
        std::cerr << "Error: Could not open file " << filen << "\n";
        return 1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();

    // Lexical analysis
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();

    // Parsing
    Parser parser(tokens);
    parser.parse();

    return 0;
}


int main(int argc, char **argv) {
    AP = ArgParser(argv,argc);
    AP.parse();
    if (std::string(argv[1]).substr(std::string(argv[1]).size()-5) == ".note"){
        load(argv[1]);
        exit(0);
        return 0;
    }


    // Initialize the LLVM targets, which are required to handle different platforms
    std::cout << "Arguments: " << std::to_string(argc) << std::endl;
    std::string targetTriple;
    if (argc  != 1){
    targetTriple = argv[1];
    } else {
        targetTriple = llvm::sys::getDefaultTargetTriple();
    }
    std::cout << "Target: " << targetTriple << std::endl;
    /*


    auto LIR = lir();
    LIR.setTarget(targetTriple);
    Module* m = LIR.newModule("entrypt");
    FunctionType *printfType = FunctionType::get(IntegerType::getInt32Ty(LIR.context), {LIR.getptr(LIR.getint("i8",0))->getType()}, true);
    LIR.linkext(printfType,"printf");
    FunctionType* strlenType = llvm::FunctionType::get(llvm::Type::getInt32Ty(LIR.context), {LIR.getptr(LIR.getint("i8",0))->getType()}, false);
    LIR.linkext(strlenType,"strlen");
    FunctionType* strcpyTy = llvm::FunctionType::get(LIR.getptr(LIR.getint("i8",0))->getType(), {LIR.getptr(LIR.getint("i8",0))->getType(),LIR.getptr(LIR.getint("i8",0))->getType()}, false);
    LIR.linkext(strcpyTy,"strcpy");
    FunctionType* strcatTy = llvm::FunctionType::get(LIR.getptr(LIR.getint("i8",0))->getType(), {LIR.getptr(LIR.getint("i8",0))->getType(),LIR.getptr(LIR.getint("i8",0))->getType()}, false);
    LIR.linkext(strcatTy,"strcat");
    FunctionType* tostrTy = llvm::FunctionType::get(LIR.getint("i32",0)->getType(), {LIR.getptr(LIR.getint("i8",0))->getType(),LIR.getptr(LIR.getint("i8",0))->getType()}, true);
    LIR.linkext(tostrTy,"sprintf");
    
    
    

    
    
    
    LIR.initfn("println",FunctionType::get(IntegerType::getInt32Ty(LIR.context), {LIR.getptr(LIR.getint("i8",0))->getType()}, true));
    auto my_str = LIR.fns["println"]->getArg(0);
    
    LIR.assign("res","a100",LIR.getstr("Character","102"),0,"Character");

    
    
    

    LIR.callfn("strcpy",{LIR.load("res","a100",LIR.gettypings()["u8"]),my_str}); // gettype fn
    
    LIR.callfn("strcat",{LIR.load("res","a100",LIR.gettypings()["u8"]),LIR.getstr("String","\n")});
    
    LIR.callfn("printf",{LIR.load("res","a100",LIR.gettypings()["u8"])});
    
    //auto alloc = LIR.alloc("u8",LIR.getop("+",LIR.callfn("strlen",{my_str}),LIR.callfn("strlen",{LIR.getstr("String","\n")})));
    //LIR.builder.CreateMemCpy(alloc, my_str->, LIR.callfn("strlen",{my_str}));

    // Copy the second string into the allocated space
    //LIR.builder.CreateMemCpy(builder.CreateGEP(alloc, len1), str2, len2);
    
    LIR.endfn(LIR.getint("i32",1));
    LIR.initmain();
    

    


    LIR.assign("my_i8","i8",LIR.getint("i8",static_cast<double>(1)));



    auto clr = LIR.callfn("printf",{LIR.getptr(LIR.getstr("String","Target: " + targetTriple + "\n"))});
    LIR.callfn("println",{LIR.getptr(LIR.getstr("String","Hello World and"))});
    LIR.assign("pln","i32",LIR.callfn("println",{LIR.getptr(LIR.getstr("String","Hello LLVM"))}));
    LIR.assign("arr","a100",LIR.getstr("Character","102"),0,"Character");
    LIR.callfn("sprintf",{LIR.load("arr","a100",LIR.gettypings()["u8"]),LIR.getptr(LIR.getstr("String","%d")),LIR.load("pln","i32",LIR.gettypings()["i32"])});
    LIR.callfn("println",{LIR.load("arr","a100",LIR.gettypings()["u8"])});
    LIR.endmain();
    LIR.initblock("b1","main");
    LIR.assign("smth","i32",LIR.getint("f64",3.14159));
    LIR.endblock(LIR.getint("i32",1));
    //LIR.gotoblock("entry");

    

    
    LIR.build(argv[2]);
    
    



    // Integer and assignment
    //Value *intValue = builder.getInt32(42); // constant integer
    //Value *sum = builder.CreateAdd(intValue, builder.getInt32(10), "sum"); // Add two integers
    //builder.CreateStore(sum, builder.CreateAlloca(Type::getInt32Ty(context), nullptr, "result"));
    */

    // --- Strings ---
// Create a global string to hold the format string
/*
Constant *str = ConstantDataArray::getString(context, "Hello, LLVM!\n", true);
GlobalVariable *globalStr = new GlobalVariable(
    *module, str->getType(), true, GlobalValue::PrivateLinkage, str, "formatString"
);

// Cast the pointer to the appropriate type for printf
Value *strArg = builder.CreatePointerCast(globalStr, PointerType::getUnqual(Type::getInt8Ty(context)));

// Call printf with the format string
builder.CreateCall(PrintfFunc, {strArg});

    // --- Operations ---
    // Multiply two integers
    Value *multResult = builder.CreateMul(builder.getInt32(10), builder.getInt32(5), "mult_result");

    // --- Blocks ---
    BasicBlock *block1 = BasicBlock::Create(context, "block1", mainFunction);
    builder.CreateBr(block1);
    builder.SetInsertPoint(block1);

    Value *blockSum = builder.CreateAdd(builder.getInt32(15), builder.getInt32(10), "block_sum");
    builder.CreateStore(blockSum, builder.CreateAlloca(Type::getInt32Ty(context), nullptr, "block_result"));
*/
    



    // --- Output the generated IR ---

    // --- Verify the module ---



    return 0;
}
