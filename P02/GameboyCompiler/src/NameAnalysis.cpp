/**
 * @file NameAnalysis.cpp
 * @author ottojo
 * @date 1/9/22
 */

#include <gbc/NameAnalysis.hpp>

void NameAnalysis::annotateAST(AST &ast) {

    if (ast.symbolTable != nullptr) {
        throw std::runtime_error{"AST already has symbol stacks!"};
    }
    ast.symbolTable = std::make_unique<SymbolTable>();

    prefillSymbolTable(*ast.symbolTable);
    for (const auto &node: ast.globalVariableDeclarationNodes) {
        annotateNode(*ast.symbolTable, *node);
    }
    for (const auto &node: ast.functionDefinitionNodes) {
        annotateNode(*ast.symbolTable, *node);
    }
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableDeclarationNode &node) {
    node.rhs->visit([&st](auto &node) { annotateNode(st, node); });
    auto decl = std::make_shared<VariableDeclaration>(node.loc);
    decl->size = 2; //TODO (type analysis): Enter type
    if (not st.enter(node.name, decl)) {
        // Entry failed, name already used
        auto prev = st.lookup(node.name);
        throw NameError(fmt::format("Tried to declare previously declared variable \"{}\"! Previous declaration at {}",
                                    node.name, prev->loc.value_or(SourceLocation())), node.loc);
    }
    node.decl = decl;
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableAccessNode &node) {
    auto res = st.lookup(node.name);
    if (res == nullptr) {
        throw NameError(fmt::format("Tried to access undeclared variable \"{}\"!", node.name), node.loc);
    }
    if (res->getType() != DeclType::Variable) {
        throw NameError(fmt::format("\"{}\" (declared at {}) is not a variable!", node.name,
                                    res->loc.value_or(SourceLocation())), node.loc);
    }
    node.decl = std::dynamic_pointer_cast<VariableDeclaration>(res);
    assert(node.decl != nullptr);
}

void NameAnalysis::annotateNode(SymbolTable &st, VariableAssignmentNode &node) {
    node.rhs->visit([&st](auto &node) { annotateNode(st, node); });
    auto res = st.lookup(node.name);
    if (res == nullptr) {
        throw NameError(fmt::format("Tried to assign to undeclared variable \"{}\"!", node.name), node.loc);
    }
    if (res->getType() != DeclType::Variable) {
        throw NameError(fmt::format("\"{}\" (declared at {}) is not a variable!", node.name,
                                    res->loc.value_or(SourceLocation())), node.loc);
    }
    node.decl = std::dynamic_pointer_cast<VariableDeclaration>(res);
    assert(node.decl != nullptr);
}

void NameAnalysis::annotateNode(SymbolTable &st, FunctionCallNode &node) {
    for (auto &n: node.argumentList) {
        n->visit([&st](auto &node) { annotateNode(st, node); });
    }
    auto res = st.lookup(node.name);
    if (res == nullptr) {
        throw NameError(fmt::format("Tried to call undeclared function \"{}\"!", node.name), node.loc);
    }
    if (res->getType() != DeclType::Function) {
        throw NameError(fmt::format("\"{}\" (declared at {}) is not a function!", node.name,
                                    res->loc.value_or(SourceLocation())), node.loc);
    }
    //node.methodDecl = *res;
}

void NameAnalysis::prefillSymbolTable(SymbolTable &/*stacks*/) {
    //stacks.enter("printWord", FunctionDeclaration());
}

void NameAnalysis::annotateNode(SymbolTable &st, ArithmeticExpressionNode &node) {
    node.lhs->visit([&st](auto &node) { annotateNode(st, node); });
    node.rhs->visit([&st](auto &node) { annotateNode(st, node); });
}

void NameAnalysis::annotateNode(SymbolTable &, IntegerConstantNode &) {}

void NameAnalysis::annotateNode(SymbolTable &st, FunctionDefinitionNode &node) {
    auto entered = st.enter(node.name, std::make_shared<FunctionDeclaration>(node.loc));
    if (not entered) {
        // Entry failed, name already used
        auto prev = st.lookup(node.name);
        throw NameError(fmt::format("Tried to declare previously declared function \"{}\"! Previous declaration at {}",
                                    node.name, prev->loc.value_or(SourceLocation())), node.loc);
    }
    st.enterScope();

    for (const auto &arg: node.arguments) {
        // TODO: Special case of variable declaration (not initialization)?
        auto decl = std::make_shared<VariableDeclaration>(node.loc);
        decl->size = 2; // TODO (type analysis): sizes
        auto entered_arg = st.enter(arg.identifier, decl);
        if (not entered_arg) {
            // Entry failed, name already used
            auto prev = st.lookup(node.name);
            throw NameError(fmt::format("Name of argument \"{}\" already declared at {}",
                                        node.name, prev->loc.value_or(SourceLocation())), node.loc);
        }
    }

    for (const auto &n: node.methodBody) {
        n->visit([&st](auto &node) { annotateNode(st, node); });
    }
    st.leaveScope();
}


NameError::NameError(const std::string &m, const SourceLocation &loc) :
        CompilerError(fmt::format("Name analysis error: {}", m), loc) {}
