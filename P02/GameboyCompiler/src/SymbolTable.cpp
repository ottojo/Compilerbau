/**
 * @file SymbolTable.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include <gbc/SymbolTable.hpp>

bool SymbolTable::enter(const std::string &id, const VariableDeclaration &decl) {
    auto[_, inserted] = table.emplace(id, std::make_unique<VariableDeclaration>(decl));
    return inserted;
}

bool SymbolTable::enter(const std::string &id, const FunctionDeclaration &decl) {
    auto[_, inserted] = table.emplace(id, std::make_unique<FunctionDeclaration>(decl));
    return inserted;
}

std::optional<SymbolTable::ConstIterator> SymbolTable::lookup(const std::string &id) const {
    auto r = table.find(id);
    if (r == table.end()) {
        return std::nullopt;
    }
    return r;
}

DeclType VariableDeclaration::getType() {
    return DeclType::Variable;
}

VariableDeclaration::VariableDeclaration(const SourceLocation &loc) : Declaration(loc) {}

DeclType FunctionDeclaration::getType() {
    return DeclType::Function;
}

Declaration::Declaration(const SourceLocation &loc) : loc(loc) {}
