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
        static void generateAssembly(AssemblyOutput &out, const AST &ast);

    private:
        /**
         * Generic method that calls the generation method for the specific type of node
         * @param out Assembly output
         * @param node Generic pointer to a node
         */
        static void generateAssembly(AssemblyOutput &out, const AST::MutNodePtr &node);

        static void generateAssembly(AssemblyOutput &out, const ArithmeticExpressionNode &node);

        static void generateAssembly(AssemblyOutput &out, const MethodCallNode &node);

        static void generateAssembly(AssemblyOutput &out, const VariableDeclarationNode &node);

        static void generateAssembly(AssemblyOutput &out, const VariableAssignmentNode &node);

        static void generateAssembly(AssemblyOutput &out, const IntegerConstantNode &node);

        static void generateAssembly(AssemblyOutput &out, const VariableAccessNode &node);


};


#endif //GAMEBOYCOMPILER_CODEGENERATOR_HPP
