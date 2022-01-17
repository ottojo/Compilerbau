/**
 * @file TypeAnalysis.hpp
 * @author ottojo
 * @date 1/16/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_TYPEANALYSIS_HPP
#define GAMEBOYCOMPILER_TYPEANALYSIS_HPP


#include "AST.hpp"

class TypeAnalysis {
    public:
        static void analyzeTypes(AST &ast);

        static void prefillTypeTable(TypeTable &table);
};


#endif //GAMEBOYCOMPILER_TYPEANALYSIS_HPP
