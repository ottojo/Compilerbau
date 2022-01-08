//
// Created by jonas on 14.12.21.
//

#include <fmt/core.h>
#include "GameboyVisitor.hpp"


antlrcpp::Any GameboyVisitor::visitProgram(gbparser::GameboyLanguageParser::ProgramContext *ctx) {
    AST ast;
    ast.nodes = visitStatementList(ctx->statementList()).as<std::vector<AST::NodePtr>>();
    return ast;
}

antlrcpp::Any GameboyVisitor::visitStatementList(gbparser::GameboyLanguageParser::StatementListContext *ctx) {
    std::vector<AST::NodePtr> nodeList;
    for (const auto &stmt: ctx->statement()) {
        nodeList.emplace_back(visitStatement(stmt).as<AST::NodePtr>());
    }
    return nodeList;
}

antlrcpp::Any GameboyVisitor::visitStatement(gbparser::GameboyLanguageParser::StatementContext *ctx) {
    return GameboyLanguageBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any GameboyVisitor::visitAssignment(gbparser::GameboyLanguageParser::AssignmentContext *ctx) {
    AST::NodePtr node = std::make_shared<VariableAssignmentNode>(
            ctx->ID()->getText(),
            visitExpression(ctx->expression()).as<AST::NodePtr>());
    return node;
}

antlrcpp::Any GameboyVisitor::visitExpression(gbparser::GameboyLanguageParser::ExpressionContext *ctx) {
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
        AST::NodePtr node = std::make_shared<ArithmeticExpressionNode>(
                op,
                visitTerm(ctx->term(0)).as<AST::NodePtr>(),
                visitTerm(ctx->term(1)).as<AST::NodePtr>());
        return node;
    }
}

antlrcpp::Any GameboyVisitor::visitTerm(gbparser::GameboyLanguageParser::TermContext *ctx) {

    if (ctx->literal()) {
        return visitLiteral(ctx->literal());
    } else if (ctx->ID()) {
        AST::NodePtr node = std::make_shared<VariableAccessNode>(ctx->ID()->getText());
        return node;
    } else if (ctx->call()) {
        return visitCall(ctx->call());
    } else if (ctx->expression()) {
        return visitExpression(ctx->expression());
    }

    return GameboyLanguageBaseVisitor::visitTerm(ctx);
}

antlrcpp::Any GameboyVisitor::visitLiteral(gbparser::GameboyLanguageParser::LiteralContext *ctx) {
    int val = stoi(ctx->getText());
    AST::NodePtr node = std::make_shared<IntegerConstantNode>(val);
    return node;
}

antlrcpp::Any GameboyVisitor::visitCall(gbparser::GameboyLanguageParser::CallContext *ctx) {
    AST::NodePtr node = std::make_shared<MethodCallNode>(
            ctx->ID()->getText(),
            ctx->parameterList() ? visitParameterList(ctx->parameterList()).as<std::vector<AST::NodePtr>>()
                                 : std::vector<AST::NodePtr>{});
    return node;
}

antlrcpp::Any GameboyVisitor::visitParameterList(gbparser::GameboyLanguageParser::ParameterListContext *ctx) {
    std::vector<AST::NodePtr> list;
    auto remainingList = ctx;
    do {
        list.emplace_back(visitExpression(remainingList->expression()).as<AST::NodePtr>());
        remainingList = remainingList->parameterList();
    } while (remainingList != nullptr);
    return list;
}

antlrcpp::Any GameboyVisitor::visitDeclaration(gbparser::GameboyLanguageParser::DeclarationContext *ctx) {

    AST::NodePtr node = std::make_shared<VariableDeclarationNode>(
            ctx->ID(0)->getText(),
            ctx->ID(1)->getText(),
            visitExpression(ctx->expression()).as<AST::NodePtr>());
    return node;
}
