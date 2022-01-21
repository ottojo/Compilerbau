/**
 * @file Declaration.hpp
 * @author ottojo
 * @date 1/21/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_DECLARATION_HPP
#define GAMEBOYCOMPILER_DECLARATION_HPP

#include <gbc/Type.hpp>
#include <gbc/VarAccess.hpp>

class VariableDeclaration {
    public:
        VariableDeclaration(SourceLocation sourceLocation, std::string name, TypePtr type,
                            bool global);

        SourceLocation sourceLocation;
        std::string name;
        TypePtr type;
        bool global;
        std::optional<VarAccess::Location> location; ///< Filled during code generation
};

class FunctionDeclaration {
    public:
        FunctionDeclaration(SourceLocation sourceLocation, std::string name, bool builtin,
                            OptionalTypePtr returnType, std::vector<std::shared_ptr<VariableDeclaration>> arguments);

        SourceLocation sourceLocation;
        std::string name;
        bool builtin;
        OptionalTypePtr returnType;
        std::vector<std::shared_ptr<VariableDeclaration>> arguments;
};

#endif //GAMEBOYCOMPILER_DECLARATION_HPP
