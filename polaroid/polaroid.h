#ifndef INSTRUCTION_CONTAINER_H
#define INSTRUCTION_CONTAINER_H

#include <any>
#include <string>
#include <vector>
#include <unordered_map>
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



class InstructionContainer {
public:
    std::any value;
    InstructionContainer(std::any value);
    llvm::Value* getvalue();
    llvm::Function* getfunction();
};

struct Instruction {
    bool stored=false;
    bool givebackptr = false;
    std::shared_ptr<InstructionContainer> storedv = nullptr;
    virtual InstructionContainer execute(llvm::LLVMContext &context, llvm::Module &module, llvm::IRBuilder<> &builder,
                         std::unordered_map<std::string, llvm::Value *> &variables , std::unordered_map<std::string, llvm::Function *> & functions)  = 0;
    virtual InstructionContainer exec(llvm::LLVMContext &context, llvm::Module &module, llvm::IRBuilder<> &builder,
                         std::unordered_map<std::string, llvm::Value *> &variables , std::unordered_map<std::string, llvm::Function *> & functions);
    virtual void setGiveBackPtr(bool b){

        this->givebackptr = b;
    }
    virtual ~Instruction() = default;
};

struct nop_instruction :  public Instruction {





    nop_instruction() /*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct resource_instruction : public Instruction {
    std::string type;
    std::any value;

    resource_instruction(const std::string &type, std::any v);

    InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, llvm::IRBuilder<> &builder,
        std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> &functions) override;
};

struct type_instruction : public Instruction {
    llvm::Type* type;
    llvm::Value* value;
    std::vector<std::shared_ptr<Instruction>> inst;
    std::string process;

    type_instruction(llvm::Type* type,llvm::Value* v=nullptr,std::vector<std::shared_ptr<Instruction>> inst={},std::string proc="") : type(type), value(v), inst(inst), process(proc) {}


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct assign_instruction : public Instruction {
    std::string name;
    std::shared_ptr<Instruction> value;
    bool global;

    assign_instruction(const std::string &n, std::shared_ptr<Instruction> v,bool global=false) : name(n), value(v), global(global) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};

struct operation_instruction : public Instruction {

    std::shared_ptr<Instruction>lhs, rhs;
    std::string op;

    operation_instruction(std::shared_ptr<Instruction>l, std::shared_ptr<Instruction>r, std::string o)
        : lhs(l), rhs(r), op(o) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};

struct load_instruction : public Instruction {
    std::string name;
    bool isfromassign;
    bool loadasptr=false;

    load_instruction(const std::string &n,bool isfromassign=false) : name(n), isfromassign(isfromassign){
    }
    std::string getName();
    void setLAP(bool lap);


    std::vector<llvm::BasicBlock*> reverseBlockOrder( llvm::IRBuilder<> &builder){

    }

    InstructionContainer execute(llvm::LLVMContext &, llvm::Module &, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};



struct cast_instruction : public Instruction {
    std::shared_ptr<Instruction> source;
    std::shared_ptr<type_instruction> dest;

    cast_instruction(std::shared_ptr<Instruction> source, std::shared_ptr<type_instruction> dest) : source(source), dest(dest) {}


    InstructionContainer execute(llvm::LLVMContext &ctx , llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct store_instruction : public Instruction {
    std::string name;
    std::shared_ptr<Instruction>value;

    std::vector<llvm::BasicBlock*> reverseBlockOrder( llvm::IRBuilder<> &builder){

    }
    store_instruction(const std::string &n, std::shared_ptr<Instruction>v) : name(n), value(v) {}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};

struct block_instruction : public Instruction {
    bool ismain;
    llvm::Function* mainfunction;
    std::vector<std::shared_ptr<Instruction>> insts;
    llvm::Function* specfunction;
    

    block_instruction(bool ismain,std::vector<std::shared_ptr<Instruction>> insts={},llvm::Function* mainfunction=nullptr) : ismain(ismain), mainfunction(mainfunction),insts(insts) {

    }

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};

struct if_instruction : public Instruction {
    std::shared_ptr<Instruction>condition;
    std::shared_ptr<block_instruction> thenInstructions;
    std::shared_ptr<block_instruction> elseInstructions;
    std::vector<std::shared_ptr<if_instruction>> elifs;
    bool isassign;

    if_instruction(std::shared_ptr<Instruction>cond,std::shared_ptr<block_instruction> then, std::shared_ptr<block_instruction> orelse=nullptr,std::vector<std::shared_ptr<if_instruction>> elifs={},bool isassign=false) : condition(cond), thenInstructions(then), elseInstructions(orelse),elifs(elifs), isassign(isassign) {}

InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, llvm::IRBuilder<> &builder,
             std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct while_instruction : public Instruction {
    std::shared_ptr<Instruction>condition;
    std::shared_ptr<block_instruction> block;
    bool isassign;


    while_instruction(std::shared_ptr<Instruction>cond,std::shared_ptr<block_instruction> block,bool isassign =false) : condition(cond), block(block), isassign(isassign) {}

InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, llvm::IRBuilder<> &builder,
             std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct return_instruction : public Instruction {
    std::shared_ptr<Instruction> value;

    return_instruction(std::shared_ptr<Instruction> value) : value(value) {

    }
    std::vector<llvm::BasicBlock*> reverseBlockOrder( llvm::IRBuilder<> &builder){
        auto begin = builder.GetInsertBlock()->getParent()->begin();
        auto end = builder.GetInsertBlock()->getParent()->end();
        std::vector<llvm::BasicBlock*> reversable;
        for (auto i = begin; i != end; i++){
            reversable.push_back(&*i);
        }
        return reversable;

    }

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions); };

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
    std::vector<llvm::BasicBlock*> reverseBlockOrder( llvm::IRBuilder<> &builder){
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

    InstructionContainer execute(llvm::LLVMContext &ctx, llvm::Module &mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> & variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};

struct declare_function_instruction : public Instruction {
    std::string name;
    std::vector<std::shared_ptr<Instruction>> argTypes;
    std::shared_ptr<Instruction> returnType;
    bool isvdic;

    declare_function_instruction(std::string name,std::vector<std::shared_ptr<Instruction>> &args,std::shared_ptr<Instruction> returnType,bool isvdic=false) : name(name), argTypes(args), returnType(returnType), isvdic(isvdic) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};
struct call_instruction : public Instruction {
    std::shared_ptr<Instruction> source;
    std::vector<std::shared_ptr<Instruction>> args;
    std::shared_ptr<type_instruction> fty;



    call_instruction(std::shared_ptr<Instruction> source,std::vector<std::shared_ptr<Instruction>> &args,std::shared_ptr<type_instruction>fty) :source(source), args(args), fty(fty) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions) ;};


struct pointer_instruction : public Instruction {
    std::shared_ptr<Instruction> ptrvalue;
    bool isborrow;




    pointer_instruction(std::shared_ptr<Instruction> ptrvalue,bool isborrow=false) :ptrvalue(ptrvalue), isborrow(isborrow) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};
struct borrowed_pointer_instruction :  public Instruction {
    std::shared_ptr<Instruction> ptr;




    borrowed_pointer_instruction(std::shared_ptr<Instruction> ptr) :ptr(ptr) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct dereference_instruction :  public Instruction {
    std::shared_ptr<Instruction> ptr;
    std::shared_ptr<type_instruction> type;



    dereference_instruction(std::shared_ptr<Instruction> ptr,std::shared_ptr<type_instruction> type) :ptr(ptr),type(type) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct pointer_store_instruction :  public Instruction {
    std::shared_ptr<Instruction> ptr;
    std::shared_ptr<Instruction> value;




    pointer_store_instruction(std::shared_ptr<Instruction> ptr,std::shared_ptr<Instruction> value) :ptr(ptr), value(value) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct struct_decl_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction> >fields;
    std::string name;
    bool ispacked;




    struct_decl_instruction(std::string& name,std::vector<std::shared_ptr<Instruction> > fields,bool ispacked=false) :name(name),fields(fields),ispacked(ispacked) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct struct_instance_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction> >fields;
    std::shared_ptr<Instruction> pstruct;
    std::vector<std::string> sfields;
    bool ispacked;
    int vid;




    struct_instance_instruction(std::shared_ptr<Instruction> pstruct,std::vector<std::shared_ptr<Instruction> > fields,std::vector<std::string> sfields,bool ispacked=false,int vid=0) :pstruct(pstruct),fields(fields),sfields(sfields),ispacked(ispacked),vid(vid) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};



struct access_struct_member_instruction :  public Instruction {
    std::shared_ptr<Instruction> structinstance;
    std::vector<std::shared_ptr<type_instruction>> types;
    std::string field;
    int id;
    bool assign=false;




    access_struct_member_instruction(std::shared_ptr<Instruction> structinstance,std::string field,int id,std::vector<std::shared_ptr<type_instruction>> types,bool assign=false) :structinstance(structinstance),field(field),id(id),types(types),assign(assign){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};



struct tuple_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction> >fields;




    tuple_instruction(std::vector<std::shared_ptr<Instruction> > fields) :fields(fields) {

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct access_tuple_member_instruction :  public Instruction {
    std::shared_ptr<Instruction> tupleinstance;
    std::vector<std::shared_ptr<type_instruction>> types;
    int id;




    access_tuple_member_instruction(std::shared_ptr<Instruction> tupleinstance,int id,std::vector<std::shared_ptr<type_instruction>> types) :tupleinstance(tupleinstance),id(id),types(types){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct array_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction>> elements;
    int sz;




    array_instruction(std::vector<std::shared_ptr<Instruction>> elements,int sz) :sz(sz),elements(elements){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct access_array_instruction :  public Instruction {
    std::shared_ptr<Instruction> array;
    std::vector<std::shared_ptr<type_instruction>> loadty;
    std::shared_ptr<Instruction> index;




    access_array_instruction(std::shared_ptr<Instruction> array,std::vector<std::shared_ptr<type_instruction>> loadty,std::shared_ptr<Instruction> index) :array(array),index(index),loadty(loadty){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};
struct extend_instruction :  public Instruction {
    std::shared_ptr<Instruction> extender;
    std::vector<std::shared_ptr<Instruction>> extensions;




    extend_instruction(std::shared_ptr<Instruction> extender,std::vector<std::shared_ptr<Instruction>> extensions) :extender(extender),extensions(extensions){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct access_pointer_instruction :  public Instruction {
    std::shared_ptr<Instruction> array;
    std::vector<std::shared_ptr<type_instruction>> loadty;
    std::shared_ptr<Instruction> index;




    access_pointer_instruction(std::shared_ptr<Instruction> array,std::vector<std::shared_ptr<type_instruction>> loadty,std::shared_ptr<Instruction> index) :array(array),index(index),loadty(loadty){

    }


    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};
struct access_extend_instruction :  public Instruction {
    std::shared_ptr<Instruction> extender;
    
    std::string extname;
    bool ismethod;
    int vid;


    access_extend_instruction(std::shared_ptr<Instruction> extender/*,std::shared_ptr<type_instruction> type*/,std::string extname,bool ismethod,int vid) :extender(extender)/*,type(type)*/, extname(extname), ismethod(ismethod),vid(vid){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct size_of_instruction :  public Instruction {
    std::shared_ptr<Instruction> obj;



    size_of_instruction(std::shared_ptr<Instruction> obj) :obj(obj)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct stack_size_instruction :  public Instruction {



    stack_size_instruction(){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct type_id_instruction :  public Instruction {
    std::shared_ptr<Instruction> obj;
    std::shared_ptr<type_instruction> ty;



    type_id_instruction(std::shared_ptr<Instruction> obj,std::shared_ptr<type_instruction> ty=nullptr) :obj(obj),ty(ty){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct alt_assign_instruction :  public Instruction {
    std::shared_ptr<Instruction> assignee;
    std::shared_ptr<Instruction> assigned;



    alt_assign_instruction(std::shared_ptr<Instruction> assignee,std::shared_ptr<Instruction> assigned) :assignee(assignee),assigned(assigned)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct asm_instruction :  public Instruction {
    std::shared_ptr<resource_instruction> ASMstring;
    std::vector<std::string> regs;
    std::unordered_map<std::string,std::shared_ptr<Instruction>> in;
    std::unordered_map<std::string,std::shared_ptr<Instruction>> out;
    std::unordered_map<std::string,std::shared_ptr<Instruction>> inout;



    asm_instruction(std::shared_ptr<resource_instruction> ASMstring,std::vector<std::string> regs,std::unordered_map<std::string,std::shared_ptr<Instruction>> in,std::unordered_map<std::string,std::shared_ptr<Instruction>> out,std::unordered_map<std::string,std::shared_ptr<Instruction>> inout) :ASMstring(ASMstring),regs(regs),in(in),out(out),inout(inout)/*,type(type)*/{}
    std::unordered_map<std::string,llvm::Value*> genV(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions,std::unordered_map<std::string,std::shared_ptr<Instruction>> v ){
        std::unordered_map<std::string,llvm::Value*> V;
        for (auto& k: v){
            V[k.first] = k.second->exec(ctx,mod,builder,variables,functions).getvalue();
        }
        return V;
    }
    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};


struct ptrtoint_instruction :  public Instruction {
    std::shared_ptr<Instruction> val;




    ptrtoint_instruction(std::shared_ptr<Instruction> val) :val(val)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct tag_instruction :  public Instruction {
    std::shared_ptr<function_instruction> val;




    tag_instruction(std::shared_ptr<function_instruction> val) :val(val)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct chain_instruction :  public Instruction {
    std::vector<std::shared_ptr<Instruction>> vec;




    chain_instruction(std::vector<std::shared_ptr<Instruction>> vec) :vec(vec)/*,type(type)*/{}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

struct get_tag_instruction :  public Instruction {
    std::shared_ptr<load_instruction> val;
    std::shared_ptr<type_instruction> type;




    get_tag_instruction(std::shared_ptr<load_instruction> val, std::shared_ptr<type_instruction> type) :val(val),type(type){}

    InstructionContainer execute(llvm::LLVMContext & ctx, llvm::Module & mod, llvm::IRBuilder<> &builder,
                 std::unordered_map<std::string, llvm::Value *> &variables, std::unordered_map<std::string, llvm::Function *> & functions);};

class Instructor {
    public:
    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;
    std::unordered_map<std::string, llvm::Value *> variables;
    std::unordered_map<std::string, llvm::Function *> functions;
    std::unique_ptr<llvm::TargetMachine> targetMachine;


    Instructor(std::string modulename) : module(modulename, context), builder(context) {}
    std::string inittarget(std::string target, std::string reloc="PIC");
    void executeInstruction(std::shared_ptr<Instruction>inst);
    void buildAndOutput(const std::string &filename,std::unordered_map<std::string,bool> options={});
    private:
};


#endif // INSTRUCTION_CONTAINER_H
