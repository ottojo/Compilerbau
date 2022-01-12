/**
 * @file NameAnalysis.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "NameAnalysis.hpp"
#include "SymbolTable.hpp"

std::shared_ptr<SymbolTable> NameAnalysis::annotateAST(AST &ast) {
    auto st = std::make_shared<SymbolTable>();
    prefillSymbolTable(*st);
    for (auto &node: ast.nodes) {
        annotateNode(*st, node);
    }
    return st;
}

void NameAnalysis::annotateNode(SymbolTable &st, const AST::MutNodePtr &node) {
    using enum ASTNodeType;
    switch (node->getType()) {
        case MethodCall:
            annotateNode(st, *std::dynamic_pointer_cast<MethodCallNode>(node));
            break;
        case VariableDeclaration:
            annotateNode(st, *std::dynamic_pointer_cast<VariableDeclarationNode>(node));
            break;
        case VariableAssignment:
            annotateNode(st, *std::dynamic_pointer_cast<VariableAssignmentNode>(node));
            break;
        case VariableAccess:
            annotateNode(st, *std::dynamic_pointer_cast<VariableAccessNode>(node));
            break;
        case ArithmeticExpression:
            annotateNode(st, *std::dynamic_pointer_cast<ArithmeticExpressionNode>(node));
            break;
        case Constant:
            break;
    }
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableDeclarationNode &node) {
    annotateNode(st, node.rhs);
    if (not st.enter(node.name, VariableDeclaration(node.loc))) {
        // Entry failed, name already used
        auto prev = st.lookup(node.name);
        throw NameError(fmt::format("Tried to declare previously declared variable \"{}\"! Previous declaration at {}",
                                    node.name, prev.value()->second->loc.value_or(SourceLocation())), node.loc);
    }
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableAccessNode &node) {
    auto res = st.lookup(node.name);
    if (not res.has_value()) {
        throw NameError(fmt::format("Tried to access undeclared variable \"{}\"!", node.name), node.loc);
    }
    if ((*res)->second->getType() != DeclType::Variable) {
        throw NameError(fmt::format("\"{}\" (declared at {}) is not a variable!", node.name,
                                    (*res)->second->loc.value_or(SourceLocation())), node.loc);
    }
    node.varDecl = *res;
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableAssignmentNode &node) {
    annotateNode(st, node.rhs);
    auto res = st.lookup(node.name);
    if (not res.has_value()) {
        throw NameError(fmt::format("Tried to assign to undeclared variable \"{}\"!", node.name), node.loc);
    }
    if ((*res)->second->getType() != DeclType::Variable) {
        throw NameError(fmt::format("\"{}\" (declared at {}) is not a variable!", node.name,
                                    (*res)->second->loc.value_or(SourceLocation())), node.loc);
    }
    node.varDecl = *res;
}

void NameAnalysis::annotateNode(SymbolTable &st, MethodCallNode &node) {
    for (auto &n: node.argumentList) {
        annotateNode(st, n);
    }
    auto res = st.lookup(node.name);
    if (not res.has_value()) {
        throw NameError(fmt::format("Tried to call undeclared function \"{}\"!", node.name), node.loc);
    }
    if ((*res)->second->getType() != DeclType::Function) {
        throw NameError(fmt::format("\"{}\" (declared at {}) is not a function!", node.name,
                                    (*res)->second->loc.value_or(SourceLocation())), node.loc);
    }
    node.methodDecl = *res;
}

void NameAnalysis::prefillSymbolTable(SymbolTable &table) {
    table.enter("printWord", FunctionDeclaration());
}

void NameAnalysis::annotateNode(SymbolTable &st, ArithmeticExpressionNode &node) {
    annotateNode(st, node.lhs);
    annotateNode(st, node.rhs);
}


NameError::NameError(const std::string &m, const SourceLocation &loc) :
        CompilerError(fmt::format("Name analysis error: {}", m), loc) {}
