/**
 * @file AST.cpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#include "AST.hpp"

#include <utility>

ASTNodeType ArithmeticExpressionNode::getType() {
    return ArithmeticExpression;
}

void AST::generateCode(AssemblyOutput &out) {
    out.preamble();
    for (const auto &node: nodes) {
        node->generateCode(out);
    }
    out.finalize();
}

ArithmeticExpressionNode::ArithmeticExpressionNode(ArithmeticExpressionNode::Operation op, AST::NodePtr lhs,
                                                   AST::NodePtr rhs) :
        op(op),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)) {}

void ArithmeticExpressionNode::generateCode(AssemblyOutput &out) const {
    out.comment("LHS:");
    lhs->generateCode(out);
    out.comment("RHS:");
    rhs->generateCode(out);
    out.comment("OP:");
    switch (op) {
        case Operation::PLUS:
            out.stackAdd16();
            break;
        case Operation::MINUS:
        case Operation::DIV:
        case Operation::MULT:
        case Operation::XOR:
            throw std::runtime_error{"not implemented"};
            break;
    }
}


ASTNodeType MethodCallNode::getType() {
    return MethodCall;
}

MethodCallNode::MethodCallNode(std::string name, std::vector<AST::NodePtr> args) :
        name(std::move(name)),
        argumentList(std::move(args)) {}

void MethodCallNode::generateCode(AssemblyOutput &out) const {

    out.comment("Evaluate arguments:");
    for (const auto &arg: argumentList) {
        arg->generateCode(out);
    }

    out.comment("Call func:");
    if (name == "printWord") {
        out.pop16BitReg(Reg16::HL);
        out.call("printWord");
    } else {
        throw std::runtime_error{"Unknown function"};
    }
}

ASTNodeType VariableDeclarationNode::getType() {
    return VariableDeclaration;
}

VariableDeclarationNode::VariableDeclarationNode(std::string type, std::string name, AST::NodePtr rhs) :
        type(std::move(type)),
        name(std::move(name)),
        rhs(std::move(rhs)) {}

ASTNodeType VariableAssignmentNode::getType() {
    return VariableAssignment;
}

VariableAssignmentNode::VariableAssignmentNode(std::string name, AST::NodePtr rhs) :
        name(std::move(name)),
        rhs(std::move(rhs)) {}

ASTNodeType IntegerConstantNode::getType() {
    return Constant;
}

IntegerConstantNode::IntegerConstantNode(int val) : value(val) {}

void IntegerConstantNode::generateCode(AssemblyOutput &out) const {
    // TODO: range check
    out.push16BitConst(value);
}

ASTNodeType VariableAccessNode::getType() {
    return VariableAccess;
}

VariableAccessNode::VariableAccessNode(std::string n) : name(std::move(n)) {}

void ASTNode::generateCode(AssemblyOutput &/*out*/) const {
    throw std::runtime_error("Assembly generation not implemented");
}
