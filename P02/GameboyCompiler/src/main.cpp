#include <antlr4-runtime.h>
#include "GameboyLanguageLexer.h"
#include "GameboyLanguageParser.h"
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

    auto symbolTable = NameAnalysis::annotateAST(ast);

    AssemblyOutput output("out.asm");
    CodeGenerator(output, ast, *symbolTable).generateAssembly();

    return 0;
}
