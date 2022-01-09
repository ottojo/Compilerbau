/**
 * @file SymbolTable.cpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#include "SymbolTable.hpp"

bool SymbolTable::enter(const std::string &id, const Declaration &decl) {
    auto[_, inserted] = table.emplace(id, decl);
    return inserted;
}

std::optional<SymbolTable::ConstIterator> SymbolTable::lookup(const std::string &id) const {
    auto r = table.find(id);
    if (r == table.end()) {
        return std::nullopt;
    }
    return r;
}
