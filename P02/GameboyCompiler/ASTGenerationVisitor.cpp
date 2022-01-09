//
// Created by jonas on 14.12.21.
//

#include <fmt/core.h>
#include "ASTGenerationVisitor.hpp"


antlrcpp::Any ASTGenerationVisitor::visitProgram(gbparser::GameboyLanguageParser::ProgramContext *ctx) {
    AST ast;
    ast.nodes = visitStatementList(ctx->statementList()).as<std::vector<AST::MutNodePtr>>();
    return ast;
}

antlrcpp::Any ASTGenerationVisitor::visitStatementList(gbparser::GameboyLanguageParser::StatementListContext *ctx) {
    std::vector<AST::MutNodePtr> nodeList;
    for (const auto &stmt: ctx->statement()) {
        nodeList.emplace_back(visitStatement(stmt).as<AST::MutNodePtr>());
    }
    return nodeList;
}

antlrcpp::Any ASTGenerationVisitor::visitStatement(gbparser::GameboyLanguageParser::StatementContext *ctx) {
    return GameboyLanguageBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any ASTGenerationVisitor::visitAssignment(gbparser::GameboyLanguageParser::AssignmentContext *ctx) {
    AST::MutNodePtr node = std::make_shared<VariableAssignmentNode>(
            SourceLocation(ctx->getStart()),
            ctx->ID()->getText(),
            visitExpression(ctx->expression()).as<AST::MutNodePtr>());
    return node;
}

antlrcpp::Any ASTGenerationVisitor::visitExpression(gbparser::GameboyLanguageParser::ExpressionContext *ctx) {
    if (ctx->term().size() == 1) {
        return visitTerm(ctx->term(0));
    } else {
        ArithmeticExpressionNode::Operation op;
        if (ctx->PLUS()) {
            op = ArithmeticExpressionNode::Operation::PLUS;
        } else if (ctx->MINUS()) {
            op = ArithmeticExpressionNode::Operation::MINUS;
        } else if (ctx->DIV()) {
            op = ArithmeticExpressionNode::Operation::DIV;
        } else if (ctx->TIMES()) {
            op = ArithmeticExpressionNode::Operation::MULT;
        } else if (ctx->XOR()) {
            op = ArithmeticExpressionNode::Operation::XOR;
        } else {
            return {};
        }
        AST::MutNodePtr node = std::make_shared<ArithmeticExpressionNode>(
                SourceLocation(ctx->getStart()),
                op,
                visitTerm(ctx->term(0)).as<AST::MutNodePtr>(),
                visitTerm(ctx->term(1)).as<AST::MutNodePtr>());
        return node;
    }
}

antlrcpp::Any ASTGenerationVisitor::visitTerm(gbparser::GameboyLanguageParser::TermContext *ctx) {

    if (ctx->literal()) {
        return visitLiteral(ctx->literal());
    } else if (ctx->ID()) {
        AST::MutNodePtr node = std::make_shared<VariableAccessNode>(SourceLocation(ctx->getStart()),
                                                                    ctx->ID()->getText());
        return node;
    } else if (ctx->call()) {
        return visitCall(ctx->call());
    } else if (ctx->expression()) {
        return visitExpression(ctx->expression());
    }

    return GameboyLanguageBaseVisitor::visitTerm(ctx);
}

antlrcpp::Any ASTGenerationVisitor::visitLiteral(gbparser::GameboyLanguageParser::LiteralContext *ctx) {
    int val = stoi(ctx->getText());
    AST::MutNodePtr node = std::make_shared<IntegerConstantNode>(SourceLocation(ctx->getStart()), val);
    return node;
}

antlrcpp::Any ASTGenerationVisitor::visitCall(gbparser::GameboyLanguageParser::CallContext *ctx) {
    AST::MutNodePtr node = std::make_shared<MethodCallNode>(
            SourceLocation(ctx->getStart()),
            ctx->ID()->getText(),
            ctx->parameterList() ? visitParameterList(ctx->parameterList()).as<std::vector<AST::MutNodePtr>>()
                                 : std::vector<AST::MutNodePtr>{});
    return node;
}

antlrcpp::Any ASTGenerationVisitor::visitParameterList(gbparser::GameboyLanguageParser::ParameterListContext *ctx) {
    std::vector<AST::MutNodePtr> list;
    auto remainingList = ctx;
    do {
        list.emplace_back(visitExpression(remainingList->expression()).as<AST::MutNodePtr>());
        remainingList = remainingList->parameterList();
    } while (remainingList != nullptr);
    return list;
}

antlrcpp::Any ASTGenerationVisitor::visitDeclaration(gbparser::GameboyLanguageParser::DeclarationContext *ctx) {

    AST::MutNodePtr node = std::make_shared<VariableDeclarationNode>(
            SourceLocation(ctx->getStart()),
            ctx->ID(0)->getText(),
            ctx->ID(1)->getText(),
            visitExpression(ctx->expression()).as<AST::MutNodePtr>());
    return node;
}

AST ASTGenerationVisitor::generateAST(gbparser::GameboyLanguageParser::ProgramContext *ctx) {
    auto ast = visitProgram(ctx);
    return ast.as<AST>();
}

