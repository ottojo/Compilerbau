/**
 * @file AST.cpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#include "AST.hpp"

#include <utility>

ASTNodeType ArithmeticExpressionNode::getType() {
    return ASTNodeType::ArithmeticExpression;
}

ArithmeticExpressionNode::ArithmeticExpressionNode(const SourceLocation &loc, ArithmeticExpressionNode::Operation op,
                                                   AST::MutNodePtr lhs,
                                                   AST::MutNodePtr rhs) :
        ASTNode(loc),
        op(op),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)) {}


ASTNodeType MethodCallNode::getType() {
    return ASTNodeType::MethodCall;
}

MethodCallNode::MethodCallNode(const SourceLocation &loc, std::string name, std::vector<AST::MutNodePtr> args) :
        ASTNode(loc),
        name(std::move(name)),
        argumentList(std::move(args)) {}

ASTNodeType VariableDeclarationNode::getType() {
    return ASTNodeType::VariableDeclaration;
}

VariableDeclarationNode::VariableDeclarationNode(const SourceLocation &loc, std::string type, std::string name,
                                                 AST::MutNodePtr rhs) :
        ASTNode(loc),
        type(std::move(type)),
        name(std::move(name)),
        rhs(std::move(rhs)) {}

ASTNodeType VariableAssignmentNode::getType() {
    return ASTNodeType::VariableAssignment;
}

VariableAssignmentNode::VariableAssignmentNode(const SourceLocation &loc, std::string name, AST::MutNodePtr rhs) :
        ASTNode(loc),
        name(std::move(name)),
        rhs(std::move(rhs)) {}

ASTNodeType IntegerConstantNode::getType() {
    return ASTNodeType::Constant;
}

IntegerConstantNode::IntegerConstantNode(const SourceLocation &loc, int val) :
        ASTNode(loc), value(val) {}

ASTNodeType VariableAccessNode::getType() {
    return ASTNodeType::VariableAccess;
}

VariableAccessNode::VariableAccessNode(const SourceLocation &loc, std::string name) :
        ASTNode(loc),
        name(std::move(name)) {}

ASTNode::ASTNode(SourceLocation loc) : loc(std::move(loc)) {}
