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

ArithmeticExpressionNode::ArithmeticExpressionNode(ArithmeticExpressionNode::Operation op, AST::NodePtr lhs,
                                                   AST::NodePtr rhs) :
        op(op),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)) {}


ASTNodeType MethodCallNode::getType() {
    return MethodCall;
}

MethodCallNode::MethodCallNode(std::string name, std::vector<AST::NodePtr> args) :
        name(std::move(name)),
        argumentList(std::move(args)) {}

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

ASTNodeType VariableAccessNode::getType() {
    return VariableAccess;
}

VariableAccessNode::VariableAccessNode(std::string n) : name(std::move(n)) {}

