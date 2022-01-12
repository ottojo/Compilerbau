/**
 * @file NameAnalysis.hpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_NAMEANALYSIS_HPP
#define GAMEBOYCOMPILER_NAMEANALYSIS_HPP


#include "AST.hpp"
#include "SymbolTable.hpp"
#include "CompilerError.hpp"

class NameError : public CompilerError {
    public:
        NameError(const std::string &m, const SourceLocation &loc);
};

class NameAnalysis {
    public:
        static std::shared_ptr<SymbolTable> annotateAST(AST &ast);

    private:
        static void annotateNode(SymbolTable &st, const AST::MutNodePtr &node);

        static void annotateNode(SymbolTable &st, VariableDeclarationNode &node);

        static void annotateNode(SymbolTable &st, VariableAccessNode &node);

        static void annotateNode(SymbolTable &st, VariableAssignmentNode &node);

        static void annotateNode(SymbolTable &st, MethodCallNode &node);

        static void annotateNode(SymbolTable &st, ArithmeticExpressionNode &node);

        static void prefillSymbolTable(SymbolTable &table);
};


#endif //GAMEBOYCOMPILER_NAMEANALYSIS_HPP
