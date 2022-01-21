/**
 * @file Declaration.cpp
 * @author ottojo
 * @date 1/21/22
 * Description here TODO
 */

#include <gbc/Declaration.hpp>
#include <utility>


VariableDeclaration::VariableDeclaration(SourceLocation sourceLocation, std::string name,
                                         TypePtr type, bool global) : sourceLocation(std::move(
        sourceLocation)), name(std::move(name)), type(std::move(type)), global(global) {}

FunctionDeclaration::FunctionDeclaration(SourceLocation sourceLocation, std::string name, bool builtin,
                                         OptionalTypePtr returnType,
                                         std::vector<std::shared_ptr<VariableDeclaration>> arguments)
        : sourceLocation(std::move(
        sourceLocation)), name(std::move(name)), builtin(builtin), returnType(std::move(returnType)),
          arguments(std::move(arguments)) {}
