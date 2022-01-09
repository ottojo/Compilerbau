/**
 * @file CodeGenerator.hpp
 * @author ottojo
 * @date 1/9/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_CODEGENERATOR_HPP
#define GAMEBOYCOMPILER_CODEGENERATOR_HPP


#include "AssemblyOutput.hpp"
#include "AST.hpp"

class CodeGenerator {
    public:

    public:
        CodeGenerator(AssemblyOutput &out, const AST &ast, SymbolTable &symbolTable);

        void generateAssembly();

    private:

        AssemblyOutput &out;
        const AST &ast;
        SymbolTable &symbolTable;

        int ind = 0;

        /**
         * Generic method that calls the generation method for the specific type of node
         * @param node Generic pointer to a node
         */
        void generateAssembly(const AST::MutNodePtr &node);

        void generateAssembly(const ArithmeticExpressionNode &node);

        void generateAssembly(const MethodCallNode &node);

        void generateAssembly(const VariableDeclarationNode &node);

        void generateAssembly(const VariableAssignmentNode &node);

        void generateAssembly(const IntegerConstantNode &node);

        void generateAssembly(const VariableAccessNode &node);

        void assignGlobals();

        static uint16_t addressOfGlobal(const std::string &id, const SymbolTable &symbolTable);
};


#endif //GAMEBOYCOMPILER_CODEGENERATOR_HPP
