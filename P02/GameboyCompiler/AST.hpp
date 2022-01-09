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
#include "SymbolTable.hpp"

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
        ASTNode(SourceLocation loc);

        virtual ASTNodeType getType() = 0;

        virtual ~ASTNode() = default;

        const SourceLocation loc;
};

class AST {
    public:
        using MutNodePtr = std::shared_ptr<ASTNode>;

        std::vector<MutNodePtr> nodes;
};

class ArithmeticExpressionNode : public ASTNode {
    public:
        enum class Operation {
            PLUS, MINUS, DIV, MULT, XOR
        };

        ArithmeticExpressionNode(const SourceLocation &loc, Operation op, AST::MutNodePtr lhs, AST::MutNodePtr rhs);

        ASTNodeType getType() override;

        ~ArithmeticExpressionNode() override = default;

        Operation op;
        AST::MutNodePtr lhs;
        AST::MutNodePtr rhs;
};

class MethodCallNode : public ASTNode {
    public:
        MethodCallNode(const SourceLocation &loc, std::string name, std::vector<AST::MutNodePtr> args);

        ASTNodeType getType() override;

        ~MethodCallNode() override = default;

        std::string name;
        SymbolTable::ConstIterator methodDecl;
        std::vector<AST::MutNodePtr> argumentList;
};

class VariableDeclarationNode : public ASTNode {
    public:
        VariableDeclarationNode(const SourceLocation &loc, std::string type, std::string name, AST::MutNodePtr rhs);

        ASTNodeType getType() override;

        ~VariableDeclarationNode() override = default;

        std::string type;
        std::string name;
        SymbolTable::ConstIterator declEntry;
        AST::MutNodePtr rhs;
};

class VariableAssignmentNode : public ASTNode {
    public:
        VariableAssignmentNode(const SourceLocation &loc, std::string name, AST::MutNodePtr rhs);

        ASTNodeType getType() override;

        ~VariableAssignmentNode() override = default;

        std::string name;
        SymbolTable::ConstIterator varDecl;
        AST::MutNodePtr rhs;
};

class IntegerConstantNode : public ASTNode {
    public:
        explicit IntegerConstantNode(const SourceLocation &loc, int val);

        ASTNodeType getType() override;

        ~IntegerConstantNode() override = default;

        int value = 0;
};

class VariableAccessNode : public ASTNode {
    public:
        explicit VariableAccessNode(const SourceLocation &loc, std::string name);

        ASTNodeType getType() override;

        ~VariableAccessNode() override = default;

        SymbolTable::ConstIterator varDecl;
        std::string name;
};

#endif //GAMEBOYCOMPILER_AST_HPP
