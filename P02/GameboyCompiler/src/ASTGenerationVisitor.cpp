//
// Created by jonas on 14.12.21.
//

#include <fmt/core.h>
#include <gbc/ASTGenerationVisitor.hpp>
#include <gbc/CompilerError.hpp>


antlrcpp::Any ASTGenerationVisitor::visitProgram(gbparser::GameboyLanguageParser::ProgramContext *ctx) {
    AST ast;
    ast.nodes = visitStatementList(ctx->statementList()).as<std::vector<AST::MutNodePtr>>();
    return ast;
}

antlrcpp::Any ASTGenerationVisitor::visitStatementList(gbparser::GameboyLanguageParser::StatementListContext *ctx) {
    std::vector<AST::MutNodePtr> nodeList;
    for (const auto &stmt: ctx->statement()) {
        auto a = visitStatement(stmt);
        try {
            nodeList.emplace_back(a.as<AST::MutNodePtr>());
        } catch (std::bad_cast &e) {
            throw CompilerError("Statement could not be parsed!: " + stmt->toStringTree(true),
                                SourceLocation(stmt->start));
        }
    }
    return nodeList;
}

antlrcpp::Any ASTGenerationVisitor::visitStatement(gbparser::GameboyLanguageParser::StatementContext *ctx) {
    return GameboyLanguageBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any ASTGenerationVisitor::visitAssignment(gbparser::GameboyLanguageParser::AssignmentContext *ctx) {
    AST::MutNodePtr node = std::make_shared<VariableAssignmentNode>(
            SourceLocation(ctx->getStart()),
            ctx->variableName->getText(),
            visitExpression(ctx->rhs).as<AST::MutNodePtr>());
    return node;
}

antlrcpp::Any ASTGenerationVisitor::visitExpression(gbparser::GameboyLanguageParser::ExpressionContext *ctx) {
    if (ctx->singleTerm != nullptr) {
        return visitTerm(ctx->singleTerm);
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
                visitTerm(ctx->lhs).as<AST::MutNodePtr>(),
                visitTerm(ctx->rhs).as<AST::MutNodePtr>());
        return node;
    }
}

antlrcpp::Any ASTGenerationVisitor::visitTerm(gbparser::GameboyLanguageParser::TermContext *ctx) {
    if (ctx->literal()) {
        return visitLiteral(ctx->literal());
    } else if (ctx->variable) {
        AST::MutNodePtr node = std::make_shared<VariableAccessNode>(SourceLocation(ctx->getStart()),
                                                                    ctx->variable->getText());
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
            ctx->functionName->getText(),
            ctx->parameterList() ? visitParameterList(ctx->parameterList()).as<std::vector<AST::MutNodePtr>>()
                                 : std::vector<AST::MutNodePtr>{});
    return node;
}

antlrcpp::Any ASTGenerationVisitor::visitParameterList(gbparser::GameboyLanguageParser::ParameterListContext *ctx) {
    std::vector<AST::MutNodePtr> list;
    for (const auto &exp: ctx->expression()) {
        list.emplace_back(visit(exp).as<AST::MutNodePtr>());
    }
    return list;
}

antlrcpp::Any
ASTGenerationVisitor::visitVarInitialization(gbparser::GameboyLanguageParser::VarInitializationContext *ctx) {

    AST::MutNodePtr node = std::make_shared<VariableDeclarationNode>(
            SourceLocation(ctx->getStart()),
            ctx->varDeclaration()->typeName->getText(),
            ctx->varDeclaration()->variableName->getText(),
            visit(ctx->expression()).as<AST::MutNodePtr>());
    return node;
}

AST ASTGenerationVisitor::generateAST(gbparser::GameboyLanguageParser::ProgramContext *ctx) {
    auto ast = visitProgram(ctx);
    return std::move(ast.as<AST>());
}

antlrcpp::Any ASTGenerationVisitor::visitFuncDeclaration(gbparser::GameboyLanguageParser::FuncDeclarationContext *ctx) {
    std::vector<MethodArgument> args;
    for (const auto &a: ctx->funcSignature()->argumentList()->varDeclaration()) {
        args.emplace_back(MethodArgument{.typeName=a->typeName->getText(), .identifier=a->variableName->getText()});
    }

    std::optional<std::string> ret = std::nullopt;
    if (ctx->funcSignature()->returnType != nullptr) {
        ret = ctx->funcSignature()->returnType->getText();
    }

    AST::MutNodePtr node = std::make_shared<MethodDefinitionNode>(
            SourceLocation(ctx->getStart()),
            ctx->funcSignature()->funcName->getText(),
            args,
            ret,
            visit(ctx->block()->statementList()).as<std::vector<AST::MutNodePtr>>());
    return node;
}
