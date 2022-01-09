/**
 * @file CodeGenerator.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "CodeGenerator.hpp"

void CodeGenerator::generateAssembly(AssemblyOutput &out, const AST &ast, SymbolTable &symbolTable) {
    out.preamble();
    assignGlobals(out, symbolTable);
    for (const auto &node: ast.nodes) {
        generateAssembly(out, symbolTable, node);
    }
    out.finalize();
}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const SymbolTable &symbolTable, const AST::MutNodePtr &node) {
    using enum ASTNodeType;
    switch (node->getType()) {
        case ArithmeticExpression:
            generateAssembly(out, symbolTable, *std::dynamic_pointer_cast<ArithmeticExpressionNode>(node));
            return;
        case MethodCall:
            generateAssembly(out, symbolTable, *std::dynamic_pointer_cast<MethodCallNode>(node));
            return;
        case VariableDeclaration:
            generateAssembly(out, symbolTable, *std::dynamic_pointer_cast<VariableDeclarationNode>(node));
            return;
        case VariableAssignment:
            generateAssembly(out, symbolTable, *std::dynamic_pointer_cast<VariableAssignmentNode>(node));
            return;
        case Constant:
            generateAssembly(out, symbolTable, *std::dynamic_pointer_cast<IntegerConstantNode>(node));
            return;
        case VariableAccess:
            generateAssembly(out, symbolTable, *std::dynamic_pointer_cast<VariableAccessNode>(node));
            return;
    }

    throw std::runtime_error("Unknown node type");
}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const SymbolTable &symbolTable,
                                     const ArithmeticExpressionNode &node) {
    out.comment("LHS:");
    generateAssembly(out, symbolTable, node.lhs);
    out.comment("RHS:");
    generateAssembly(out, symbolTable, node.rhs);
    out.comment("OP:");
    using Operation = ArithmeticExpressionNode::Operation;
    switch (node.op) {
        case Operation::PLUS:
            out.stackAdd16();
            break;
        case Operation::MINUS:
        case Operation::DIV:
        case Operation::MULT:
        case Operation::XOR:
            throw std::runtime_error{"not implemented"};
    }
}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const SymbolTable &symbolTable, const MethodCallNode &node) {
    out.comment("Evaluate arguments:");
    for (const auto &arg: node.argumentList) {
        generateAssembly(out, symbolTable, arg);
    }

    out.comment("Call func:");
    if (node.name == "printWord") {
        out.pop16BitReg(Reg16::HL);
        out.call("printWord");
    } else {
        throw std::runtime_error{"Unknown function"};
    }
}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const SymbolTable &symbolTable,
                                     const VariableDeclarationNode &node) {

    // Evaluate RHS for initialization
    out.comment(fmt::format("Evaluate RHS for initialization of {}", node.name));
    generateAssembly(out, symbolTable, node.rhs);
    auto addr = addressOfGlobal(node.name, symbolTable);
    out.comment(fmt::format("Initializing {} at address {:#x}", node.name, addr));
    out.pop16ToAddr(Address{.a=addr});
}

void CodeGenerator::generateAssembly(AssemblyOutput &/*out*/, const SymbolTable &/*symbolTable*/,
                                     const VariableAssignmentNode &/*node*/) {
    throw std::runtime_error{"Code generation for variable assignment not implemented"};

}

void
CodeGenerator::generateAssembly(AssemblyOutput &out, const SymbolTable &/*symbolTable*/, const IntegerConstantNode &node) {
    // TODO: Range
    out.push16BitConst(node.value);

}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const SymbolTable &symbolTable,
                                     const VariableAccessNode &node) {

    auto addr = addressOfGlobal(node.name, symbolTable);
    out.comment(fmt::format("Reading {} from address {:#x}", node.name, addr));
    out.push16FromAddr(Address{.a=addr});
}

void CodeGenerator::assignGlobals(AssemblyOutput &/*out*/, SymbolTable &symbolTable) {
    int nextFree = 0xC000;
    constexpr int lastAddress = 0xDFFF;
    for (auto &[id, s]: symbolTable.table) {
        if (s->getType() == DeclType::Variable) {
            auto addr = nextFree;
            nextFree += 2;
            if (nextFree > lastAddress + 1) {
                throw std::runtime_error{"Too many globals!"};
            }
            fmt::print("Global \"{}\" at {:#x}\n", id, addr);
            auto decl = dynamic_cast<VariableDeclaration *>(s.get());
            decl->address = addr;
        }
    }
}

uint16_t CodeGenerator::addressOfGlobal(const std::string &id, const SymbolTable &symbolTable) {
    auto varDecl = dynamic_cast<const VariableDeclaration *>(symbolTable.lookup(id).value()->second.get());
    return varDecl->address;
}
