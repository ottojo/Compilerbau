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
#include <gbc/AssemblyOutput.hpp>
#include <gbc/SymbolTable.hpp>
#include "TypeTable.hpp"

enum class ASTNodeType {
    ArithmeticExpression,
    MethodDefinition,
    MethodCall,
    VariableDeclaration,
    VariableAssignment,
    Constant,
    VariableAccess
};

class ASTNode {
    public:
        explicit ASTNode(SourceLocation loc);

        [[nodiscard]] virtual ASTNodeType getType() const = 0;

        virtual ~ASTNode() = default;

        const SourceLocation loc;

        /**
         * Visit the node (does not visit children automatically!)
         * @tparam GenericVisitorLambda
         * @param visitor
         */
        template<typename GenericVisitorLambda>
        void visit(GenericVisitorLambda visitor);
};

class FunctionDefinitionNode;

using FunctionDefPtr = std::shared_ptr<FunctionDefinitionNode>;

class VariableDeclarationNode;

using VariableDeclNodePtr = std::shared_ptr<VariableDeclarationNode>;

class AST {
    public:
        using MutNodePtr = std::shared_ptr<ASTNode>;

        // TODO: Gehören die hier rein?
        std::unique_ptr<TypeTable> typeTable;
        std::unique_ptr<SymbolTable> symbolTable;

        std::vector<std::shared_ptr<FunctionDefinitionNode>> functionDefinitionNodes;
        std::vector<VariableDeclNodePtr> globalVariableDeclarationNodes;

        /**
         * Visit all top-level nodes (does not visit children automatically!)
         * @tparam GenericVisitorLambda
         * @param visitor
         */
        template<typename GenericVisitorLambda>
        void traverse(GenericVisitorLambda visitor) const;

};

class ArithmeticExpressionNode :
        public ASTNode {
    public:
        enum class Operation {
            PLUS, MINUS, DIV, MULT, XOR
        };

        ArithmeticExpressionNode(const SourceLocation &loc, Operation op, AST::MutNodePtr lhs, AST::MutNodePtr rhs);

        [[nodiscard]] ASTNodeType getType() const override;

        ~ArithmeticExpressionNode() override = default;

        Operation op;
        AST::MutNodePtr lhs;
        AST::MutNodePtr rhs;
};

struct MethodArgument {
    std::string typeName;
    std::string identifier;
};

template<>
struct fmt::formatter<MethodArgument> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(MethodArgument c, FormatContext &ctx) {
        return formatter<string_view>::format(c.typeName + " " + c.identifier, ctx);
    }
};


class FunctionDefinitionNode {
    public:
        FunctionDefinitionNode(SourceLocation loc, std::string name,
                               std::vector<MethodArgument> arguments,
                               std::optional<std::string> returnTypeName);

        SourceLocation loc;
        std::string name;
        std::vector<MethodArgument> arguments;
        std::optional<std::string> returnTypeName;
        std::vector<AST::MutNodePtr> methodBody;
        bool builtin = false;
};

class MethodCallNode :
        public ASTNode {
    public:
        MethodCallNode(const SourceLocation &loc, std::string name, std::vector<AST::MutNodePtr> args);

        [[nodiscard]] ASTNodeType getType() const override;


        ~MethodCallNode() override = default;

        std::string name;
        SymbolTable::ConstIterator methodDecl;
        std::vector<AST::MutNodePtr> argumentList;

        bool builtinMethod = true; // Use register calling convention from framework
};

class VariableDeclarationNode :
        public ASTNode {
    public:
        VariableDeclarationNode(const SourceLocation &loc, std::string type, std::string name, AST::MutNodePtr rhs);

        [[nodiscard]] ASTNodeType getType() const override;


        ~VariableDeclarationNode() override = default;

        std::string type;
        std::string name;
        SymbolTable::ConstIterator declEntry;
        AST::MutNodePtr rhs;
};

class VariableAssignmentNode :
        public ASTNode {
    public:
        VariableAssignmentNode(const SourceLocation &loc, std::string name, AST::MutNodePtr rhs);

        [[nodiscard]] ASTNodeType getType() const override;

        ~VariableAssignmentNode() override = default;

        std::string name;
        SymbolTable::ConstIterator varDecl;
        AST::MutNodePtr rhs;
};

class IntegerConstantNode :
        public ASTNode {
    public:
        explicit IntegerConstantNode(const SourceLocation &loc, int val);

        [[nodiscard]] ASTNodeType getType() const override;

        ~IntegerConstantNode() override = default;

        int value = 0;
};

class VariableAccessNode :
        public ASTNode {
    public:
        explicit VariableAccessNode(const SourceLocation &loc, std::string name);

        [[nodiscard]] ASTNodeType getType() const override;

        ~VariableAccessNode() override = default;

        //SymbolTable::ConstIterator varDecl;
        std::string name;
};

template<typename GenericVisitorLambda>
void ASTNode::visit(GenericVisitorLambda visitor) {
    using enum ASTNodeType;
    switch (getType()) {
        case MethodCall:
            visitor(*dynamic_cast<MethodCallNode *>(this));
            break;
        case VariableDeclaration:
            visitor(*dynamic_cast<VariableDeclarationNode *>(this));
            break;
        case VariableAssignment:
            visitor(*dynamic_cast<VariableAssignmentNode *>(this));
            break;
        case VariableAccess:
            visitor(*dynamic_cast<VariableAccessNode *>(this));
            break;
        case ArithmeticExpression:
            visitor(*dynamic_cast<ArithmeticExpressionNode *>(this));
            break;
        case Constant:
            visitor(*dynamic_cast<IntegerConstantNode *>(this));
            break;
        case MethodDefinition:
            visitor(*dynamic_cast<FunctionDefinitionNode *>(this));
            break;
    }
}

template<typename GenericVisitorLambda>
void AST::traverse(GenericVisitorLambda visitor) const {
    for (auto &node: this->functionDefinitionNodes) {
        visitor(*node);
    }
    for (auto &node: this->globalVariableDeclarationNodes) {
        visitor(*node);
    }
}

#endif //GAMEBOYCOMPILER_AST_HPP
