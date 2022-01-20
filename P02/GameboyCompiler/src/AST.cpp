/**
 * @file AST.cpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#include <gbc/AST.hpp>

#include <utility>

ASTNodeType ArithmeticExpressionNode::getType() const {
    return ASTNodeType::ArithmeticExpression;
}

ArithmeticExpressionNode::ArithmeticExpressionNode(const SourceLocation &loc, ArithmeticExpressionNode::Operation op,
                                                   AST::MutNodePtr lhs,
                                                   AST::MutNodePtr rhs) :
        ASTNode(loc),
        op(op),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)) {}


ASTNodeType FunctionCallNode::getType() const {
    return ASTNodeType::MethodCall;
}

FunctionCallNode::FunctionCallNode(const SourceLocation &loc, std::string name, std::vector<AST::MutNodePtr> args) :
        ASTNode(loc),
        name(std::move(name)),
        argumentList(std::move(args)) {}

ASTNodeType VariableDeclarationNode::getType() const {
    return ASTNodeType::VariableDeclaration;
}

VariableDeclarationNode::VariableDeclarationNode(const SourceLocation &loc, std::string type, std::string name,
                                                 AST::MutNodePtr rhs) :
        ASTNode(loc),
        type(std::move(type)),
        name(std::move(name)),
        rhs(std::move(rhs)) {}

ASTNodeType VariableAssignmentNode::getType() const {
    return ASTNodeType::VariableAssignment;
}

VariableAssignmentNode::VariableAssignmentNode(const SourceLocation &loc, std::string name, AST::MutNodePtr rhs) :
        ASTNode(loc),
        name(std::move(name)),
        rhs(std::move(rhs)) {}

ASTNodeType IntegerConstantNode::getType() const {
    return ASTNodeType::Constant;
}

IntegerConstantNode::IntegerConstantNode(const SourceLocation &loc, int val) :
        ASTNode(loc), value(val) {}

ASTNodeType VariableAccessNode::getType() const {
    return ASTNodeType::VariableAccess;
}

VariableAccessNode::VariableAccessNode(const SourceLocation &loc, std::string name) :
        ASTNode(loc),
        name(std::move(name)) {}

ASTNode::ASTNode(SourceLocation loc) : loc(std::move(loc)) {}

FunctionDefinitionNode::FunctionDefinitionNode(SourceLocation loc, std::string name,
                                               std::vector<MethodArgument> arguments,
                                               std::optional<std::string> returnTypeName) :
        loc(std::move(loc)),
        name(std::move(name)),
        arguments(std::move(arguments)),
        returnTypeName(std::move(returnTypeName)) {}

