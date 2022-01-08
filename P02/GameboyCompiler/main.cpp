#include <antlr4-runtime.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include "GameboyLanguageLexer.h"
#include "GameboyLanguageParser.h"
#include "GameboyVisitor.hpp"


int main() {
    std::string filename = "tests/inputs/assignments.gb";
    std::ifstream ifstream(filename);
    if (not ifstream.is_open()) {
        throw fmt::system_error(errno, "cannot open file '{}'", filename);
    }
    antlr4::ANTLRInputStream input(ifstream);
    gbparser::GameboyLanguageLexer lexer(&input);
    antlr4::CommonTokenStream tokenStream(&lexer);
    gbparser::GameboyLanguageParser parser(&tokenStream);

    //antlr4::tree::ParseTree *parseTree = parser.program();
    //fmt::print("Parse tree:\n{}\n", parseTree->toStringTree(&parser, true));

    parser.reset();

    GameboyVisitor visitor;
    AST ast = visitor.visitProgram(parser.program()).as<AST>();
    //fmt::print("{}\n", ast.nodes.size());

    AssemblyOutput output("out.asm");
    ast.generateCode(output);

    return 0;
}
