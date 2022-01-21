/**
 * @file NameAnalysis.hpp
 * @author ottojo
 * @date 1/9/22
 */

#ifndef GAMEBOYCOMPILER_NAMEANDTYPEANALYSIS_HPP
#define GAMEBOYCOMPILER_NAMEANDTYPEANALYSIS_HPP


#include <gbc/AST.hpp>
#include <gbc/SymbolTable.hpp>
#include <gbc/CompilerError.hpp>

// TODO: Global symbol stacks, and then for each function? How do we track local vars?

// TODO (name+type analysis): Merge both? -> Can calculate FP offsets directly, no need to traverse again?

class NameError : public CompilerError {
    public:
        NameError(const std::string &m, const SourceLocation &loc);
};

class TypeError : public CompilerError {
    public:
        TypeError(const std::string &m, const SourceLocation &loc);
};

enum class Context {
    GLOBAL, ///< Variables are placed globally
    ARGUMENT, ///< Variables are placed before FP
    BLOCK ///< Variables are placed on stack
};

class NameAndTypeAnalysis {
    public:
        NameAndTypeAnalysis(AST &ast);

        void annotateAST();

    private:
        void annotateNode(VariableInitializationNode &node);

        void annotateNode(IntegerConstantNode &node);

        void annotateNode(VariableAccessNode &node);

        void annotateNode(VariableAssignmentNode &node);

        void annotateNode(FunctionCallNode &node);

        void annotateNode(ArithmeticExpressionNode &node);

        void annotateNode(FunctionDefinitionNode &node);

        void annotateNode(ReturnNode &node);

        void prefillSymbolTable();

        int lastLocalVarFPOffset = 0;
        AST &ast;
        std::unique_ptr<SymbolTable> st;
        std::map<std::string, TypePtr> types;
        std::map<std::string, std::shared_ptr<FunctionDeclaration>> functions;

        Context context = Context::GLOBAL;
};


#endif //GAMEBOYCOMPILER_NAMEANDTYPEANALYSIS_HPP
