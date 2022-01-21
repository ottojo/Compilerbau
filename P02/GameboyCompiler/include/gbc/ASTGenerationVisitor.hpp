//
// Created by jonas on 14.12.21.
//

#ifndef GAMEBOYCOMPILER_ASTGENERATIONVISITOR_HPP
#define GAMEBOYCOMPILER_ASTGENERATIONVISITOR_HPP

#include "GameboyLanguageBaseVisitor.h"

#include <gbc/AST.hpp>

class ASTGenerationVisitor : public gbparser::GameboyLanguageBaseVisitor {
    public:
        AST generateAST(gbparser::GameboyLanguageParser::ProgramContext *ctx);

        antlrcpp::Any visitProgram(gbparser::GameboyLanguageParser::ProgramContext *ctx) override;

        antlrcpp::Any visitFuncSignature(gbparser::GameboyLanguageParser::FuncSignatureContext *ctx) override;

        antlrcpp::Any visitFuncDefinition(gbparser::GameboyLanguageParser::FuncDefinitionContext *ctx) override;

        antlrcpp::Any
        visitBuiltinFuncDeclaration(gbparser::GameboyLanguageParser::BuiltinFuncDeclarationContext *ctx) override;

        antlrcpp::Any
        visitGlobalVarInitialization(gbparser::GameboyLanguageParser::GlobalVarInitializationContext *ctx) override;

        antlrcpp::Any visitVarInitialization(gbparser::GameboyLanguageParser::VarInitializationContext *ctx) override;

        antlrcpp::Any visitStatementList(gbparser::GameboyLanguageParser::StatementListContext *ctx) override;

        antlrcpp::Any visitStatement(gbparser::GameboyLanguageParser::StatementContext *ctx) override;

        antlrcpp::Any visitAssignment(gbparser::GameboyLanguageParser::AssignmentContext *ctx) override;

        antlrcpp::Any visitExpression(gbparser::GameboyLanguageParser::ExpressionContext *ctx) override;

        antlrcpp::Any visitTerm(gbparser::GameboyLanguageParser::TermContext *ctx) override;

        antlrcpp::Any visitLiteral(gbparser::GameboyLanguageParser::LiteralContext *ctx) override;

        antlrcpp::Any visitCall(gbparser::GameboyLanguageParser::CallContext *ctx) override;

        antlrcpp::Any visitParameterList(gbparser::GameboyLanguageParser::ParameterListContext *ctx) override;

        antlrcpp::Any visitFuncDeclaration(gbparser::GameboyLanguageParser::FuncDeclarationContext *ctx) override;

        antlrcpp::Any visitReturnStatement(gbparser::GameboyLanguageParser::ReturnStatementContext *ctx) override;

    private:
        AST ast;
};


#endif //GAMEBOYCOMPILER_ASTGENERATIONVISITOR_HPP
