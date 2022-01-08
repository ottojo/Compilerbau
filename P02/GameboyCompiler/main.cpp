#include <antlr4-runtime.h>
#include <fmt/core.h>
#include "GameboyLanguageLexer.h"
#include "GameboyLanguageParser.h"
#include "GameboyVisitor.hpp"

int main() {
    std::string filename = "tests/inputs/assignments.gb";
    std::ifstream ifstream(filename);
    if (not ifstream.is_open()) {
        fmt::print(stderr, "Error opening file \"{}\"!", filename);
        return 1;
    }
    antlr4::ANTLRInputStream input(ifstream);
    gbparser::GameboyLanguageLexer lexer(&input);
    antlr4::CommonTokenStream tokenStream(&lexer);
    gbparser::GameboyLanguageParser parser(&tokenStream);

    antlr4::tree::ParseTree *parseTree = parser.program();
    fmt::print("Parse tree:\n{}\n", parseTree->toStringTree(&parser, true));

    parser.reset();

    GameboyVisitor visitor;
    AST ast = visitor.visitProgram(parser.program()).as<AST>();

    return 0;
}
