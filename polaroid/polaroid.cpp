#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include <llvm/IR/InlineAsm.h>
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <llvm/IR/Metadata.h>
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
#include <cxxabi.h>
#include "llvm/IR/DerivedTypes.h"
#include <sstream>
#include <iostream>
#include <ordered_map.h>
int tmp = 0;
uint64_t StackSize = 0;
std::string gettmp(){
    auto sv = "tmp" + std::to_string(tmp);
    tmp += 1;
    return sv;
}
std::string debugreplace(std::string s, std::string from, std::string to)
{
    if(!from.empty())
        for(std::string::size_type pos = 0; (pos = s.find(from, pos) + 1); pos += to.size())
            s.replace(--pos, from.size(), to);
    return s;
}
bool debugin(std::string lookfor, std::vector<std::string> pool){
    for (auto& value : pool){
        if (value == lookfor){
            return true;
        }
    }
    return false;
}


using RegisterMap = std::unordered_map<std::string, llvm::Value*>;

// This wrapper creates an inline asm call from a provided assembly template string
// and a vector defining the register order (i.e. the positions for placeholders).
// The three maps tell which registers are pure inputs, outputs, or inout.
tsl::ordered_map<std::string, llvm::Value*>
emitInlineAsm(llvm::IRBuilder<> &builder,
              llvm::Module *module,
              std::string asmTemplate,           // e.g., "mov {0}, {1}"
              const std::vector<std::string> &regOrder,   // order of registers (e.g., {"x", "y"})
              const RegisterMap &inMap,
              const RegisterMap &outMap,
              const RegisterMap &inoutMap) {
    // Build the constraint string and prepare the operand list.
    // For each register (by position) we choose:
    //  - "r"   for a pure input (and add its LLVM value)
    //  - "=r"  for an output (no input value is provided)
    //  - "+r"  for an inout (input value provided, and later updated)
    std::string constraintStr;
    std::vector<std::string> outStr;
    std::vector<std::string> inStr;
    std::vector<std::string> inoutStr;
    std::vector<llvm::Value*> inputOperands;
    tsl::ordered_map<int,std::string> regs;
    // Keep track of which operand indices will be used as outputs.
    std::unordered_map<unsigned, std::string> outputMapping;
    bool neverinOUT = true;
    for (size_t i = 0; i < regOrder.size(); ++i) {
        const std::string &regName = regOrder[i];
        //std::string selstr = neverinOUT  && inoutMap.find(regName) != inoutMap.end() ? "" : ",";
        bool isIn    = inMap.find(regName)    != inMap.end();
        bool isOut   = outMap.find(regName)   != outMap.end();
        bool isInOut = inoutMap.find(regName) != inoutMap.end();
        if (isInOut){
            neverinOUT = false;
        }

        if (isIn && !isInOut && !isOut) {
            // Pure input operand.
            inStr.push_back("r");
            inputOperands.push_back(inMap.at(regName));
        }
        else if (isOut && !isIn && !isInOut) {
            // Pure output operand.
            outStr.push_back("=r");
            // Do not add an input value; mark this index for extraction later.
            outputMapping[i] = regName;
        }
        else if (isInOut || (isIn && isOut)) {
            // Inout operand.
            inoutStr.push_back("+r");
            // Provide the input value (from inoutMap or from inMap).
            llvm::Value* val = isInOut ? inoutMap.at(regName) : inMap.at(regName);
            inputOperands.push_back(val);
            // Mark for output extraction as well.
            outputMapping[i] = regName;
        }
        else {
            // If not explicitly provided, default to input.
            inStr.push_back("r");
            // In a robust implementation you might signal an error here.
            // For now, we assume an input exists.
            assert(false && "Operand not provided in any map!");
        }
        //if (i < regOrder.size() - 1)
            //constraintStr += ",";
        regs[i] = regName;
    }

    // Determine the LLVM function type for the inline asm.
    // The arguments correspond to the values provided (i.e. inputs and inouts).
    std::vector<llvm::Type*> paramTypes;
    for (llvm::Value* val : inputOperands)
        paramTypes.push_back(val->getType());

    // Decide on the return type.
    // For a single output, we assume its type.
    // For multiple outputs, we pack them into an LLVM struct type.
    llvm::Type *retType = nullptr;
    unsigned numOutputs = outputMapping.size();
    if (numOutputs == 0) {
        retType = llvm::Type::getVoidTy(builder.getContext());
    }
    else if (numOutputs == 1) {
        // For a pure output operand, assume a default type (say, i64).
        // For an inout, we use its type.
        auto opIt = outputMapping.begin();
        std::string regName = opIt->second;
        if (inoutMap.find(regName) != inoutMap.end())
            retType = inoutMap.at(regName)->getType();
        else
            retType = llvm::Type::getInt32Ty(builder.getContext());
    }
    else {
        // Multiple outputs: create a struct type with one field per output.
        std::vector<llvm::Type*> outTypes;
        for (size_t i = 0; i < regOrder.size(); ++i) {
            const std::string &regName = regOrder[i];
            if ( (outMap.find(regName) != outMap.end()) ||
                 (inoutMap.find(regName) != inoutMap.end()) ) {
                if (inoutMap.find(regName) != inoutMap.end())
                    outTypes.push_back(inoutMap.at(regName)->getType());
                else
                    outTypes.push_back(llvm::Type::getInt32Ty(builder.getContext()));
            }
        }
        retType = llvm::StructType::get(builder.getContext(), outTypes);
    }

    llvm::FunctionType *asmFTy = llvm::FunctionType::get(retType, paramTypes, false);

    // NOTE: LLVM’s inline assembly expects placeholders as $0, $1, ….
    // If your template uses {0}, {1} etc., you should convert them before this call.
    // For brevity we assume asmTemplate is already formatted appropriately.
    bool first = false;
    for (auto& v : inoutStr){
        constraintStr += (first ? "," : "") + std::string("=r,r");
        first = true;
    }
    for (auto& v : outStr){
        constraintStr += (first ? "," : "") + v;
        first = true;
    }
    for (auto& v : inStr){
        constraintStr += (first ? "," : "") + v;
        first = true;
    }
    std::unordered_map<int,int> inputregs;
    auto inputspre = 0;
    for (int i = 0; i != regOrder.size();i++){
        if (inMap.find(regOrder[i]) != inMap.end()){
        //std::cout << "putting " << regOrder[i] << " as " << i << " -> " << (inoutStr.size()) + (outStr.size())  + inputspre << std::endl;
        inputregs[i] = (inoutStr.size()) + (outStr.size())  + inputspre;
        inputspre += 1;
        }
    }
    //constraintStr = inoutStr  + outStr  + inStr;
    auto inputs = 0;
    for (auto& r : regs){
        //std::cout << r.second << std::endl;
        if (outMap.find(r.second) != outMap.end() || inoutMap.find(r.second) != inoutMap.end()){
            asmTemplate = debugreplace(asmTemplate,"{" + std::to_string(r.first) + "}","$" + std::to_string(r.first-inputs));
        }
        if (inMap.find(r.second) != inMap.end()){
            asmTemplate = debugreplace(asmTemplate,"{:" + std::to_string(r.first) + "}","$" + std::to_string(inputregs[r.first]));
            asmTemplate = debugreplace(asmTemplate,"{" + std::to_string(r.first) + "}","$" + std::to_string(inputregs[r.first]));
            inputs += 1;
        }
        std::string toreplace = "{}";
        size_t pos = asmTemplate.find(toreplace);
        if (pos != std::string::npos) {
        asmTemplate.replace(pos, toreplace.length(), "$" + std::to_string(r.first));
        }

        
    }
    //asmTemplate = debugreplace(asmTemplate,"\n","\0A");
    auto inlineAsm = llvm::InlineAsm::get(asmFTy, asmTemplate, constraintStr, /*hasSideEffects=*/true);

    // Create the call instruction.
    llvm::CallInst *callInst = builder.CreateCall(inlineAsm, inputOperands,(retType->isVoidTy() ? "" : gettmp()));

    // Prepare the result mapping.
    tsl::ordered_map<std::string, llvm::Value*> results;
    if (retType->isVoidTy())
        return results;

    if (!retType->isStructTy()) {
        // For a single output operand, the call's return value is the result.
        for (const auto &entry : outputMapping)
            results[entry.second] = callInst;
    } else {
        // For multiple outputs, extract each element.
        unsigned idx = 0;
        for (int i = 0; i != outputMapping.size();i++) {
            std::string regName = outputMapping[i];
            if ( (outMap.find(regName) != outMap.end()) ||
                 (inoutMap.find(regName) != inoutMap.end()) ) {
                llvm::Value* extracted = builder.CreateExtractValue(callInst, idx,gettmp());
                //std::cout << regName << ": " << idx << std::endl;
                results[regName] = extracted;
                idx++;
            }
        }
    }
    return results;
}


int selfty = 0;
std::shared_ptr<llvm::DataLayout> DL = nullptr;

// Cantor Pairing function
// (x+y) (x+y+1)
// ------------- + y
//        2
uint64_t debugCPf( uint64_t x,uint64_t y){
    if (x>y){std::swap(x,y);}
    return (((x+y) * (x+y+1))/2) + y;

}

std::string getName(llvm::Value * val){
            if (auto load = llvm::dyn_cast<llvm::LoadInst>(val)){
                if (!load->getPointerOperand()->hasName()){
                    return getName(load->getPointerOperand());
                }
                return load->getPointerOperand()->getName().str();
                } else if (auto GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(val)){
                if (!GEP->getPointerOperand()->hasName()){
                    return getName(GEP->getPointerOperand());
                }
                return GEP->getPointerOperand()->getName().str();
                } else if (auto Sel = llvm::dyn_cast<llvm::SelectInst>(val)){
                return getName(Sel->getTrueValue());
                } else {
                return val->getName().str();
                }
}
std::string debugvectorToString(const std::vector<std::string>& vec) {
    if (vec.empty()) return ""; 
    
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) {
            oss << ", ";
        }
    }
    return oss.str();
}
std::unordered_map<int,std::vector<std::string>> extmap;
std::unordered_map<std::string, std::vector<llvm::Value*>> mman;
std::unordered_map<std::string, llvm::Value*> globals;
std::unordered_map<std::string, std::vector<std::string>> mmanstr;
std::unordered_map<std::string,llvm::Value*> strs;
//std::unordered_map<std::string,std::vector<std::string>> linkedblocks;


bool ismainfunction = false;
bool ismainblock = false;

bool isnonspecblock = false;
bool iswhileblock = false;

class MetadataManager {
    std::unordered_map<std::string,std::unordered_map<llvm::Value*, llvm::MDNode*>> metadataMap;
    std::vector<std::string> types{"pointee","structee"};


public:
    void attachMetadata(llvm::Value* value, llvm::MDNode* md,std::string type="pointee") {
        //std::cout << "attached to : " << type << reinterpret_cast<uintptr_t>(value) << std::endl;
        if (metadataMap.find(type + "_type") == metadataMap.end()){
            metadataMap[type + "_type"] = {};
        }
        if (llvm::Instruction* inst = llvm::dyn_cast<llvm::Instruction>(value)){
        inst->setMetadata(type + "_type", md);
        }
        metadataMap[type+"_type"][value] = md;
        

    }
    llvm::MDNode* genmetadata(llvm::LLVMContext & ctx ,std::string v){
        llvm::MDNode* mdNode = llvm::MDNode::get(ctx, { llvm::MDString::get(ctx, v) });
        return mdNode;
    }

    llvm::MDNode* getMetadata(llvm::Value* value,std::string type) {
        auto it = metadataMap[type + "_type"].find(value);
        if (it == metadataMap[type + "_type"].end()){
            //std::cout << "unfound MD: " << type << " - " << reinterpret_cast<uintptr_t>(value) << std::endl;
        } else {
            //std::cout << "***found MD: " << reinterpret_cast<uintptr_t>(value) << std::endl;
        }
        
        return (it != metadataMap[type + "_type"].end()) ? it->second : nullptr;
    }
    std::string getMetadataStr(llvm::Value* value,std::string type){
        auto mdn = getMetadata(value,type);
        if (auto MDStr = llvm::dyn_cast<llvm::MDString>(mdn->getOperand(0))){
            return MDStr->getString().str(); // 5275460752
        }
    }

    void propagateMetadata(llvm::Value* from, llvm::Value* to) {
        for (auto& ty: types){
        //std::cout << "propogating " << ty << std::endl;
        //if (!getMetadata(to,ty)){ // don't want overwrites?
        if (llvm::MDNode* md = getMetadata(from,ty)) {
            //std::cout << "propogated to : " << reinterpret_cast<uintptr_t>(to) << " from: " << reinterpret_cast<uintptr_t>(from)<<  std::endl;
            if (llvm::Instruction* inst = llvm::dyn_cast<llvm::Instruction>(to)){
            inst->setMetadata(ty + "_type", md);
            
            }
            attachMetadata(to, md,ty);

        }
        //}
        }
    }
};


MetadataManager MetadataRegistry;

class MetadataIRBuilder : public llvm::IRBuilder<> {
public:
    using llvm::IRBuilder<>::IRBuilder;

    llvm::LoadInst* CreateLoad(llvm::Type* Ty, llvm::Value* Ptr, const llvm::Twine& Name = "") {
        llvm::LoadInst* load = IRBuilder<>::CreateLoad(Ty, Ptr, Name);
        //MetadataRegistry.propagateMetadata(Ptr, load);
        return load;
    }

    llvm::StoreInst* CreateStore(llvm::Value* Val, llvm::Value* Ptr, bool isVolatile = false) {
        llvm::StoreInst* store = IRBuilder<>::CreateStore(Val, Ptr, isVolatile);
        //MetadataRegistry.propagateMetadata(Val, store);
        return store;
    }

    llvm::Value* CreateGEP(llvm::Type* Ty, llvm::Value* Ptr, llvm::ArrayRef<llvm::Value*> IdxList, const llvm::Twine& Name = "") {
        llvm::Value* gep = llvm::IRBuilder<>::CreateGEP(Ty, Ptr, IdxList, Name);
        //MetadataRegistry.propagateMetadata(Ptr, gep);
        return gep;
    }

    llvm::Value* CreateAlloca(llvm::Type *Ty, llvm::Value *ArraySize = (llvm::Value *)nullptr, const llvm::Twine &Name = ""){
        uint64_t sz = DL->getTypeAllocSize(Ty);
        StackSize += sz;
        return llvm::IRBuilder<>::CreateAlloca(Ty,ArraySize,Name);
    }
};

std::string debugdemangle(const char* mangledName) {
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


std::unordered_map<std::string, llvm::Type*> types;
std::unordered_map<std::string, llvm::Type*> globaltypes;
std::vector<std::string> doneblocks;
std::unordered_map<std::string,std::vector<std::string>> metadata;
std::unordered_map<uintptr_t,llvm::Type*> ptrtypes;
std::unordered_map<uintptr_t,std::vector<std::string>> structfields;
std::unordered_map<uintptr_t,llvm::Type*> structtypes;
std::unordered_map<llvm::Value*, int> has_been_used;


class InstructionContainer{
    public:
    std::any value;
    InstructionContainer(std::any value): value(value){}
    llvm::Value* getvalue(){
        
        if (value.type() == typeid(nullptr)){
            return nullptr;
        }
        //std::cout << debugdemangle(value.type().name()) << std::endl;
        if (value.type() == typeid(llvm::Function*)){

            llvm::Value* v = std::any_cast<llvm::Function*>(value);
            has_been_used[v] = 1;
            return v;
        }
        has_been_used[std::any_cast<llvm::Value*>(value)] = 1;



        return std::any_cast<llvm::Value*>(value);
    }
    bool isType(){
        return value.type() == typeid(llvm::Type*);
    }
    llvm::Type* gettype(){
        if (value.type() == typeid(llvm::Value*)){
            auto v = std::any_cast<llvm::Value*>(value);
            auto ty = v->getType();
            if (has_been_used.find(v) != has_been_used.end()){
                return ty;
            }
            /*if (llvm::Instruction *inst = llvm::dyn_cast<llvm::Instruction>(v)) {
                inst->eraseFromParent(); // Remove an instruction from its basic block
            } else if (llvm::BasicBlock *bb = llvm::dyn_cast<llvm::BasicBlock>(v)) {
                bb->eraseFromParent(); // Remove a basic block from a function
            } else if (llvm::Function *func = llvm::dyn_cast<llvm::Function>(v)) {
                func->replaceAllUsesWith(llvm::UndefValue::get(func->getType())); // Ensure no references exist
                func->eraseFromParent(); // Remove a function from the module
            } else if (llvm::GlobalVariable *global = llvm::dyn_cast<llvm::GlobalVariable>(v)) {
                //global->replaceAllUsesWith(llvm::UndefValue::get(global->getType()));
                global->eraseFromParent(); // Remove a global variable
            } else if (llvm::Constant *constant = llvm::dyn_cast<llvm::Constant>(v)) {
            } else if (llvm::Argument *arg = llvm::dyn_cast<llvm::Argument>(v)) {
                // Arguments are tricky. You cannot remove them, but you can make them unused.
                arg->replaceAllUsesWith(llvm::UndefValue::get(arg->getType()));
            
            
        
            }*/
            return ty;
        }
        return std::any_cast<llvm::Type*>(value);
    }
    llvm::Function* getfunction(MetadataIRBuilder& builder,llvm::FunctionType* ty){
        if (value.type() == typeid(llvm::Value*)){
            auto v = std::any_cast<llvm::Value*>(value);
            if (v->getType()->isFunctionTy()){
                return llvm::dyn_cast<llvm::Function>(v);
            } else if (v->getType()->isPointerTy()){
                // Bitcast the opaque pointer to the pointer-to-function type.

                
                return llvm::dyn_cast<llvm::Function>(v);
            }
        }
        return std::any_cast<llvm::Function*>(value);
    }
    private:
};

bool global_redo = false;
struct Instruction {
    bool stored=false;
    bool givebackptr = false;
    std::shared_ptr<InstructionContainer> storedv = nullptr;
    virtual InstructionContainer execute(llvm::LLVMContext &context, llvm::Module &module, MetadataIRBuilder &builder,
                         std::unordered_map<std::string, llvm::Value *> &variables , std::unordered_map<std::string, llvm::Function *> & functions)  = 0;
    virtual InstructionContainer exec(llvm::LLVMContext &context, llvm::Module &module, MetadataIRBuilder &builder,
                         std::unordered_map<std::string, llvm::Value *> &variables , std::unordered_map<std::string, llvm::Function *> & functions){
                            if (stored && !global_redo){
                                return *storedv;
                            }
                            storedv = std::make_shared<InstructionContainer>(execute(context,module,builder,variables,functions).value);
                            stored = true;
                            return *storedv;
                         }
    virtual void setGiveBackPtr(bool b){

        this->givebackptr = b;
    }
    virtual ~Instruction() = default;
};

struct nop_instruction :  public Instruction {





    nop_instruction() /*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    return InstructionContainer(nullptr);
                 }
};

struct value_instruction :  public Instruction {
    llvm::Value* val;




    value_instruction(llvm::Value* val) :val(val)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    return InstructionContainer(val);
                 }
};

struct resource_instruction : public Instruction {
    std::string type;
    std::any value;

    resource_instruction(const std::string& type, std::any v) : type(type), value(v) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::vector<std::string> inttypes = {
        "i8", "i16", "i32", "i64",    // Signed integer types
        "u8", "u16", "u32", "u64",    // Unsigned integer types
        "f32", "f64",                 // Floating-point types
        "isize","usize"
        };
        if (debugin(type,inttypes)){
            llvm::Value* v;
            
            if (type == "i1") {
                v = llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), std::any_cast<int>(value), false); // signed 1-bit
            } 
            else if (type == "u8") {
                v = llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::any_cast<uint8_t>(value), false); // Unsigned 8-bit
            } else if (type == "i8") {
                v = llvm::ConstantInt::get(llvm::Type::getInt8Ty(ctx), std::any_cast<int8_t>(value), true); // Signed 8-bit
            } else if (type == "u16") {
                v = llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::any_cast<uint16_t>(value),false); // Unsigned 16-bit
            } else if (type == "i16") {
                v = llvm::ConstantInt::get(llvm::Type::getInt16Ty(ctx), std::any_cast<int16_t>(value), true); // Signed 16-bit
            } else if (type == "u32") {
                v = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::any_cast<uint32_t>(value), false); // Unsigned 32-bit
            } else if (type == "i32") {
                v = llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), std::any_cast<int32_t>(value), true); // Signed 32-bit
            } else if (type == "u64") {
                v = llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::any_cast<uint64_t>(value), false); // Unsigned 64-bit
            } else if (type == "i64") {
                v = llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::any_cast<int64_t>(value), true); // Signed 64-bit
            } else if (type == "isize") {
                v = llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), std::any_cast<uint64_t>(value), false); // Assume size_t is 64-bit
            } else if (type == "f32") {
                llvm::Value * v = llvm::ConstantFP::get(llvm::Type::getFloatTy(ctx), std::any_cast<float>(value));
                return InstructionContainer(v); // 32-bit float
            } else if (type == "f64") {
                llvm::Value * val = llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), std::any_cast<double>(value));
                return InstructionContainer(val); // 64-bit double
            }
            return InstructionContainer(v);
        } else if (type == "Character"){

            llvm::Type *charType = llvm::Type::getInt8Ty(ctx);
            llvm::Value *charValue;
            if (value.type() == typeid(int)){
                charValue = llvm::ConstantInt::get(charType, std::any_cast<int>(value));
            } else if (value.type() == typeid(char)){
                charValue = llvm::ConstantInt::get(charType, std::any_cast<char>(value));
            } else {
                charValue = llvm::ConstantInt::get(charType, std::any_cast<std::string>(value)[0]);
            

            
            }
            return InstructionContainer(charValue);
        } else if (type == "RawString"){
            auto strval =std::any_cast<std::string>(value);
            if (strs.find(strval) != strs.end()){
                return InstructionContainer(strs[strval]);
            }
            llvm::StringRef ref(strval);
            llvm::Value * StringValue = builder.CreateGlobalString(strval,gettmp());
            strs[strval] = StringValue;
            return InstructionContainer(StringValue);
        } else if (type == "Bool"){
            llvm::Value * boolval;
            auto boolean = std::any_cast<bool>(value);
            if (boolean){
                boolval = llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), 1, false);
            } else {
                boolval = llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), 0, false);
            }
            return InstructionContainer(boolval);
        } else if (type == "Null"){
                llvm::Value *nullPtr = llvm::ConstantPointerNull::get(llvm::PointerType::get(ctx,0));
                return InstructionContainer(nullPtr);
        }else if (type == "Void"){
            if (functions.find("MavenVoidFactory") == functions.end() ){
                llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
                llvm::Function* MVF = llvm::Function::Create(funcType, llvm::Function::InternalLinkage, "MavenVoidFactory", mod);
                auto cblock = builder.GetInsertBlock();
                auto entryBB = llvm::BasicBlock::Create(ctx,gettmp(),MVF);
                builder.SetInsertPoint(entryBB);
                builder.CreateRetVoid();
                builder.SetInsertPoint(cblock);
                functions["MavenVoidFactory"] = MVF;
            }
            llvm::Value* voidv = builder.CreateCall(functions["MavenVoidFactory"]);
            return InstructionContainer(voidv);
        } else if (type == "none"){
            if (globals.find("none") == globals.end()) {
                llvm::StructType* structtype = llvm::StructType::create(ctx,"none_struct");
                llvm::Type * sty = structtype;
                llvm::Value* alloc = new llvm::GlobalVariable(mod,sty,false,llvm::GlobalValue::InternalLinkage,llvm::ConstantAggregateZero::get(sty),"none");
                //builder.CreateStore(alloca,alloc);
                //globals["none"] = alloc;
                globaltypes["none"] = sty;
                std::vector<llvm::Constant*> cons{};
                globals["none"] = new llvm::GlobalVariable(mod,sty->getPointerTo(),true,llvm::GlobalValue::InternalLinkage,llvm::dyn_cast<llvm::Constant>(alloc),"noneload");
            }
            return InstructionContainer(globals["none"]);
        }
        return InstructionContainer(nullptr);
    }

};

std::unordered_map<std::string, llvm::Type*> stids;

struct type_instruction : public Instruction {
    llvm::Type* type;
    llvm::Value* value;
    std::vector<std::shared_ptr<Instruction>> inst;
    std::vector<std::string> procs{"struct","fty"}; // pointer is one arg so ignore
    std::string process;

    type_instruction(llvm::Type* type,llvm::Value* v=nullptr,std::vector<std::shared_ptr<Instruction>> inst={},std::string proc="") : type(type), value(v), inst(inst), process(proc) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    std::string stid = "";
                    llvm::Type* ty;


                    if (inst[0] && process == "struct"){
                        std::vector<llvm::Type*> stty;
                        for (auto& i : inst){
                            stty.push_back(i->exec(ctx,mod,builder,variables,functions).gettype());
                            stid += std::to_string(i->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID());
                        }
                            ty = llvm::StructType::get(ctx,stty,false);

                    
                        
                    } else if (inst[0] && process == "array"){
                        std::vector<llvm::Type*> arty;
                        for (auto& i : inst){
                            arty.push_back(i->exec(ctx,mod,builder,variables,functions).gettype());
                        }
                        ty = llvm::ArrayType::get(arty[0],arty.size());
                        
                    } else if (inst[0] && process == "fty"){

                        std::vector<llvm::Type*> stty;
                        for (auto& i : inst){
                            stty.push_back(i->exec(ctx,mod,builder,variables,functions).gettype());
                        }
                        auto retty = stty[stty.size()-1];
                        stty.pop_back();
                        ty = llvm::FunctionType::get(retty,stty,false);
                        
                    }else if (inst[0] && !debugin(process,procs)){
                        auto exec = inst[0]->exec(ctx,mod,builder,variables,functions);
                        
                        if (exec.isType()){
                            ty = exec.gettype();
                        } else {
                            ty = exec.gettype();
                        }

                        //return InstructionContainer(ty);
                    }  else if (value){
                        ty = value->getType();
                    } else {
                        ty = type;
                    }
                    if (process == "pointer"){
                            ty = llvm::PointerType::get(ty,0);
                    }

                    
                    return InstructionContainer(ty);

                 
                 
    }
};

struct alloc_instruction : public Instruction {
    std::string name;
    std::shared_ptr<Instruction> value;
    llvm::Type* specifiedty;
    llvm::Value * sv;
    std::vector<std::string> blocknames( MetadataIRBuilder &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();
        std::vector<std::string> reversable;
        for (auto i = begin; i != end; i++){
            reversable.push_back((&*i)->getName().str());
        }
        return reversable;

    }
    llvm::BasicBlock* getallocr( MetadataIRBuilder &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();

        for (auto i = begin; i != end; i++){
            if ((&*i)->getName().str() == "__allocreigon" + builder.GetInsertBlock()->getParent()->getName().str()){
                return &*i;
            }
            
        }
        return nullptr;


    }
    llvm::Value* globalize(std::string name,llvm::Type* ty,llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions){
        std::cout << "alloc globalizing : " << name << std::endl;
        llvm::Value* gb = new llvm::GlobalVariable(mod,ty,false,llvm::GlobalValue::InternalLinkage,llvm::Constant::getNullValue(ty),name);
        return gb;

    }

    alloc_instruction(const std::string &n, std::shared_ptr<Instruction> v,llvm::Type* specifiedty=nullptr,llvm::Value*sv = nullptr): name(n), value(v), specifiedty(specifiedty), sv(sv) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        
        bool flag = 0;
        llvm::BasicBlock* cblock;
        llvm::BasicBlock* oblock=nullptr;
        cblock = builder.GetInsertBlock();
        
   
        llvm::Value* allocinst;
        if (!specifiedty){
            auto executed = value->exec(ctx,mod,builder,variables,functions).getvalue();
            oblock = builder.GetInsertBlock();
            if (variables.find(name) != variables.end()){
                return InstructionContainer(nullptr); // bad_handling
            }
            
            


            types[name] = executed->getType();
            // Allocreigon 1
            
            builder.SetInsertPoint(getallocr(builder));
            //if (!ismainfunction){

            allocinst = builder.CreateAlloca(executed->getType(), nullptr, name);
            //} 
            /*else {
                allocinst = globalize(name,executed->getType(),ctx,mod,builder,variables,functions);
            }*/
            if (sv){
                builder.CreateStore(sv,allocinst);
                flag = 1;
            }
            
        } else {
            types[name] = specifiedty;
            //auto cblock = builder.GetInsertBlock();
            builder.SetInsertPoint(getallocr(builder));
            /*if (!ismainfunction || !ismainblock){*/
                allocinst = builder.CreateAlloca(specifiedty, nullptr, name);
            /*}*/ /*else if (ismainfunction) {
                allocinst = globalize(name,specifiedty,ctx,mod,builder,variables,functions);
  
            }*/
            if (sv){
                builder.CreateStore(sv,allocinst);
                flag = 1;
            }
        }
        
        variables[name] = allocinst;
        if (iswhileblock){
        
        variables["flag" + name] = builder.CreateAlloca(builder.getInt1Ty(),nullptr, "flag" + name);
        builder.CreateStore(resource_instruction("Bool",flag).exec(ctx,mod,builder,variables,functions).getvalue(),variables["flag" + name]);
        }
        if (!oblock){
        builder.SetInsertPoint(cblock);
        } else {
            builder.SetInsertPoint(oblock);
        }
        

        
        return InstructionContainer(allocinst); // unnatural
    }

};

struct store_instruction : public Instruction {
    std::string name;
    std::shared_ptr<Instruction>value;
    llvm::Value* sv;
    bool usename = false;

    std::vector<llvm::BasicBlock*> reverseBlockOrder( MetadataIRBuilder &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();
        std::vector<llvm::BasicBlock*> reversable;
        for (auto i = begin; i != end; i++){
            reversable.push_back(&*i);
        }
        return reversable;

    }
    store_instruction(const std::string &n, std::shared_ptr<Instruction>v,llvm::Value* sv=nullptr) : name(n), value(v), sv(sv) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::string varname;
        
        if (name.substr(0,6) != "retval"){
            varname = builder.GetInsertBlock()->getName().str() + name;

        } else {
            varname = name;
        }
        if (!usename){
        if (variables.find(varname) == variables.end()) {
            auto reversable = reverseBlockOrder(builder);
            for (int b = reversable.size()-1;b != 0;b-- ){
                llvm::BasicBlock* i = reversable[b];
                if (!debugin(i->getName().str(),doneblocks)){
                if (variables.find(i->getName().str() + name) != variables.end()){
                    varname = i->getName().str() + name;
                    break;
                }
                if (globals.find(i->getName().str() + name) != globals.end()){
                    if (!sv){
                        builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),globals[i->getName().str() + name]);
                    } else {
                        builder.CreateStore(sv,globals[i->getName().str() + name]);
                    }
                    return InstructionContainer(nullptr);
                }
                }
            }
            if (globals.find(name) != globals.end()){
                if (!sv){
                    builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),globals[name]);
                } else {
                    builder.CreateStore(sv,globals[name]);
                }
                return InstructionContainer(nullptr);
            }
            if (varname == builder.GetInsertBlock()->getName().str() + name){


            throw std::runtime_error("Store Variable not found: " + name + ":" + debugvectorToString(doneblocks));
            }
        }
        
        if (sv){
            builder.CreateStore(sv,variables[varname]);
        } else {
        builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),variables[varname]);
        }
        } else {
        if (sv){
        builder.CreateStore(sv,variables[name]);
        } else {
        builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),variables[name]);
        }  
        }
        if (variables.find("flag" + varname) != variables.end()){
            builder.CreateStore(resource_instruction("Bool",true).exec(ctx,mod,builder,variables,functions).getvalue(),variables["flag" + varname]);
        }
        
        auto vn = usename ? name : varname;
        return InstructionContainer(nullptr);
    }
};


struct assign_instruction : public Instruction {
    std::string name;
    std::shared_ptr<Instruction> value;
    bool global = false;
    bool usename = false;

    assign_instruction(const std::string &n, std::shared_ptr<Instruction> v,bool global=false) : name(n), value(v), global(global) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {


        auto varname =  builder.GetInsertBlock()->getName().str()+name;
        if (!global && ((( !ismainfunction) || (!ismainblock && ismainfunction)) || isnonspecblock)){
        if (variables.find(varname) == variables.end()) {
            alloc_instruction(varname,value).execute(ctx,mod,builder,variables,functions);
            /*if (ismainfunction && ismainblock){
                globals[name] = variables[varname];
                globaltypes[name] = value->exec(ctx,mod,builder,variables,functions).gettype();

            }*/

        }
        } else {
            //std::cout << "assign globalizing " << varname << ":" << global << ":" << ismainfunction << std::endl;
            if (!usename){
            if (variables.find(varname) == variables.end()) {
                llvm::Value* v = new llvm::GlobalVariable(mod,value->exec(ctx,mod,builder,variables,functions).gettype(),false,llvm::GlobalValue::InternalLinkage,llvm::Constant::getNullValue(value->exec(ctx,mod,builder,variables,functions).gettype()),varname);
                variables[varname] = v;
                types[varname] = value->exec(ctx,mod,builder,variables,functions).gettype();
                if (ismainfunction){
                    globals[name] = v;
                    globaltypes[name] = value->exec(ctx,mod,builder,variables,functions).gettype();
                }
            }
            } else {
            if (variables.find(name) == variables.end()) {
                llvm::Value* v = new llvm::GlobalVariable(mod,value->exec(ctx,mod,builder,variables,functions).gettype(),false,llvm::GlobalValue::InternalLinkage,llvm::Constant::getNullValue(value->exec(ctx,mod,builder,variables,functions).gettype()),name);
                variables[name] = v;
                types[name] = value->exec(ctx,mod,builder,variables,functions).gettype();
                //std::cout << "Created " + name << std::endl;
                if (ismainfunction){
                    globals[name] = variables[name];
                    globaltypes[name] = types[name];
                }
            }
            }
        }
        
        //builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),variables[varname]);
        auto sti = store_instruction(name,value);
        sti.usename = usename;
        sti.exec(ctx,mod,builder,variables,functions);
        
        return InstructionContainer(nullptr);
    }

};

struct operation_instruction : public Instruction {

    std::shared_ptr<Instruction>lhs, rhs;
    std::string op;

    operation_instruction(std::shared_ptr<Instruction>l, std::shared_ptr<Instruction>r, std::string o)
        : lhs(l), rhs(r), op(o) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        llvm::Value *result = nullptr;
        auto vlhs = lhs->exec(ctx,mod,builder,variables,functions).getvalue();
        auto vrhs = rhs->exec(ctx,mod,builder,variables,functions).getvalue();
        
        
        if (op == "+"){
                result = builder.CreateAdd(vlhs, vrhs, gettmp());
                
        }
        else if (op == "-"){
                result = builder.CreateSub(vlhs, vrhs, gettmp());
                
        }
        else if (op == "*"){
                result = builder.CreateMul(vlhs, vrhs, gettmp());
                
        }
        else if (op == "/"){
                result = builder.CreateSDiv(vlhs, vrhs, gettmp());
        }
        else if (op == "=="){
                result = builder.CreateICmpEQ(vlhs,vrhs,gettmp());
        }
        else if (op == "!="){
                result = builder.CreateICmpNE(vlhs,vrhs,gettmp());
        } else if (op == "||"){
                result = builder.CreateOr(vlhs,vrhs,gettmp());
        } else if (op == "&&"){
                result = builder.CreateAnd(vlhs,vrhs,gettmp());
        } else if (op == "b|"){
                result = builder.CreateOr(vlhs,vrhs,gettmp());
        } else if (op == "b&"){
                result = builder.CreateAnd(vlhs,vrhs,gettmp());
        } else if (op == "b^"){
                result = builder.CreateXor(vlhs,vrhs,gettmp());
        } else if (op == ">>"){
                result = builder.CreateLShr(vlhs,vrhs,gettmp());
        } else if (op == ">>"){
                result = builder.CreateShl(vlhs,vrhs,gettmp());
        } else {
                throw std::invalid_argument("Invalid operation: " + op);
        }
        return InstructionContainer(result);
    }
};

struct pointer_instruction : public Instruction {
    std::shared_ptr<Instruction> ptrvalue;
    bool isborrow;




    pointer_instruction(std::shared_ptr<Instruction> ptrvalue,bool isborrow=false) :ptrvalue(ptrvalue), isborrow(isborrow) {

    }
    std::shared_ptr<Instruction> getBase(){
        return ptrvalue;
    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {

        auto value = ptrvalue->exec(ctx,mod,builder,variables,functions).gettype();
        
        auto ptrty = llvm::PointerType::get(value,0);
        auto pblock = builder.GetInsertBlock()->getName().str();
        auto ptrid = gettmp();
        types[pblock + ptrid] = ptrty;
        auto alloca = alloc_instruction(pblock + ptrid,ptrvalue,ptrty).exec(ctx,mod,builder,variables,functions);
        ptrtypes[reinterpret_cast<uintptr_t>(alloca.getvalue())] = value;
        auto stinst = store_instruction(ptrid,ptrvalue).exec(ctx,mod,builder,variables,functions);


        return InstructionContainer(variables[pblock + ptrid]);




    }

};


struct cast_instruction : public Instruction {
    std::shared_ptr<Instruction> source;
    std::shared_ptr<type_instruction> dest;

    cast_instruction(std::shared_ptr<Instruction> source, std::shared_ptr<type_instruction> dest) : source(source), dest(dest) {}


    InstructionContainer execute(llvm::LLVMContext &ctx , llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        auto ty = dest->exec(ctx,mod,builder,variables,functions).gettype();
        unsigned srcBits;
        unsigned dstBits;
        if (ty->isIntegerTy()){
            srcBits = source->exec(ctx,mod,builder,variables,functions).gettype()->getIntegerBitWidth();
            dstBits =  ty->getIntegerBitWidth();
        }
        llvm::Value* Cast;
        if (ty->isFunctionTy()){
            Cast = builder.CreateBitCast(source->exec(ctx,mod,builder,variables,functions).getvalue(),ty->getPointerTo(),gettmp());
        } else if (ty->isPointerTy()){
            Cast = builder.CreatePointerCast(source->exec(ctx,mod,builder,variables,functions).getvalue(),ty,gettmp());
        } else if (srcBits > dstBits && ty->isIntegerTy()) {
            Cast = builder.CreateTrunc(source->exec(ctx,mod,builder,variables,functions).getvalue(),ty,gettmp());
        } else if (ty->isIntegerTy()) {
            Cast = builder.CreateSExt(source->exec(ctx,mod,builder,variables,functions).getvalue(),ty,gettmp());
            // ZExt for unsigned
        }
        //llvm::Value* load = builder.CreateLoad(ty,Cast);
        return InstructionContainer(Cast);
    }
};


struct load_instruction : public Instruction {
    std::string name;
    bool isfromassign;

    load_instruction(const std::string &n,bool isfromassign=false) : name(n), isfromassign(isfromassign){
    }
    std::string getName(){
        return name;
    }

    std::vector<llvm::BasicBlock*> reverseBlockOrder( MetadataIRBuilder &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();
        std::vector<llvm::BasicBlock*> reversable;
        for (auto i = begin; i != end; i++){
            reversable.push_back(&*i);
        }
        return reversable;

    }

    InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::string varname;
        if (name.substr(0,6) != "retval"){
            varname = builder.GetInsertBlock()->getName().str() + name;
        } else {
            varname = name;
        }
        std::string vnf = "";
        if (variables.find(varname) == variables.end()) {
            if (functions.find(varname) != functions.end()){
                    

                    return InstructionContainer(functions[varname]);
            }


            auto reversable = reverseBlockOrder(builder);
            for (int b = reversable.size()-1;b != 0;b-- ){
                llvm::BasicBlock* i = reversable[b];

                if (!debugin(i->getName().str(),doneblocks)){
                if (variables.find(i->getName().str() + name) != variables.end()){
                    varname = i->getName().str() + name;
                    vnf = varname;
                    break;
                }
                if (functions.find(i->getName().str() + name) != functions.end()){

                    return InstructionContainer(functions[i->getName().str() + name]);
                }
                }
            }

            if (varname == builder.GetInsertBlock()->getName().str() + name){
            if (functions.find(name) != functions.end()){
                    

                    return InstructionContainer(functions[name]);
            }
            if (globals.find(name) != globals.end()){
                if (!globals[name]){
                    throw std::runtime_error("BAD GVALUE");
                }
                if (!globaltypes[name]){
                    throw std::runtime_error("BAD GTYPE");
                }
                if (givebackptr){
                    return InstructionContainer(globals[name]);
                }
                llvm::Value * gbl = builder.CreateLoad(globaltypes[name],globals[name], gettmp());
                return InstructionContainer(gbl);
            }
            std::error_code error;

            llvm::raw_fd_ostream file("Test.ll", error, llvm::sys::fs::OF_None);
            
            mod.print(file, nullptr);

            
            
            throw std::runtime_error("Variable not found: " + name + " (debug: " + varname + ":" + vnf + ":" + std::to_string(givebackptr) + ")");
            }
            
        }
        std::string tmp = gettmp();
        
        if (givebackptr){
            /*if (types[varname]->isPointerTy()){
                llvm::Value* loadedvalue = builder.CreateLoad(types[varname],variables[varname],tmp);
                return InstructionContainer(loadedvalue);
            }*/
                return InstructionContainer(variables[varname]);
        }
        llvm::Value* loadedvalue = builder.CreateLoad(types[varname],variables[varname],tmp);

        /*if (tmp == "tmp139"){
            throw std::runtime_error(varname + " : " + (types[varname]->isStructTy() ? "true" : "false"));
        }*/
        if (varname != builder.GetInsertBlock()->getName().str() + name && name.substr(0,6) != "retval" && ( iswhileblock)){
            alloc_instruction(builder.GetInsertBlock()->getName().str() + name,nullptr,types[varname],nullptr).exec(ctx,mod,builder,variables,functions);
            auto cmp = builder.CreateICmpEQ(builder.CreateLoad(builder.getInt1Ty(),variables["flag" + builder.GetInsertBlock()->getName().str() + name]),resource_instruction("Bool",true).exec(ctx,mod,builder,variables,functions).getvalue());
            auto select = builder.CreateSelect(cmp,variables[builder.GetInsertBlock()->getName().str() + name],variables[varname]);
            auto selectstore = builder.CreateSelect(cmp,builder.CreateLoad(types[varname],variables[builder.GetInsertBlock()->getName().str() + name],tmp),loadedvalue);
            store_instruction(name,nullptr,selectstore).exec(ctx,mod,builder,variables,functions);


            types[builder.GetInsertBlock()->getName().str() + name] = types[varname];
            
            auto debugtmp =gettmp();
            if (givebackptr){
                return InstructionContainer(select);
            }
            loadedvalue = builder.CreateLoad(types[builder.GetInsertBlock()->getName().str() + name],select,debugtmp);
        }


        

        return InstructionContainer(loadedvalue);
    }
};



struct arg_instruction : public Instruction {
    int index;


    arg_instruction(int index) : index(index) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        llvm::Function* fn = builder.GetInsertBlock()->getParent();
        llvm::Value* arg = std::next(fn->arg_begin(),index);
        return InstructionContainer(arg);
    }
};



struct block_instruction : public Instruction {
    bool ismain;
    llvm::Function* mainfunction;
    std::vector<std::shared_ptr<Instruction>> insts;
    llvm::Function* specfunction;

    block_instruction(bool ismain,std::vector<std::shared_ptr<Instruction>> insts={},llvm::Function* mainfunction=nullptr) : ismain(ismain), mainfunction(mainfunction),insts(insts) {
    }

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        bool prev = ismainblock;
        bool prevwhile = isnonspecblock;
        isnonspecblock = false;
        if (ismain){
            for (auto& f: functions){
                mainfunction = f.second;
            }
            ismainblock = true;
        } else {
            ismainblock = false;
        }
        

        llvm::BasicBlock* block;
        InstructionContainer IC(nullptr);
        llvm::Function* fn;
        
        if (!ismain && !specfunction){
            
            llvm::BasicBlock* insertpt = builder.GetInsertBlock();

            fn = insertpt->getParent();
  
            block = llvm::BasicBlock::Create(ctx,gettmp(),fn);
            builder.CreateBr(block);
            builder.SetInsertPoint(block);
        } else if (specfunction){
            llvm::BasicBlock* insertpt = builder.GetInsertBlock();

            fn = specfunction;
  
            block = llvm::BasicBlock::Create(ctx,gettmp(),fn);
            builder.SetInsertPoint(block);
        }else {
            fn = mainfunction;
            block = llvm::BasicBlock::Create(ctx,gettmp(),mainfunction);
            builder.SetInsertPoint(block);
        }
        llvm::BasicBlock* allocr;
        if (ismain){
        allocr = llvm::BasicBlock::Create(ctx,"__allocreigon" + fn->getName().str(),fn,block);
        }


        for (auto& inst: insts){
            IC = inst->exec(ctx,mod,builder,variables,functions);
        } // add return value later here
        doneblocks.push_back(block->getName().str());
        ismainblock = prev;
        if (builder.GetInsertBlock()->getName().str() != block->getName().str()){
            doneblocks.push_back(builder.GetInsertBlock()->getName().str());
        }
        
        if (!ismain){
            auto jumpblock = llvm::BasicBlock::Create(ctx,gettmp(),fn);
            builder.CreateBr(jumpblock);
            builder.SetInsertPoint(jumpblock);
        } else {


            if (variables.find("retval" + fn->getName().str()) != variables.end()){
        
            auto ty = load_instruction("retval" + fn->getName().str()).execute(ctx,mod,builder,variables,functions).getvalue();
            
            
            builder.CreateRet(ty);
            }
                        auto cblock = builder.GetInsertBlock();
            builder.SetInsertPoint(allocr);
            builder.CreateBr(block);
            builder.SetInsertPoint(cblock);
        }
        if (ismain){
            ismainblock = prev;
        }
        isnonspecblock = false;
        iswhileblock = false;

        return InstructionContainer(IC.value);


    }

};

struct if_instruction : public Instruction {
    std::shared_ptr<Instruction>condition;
    std::shared_ptr<block_instruction> thenInstructions;
    std::shared_ptr<block_instruction> elseInstructions;
    std::vector<std::shared_ptr<if_instruction>> elifs;
    bool isassign;

    if_instruction(std::shared_ptr<Instruction>cond,std::shared_ptr<block_instruction> then, std::shared_ptr<block_instruction> orelse=nullptr,std::vector<std::shared_ptr<if_instruction>> elifs={},bool isassign=false) : condition(cond), thenInstructions(then), elseInstructions(orelse),elifs(elifs), isassign(isassign) {}

InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, MetadataIRBuilder &builder,
             std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
    bool prevwhile = isnonspecblock;
    isnonspecblock = true;
    iswhileblock = false;
    llvm::Function *func = builder.GetInsertBlock()->getParent();
    llvm::Value* assignret = nullptr;
    std::string atmp = gettmp();
    std::string assigntmp;
    llvm::Type* assignty;
    if (isassign){
        assigntmp = builder.GetInsertBlock()->getName().str()  + atmp;
    }
    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(ctx, gettmp(), func);
    llvm::BasicBlock * elseBB;
    if (elseInstructions){
    elseBB = llvm::BasicBlock::Create(ctx, gettmp(),func);
    }
    llvm::BasicBlock *elifBB;
    if (elifs.size() != 0){
    elifBB = llvm::BasicBlock::Create(ctx, gettmp(),func);
    }
    llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(ctx, gettmp(),func);

    if (!elseInstructions && elifs.size() == 0){
        builder.CreateCondBr(condition->exec(ctx,mod,builder,variables,functions).getvalue(), thenBB, mergeBB);
    
    } else if (elifs.size() == 0){
        builder.CreateCondBr(condition->exec(ctx,mod,builder,variables,functions).getvalue(), thenBB, elseBB);
    } else {
        builder.CreateCondBr(condition->exec(ctx,mod,builder,variables,functions).getvalue(), thenBB, elifBB);
    }
    
    builder.SetInsertPoint(thenBB);
    for (auto &inst : thenInstructions->insts) {
        inst->exec(ctx, mod, builder,variables,functions);
        if (inst == thenInstructions->insts[thenInstructions->insts.size()-1] && isassign){
            if (variables.find(assigntmp) == variables.end()){
                alloc_instruction(assigntmp,inst).exec(ctx, mod, builder,variables,functions);
                assignty = inst->exec(ctx, mod, builder,variables,functions).gettype();
            }
            store_instruction(atmp,inst).exec(ctx, mod, builder,variables,functions);
        }
    }
    doneblocks.push_back(thenBB->getName().str());
    builder.CreateBr(mergeBB);
    
    if (elifs.size() != 0){
    auto prevblock = elifBB;
    
    for (int i = 0; i != elifs.size(); i++){
        auto elifinst = elifs[i];
        builder.SetInsertPoint(prevblock);
        llvm::BasicBlock *elseB = llvm::BasicBlock::Create(ctx, gettmp(),func);
        llvm::BasicBlock * condchecker;
        if (i != elifs.size()-1 || elseInstructions){
        condchecker = llvm::BasicBlock::Create(ctx, gettmp(),func);
        builder.CreateCondBr(elifinst->condition->exec(ctx,mod,builder,variables,functions).getvalue(),elseB,condchecker);
        prevblock = condchecker;
        } else {
            builder.CreateCondBr(elifinst->condition->exec(ctx,mod,builder,variables,functions).getvalue(),elseB,mergeBB);
        }
        builder.SetInsertPoint(elseB);
        for (auto& EI: elifinst->thenInstructions->insts){
            EI->exec(ctx, mod, builder,variables,functions);
            if (EI == elifinst->thenInstructions->insts[elifinst->thenInstructions->insts.size()-1] && isassign){
                if (variables.find(assigntmp) == variables.end()){
                    alloc_instruction(assigntmp,EI).exec(ctx, mod, builder,variables,functions);
                }
                store_instruction(atmp,EI).exec(ctx, mod, builder,variables,functions);
            }
        }
        doneblocks.push_back(elseB->getName().str());
        builder.CreateBr(mergeBB);

    }

    if (elseInstructions){
        builder.SetInsertPoint(prevblock);
        builder.CreateBr(elseBB);
        builder.SetInsertPoint(elseBB);
        for (auto &inst : elseInstructions->insts) {
            inst->exec(ctx, mod, builder,variables,functions);
            if (inst == elseInstructions->insts[elseInstructions->insts.size()-1] && isassign){
                if (variables.find(assigntmp) == variables.end()){
                    alloc_instruction(assigntmp,inst).exec(ctx, mod, builder,variables,functions);
                }
                store_instruction(atmp,inst).exec(ctx, mod, builder,variables,functions);
            }
        }
        doneblocks.push_back(elseBB->getName().str());
        builder.CreateBr(mergeBB);
    }
    }
    if (elifs.size() == 0 && elseInstructions){
    

    //func->getBasicBlockList().push_back(elseBB);
    builder.SetInsertPoint(elseBB);
    for (auto &inst : elseInstructions->insts) {
        inst->exec(ctx, mod, builder,variables,functions);
        if (inst == elseInstructions->insts[elseInstructions->insts.size()-1] && isassign){
            if (variables.find(assigntmp) == variables.end()){
                alloc_instruction(assigntmp,inst).exec(ctx, mod, builder,variables,functions);
            }
            store_instruction(atmp,inst).exec(ctx, mod, builder,variables,functions);
        }
    }
    doneblocks.push_back(elseBB->getName().str());
    builder.CreateBr(mergeBB);
    }


    //func->getBasicBlockList().push_back(mergeBB);
    builder.SetInsertPoint(mergeBB);
    isnonspecblock = prevwhile;
    iswhileblock = false;
    if (isassign){
        llvm::Value * load = builder.CreateLoad(assignty,variables[assigntmp],gettmp());
        return InstructionContainer(load);
    }
    return InstructionContainer(nullptr);
}

};

struct while_instruction : public Instruction {
    std::shared_ptr<Instruction>condition;
    std::shared_ptr<block_instruction> block;
    bool isassign;


    while_instruction(std::shared_ptr<Instruction>cond,std::shared_ptr<block_instruction> block,bool isassign =false) : condition(cond), block(block), isassign(isassign) {}

InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, MetadataIRBuilder &builder,
             std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
             auto whileBB = llvm::BasicBlock::Create(ctx,gettmp(),builder.GetInsertBlock()->getParent());
             auto mergeBB = llvm::BasicBlock::Create(ctx,gettmp(),builder.GetInsertBlock()->getParent());
            std::string atmp = gettmp();
            std::string assigntmp;
            llvm::Type* assignty;
            if (isassign){
                assigntmp = builder.GetInsertBlock()->getName().str()  + atmp;
            }
             auto cond = condition->exec(ctx,mod,builder,variables,functions);
             builder.CreateCondBr(cond.getvalue(),whileBB,mergeBB);
             builder.SetInsertPoint(whileBB);
             bool prevwhile = isnonspecblock;
             bool previswhile = iswhileblock;
             iswhileblock = true;
             isnonspecblock = true;
             for (auto inst: block->insts){
                inst->exec(ctx,mod,builder,variables,functions);
                if (isassign && inst == block->insts[block->insts.size()-1]){
                    if (variables.find(assigntmp) == variables.end()){
                        alloc_instruction(assigntmp,inst).exec(ctx,mod,builder,variables,functions);
                        assignty = inst->exec(ctx,mod,builder,variables,functions).gettype();
                    }
                    store_instruction(atmp,inst).exec(ctx,mod,builder,variables,functions);
                }
             }
             
             
             bool tmp = global_redo;
             global_redo = true;
             cond = condition->exec(ctx,mod,builder,variables,functions);
             global_redo = tmp;
             builder.CreateCondBr(cond.getvalue(),whileBB,mergeBB);
             isnonspecblock = prevwhile;
             iswhileblock = previswhile;
             doneblocks.push_back(whileBB->getName().str());
             builder.SetInsertPoint(mergeBB);
             if (isassign){
                llvm::Value * load = builder.CreateLoad(assignty,variables[assigntmp],gettmp());
                return InstructionContainer(load);
             }
             return InstructionContainer(nullptr);
             }
};


struct return_instruction : public Instruction {
    std::shared_ptr<Instruction> value;

    return_instruction(std::shared_ptr<Instruction> value) : value(value) {

    }
    std::vector<llvm::BasicBlock*> reverseBlockOrder( MetadataIRBuilder &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();
        std::vector<llvm::BasicBlock*> reversable;
        for (auto i = begin; i != end; i++){
            reversable.push_back(&*i);
        }
        return reversable;

    }

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        
        std::string cfn = builder.GetInsertBlock()->getParent()->getName().str();
        //(builder);
        value->setGiveBackPtr(false);
        llvm::Value* ret;

        if (variables.find("retval" + cfn) == variables.end()){
            alloc_instruction("retval" + cfn,value).execute(ctx,mod,builder,variables,functions);
            store_instruction("retval" + cfn,value).execute(ctx,mod,builder,variables,functions);
        } else {
            store_instruction("retval" + cfn,value).execute(ctx,mod,builder,variables,functions);
        }
        
        return InstructionContainer(nullptr);




    }

};




struct borrowed_pointer_instruction :  public Instruction {
    std::shared_ptr<Instruction> ptr;




    borrowed_pointer_instruction(std::shared_ptr<Instruction> ptr) :ptr(ptr) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {

        auto ptrtodata = ptr->exec(ctx,mod,builder,variables,functions).getvalue();
        /*auto pblock = builder.GetInsertBlock()->getName().str();
        auto bptrid = gettmp();
        types[pblock + bptrid] = ptrtodata->getType();
        
        auto alloca = alloc_instruction(pblock + bptrid,ptr,ptrtodata->getType()).exec(ctx,mod,builder,variables,functions);
        auto stinst = store_instruction(bptrid,ptr).exec(ctx,mod,builder,variables,functions);
        //auto loadinst = load_instruction(bptrid).exec(ctx,mod,builder,variables,functions);*/
        return InstructionContainer(ptrtodata);




    }

};

struct function_instruction : public Instruction {
    std::string name;
    std::shared_ptr<Instruction> returnType;
    std::vector<std::string> argnames;
    std::vector<std::shared_ptr<Instruction>> argTypes;
    std::shared_ptr<block_instruction> body;
    bool external;
    bool variadic;
    bool ismainfn;
    bool isnonereturn;
    std::string extname;
    std::vector<llvm::BasicBlock*> reverseBlockOrder( MetadataIRBuilder &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();
        std::vector<llvm::BasicBlock*> reversable;
        for (auto i = begin; i != end; i++){
            reversable.push_back(&*i);
        }
        return reversable;

    }

    function_instruction(const std::string &n, std::shared_ptr<Instruction> ret, const std::vector<std::shared_ptr<Instruction>> &args,std::shared_ptr<block_instruction> body,bool ismainfn = false, bool ext=false,bool variadic =false,std::vector<std::string> argnames={},std::string extname="", bool isnonereturn=false)
        : name(n), returnType(ret), argTypes(args), external(ext), variadic(variadic), body(body), ismainfn(ismainfn), argnames(argnames), extname(extname), isnonereturn(isnonereturn) {}

    InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> & variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        if (globals.find("none") == globals.end()) {
                resource_instruction("none",nullptr).exec(ctx,mod,builder,variables,functions);
                //globals["noneload"] = new llvm::GlobalVariable(mod,sty,true,llvm::GlobalValue::InternalLinkage,llvm::dyn_cast<llvm::Constant>(globals["none"]),"noneload");
        }
        std::vector<llvm::Type*>vecargtypes;
        bool prev = ismainfunction;


        
        for (auto& inst : argTypes){
            

            auto aty = inst->exec(ctx,mod,builder,variables,functions);
            
            vecargtypes.push_back(aty.gettype());
            
        }
        if (argnames.size() > 0 ){
            if(argnames[0] == "self"){
                if (auto ptrinst = std::dynamic_pointer_cast<pointer_instruction>(argTypes[0])){
                    selfty = ptrinst->getBase()->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID();
        }
        }
        }
        
        llvm::FunctionType *funcType = llvm::FunctionType::get(returnType->exec(ctx,mod,builder,variables,functions).gettype(), vecargtypes, variadic);
        
        llvm::Function* fn;
        llvm::Function* internalfn;
        if (external) {
            if (extname != ""){
                std::hash<std::string> hashery{};
                fn = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, std::to_string(hashery(extname)) + name, mod);
            } else {
                fn = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, mod);
            }
        } else if (ismainfn) {

            
            fn = llvm::Function::Create(funcType, llvm::Function::InternalLinkage, name + "MavenStart", mod);
            internalfn = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, mod);
            auto callblock = llvm::BasicBlock::Create(ctx,gettmp(),internalfn);
            builder.SetInsertPoint(callblock);
            builder.CreateRet(builder.CreateCall(fn,{},gettmp()));
            
            
        } else {
            fn = llvm::Function::Create(funcType, llvm::Function::InternalLinkage, name, mod);
        }
        
        if (ismainfn){
            functions["main" + name] = fn;
            ismainfunction = true;
        } else {
            ismainfunction = false;
            
            functions[builder.GetInsertPoint()->getName().str() + name] = fn;
        }
        
        llvm::BasicBlock* cblock;
        if (!ismainfn){
            cblock = builder.GetInsertBlock();
        }
        
        body->specfunction = fn;
        body->ismain = true;
        std::vector<std::shared_ptr<Instruction>> addedinstructions;
        for (int i = 0; i != argnames.size(); i++){ 
            addedinstructions.push_back(std::make_shared<assign_instruction>(argnames[i],std::make_shared<arg_instruction>(i)));
        }
        
        for (auto& inst: body->insts){
            addedinstructions.push_back(inst);
        }
        
        if (ismainfn){
            addedinstructions.push_back(std::make_shared<return_instruction>(std::make_shared<resource_instruction>("i32",0)));
        }
        
        body->insts = addedinstructions;
        
        metadata[name] = argnames;

        if (isnonereturn){

            body->insts.push_back(std::make_shared<return_instruction>(std::make_shared<resource_instruction>("none",nullptr)));
        }
        
        
        

        
        auto bvalue = body->exec(ctx,mod,builder,variables,functions);
        
        if (!ismainfn){
            if (extname == "" && !external){
            if (variables.find("retval" + name) == variables.end()){
                builder.CreateRet(bvalue.getvalue());
            }
            } else {
            std::hash<std::string> hashery{};
            if (variables.find("retval" + std::to_string(hashery(extname)) + name) == variables.end() && variables.find("retval" + name) == variables.end()){
                builder.CreateRet(bvalue.getvalue());
            }
            }
            builder.SetInsertPoint(cblock);
        } else if (ismainfn){
            /*if (variables.find("retval" + name) != variables.end()){
                builder.CreateRet(variables["retval" + name]);
            }*/
        }
        
        llvm::Value* fnv = fn;

        ismainfunction = prev;
        

        
        return InstructionContainer(fnv);
    }
};

struct declare_function_instruction : public Instruction {
    std::string name;
    std::vector<std::shared_ptr<Instruction>> argTypes;
    std::shared_ptr<Instruction> returnType;
    bool isvdic;

    declare_function_instruction(std::string name,std::vector<std::shared_ptr<Instruction>> &args,std::shared_ptr<Instruction> returnType,bool isvdic=false) : name(name), argTypes(args), returnType(returnType), isvdic(isvdic) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {


        std::vector<llvm::Type*>vecargtypes;

        for (auto& inst : argTypes){
            vecargtypes.push_back(inst->exec(ctx,mod,builder,variables,functions).gettype());
        }

        llvm::FunctionType *funcType = llvm::FunctionType::get(returnType->exec(ctx,mod,builder,variables,functions).gettype(), vecargtypes, isvdic);

        llvm::Function* extfn = llvm::Function::Create(funcType,llvm::Function::ExternalLinkage,name,mod);

        functions[name] = extfn;

        return InstructionContainer(nullptr);




    }

};



struct call_instruction : public Instruction {
    std::shared_ptr<Instruction> source;
    std::vector<std::shared_ptr<Instruction>> args;
    std::shared_ptr<type_instruction> fty;
    



    call_instruction(std::shared_ptr<Instruction> source,std::vector<std::shared_ptr<Instruction>> &args,std::shared_ptr<type_instruction>fty) :source(source), args(args), fty(fty) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::vector<llvm::Value*>vecargs;
        for (auto& inst : args){
            vecargs.push_back(inst->exec(ctx,mod,builder,variables,functions).getvalue());
        }




        llvm::Function*  fn;
        if (fty->exec(ctx,mod,builder,variables,functions).gettype()->isFunctionTy()){
            llvm::FunctionType* fnt = llvm::dyn_cast<llvm::FunctionType>(fty->exec(ctx,mod,builder,variables,functions).gettype());
        
            fn = source->exec(ctx,mod,builder,variables,functions).getfunction(builder,llvm::dyn_cast<llvm::FunctionType>(fty->exec(ctx,mod,builder,variables,functions).gettype()));
        } else {
            fn = source->exec(ctx,mod,builder,variables,functions).getfunction(builder,nullptr);
        }


        llvm::Value* call_value ;
        if (!fn){
           call_value = builder.CreateCall(llvm::dyn_cast<llvm::FunctionType>(fty->exec(ctx,mod,builder,variables,functions).gettype()), source->exec(ctx,mod,builder,variables,functions).getvalue(),vecargs,gettmp());
        } else {

        
        

            call_value = builder.CreateCall(fn,vecargs,gettmp());
        }
        
        return InstructionContainer(call_value);




    }

};



struct dereference_instruction :  public Instruction {
    std::shared_ptr<Instruction> ptr;
    std::shared_ptr<type_instruction> type;



    dereference_instruction(std::shared_ptr<Instruction> ptr,std::shared_ptr<type_instruction> type) :ptr(ptr),type(type) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        
        auto ptrtodata = ptr->exec(ctx,mod,builder,variables,functions);

        auto ty = type->exec(ctx,mod,builder,variables,functions);

        llvm::Value* loadv;
        //std::cout << "derefing: " << reinterpret_cast<uintptr_t>(ptrtodata) << std::endl;
        if (ty.gettype()->isStructTy()){
            loadv = builder.CreateLoad(ty.gettype(),ptrtodata.getvalue(),"deref" + gettmp());
        } else {
            loadv = builder.CreateLoad(ty.gettype(),ptrtodata.getvalue(),"deref" + gettmp());
        }
        //MetadataRegistry.propagateMetadata(mman[MetadataRegistry.getMetadataStr(ptrtodata,"pointee")][0],loadv);
        return InstructionContainer(loadv);




    }

};

struct pointer_store_instruction :  public Instruction {
    std::shared_ptr<Instruction> ptr;
    std::shared_ptr<Instruction> value;




    pointer_store_instruction(std::shared_ptr<Instruction> ptr,std::shared_ptr<Instruction> value) :ptr(ptr), value(value) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        
        auto ptrtodata = ptr->exec(ctx,mod,builder,variables,functions).getvalue();
        builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),ptrtodata);
        return InstructionContainer(nullptr);




    }

};

struct struct_decl_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction> >fields;
    std::string name;
    bool ispacked;




    struct_decl_instruction(std::string& name,std::vector<std::shared_ptr<Instruction> > fields,bool ispacked=false) :name(name),fields(fields),ispacked(ispacked) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::vector<llvm::Type*> typearray = {};
        std::string stid = "";
        for (auto& field: fields){
            typearray.push_back(field->exec(ctx,mod,builder,variables,functions).gettype());
            stid += std::to_string(field->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID());
        }
        auto origname = name;
        name = builder.GetInsertBlock()->getName().str() + name;
        llvm::Type* structy;
        if (typearray.empty()){
            typearray.push_back(resource_instruction("none","").exec(ctx,mod,builder,variables,functions).gettype()); // weird but apparently empty structs are a no
        }
        structy = llvm::StructType::create(typearray,name,ispacked);

        types[name] = structy;
        variables[name] = alloc_instruction(gettmp(),nullptr,structy).exec(ctx,mod,builder,variables,functions).getvalue(); // DEAD_CODE
        //types[origname] = structy;


        llvm::GlobalVariable *GlobalStruct = new llvm::GlobalVariable(
            mod,
            structy,  
            true,     
            llvm::GlobalValue::ExternalLinkage,
            llvm::ConstantAggregateZero::get(structy),   
            gettmp()
        );
        globals[origname] = GlobalStruct;
        globaltypes[origname] = structy;
        return InstructionContainer(structy);




    }

};

struct struct_instance_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction> >fields;
    std::shared_ptr<Instruction> pstruct;
    std::vector<std::string> sfields;
    bool ispacked;
    int vid;




    struct_instance_instruction(std::shared_ptr<Instruction> pstruct,std::vector<std::shared_ptr<Instruction> > fields,std::vector<std::string> sfields,bool ispacked=false,int vid=0) :pstruct(pstruct),fields(fields),sfields(sfields),ispacked(ispacked),vid(vid) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::vector<llvm::Value*> fvalues;
        for (auto& field: fields){
            fvalues.push_back(field->exec(ctx,mod,builder,variables,functions).getvalue());
        }
        if (extmap.find(pstruct->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID()) != extmap.end()){
            for (auto& n : extmap[pstruct->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID()]){
                assign_instruction("__" + std::to_string(vid) + n,std::make_shared<load_instruction>(n),true).exec(ctx,mod,builder,variables,functions);
            }
        }
        
        auto tmpc = gettmp();
        auto allocd = alloc_instruction(tmpc,nullptr,pstruct->exec(ctx,mod,builder,variables,functions).gettype()).exec(ctx,mod,builder,variables,functions);
        int i = 0;
        std::vector<llvm::Value*> metaty{pstruct->exec(ctx,mod,builder,variables,functions).getvalue()};
        for (auto& value: fvalues){
            auto fptr = builder.CreateStructGEP(pstruct->exec(ctx,mod,builder,variables,functions).gettype(),allocd.getvalue(),i,gettmp());
            metaty.push_back(value);
            builder.CreateStore(value,fptr);
            i++;
        }
        auto stmp = gettmp();
        MetadataRegistry.attachMetadata(variables[tmpc],MetadataRegistry.genmetadata(ctx,stmp),"structee");
        MetadataRegistry.propagateMetadata(pstruct->exec(ctx,mod,builder,variables,functions).getvalue(),variables[tmpc]);
        mman[stmp] = metaty;
        mmanstr[stmp] = sfields;
        llvm::Value* load = builder.CreateLoad(pstruct->exec(ctx,mod,builder,variables,functions).gettype(),variables[tmpc],gettmp());
        
        return InstructionContainer(load);




    }

};

struct access_struct_member_instruction :  public Instruction {
    std::shared_ptr<Instruction> structinstance;
    std::vector<std::shared_ptr<type_instruction>> types;
    std::string field;
    int id;
    bool assign=false;





    access_struct_member_instruction(std::shared_ptr<Instruction> structinstance,std::string field,int id,std::vector<std::shared_ptr<type_instruction>> types,bool assign=false) :structinstance(structinstance),field(field),id(id),types(types),assign(assign){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        InstructionContainer v(nullptr);
        /*if (auto li = std::dynamic_pointer_cast<load_instruction>(structinstance)){
            li->setGiveBackPtr(true);
            v = li->exec(ctx,mod,builder,variables,functions);
        } else {*/
            structinstance->setGiveBackPtr(true);
            v = structinstance->exec(ctx,mod,builder,variables,functions);
        //}
        
        if (!v.gettype()->isPointerTy()){
            auto gtmp = gettmp();
            alloc_instruction(gtmp,nullptr,v.gettype(),nullptr).exec(ctx,mod,builder,variables,functions);
            builder.CreateStore(v.getvalue(),variables[gtmp]);  
            v = InstructionContainer(variables[gtmp]);          
        }
        auto fptr = builder.CreateStructGEP(types[0]->exec(ctx,mod,builder,variables,functions).gettype(),v.getvalue(),id,gettmp());
        if (givebackptr){
            return InstructionContainer(fptr); 
        }
        if ((types[1]->exec(ctx,mod,builder,variables,functions).gettype()->isStructTy() || types[1]->exec(ctx,mod,builder,variables,functions).gettype()->isArrayTy() || assign) && givebackptr){
            return InstructionContainer(fptr);
        }

        llvm::Value* load = builder.CreateLoad(types[1]->exec(ctx,mod,builder,variables,functions).gettype(),fptr,gettmp());


        return InstructionContainer(load);




    }

};

struct tuple_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction> >fields;




    tuple_instruction(std::vector<std::shared_ptr<Instruction> > fields) :fields(fields) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        std::vector<llvm::Type*> typearray;
        std::string stid = "";
        for (auto& field: fields){
            typearray.push_back(field->exec(ctx,mod,builder,variables,functions).gettype());
            stid += std::to_string(field->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID());
        }

        auto name = gettmp();
        llvm::Type* structy;
        if (stids.find(stid) != stids.end()){
            structy = stids[stid];
        } else {
            structy = llvm::StructType::create(typearray,name,false);
            stids[stid] = structy;
        }
        types[name] = structy;
        auto allocd = alloc_instruction(gettmp(),nullptr,structy).exec(ctx,mod,builder,variables,functions).getvalue();
        int i = 0;
        for (auto& value: fields){
            auto fptr = builder.CreateStructGEP(structy,allocd,i,gettmp());
            builder.CreateStore(value->exec(ctx,mod,builder,variables,functions).getvalue(),fptr);
            i++;
        }
        llvm::Value* load = builder.CreateLoad(structy,allocd,gettmp());
        
        
        return InstructionContainer(load);
        




    }

};

struct access_tuple_member_instruction :  public Instruction {
    std::shared_ptr<Instruction> tupleinstance;
    std::vector<std::shared_ptr<type_instruction>> types;
    int id;




    access_tuple_member_instruction(std::shared_ptr<Instruction> tupleinstance,int id,std::vector<std::shared_ptr<type_instruction>> types) :tupleinstance(tupleinstance),id(id),types(types){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        auto v = tupleinstance->exec(ctx,mod,builder,variables,functions);
        if (!v.gettype()->isPointerTy()){
            auto gtmp = gettmp();
            alloc_instruction(gtmp,nullptr,v.gettype(),nullptr).exec(ctx,mod,builder,variables,functions);
            builder.CreateStore(v.getvalue(),variables[gtmp]);  
            v = InstructionContainer(variables[gtmp]);          
        }
        

        auto fptr = builder.CreateStructGEP(types[0]->exec(ctx,mod,builder,variables,functions).gettype(),v.getvalue(),id,gettmp());
        if (types[1]->exec(ctx,mod,builder,variables,functions).gettype()->isStructTy() || types[1]->exec(ctx,mod,builder,variables,functions).gettype()->isArrayTy()){
            return InstructionContainer(fptr);
        } 
        auto gtmp = gettmp();
        
        llvm::Value* load = builder.CreateLoad(types[1]->exec(ctx,mod,builder,variables,functions).gettype(),fptr,gtmp);
    

        return InstructionContainer(load);
        
        




    }

};


struct array_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction>> elements;
    int sz;




    array_instruction(std::vector<std::shared_ptr<Instruction>> elements,int sz) :sz(sz),elements(elements){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        // do something for empty lists
        llvm::Type* elemty = elements[0]->exec(ctx,mod,builder,variables,functions).gettype();
        
        auto arrayty = llvm::ArrayType::get(elemty,sz);
        auto atmp = gettmp();
        alloc_instruction(atmp,nullptr,arrayty,nullptr).exec(ctx,mod,builder,variables,functions).gettype();
        int i = 0;
        for (auto& elem: elements){
            auto gep = builder.CreateGEP(arrayty, variables[atmp],{builder.getInt32(0), builder.getInt32(i)});
            builder.CreateStore(elem->exec(ctx,mod,builder,variables,functions).getvalue(),gep);
            i++;
        }
        llvm::Value* load = builder.CreateLoad(arrayty,variables[atmp],gettmp());
        return InstructionContainer(load);
        }
        




    

};

struct access_array_instruction :  public Instruction {
    std::shared_ptr<Instruction> array;
    std::vector<std::shared_ptr<type_instruction>> loadty;
    std::shared_ptr<Instruction> index;




    access_array_instruction(std::shared_ptr<Instruction> array,std::vector<std::shared_ptr<type_instruction>> loadty,std::shared_ptr<Instruction> index) :array(array),index(index),loadty(loadty){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        // do something for empty lists
        llvm::Type* elemty = loadty[1]->exec(ctx,mod,builder,variables,functions).gettype();
        
        auto arrayty = loadty[0]->exec(ctx,mod,builder,variables,functions).gettype();
        array->setGiveBackPtr(true);
        auto v = array->exec(ctx,mod,builder,variables,functions);
        if (!v.gettype()->isPointerTy()){
            auto gtmp = gettmp();
            alloc_instruction(gtmp,nullptr,v.gettype(),nullptr).exec(ctx,mod,builder,variables,functions);
            builder.CreateStore(v.getvalue(),variables[gtmp]);  
            v = InstructionContainer(variables[gtmp]);          
        }



        auto gep = builder.CreateGEP(arrayty, v.getvalue(),{builder.getInt32(0), index->exec(ctx,mod,builder,variables,functions).getvalue()},gettmp());

        if (givebackptr){
            return InstructionContainer(gep);
        }
        llvm::Value* load = builder.CreateLoad(elemty,gep,gettmp());
        
        if (elemty->isStructTy() || elemty->isArrayTy()){
            return InstructionContainer(gep);
        } 
        return InstructionContainer(load);
        
        
        }
        




    

};

struct access_pointer_instruction :  public Instruction {
    std::shared_ptr<Instruction> array;
    std::vector<std::shared_ptr<type_instruction>> loadty;
    std::shared_ptr<Instruction> index;




    access_pointer_instruction(std::shared_ptr<Instruction> array,std::vector<std::shared_ptr<type_instruction>> loadty,std::shared_ptr<Instruction> index) :array(array),index(index),loadty(loadty){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
        // do something for empty lists
        llvm::Type* elemty = loadty[1]->exec(ctx,mod,builder,variables,functions).gettype();
        
        auto arrayty = loadty[0]->exec(ctx,mod,builder,variables,functions).gettype();
        array->setGiveBackPtr(true);
        auto v = array->exec(ctx,mod,builder,variables,functions);
        if (!v.gettype()->isPointerTy()){
            auto gtmp = gettmp();
            alloc_instruction(gtmp,nullptr,v.gettype(),nullptr).exec(ctx,mod,builder,variables,functions);
            builder.CreateStore(v.getvalue(),variables[gtmp]);  
            v = InstructionContainer(variables[gtmp]);          
        }



        auto gep = builder.CreateGEP(arrayty, v.getvalue(),{index->exec(ctx,mod,builder,variables,functions).getvalue()},gettmp());

        if (givebackptr){
            return InstructionContainer(gep);
        }
        llvm::Value* load = builder.CreateLoad(elemty,gep,gettmp());
        
        if (elemty->isStructTy() || elemty->isArrayTy()){
            return InstructionContainer(gep);
        } 
        return InstructionContainer(load);
        
        
        }
        




    

};


struct extend_instruction :  public Instruction {
    std::shared_ptr<Instruction> extender;
    std::vector<std::shared_ptr<Instruction>> extensions;




    extend_instruction(std::shared_ptr<Instruction> extender,std::vector<std::shared_ptr<Instruction>> extensions) :extender(extender),extensions(extensions){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                llvm::Value* ext = extender->exec(ctx,mod,builder,variables,functions).getvalue();
                std::vector<std::string> attache;
                auto extty = ext->getType()->getTypeID();
                auto cbn = builder.GetInsertBlock()->getName().str();
                auto exttmp = getName(ext);

                for (auto& exten : extensions){
                    if (auto method = std::dynamic_pointer_cast<function_instruction>(exten)){
                        std::string origname =  method->name;
                        
                        method->name = gettmp() + "method" + method->name;
                        llvm::Function * methfn = method->exec(ctx,mod,builder,variables,functions).getfunction(builder,nullptr);
                        functions[/*exttmp+*/std::to_string(extty) + "method" + origname] = methfn;
                        globals[method->name] = variables[method->name];
                        globaltypes[method->name] = types[method->name];
                        
                    } else if (auto cons = std::dynamic_pointer_cast<assign_instruction>(exten)){
                        cons->global = true;
                        std::string ncbnname = std::to_string(extty) + "field" + cons->name;
                        cons->name = /*exttmp + */std::to_string(extty) + "field" + cons->name; // for now stop
                        //std::cout << "spawned " << cons->name << std::endl;
                        cons->usename = true;
                        cons->exec(ctx,mod,builder,variables,functions);
                        globals[cons->name] = variables[cons->name]; // ncbnname should not be used once a better idea is thought up
                        globaltypes[cons->name] = types[cons->name];
                        attache.push_back(cons->name);
                    }
                }
                extmap[extty] = attache;


   
                 
                return InstructionContainer(nullptr);

                 }
};

struct access_extend_instruction :  public Instruction {
    std::shared_ptr<Instruction> extender;
    
    std::string extname;
    bool ismethod;
    int vid;


    access_extend_instruction(std::shared_ptr<Instruction> extender/*,std::shared_ptr<type_instruction> type*/,std::string extname,bool ismethod,int vid) :extender(extender)/*,type(type)*/, extname(extname), ismethod(ismethod),vid(vid){}
    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    llvm::Value* ext = extender->exec(ctx,mod,builder,variables,functions).getvalue();
                    std::string ty = ismethod ? "method" : "field";
                    
                    int extty = ext->getType()->getTypeID();
                    if (ext->getType()->isPointerTy()){
                        extty = selfty; // this is such a dumb way of doing things....
                    }
                    std::string vidStr = "";
                    if (vid != 0){
                        vidStr = "__" + std::to_string(vid);
                    }

                    
                    
                    
                    auto load = load_instruction( vidStr +std::to_string(extty) + ty + extname);
                    load.setGiveBackPtr(givebackptr);
                    
                    return load.exec(ctx,mod,builder,variables,functions);
                 }
};


struct size_of_instruction :  public Instruction {
    std::shared_ptr<Instruction> obj;



    size_of_instruction(std::shared_ptr<Instruction> obj) :obj(obj)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    uint64_t sz = DL->getTypeAllocSize(obj->exec(ctx,mod,builder,variables,functions).gettype());
                    return resource_instruction("u64",sz).exec(ctx,mod,builder,variables,functions);
                 }
};

struct stack_size_instruction :  public Instruction {



    stack_size_instruction(){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    return resource_instruction("u64",StackSize).exec(ctx,mod,builder,variables,functions);
                 }
};

struct type_id_instruction :  public Instruction {
    std::shared_ptr<Instruction> obj;
    std::shared_ptr<type_instruction> ty;



    type_id_instruction(std::shared_ptr<Instruction> obj,std::shared_ptr<type_instruction> ty=nullptr) :obj(obj),ty(ty){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    uint64_t nsz = 0;
                    if (ty){
                        nsz = ty->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID();
                        //std::cout << "CpF (" << obj->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID() << "," << ty->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID() << ")" << std::endl;
                    }
                    uint64_t sz = obj->exec(ctx,mod,builder,variables,functions).gettype()->getTypeID();
                    sz = debugCPf(sz,nsz);
                    return resource_instruction("u64",sz).exec(ctx,mod,builder,variables,functions);
                 }
};

struct alt_assign_instruction :  public Instruction {
    std::shared_ptr<Instruction> assignee;
    std::shared_ptr<Instruction> assigned;



    alt_assign_instruction(std::shared_ptr<Instruction> assignee,std::shared_ptr<Instruction> assigned) :assignee(assignee),assigned(assigned)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    assignee->setGiveBackPtr(true);
                    auto tstfrm = assignee->exec(ctx,mod,builder,variables,functions).getvalue();
                    
                    auto strval = assigned->exec(ctx,mod,builder,variables,functions).getvalue();
                    builder.CreateStore(strval,tstfrm);
                    return InstructionContainer(nullptr);
                 }
};



struct asm_instruction :  public Instruction {
    std::shared_ptr<resource_instruction> ASMstring;
    std::vector<std::string> regs;
    std::unordered_map<std::string,std::shared_ptr<Instruction>> in;
    std::unordered_map<std::string,std::shared_ptr<Instruction>> out;
    std::unordered_map<std::string,std::shared_ptr<Instruction>> inout;



    asm_instruction(std::shared_ptr<resource_instruction> ASMstring,std::vector<std::string> regs,std::unordered_map<std::string,std::shared_ptr<Instruction>> in,std::unordered_map<std::string,std::shared_ptr<Instruction>> out,std::unordered_map<std::string,std::shared_ptr<Instruction>> inout) :ASMstring(ASMstring),regs(regs),in(in),out(out),inout(inout)/*,type(type)*/{}
    std::unordered_map<std::string,llvm::Value*> genV(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions,std::unordered_map<std::string,std::shared_ptr<Instruction>> v ){
        std::unordered_map<std::string,llvm::Value*> V;
        for (auto& k: v){
            V[k.first] = k.second->exec(ctx,mod,builder,variables,functions).getvalue();
        }
        return V;
    }
    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    auto res = emitInlineAsm(builder,&mod,std::any_cast<std::string>(ASMstring->value),regs,genV(ctx,mod,builder,variables,functions,in),genV(ctx,mod,builder,variables,functions,out),genV(ctx,mod,builder,variables,functions,inout));
                    for (auto& set: res){
                        assign_instruction(set.first,std::make_shared<value_instruction>(set.second)).exec(ctx,mod,builder,variables,functions);
                        

                    }
                    return InstructionContainer(nullptr);
                 }
};

struct ptrtoint_instruction :  public Instruction {
    std::shared_ptr<Instruction> val;




    ptrtoint_instruction(std::shared_ptr<Instruction> val) :val(val)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    return InstructionContainer(builder.CreatePtrToInt(val->exec(ctx,mod,builder,variables,functions).getvalue(),builder.getInt64Ty(),gettmp()));
                 }
};

struct tag_instruction :  public Instruction {
    std::shared_ptr<function_instruction> val;




    tag_instruction(std::shared_ptr<function_instruction> val) :val(val)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    std::string origname = val->name;
                    val->name = gettmp();
                    auto v = val->exec(ctx,mod,builder,variables,functions).getvalue();
                    auto fn = functions[val->name];
                    auto tyid = llvm::dyn_cast<llvm::ConstantInt>(type_id_instruction(val->argTypes[0]).exec(ctx,mod,builder,variables,functions).getvalue())->getSExtValue();
                    functions[origname + std::to_string(tyid)] = functions[val->name];
                    fn->setName(origname + std::to_string(tyid));
                    functions.erase(val->name);
                    return InstructionContainer(v);
                 }
};

struct get_tag_instruction :  public Instruction {
    std::shared_ptr<load_instruction> val;
    std::shared_ptr<type_instruction> type;




    get_tag_instruction(std::shared_ptr<load_instruction> val, std::shared_ptr<type_instruction> type) :val(val),type(type){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    auto tyid = llvm::dyn_cast<llvm::ConstantInt>(type_id_instruction(type).exec(ctx,mod,builder,variables,functions).getvalue())->getSExtValue();
                    val->name = val->name + std::to_string(tyid);
                    return val->exec(ctx,mod,builder,variables,functions);
                 }
};

struct chain_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction>> vec;




    chain_instruction(std::vector<std::shared_ptr<Instruction>> vec) :vec(vec)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, MetadataIRBuilder &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) override {
                    for (auto& inst : vec){
                        inst->exec(ctx,mod,builder,variables,functions);
                    }
                 return InstructionContainer(nullptr);
                 }
                 
};

class Instructor {
    public:
    llvm::LLVMContext context;
    llvm::Module module;
    MetadataIRBuilder builder;
    std::unordered_map<std::string, llvm::Value *> variables;
    std::unordered_map<std::string, llvm::Function *> functions;
    std::unique_ptr<llvm::TargetMachine> targetMachine;


    Instructor(std::string modulename) : module(modulename, context), builder(context) {}
    std::string inittarget(std::string target, std::string reloc="PIC"){ 
    if (!DL){
        DL = std::make_shared<llvm::DataLayout>(&module);
    }
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
    std::string errorr;

    if (target == "native"){
        target = llvm::sys::getDefaultTargetTriple();
    }
    
    llvm::StringRef targetTripleRef(target);
    
    const llvm::Target *targettype = llvm::TargetRegistry::lookupTarget(target, errorr);
    if (!targettype) {
        std::cout << "Error: Unable to find target: " << target << "\n";
        exit(0);
    }


    llvm::TargetOptions options;
    if (reloc == "relocstatic"){
        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(targettype->createTargetMachine(target, "generic", "", options, llvm::Reloc::Static));
    }
    else if (reloc == "relocrwpi"){

        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(targettype->createTargetMachine(target, "generic", "", options, llvm::Reloc::RWPI));
    }
    else if (reloc == "relocropi"){

        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(targettype->createTargetMachine(target, "generic", "", options, llvm::Reloc::ROPI));
    }
    else if (reloc == "relocropirwpi"){

        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(targettype->createTargetMachine(target, "generic", "", options, llvm::Reloc::ROPI_RWPI));
    }
    else if (reloc == "relocdynnopic"){

        this->targetMachine = std::unique_ptr<llvm::TargetMachine>(targettype->createTargetMachine(target, "generic", "", options, llvm::Reloc::DynamicNoPIC));
    } else {

    this->targetMachine = std::unique_ptr<llvm::TargetMachine>(targettype->createTargetMachine(target, "generic", "", options, llvm::Reloc::PIC_));
    }
    module.setDataLayout(targetMachine->createDataLayout());
    module.setTargetTriple(target);
    return target;
    }

    
    void executeInstruction(std::shared_ptr<Instruction>inst) {
        inst->exec(context, module, builder,variables,functions);
    }

    void buildAndOutput(const std::string &filename,std::unordered_map<std::string,bool> options={}) {
            std::error_code EC;
            std::error_code error;
            if (options.find("emit_llvm-ir") != options.end()){
            llvm::raw_fd_ostream file(filename + ".ll", error, llvm::sys::fs::OF_None);
            
            module.print(file, nullptr);
            }
            if (options.find("no_verify_llvm-ir") == options.end()){
            if (verifyModule(module,&llvm::errs())) {
                llvm::errs() << "Error in module verification!\n";
                exit(0);
            }
            }
            
            llvm::raw_fd_ostream dest(filename + ".o", EC,llvm::sys::fs::OF_None);
            

            llvm::legacy::PassManager pass;
            if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
                llvm::errs() << "Target machine can't emit a file of this type";
                return;
            }
            pass.run(module);
            dest.flush();
    }
    private:
};


int example() {
    Instructor interpreter("test");
    interpreter.inittarget("native","PIC");

    llvm::LLVMContext &context = interpreter.context;
    llvm::Type *i32Type = llvm::Type::getInt32Ty(context);

    auto val1 = std::make_shared<resource_instruction>("i32",42);

    auto val2 = std::make_shared<resource_instruction>("i32",13);
    auto loadx = load_instruction("x");

    assign_instruction assignInst("x", val1); // MAKE BLOCK INSTRUCTION
    //auto retinst = std::make_shared<return_instruction>(val2);
    std::vector<std::string> vecSTR{};
    
    operation_instruction opInst(std::make_shared<load_instruction>(loadx), val2 , "+");
    assign_instruction assignInst2("v", std::make_shared<operation_instruction>(opInst));
    load_instruction loadInst("v"); // create blocks later
    std::vector<std::shared_ptr<Instruction>> printfty{std::make_shared<resource_instruction>(std::string("RawString"),std::string("Ok!"))};
    auto printfdecl = std::make_shared<declare_function_instruction>(std::string("printf"),printfty,std::make_shared<resource_instruction>(std::string("i32"),13),true);
    auto loadpf = std::make_shared<load_instruction>("printf");
    loadpf->setGiveBackPtr(false);
    loadpf->getName();
    std::vector<std::shared_ptr<Instruction>> cargs{std::make_shared<resource_instruction>(std::string("RawString"),std::string("Hello world test var: %s\n")),std::make_shared<resource_instruction>(std::string("RawString"),std::string("Hah"))};
    auto cinst = std::make_shared<call_instruction>(loadpf,cargs,nullptr);
    auto retinst = std::make_shared<return_instruction>(cinst);
    auto pointerinst = std::make_shared<pointer_instruction>(val2);
    auto bptrinst = std::make_shared<borrowed_pointer_instruction>(pointerinst);
    auto derefinst = std::make_shared<dereference_instruction>(pointerinst,std::make_shared<type_instruction>(nullptr));
    auto ptrstoreinst = std::make_shared<pointer_store_instruction>(pointerinst,val2);
    auto str = std::string("");
    auto structdeclinst = std::make_shared<struct_decl_instruction>(str,printfty);
    std::vector<std::string> strvec{};
    auto strinstinst = std::make_shared<struct_instance_instruction>(structdeclinst,cargs,strvec,false,0);
    std::vector<std::shared_ptr<type_instruction>> vec02{};
    auto asinst = std::make_shared<access_struct_member_instruction>(strinstinst,"x",0,vec02);
    auto tupleinst = std::make_shared<tuple_instruction>(cargs);
    auto ati = std::make_shared<access_tuple_member_instruction>(tupleinst,0,vec02);
    auto cstinst = std::make_shared<cast_instruction>(nullptr,nullptr);
    auto winst = std::make_shared<while_instruction>(nullptr,nullptr);
    auto aainst = std::make_shared<array_instruction>(cargs,0);
    auto aainst2 = std::make_shared<access_array_instruction>(aainst,vec02,nullptr);
    auto extinst = std::make_shared<extend_instruction>(nullptr,printfty);
    auto aextinst = std::make_shared<access_extend_instruction>(nullptr,"",false,0);
    auto szinst = std::make_shared<size_of_instruction>(nullptr);
    auto aaainst = std::make_shared<alt_assign_instruction>(nullptr,nullptr);
    auto tyinst = std::make_shared<type_id_instruction>(nullptr);
    std::unordered_map<std::string,std::shared_ptr<Instruction>> xy{};
    auto asminst = std::make_shared<asm_instruction>(nullptr,vecSTR,xy,xy,xy);
    auto p2i = std::make_shared<ptrtoint_instruction>(nullptr);
    std::make_shared<chain_instruction>(cargs);
    std::make_shared<stack_size_instruction>();
    std::make_shared<value_instruction>(nullptr);
    std::make_shared<access_pointer_instruction>(nullptr,vec02,nullptr);
    std::make_shared<tag_instruction>(nullptr);
    std::make_shared<get_tag_instruction>(nullptr,nullptr);
    std::make_shared<nop_instruction>();
    
    
    block_instruction blk4(true,{std::make_shared<assign_instruction>(assignInst),std::make_shared<assign_instruction>(assignInst2),printfdecl,cinst,retinst},nullptr);
    auto ifinst = std::make_shared<if_instruction>(val2,std::make_shared<block_instruction>(blk4));
    function_instruction fninst("main",std::make_shared<resource_instruction>(resource_instruction("i32",int32_t(10))),{},std::make_shared<block_instruction>(blk4),true,false,false);
    interpreter.executeInstruction(std::make_shared<function_instruction>(fninst));
    //interpreter.executeInstruction(&opInst);
    //interpreter.executeInstruction(&loadInst);

    interpreter.buildAndOutput("test.o");
    //std::cout << "Finished build" << std::endl;

    return 0;
}
