/**
 * @file CodeGenerator.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include <gbc/CodeGenerator.hpp>
#include <gbc/ScopeIndent.hpp>
#include <fmt/ranges.h>

void CodeGenerator::generateAssembly() {
    out.preamble();
    assignGlobals();
    ast.traverse([this](auto &node) { generateAssembly(node); });
    out.finalize();
}

void CodeGenerator::generateAssembly(const ArithmeticExpressionNode &node) {
    out.comment("Arithmetic expression:");
    ScopeIndent i(out);
    {
        out.comment("LHS:");
        ScopeIndent i2(out);
        node.lhs->visit([this](auto &node) { generateAssembly(node); });
    }
    {
        out.comment("RHS:");
        ScopeIndent i2(out);
        node.rhs->visit([this](auto &node) { generateAssembly(node); });
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
            arg->visit([this](auto &node) { generateAssembly(node); });
        }
    }
    // Arguments are now on stack
    {
        out.comment("Call func:");
        ScopeIndent i2(out);

        if (node.builtinMethod) {
            out.pop16BitReg(Reg16::HL);
            out.call(node.name);
        } else {

        }
    }
}

void CodeGenerator::generateAssembly(const VariableDeclarationNode &node) {
    out.comment(fmt::format("Initialize {}", node.name));
    ScopeIndent i(out);
    {
        out.comment(fmt::format("Evaluate RHS for initialization of {}", node.name));
        ScopeIndent i2(out);
        node.rhs->visit([this](auto &node) { generateAssembly(node); });
    }
    auto addr = addressOfGlobal(node.name, *ast.symbolTable);
    {
        out.comment(fmt::format("Initializing {} at address {:#x}", node.name, addr));
        ScopeIndent i2(out);
        out.pop16ToAddr(Address{.a=addr});
    }
}

void CodeGenerator::generateAssembly(const VariableAssignmentNode &/*node*/) {
    throw std::runtime_error{"Code generation for variable assignment not implemented"};
}

void CodeGenerator::generateAssembly(const IntegerConstantNode &node) {
    // TODO: Range
    out.push16BitConst(node.value);

}

void CodeGenerator::generateAssembly(const VariableAccessNode &node) {

    auto addr = addressOfGlobal(node.name, *ast.symbolTable);
    out.comment(fmt::format("Reading {} from address {:#x}", node.name, addr));
    ScopeIndent i(out);
    out.push16FromAddr(Address{.a=addr});
}

void CodeGenerator::assignGlobals() {
    int nextFree = 0xC000;
    constexpr int lastAddress = 0xDFFF;
    for (auto &[id, s]: ast.symbolTable->table) {
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

CodeGenerator::CodeGenerator(AssemblyOutput &out, const AST &ast) :
        out(out),
        ast(ast) {}


void CodeGenerator::generateAssembly(const MethodDefinitionNode &node) {
    // TODO: Functions not in main section
    out.comment(fmt::format("{}({}) -> {}", node.name, fmt::join(node.arguments, ", "),
                            node.returnTypeName.value_or("<void>")));
    out.sectionWithLabel("_func_" + node.name);

    for (const auto &stmt: node.methodBody) {
        stmt->visit([this](auto &node) { generateAssembly(node); });
    }

    out.sectionEnd();
    // TODO
}

