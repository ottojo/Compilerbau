/**
 * @file CodeGenerator.hpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_CODEGENERATOR_HPP
#define GAMEBOYCOMPILER_CODEGENERATOR_HPP


#include <gbc/AssemblyOutput.hpp>
#include <gbc/AST.hpp>

class CodeGenerator {
    public:

    public:
        CodeGenerator(AssemblyOutput &out, const AST &ast);

        void generateAssembly();

    private:

        AssemblyOutput &out;
        const AST &ast;

        // int ind = 0;

        void generateAssembly(const ArithmeticExpressionNode &node);

        void generateAssembly(const MethodCallNode &node);

        void generateAssembly(const VariableDeclarationNode &node);

        void generateAssembly(const VariableAssignmentNode &node);

        void generateAssembly(const IntegerConstantNode &node);

        void generateAssembly(const VariableAccessNode &node);

        void generateAssembly(const FunctionDefinitionNode &node);

        void assignGlobals();

        static uint16_t addressOfGlobal(const std::string &id, const SymbolTable &symbolTable);

    void generateMain();

        void initializeGlobalVars();

        void generateFuncs();
};


#endif //GAMEBOYCOMPILER_CODEGENERATOR_HPP
