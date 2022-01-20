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
#include <gbc/SourceLocation.hpp>

enum class DeclType {
    Function,
    Variable
};

class Declaration {
    public:
        virtual DeclType getType() = 0;

        Declaration() = default;

        explicit Declaration(const SourceLocation &loc);

        virtual ~Declaration() = default;

        std::optional<SourceLocation> loc = std::nullopt;
};

class VariableDeclaration : public Declaration {
    public:

        explicit VariableDeclaration(const SourceLocation &loc);

        DeclType getType() override;

        ~VariableDeclaration() override = default;

        int address;
};

class FunctionDeclaration : public Declaration {
    public:
        FunctionDeclaration();

        explicit FunctionDeclaration(const SourceLocation &loc);

        DeclType getType() override;

        ~FunctionDeclaration() override = default;
};

struct DE;


class SymbolTable {
    public:
        using MapType = std::map<std::string, std::stack<DE> >;
        using ConstIterator = MapType::const_iterator;

        void enterScope();

        /**
         * @return All removed variables
         */
        std::vector<DE> leaveScope();

        std::size_t currentNestingLevel() const;

        /**
         * Insert a declaration into the symbol stacks
         * @param id Identifier
         * @param decl Declaration information
         * @return True if ID was not already present in stacks
         */
        //bool enter(const std::string &id, const VariableDeclaration &decl);

        /**
         * Look up a name and find its declaration
         * @param id Identifier to look up
         * @return Iterator to declaration, if found
         */
        [[nodiscard]] std::shared_ptr<Declaration> lookup(const std::string &id) const;

        // TODO: Function declarations
        bool enter(const std::string &id, const std::shared_ptr< Declaration>& decl);

        std::size_t currNl = 0; ///< Current nesting level
        MapType stacks;
        std::map<std::size_t, MapType::iterator> verticalConnections;
};

struct DE {
    std::string name;
    std::shared_ptr<Declaration> decl;
    SymbolTable::MapType::iterator nextStackAtSameNL; ///< "Vertical" connection, to next DE with the same nesting level
};

#endif //GAMEBOYCOMPILER_SYMBOLTABLE_HPP
