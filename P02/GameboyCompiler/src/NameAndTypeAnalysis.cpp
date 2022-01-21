/**
 * @file NameAnalysis.cpp
 * @author ottojo
 * @date 1/9/22
 */

#include <gbc/NameAndTypeAnalysis.hpp>

NameAndTypeAnalysis::NameAndTypeAnalysis(AST &ast) : ast(ast), st(std::make_unique<SymbolTable>()) {

}

void NameAndTypeAnalysis::annotateAST() {
    if (ast.symbolTable != nullptr) {
        throw std::runtime_error{"AST already has symbol stacks!"};
    }
    if (st == nullptr) {
        throw std::runtime_error{"Symbol table not initialized! annotateAST can only be called once!"};
    }

    prefillSymbolTable();
    context = Context::GLOBAL;
    for (const auto &node: ast.globalVariableDeclarationNodes) {
        annotateNode(*node);
    }
    for (const auto &node: ast.functionDefinitionNodes) {
        annotateNode(*node);
    }
    ast.symbolTable = std::move(st);
    st = nullptr;
}

void NameAndTypeAnalysis::annotateNode(VariableInitializationNode &node) {
    node.rhs->visit([this](auto &node) { annotateNode(node); });

    auto typeIt = types.find(node.type);
    if (typeIt == types.end()) {
        throw TypeError(fmt::format("Type {} is unknown.", node.type), node.loc);
    }

    // TODO(type analysis): Check if RHS has compatible type

    bool global = context == Context::GLOBAL;
    auto decl = std::make_shared<VariableDeclaration>(node.loc, node.name, typeIt->second, global);
    if (not global) {
        lastLocalVarFPOffset -= typeIt->second->size;
        decl->location = VarAccess::FPRelative{lastLocalVarFPOffset};
    }

    if (not st->enterVariableDeclaration(node.name, decl)) {
        // Entry failed, name already used
        // auto prev = st->lookup(node.name);
        // TODO(error messages): Find previous declaration of name
        throw NameError(fmt::format("Tried to declare previously declared variable \"{}\"!", node.name), node.loc);
    }
    node.variableDeclaration = decl;
}

void NameAndTypeAnalysis::annotateNode(VariableAccessNode &node) {
    auto res = st->lookupVariable(node.name);
    if (res == nullptr) {
        throw NameError(fmt::format("Tried to access undeclared variable \"{}\"!", node.name), node.loc);
    }
    // TODO(error messages): Find out if name exists, but is a function
    node.decl = res;
}

void NameAndTypeAnalysis::annotateNode(VariableAssignmentNode &node) {
    node.rhs->visit([this](auto &node) { annotateNode(node); });
    auto res = st->lookupVariable(node.name);
    if (res == nullptr) {
        throw NameError(fmt::format("Tried to assign to undeclared variable \"{}\"!", node.name), node.loc);
    }
    // TODO(error messages): Find out if name exists, but is a function
    node.decl = res;
}

void NameAndTypeAnalysis::annotateNode(FunctionCallNode &node) {
    for (auto &n: node.argumentList) {
        n->visit([this](auto &node) { annotateNode(node); });
    }
    auto res = functions.find(node.name);
    if (res == functions.end()) {
        throw NameError(fmt::format("Tried to call undeclared function \"{}\"!", node.name), node.loc);
    }
    // TODO(error messages): Find out if name exists, but is a variable

    // TODO: Check type of arguments

    node.functionDeclaration = res->second;
}

void NameAndTypeAnalysis::prefillSymbolTable() {
    auto int16 = std::make_shared<Type>(Type{.name="int16", .size=2});
    this->types.emplace("int16", int16);
}

void NameAndTypeAnalysis::annotateNode(ArithmeticExpressionNode &node) {
    node.lhs->visit([this](auto &node) { annotateNode(node); });
    node.rhs->visit([this](auto &node) { annotateNode(node); });
}

void NameAndTypeAnalysis::annotateNode(IntegerConstantNode &) {}

void NameAndTypeAnalysis::annotateNode(FunctionDefinitionNode &node) {
    OptionalTypePtr type = nullptr;
    if (node.returnTypeName.has_value()) {
        auto typeIt = types.find(node.returnTypeName.value());
        if (typeIt == types.end()) {
            throw TypeError{fmt::format("Return type \"{}\" not found.", node.returnTypeName.value()), node.loc};
        }
        type = typeIt->second;
    }

    std::vector<std::shared_ptr<VariableDeclaration>> argumentDeclarations;

    context = Context::ARGUMENT;
    for (const auto &arg: node.arguments) {
        // TODO: Special case of variable declaration (not initialization)?
        auto typeIt = types.find(arg.typeName);
        if (typeIt == types.end()) {
            throw TypeError(fmt::format("Type {} is unknown.", arg.typeName),
                            node.loc); // TODO(error messages): store location info for arguments
        }

        auto decl = std::make_shared<VariableDeclaration>(node.loc, arg.identifier, typeIt->second, false);
        argumentDeclarations.emplace_back(decl);
    }


    auto decl = std::make_shared<FunctionDeclaration>(node.loc, node.name, node.builtin, type, argumentDeclarations);

    if (functions.find(node.name) != functions.end()) {
        throw NameError(fmt::format("Tried to declare previously declared function \"{}\"!", node.name), node.loc);
    }

    functions.insert(std::make_pair(node.name, decl));
    // TODO(error reporting): report location of previous declaration

    st->enterScope();
    int nextOffset = +2; // Space for return address
    for (auto it = argumentDeclarations.rbegin(); it != argumentDeclarations.rend(); it++) {
        (*it)->location = VarAccess::FPRelative{nextOffset};
        nextOffset += (*it)->type->size;
    }

    for (const auto &argDecl: argumentDeclarations) {
        auto entered_arg = st->enterVariableDeclaration(argDecl->name, argDecl);
        if (not entered_arg) {
            // Entry failed, name already used
            throw NameError(fmt::format("Name of argument \"{}\" already declared", node.name), node.loc);
        }
    }


    context = Context::BLOCK;
    lastLocalVarFPOffset = 0;

    for (const auto &n: node.methodBody) {
        n->visit([this](auto &node) { annotateNode(node); });
    }
    st->leaveScope();
}

void NameAndTypeAnalysis::annotateNode(ReturnNode &/*node*/) {
    // TODO(type analysis): Check type compatibility of RHS
}


NameError::NameError(const std::string &m, const SourceLocation &loc) :
        CompilerError(fmt::format("Name analysis error: {}", m), loc) {}

TypeError::TypeError(const std::string &m, const SourceLocation &loc) :
        CompilerError(fmt::format("Type analysis error: {}", m), loc) {}
