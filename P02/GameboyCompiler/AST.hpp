/**
 * @file AST.hpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_AST_HPP
#define GAMEBOYCOMPILER_AST_HPP

#include <memory>
#include <vector>
#include "AssemblyOutput.hpp"

enum ASTNodeType {
    ArithmeticExpression,
    MethodCall,
    VariableDeclaration,
    VariableAssignment,
    Constant,
    VariableAccess
};

class ASTNode {
    public:
        virtual ASTNodeType getType() = 0;

        virtual ~ASTNode() = default;
};

class AST {
    public:
        using NodePtr = std::shared_ptr<ASTNode>;

        std::vector<NodePtr> nodes;
};

class ArithmeticExpressionNode : public ASTNode {
    public:
        enum class Operation {
            PLUS, MINUS, DIV, MULT, XOR
        };

        ArithmeticExpressionNode(Operation op, AST::NodePtr lhs, AST::NodePtr rhs);

        ASTNodeType getType() override;

        ~ArithmeticExpressionNode() override = default;

        Operation op;
        AST::NodePtr lhs;
        AST::NodePtr rhs;
};

class MethodCallNode : public ASTNode {
    public:
        MethodCallNode(std::string name, std::vector<AST::NodePtr> args);

        ASTNodeType getType() override;

        ~MethodCallNode() override = default;

        std::string name;
        std::vector<AST::NodePtr> argumentList;
};

class VariableDeclarationNode : public ASTNode {
    public:
        VariableDeclarationNode(std::string type, std::string name, AST::NodePtr rhs);

        ASTNodeType getType() override;

        ~VariableDeclarationNode() override = default;

        std::string type;
        std::string name;
        AST::NodePtr rhs;
};

class VariableAssignmentNode : public ASTNode {
    public:
        VariableAssignmentNode(std::string name, AST::NodePtr rhs);

        ASTNodeType getType() override;

        ~VariableAssignmentNode() override = default;

        std::string name;
        AST::NodePtr rhs;
};

class IntegerConstantNode : public ASTNode {
    public:
        explicit IntegerConstantNode(int val);

        ASTNodeType getType() override;

        ~IntegerConstantNode() override = default;

        int value = 0;
};

class VariableAccessNode : public ASTNode {
    public:
        explicit VariableAccessNode(std::string name);

        ASTNodeType getType() override;

        ~VariableAccessNode() override = default;

        std::string name;
};

#endif //GAMEBOYCOMPILER_AST_HPP
