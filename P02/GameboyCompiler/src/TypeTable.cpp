/**
 * @file TypeTable.cpp
 * @author ottojo
 * @date 1/15/22
 * Description here TODO
 */

#include <gbc/TypeTable.hpp>

std::tuple<TypeTable::ConstIterator, bool> TypeTable::enter(const std::string &typeName, const TypeInfo &typeInfo) {
    return table.insert({typeName, typeInfo});
}

std::optional<TypeTable::ConstIterator> TypeTable::lookup(const std::string &typeName) {
    auto r = table.find(typeName);
    if (r == table.end()) {
        return std::nullopt;
    }
    return r;
}
