#include <antlr4-runtime.h>
#include "GameboyLanguageLexer.h"
#include "GameboyLanguageParser.h"
#include "gbc/TypeAnalysis.hpp"
#include <gbc/ASTGenerationVisitor.hpp>
#include <gbc/CodeGenerator.hpp>
#include <gbc/NameAnalysis.hpp>

int main() {
    std::string filename = "tests/inputs/assignments.gb";
    antlr4::ANTLRFileStream input;
    input.loadFromFile(filename);
    gbparser::GameboyLanguageLexer lexer(&input);
    antlr4::CommonTokenStream tokenStream(&lexer);
    gbparser::GameboyLanguageParser parser(&tokenStream);

    ASTGenerationVisitor visitor;
    AST ast = visitor.generateAST(parser.program());


    NameAnalysis::annotateAST(ast);
    TypeAnalysis::analyzeTypes(ast);

    AssemblyOutput output("out.asm");
    CodeGenerator(output, ast).generateAssembly();

    return 0;
}
