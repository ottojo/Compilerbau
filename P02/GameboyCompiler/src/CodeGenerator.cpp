/**
 * @file CodeGenerator.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include <gbc/CodeGenerator.hpp>
#include <gbc/ScopeIndent.hpp>
#include <fmt/ranges.h>

constexpr auto FUNC_PREFIX = "func_";
constexpr auto GLOBAL_VAR_PREFIX = "global_";

void CodeGenerator::generateAssembly() {
    out.preamble();
    assignGlobals();
    generateMain();
    generateFuncs();
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

void CodeGenerator::generateAssembly(const FunctionCallNode &node) {
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
    // TODO: Local variables
    if (node.decl->nestingLevel == 0) {
        throw std::runtime_error{"wtf, a declatation for global " + node.name};
    }

    uint16_t addr = 7;// addressOfGlobal(node.name, *ast.symbolTable);
    {
        out.comment(fmt::format("Initializing {} at address {:#x}", node.name, addr));
        ScopeIndent i2(out);
        out.pop16ToMemory(NumericAddress{.a=addr});
    }
}

void CodeGenerator::generateAssembly(const VariableAssignmentNode &node) {
    // TODO throw std::runtime_error{"Code generation for variable assignment not implemented"};
    out.comment(fmt::format("Assign {}", node.name));
    ScopeIndent i(out);
    if (node.decl->nestingLevel == 0) {
        {
            out.comment(fmt::format("Evaluate RHS for assignment of {}", node.name));
            ScopeIndent i2(out);
            node.rhs->visit([this](auto &node) { generateAssembly(node); });
        }
        out.pop16ToMemory(GLOBAL_VAR_PREFIX + node.name);
    } else {
        out.comment("Local variable assignment not implemented");
        fmt::print(stderr, "Local variable assignment not implemented\n");
    }

}

void CodeGenerator::generateAssembly(const IntegerConstantNode &node) {
    // TODO: Range
    out.push16BitConst(node.value);

}

void CodeGenerator::generateAssembly(const VariableAccessNode &node) {
    if (node.decl->nestingLevel == 0) {
        out.comment(fmt::format("Reading global {}", node.name));
        ScopeIndent i(out);
        out.push16FromMemory(GLOBAL_VAR_PREFIX + node.name);
    } else {
        // TODO: Local variables
        out.comment("Local variable access not implemented.");
    }
}

void CodeGenerator::assignGlobals() {
    out.ramSection("Global Variables");
    for (const auto &g: ast.globalVariableDeclarationNodes) {
        // TODO (type analysis): Use correct size of type
        out.defineVariable(GLOBAL_VAR_PREFIX + g->name, 2);
    }
    out.sectionEnd();
}

CodeGenerator::CodeGenerator(AssemblyOutput &out, const AST &ast) :
        out(out),
        ast(ast) {}


void CodeGenerator::generateAssembly(const FunctionDefinitionNode &node) {
    if (node.builtin) {
        return;
    }
    out.comment(fmt::format("{}({}) -> {}", node.name, fmt::join(node.arguments, ", "),
                            node.returnTypeName.value_or("<void>")));
    out.sectionWithLabel(FUNC_PREFIX + node.name);

    for (const auto &stmt: node.methodBody) {
        stmt->visit([this](auto &node) { generateAssembly(node); });
    }

    out.sectionEnd();
    // TODO
}

void CodeGenerator::generateMain() {
    out.sectionWithLabel("main");
    {
        ScopeIndent s{out};
        initializeGlobalVars();
        // FunctionCallNode mainCall(SourceLocation(), "main", {});
        // mainCall.builtinMethod = false;
        // generateAssembly(mainCall);
        out.call(FUNC_PREFIX + std::string("main"));
    }

    out.ret();
    out.sectionEnd();
}

void CodeGenerator::initializeGlobalVars() {
    out.comment("--- Global Variables ---");
    ScopeIndent s(out);
    // Only generate assignment of initial value
    for (const auto &v: ast.globalVariableDeclarationNodes) {
        VariableAssignmentNode init(v->loc, v->name, v->rhs);
        init.decl = v->decl;
        generateAssembly(init);
    }
}

void CodeGenerator::generateFuncs() {
    out.comment("--- Functions ---");
    ScopeIndent s(out);
    for (const auto &v: ast.functionDefinitionNodes) {
        generateAssembly(*v);
    }
}

