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
        static void generateAssembly(AssemblyOutput &out, const AST &ast, SymbolTable &symbolTable);

    private:
        /**
         * Generic method that calls the generation method for the specific type of node
         * @param out Assembly output
         * @param node Generic pointer to a node
         */
        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const AST::MutNodePtr &node);

        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const ArithmeticExpressionNode &node);

        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const MethodCallNode &node);

        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const VariableDeclarationNode &node);

        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const VariableAssignmentNode &node);

        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const IntegerConstantNode &node);

        static void generateAssembly(AssemblyOutput &out,const SymbolTable &symbolTable, const VariableAccessNode &node);

        static void assignGlobals(AssemblyOutput &out, SymbolTable &symbolTable);

        static uint16_t addressOfGlobal(const std::string& id, const SymbolTable &symbolTable);

};


#endif //GAMEBOYCOMPILER_CODEGENERATOR_HPP
