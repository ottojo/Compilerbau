/**
 * @file NameAnalysis.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "NameAnalysis.hpp"
#include "SymbolTable.hpp"

void NameAnalysis::annotateAST(AST &ast) {
    SymbolTable st;
    prefillSymbolTable(st);
    for (auto &node: ast.nodes) {
        annotateNode(st, node);
    }
}

void NameAnalysis::annotateNode(SymbolTable &st, const AST::MutNodePtr &node) {
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
        case Constant:
            [[fallthrough]];
        case ArithmeticExpression:
            break;
    }
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableDeclarationNode &node) {
    annotateNode(st, node.rhs);
    if (not st.enter(node.name, Declaration{.type=Variable, .loc=node.loc})) {
        // Entry failed, name already used
        auto prev = st.lookup(node.name);
        throw NameError(fmt::format("Tried to declare previously declared variable \"{}\"! Previous declaration at {}",
                                    node.name, prev.value()->second.loc.value_or(SourceLocation())), node.loc);
    }
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableAccessNode &node) {
    auto res = st.lookup(node.name);
    if (not res.has_value()) {
        throw NameError(fmt::format("Tried to access to undeclared variable \"{}\"!", node.name), node.loc);
    }
    node.varDecl = *res;
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableAssignmentNode &node) {
    annotateNode(st, node.rhs);
    auto res = st.lookup(node.name);
    if (not res.has_value()) {
        throw NameError(fmt::format("Tried to assign to undeclared variable \"{}\"!", node.name), node.loc);
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
    node.methodDecl = *res;
}

void NameAnalysis::prefillSymbolTable(SymbolTable &table) {
    table.enter("printWord", Declaration{.type=Function});
}

const char *NameError::what() const noexcept {
    return message.c_str();
}

NameError::NameError(const std::string &m, const SourceLocation &loc) :
        message(fmt::format("{}: Name analysis error: {}", loc, m)) {}
