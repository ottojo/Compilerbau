/**
 * @file CodeGenerator.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "CodeGenerator.hpp"
#include "ScopeIndent.hpp"

void CodeGenerator::generateAssembly() {
    out.preamble();
    assignGlobals();
    for (const auto &node: ast.nodes) {
        generateAssembly(node);
    }
    out.finalize();
}

void CodeGenerator::generateAssembly(const AST::MutNodePtr &node) {
    using enum ASTNodeType;
    switch (node->getType()) {
        case ArithmeticExpression:
            generateAssembly(*std::dynamic_pointer_cast<ArithmeticExpressionNode>(node));
            return;
        case MethodCall:
            generateAssembly(*std::dynamic_pointer_cast<MethodCallNode>(node));
            return;
        case VariableDeclaration:
            generateAssembly(*std::dynamic_pointer_cast<VariableDeclarationNode>(node));
            return;
        case VariableAssignment:
            generateAssembly(*std::dynamic_pointer_cast<VariableAssignmentNode>(node));
            return;
        case Constant:
            generateAssembly(*std::dynamic_pointer_cast<IntegerConstantNode>(node));
            return;
        case VariableAccess:
            generateAssembly(*std::dynamic_pointer_cast<VariableAccessNode>(node));
            return;
    }

    throw std::runtime_error("Unknown node type");
}

void CodeGenerator::generateAssembly(const ArithmeticExpressionNode &node) {
    out.comment("Arithmetic expression:");
    ScopeIndent i(out);
    {
        out.comment("LHS:");
        ScopeIndent i2(out);
        generateAssembly(node.lhs);
    }
    {
        out.comment("RHS:");
        ScopeIndent i2(out);
        generateAssembly(node.rhs);
    }
    {
        out.comment("OP:");
        ScopeIndent i2(out);
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
}

void CodeGenerator::generateAssembly(const MethodCallNode &node) {
    out.comment(fmt::format("Function call to {}", node.name));
    ScopeIndent i(out);
    {
        out.comment("Evaluate arguments:");
        ScopeIndent i2(out);
        for (const auto &arg: node.argumentList) {
            generateAssembly(arg);
        }
    }
    {
        out.comment("Call func:");
        ScopeIndent i2(out);
        if (node.name == "printWord") {
            out.pop16BitReg(Reg16::HL);
            out.call("printWord");
        } else {
            throw std::runtime_error{"Unknown function"};
        }
    }
}

void CodeGenerator::generateAssembly(const VariableDeclarationNode &node) {
    out.comment(fmt::format("Initialize {}", node.name));
    ScopeIndent i(out);
    {
        out.comment(fmt::format("Evaluate RHS for initialization of {}", node.name));
        ScopeIndent i2(out);
        generateAssembly(node.rhs);
    }
    auto addr = addressOfGlobal(node.name, symbolTable);
    {
        out.comment(fmt::format("Initializing {} at address {:#x}", node.name, addr));
        ScopeIndent i2(out);
        out.pop16ToAddr(Address{.a=addr});
    }
}

void CodeGenerator::generateAssembly(const VariableAssignmentNode &/*node*/) {
    throw std::runtime_error{"Code generation for variable assignment not implemented"};
}

void
CodeGenerator::generateAssembly(const IntegerConstantNode &node) {
    // TODO: Range
    out.push16BitConst(node.value);

}

void CodeGenerator::generateAssembly(const VariableAccessNode &node) {

    auto addr = addressOfGlobal(node.name, symbolTable);
    out.comment(fmt::format("Reading {} from address {:#x}", node.name, addr));
    ScopeIndent i(out);
    out.push16FromAddr(Address{.a=addr});
}

void CodeGenerator::assignGlobals() {
    int nextFree = 0xC000;
    constexpr int lastAddress = 0xDFFF;
    for (auto &[id, s]: symbolTable.table) {
        if (s->getType() == DeclType::Variable) {
            auto addr = nextFree;
            nextFree += 2;
            if (nextFree > lastAddress + 1) {
                throw std::runtime_error{"Too many globals!"};
            }
            auto decl = dynamic_cast<VariableDeclaration *>(s.get());
            decl->address = addr;
        }
    }
}

uint16_t CodeGenerator::addressOfGlobal(const std::string &id, const SymbolTable &symbolTable) {
    auto varDecl = dynamic_cast<const VariableDeclaration *>(symbolTable.lookup(id).value()->second.get());
    return varDecl->address;
}

CodeGenerator::CodeGenerator(AssemblyOutput &out, const AST &ast, SymbolTable &symbolTable) :
        out(out),
        ast(ast),
        symbolTable(symbolTable) {}

