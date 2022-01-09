/**
 * @file SymbolTable.hpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_SYMBOLTABLE_HPP
#define GAMEBOYCOMPILER_SYMBOLTABLE_HPP

#include <string>
#include <optional>
#include <map>
#include "SourceLocation.hpp"

enum DeclType {
    Function,
    Variable
};

struct Declaration {
    DeclType type;
    std::optional<SourceLocation> loc = std::nullopt;
};

/*
struct DE {
    Declaration decl;
    DE *nextInNL; ///< "Vertical" connection, to next DE with the same nesting level
    DE *shadowedDE; ///< Next declaration with same ID in lower nesting level
};
*/

class SymbolTable {
    private:
        using MapType = std::map<std::string, Declaration>;
    public:
        using ConstIterator = MapType::const_iterator;

        // TODO: Implement nesting

        // void enterScope();

        // void leaveScope();

        // int currentNestingLevel();

        /**
         * Insert a declaration into the symbol table
         * @param id Identifier
         * @param decl Declaration information
         * @return True if ID was not already present in table
         */
        bool enter(const std::string &id, const Declaration &decl);

        /**
         * Look up a name and find its declaration
         * @param id Identifier to look up
         * @return Iterator to declaration, if found
         */
        [[nodiscard]] std::optional<ConstIterator> lookup(const std::string &id) const;

    private:
        // int currNl = 0; ///< Current nesting level
        std::map<std::string, Declaration> table;
};


#endif //GAMEBOYCOMPILER_SYMBOLTABLE_HPP
