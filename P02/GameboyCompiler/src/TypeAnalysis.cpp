/**
 * @file TypeAnalysis.cpp
 * @author ottojo
 * @date 1/16/22
 */

#include <gbc/TypeAnalysis.hpp>

void TypeAnalysis::analyzeTypes(AST &ast) {
    if (ast.typeTable != nullptr) {
        throw std::runtime_error{"AST already has type stacks!"};
    }
    ast.typeTable = std::make_unique<TypeTable>();

    prefillTypeTable(*ast.typeTable);
   // for (auto &node: ast.nodes) {
        //annotateNode(*ast.typeTable, node);
    //}
}

void TypeAnalysis::prefillTypeTable(TypeTable &table) {
    table.enter("int16", TypeInfo{.size=2});
}
