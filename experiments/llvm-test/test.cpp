#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

int main() {
    llvm::LLVMContext context;
    llvm::Module module("my_module", context);
    llvm::IRBuilder<> builder(context);

    // Create a function to hold code
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

    // Create a basic block in the function
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    // Generate code for 'return 0'
    builder.CreateRet(builder.getInt32(0));

    // Verify the module
    llvm::verifyModule(module, &llvm::errs());

    // Output the IR to a file
    std::error_code ec;
    llvm::raw_fd_ostream dest("output.ll", ec, llvm::sys::fs::OF_None);
    if (!ec) {
        module.print(dest, nullptr);
        dest.flush();
        llvm::outs() << "LLVM IR code has been generated in output.ll\n";
    } else {
        llvm::errs() << "Error opening file: " << ec.message() << "\n";
        return 1;
    }

    return 0;
}
