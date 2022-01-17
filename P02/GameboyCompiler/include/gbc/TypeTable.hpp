/**
 * @file TypeTable.hpp
 * @author ottojo
 * @date 1/15/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_TYPETABLE_HPP
#define GAMEBOYCOMPILER_TYPETABLE_HPP

#include <string>
#include <map>
#include <optional>
#include "SourceLocation.hpp"

struct TypeInfo {
    int size;
    std::optional<SourceLocation> declarationLocation = std::nullopt;
};

class TypeTable {
    public:
        using MapType = std::map<std::string, TypeInfo>;
        using ConstIterator = MapType::const_iterator;

        std::tuple<ConstIterator, bool> enter(const std::string &typeName, const TypeInfo &typeInfo);

        std::optional<ConstIterator> lookup(const std::string &typeName);

    private:
        MapType table;
};

#endif //GAMEBOYCOMPILER_TYPETABLE_HPP
