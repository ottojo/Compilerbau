/**
 * @file CodeGenerator.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "CodeGenerator.hpp"

void CodeGenerator::generateAssembly(AssemblyOutput &out, const AST &ast) {
    out.preamble();
    for (const auto &node: ast.nodes) {
        generateAssembly(out, node);
    }
    out.finalize();
}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const AST::MutNodePtr &node) {
    using enum ASTNodeType;
    switch (node->getType()) {
        case ArithmeticExpression:
            generateAssembly(out, *std::dynamic_pointer_cast<ArithmeticExpressionNode>(node));
            return;
        case MethodCall:
            generateAssembly(out, *std::dynamic_pointer_cast<MethodCallNode>(node));
            return;
        case VariableDeclaration:
            generateAssembly(out, *std::dynamic_pointer_cast<VariableDeclarationNode>(node));
            return;
        case VariableAssignment:
            generateAssembly(out, *std::dynamic_pointer_cast<VariableAssignmentNode>(node));
            return;
        case Constant:
            generateAssembly(out, *std::dynamic_pointer_cast<IntegerConstantNode>(node));
            return;
        case VariableAccess:
            generateAssembly(out, *std::dynamic_pointer_cast<VariableAccessNode>(node));
            return;
    }

    throw std::runtime_error("Unknown node type");
}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const ArithmeticExpressionNode &node) {
    out.comment("LHS:");
    generateAssembly(out, node.lhs);
    out.comment("RHS:");
    generateAssembly(out, node.rhs);
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

void CodeGenerator::generateAssembly(AssemblyOutput &out, const MethodCallNode &node) {
    out.comment("Evaluate arguments:");
    for (const auto &arg: node.argumentList) {
        generateAssembly(out, arg);
    }

    out.comment("Call func:");
    if (node.name == "printWord") {
        out.pop16BitReg(Reg16::HL);
        out.call("printWord");
    } else {
        throw std::runtime_error{"Unknown function"};
    }
}

void CodeGenerator::generateAssembly(AssemblyOutput &/*out*/, const VariableDeclarationNode &/*node*/) {
    //throw std::runtime_error{"Code generation for variable decl not implemented"};
}

void CodeGenerator::generateAssembly(AssemblyOutput &/*out*/, const VariableAssignmentNode &/*node*/) {
    //throw std::runtime_error{"Code generation for variable assignment not implemented"};

}

void CodeGenerator::generateAssembly(AssemblyOutput &out, const IntegerConstantNode &node) {
    // TODO: Range
    out.push16BitConst(node.value);

}

void CodeGenerator::generateAssembly(AssemblyOutput &/*out*/, const VariableAccessNode &/*node*/) {
    //throw std::runtime_error{"Code generation for variable access not implemented"};
}
