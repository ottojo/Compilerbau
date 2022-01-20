/**
 * @file NameAnalysis.hpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_NAMEANALYSIS_HPP
#define GAMEBOYCOMPILER_NAMEANALYSIS_HPP


#include <gbc/AST.hpp>
#include <gbc/SymbolTable.hpp>
#include <gbc/CompilerError.hpp>

// TODO: Global symbol stacks, and then for each function? How do we track local vars?

class NameError : public CompilerError {
    public:
        NameError(const std::string &m, const SourceLocation &loc);
};

class NameAnalysis {
    public:
        static void annotateAST(AST &ast);

    private:
        static void annotateNode(SymbolTable &st, VariableDeclarationNode &node);

        static void annotateNode(SymbolTable &st, IntegerConstantNode &node);

        static void annotateNode(SymbolTable &st, VariableAccessNode &node);

        static void annotateNode(SymbolTable &st, VariableAssignmentNode &node);

        static void annotateNode(SymbolTable &st, FunctionCallNode &node);

        static void annotateNode(SymbolTable &st, ArithmeticExpressionNode &node);

        static void annotateNode(SymbolTable &st, FunctionDefinitionNode &node);

        static void prefillSymbolTable(SymbolTable &table);
};


#endif //GAMEBOYCOMPILER_NAMEANALYSIS_HPP
