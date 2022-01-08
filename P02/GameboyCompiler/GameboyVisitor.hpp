//
// Created by jonas on 14.12.21.
//

#ifndef GAMEBOYCOMPILER_GAMEBOYVISITOR_HPP
#define GAMEBOYCOMPILER_GAMEBOYVISITOR_HPP

#include "GameboyLanguageBaseVisitor.h"

#include "AST.hpp"

class GameboyVisitor : public gbparser::GameboyLanguageBaseVisitor {
    public:
        antlrcpp::Any visitDeclaration(gbparser::GameboyLanguageParser::DeclarationContext *ctx) override;

        antlrcpp::Any visitProgram(gbparser::GameboyLanguageParser::ProgramContext *ctx) override;

        antlrcpp::Any visitStatementList(gbparser::GameboyLanguageParser::StatementListContext *ctx) override;

        antlrcpp::Any visitStatement(gbparser::GameboyLanguageParser::StatementContext *ctx) override;

        antlrcpp::Any visitAssignment(gbparser::GameboyLanguageParser::AssignmentContext *ctx) override;

        antlrcpp::Any visitExpression(gbparser::GameboyLanguageParser::ExpressionContext *ctx) override;

        antlrcpp::Any visitTerm(gbparser::GameboyLanguageParser::TermContext *ctx) override;

        antlrcpp::Any visitLiteral(gbparser::GameboyLanguageParser::LiteralContext *ctx) override;

        antlrcpp::Any visitCall(gbparser::GameboyLanguageParser::CallContext *ctx) override;

        antlrcpp::Any visitParameterList(gbparser::GameboyLanguageParser::ParameterListContext *ctx) override;
};


#endif //GAMEBOYCOMPILER_GAMEBOYVISITOR_HPP
