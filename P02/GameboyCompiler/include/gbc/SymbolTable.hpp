/**
 * @file SymbolTable.hpp
 * @author ottojo
 * @date 1/9/22
 */

#ifndef GAMEBOYCOMPILER_SYMBOLTABLE_HPP
#define GAMEBOYCOMPILER_SYMBOLTABLE_HPP

#include <string>
#include <optional>
#include <map>
#include <gbc/SourceLocation.hpp>
#include <variant>
#include <gbc/VarAccess.hpp>
#include <gbc/Declaration.hpp>

struct DE;

using MapOfStacks = std::map<std::string, std::stack<DE> >;

struct DE {
    std::string name;
    std::shared_ptr<VariableDeclaration> decl;
    MapOfStacks::iterator nextStackAtSameNL; ///< "Vertical" connection, to next DE with the same nesting level
};

class SymbolTable {
    public:
        using ConstIterator = MapOfStacks::const_iterator;

        void enterScope();

        /**
         * @return All removed variables
         */
        std::vector<DE> leaveScope();

        [[nodiscard]] std::size_t currentNestingLevel() const;

        /**
         * Look up a name and find its declaration
         * @param id Identifier to look up
         * @return Iterator to declaration, if found
         */
        [[nodiscard]] std::shared_ptr<VariableDeclaration> lookupVariable(const std::string &id) const;

        bool enterVariableDeclaration(const std::string &id, const std::shared_ptr<VariableDeclaration> &decl);

        std::size_t currNl = 0; ///< Current nesting level
        MapOfStacks stacks;
        std::map<std::size_t, MapOfStacks::iterator> verticalConnections;
        std::stack<int> currentFPoffset;
};

#endif //GAMEBOYCOMPILER_SYMBOLTABLE_HPP
