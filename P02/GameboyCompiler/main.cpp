#include <iostream>
#include <antlr4-runtime.h>
#include "GameboyLanguageLexer.h"
#include "GameboyLanguageParser.h"

int main() {
    antlr4::ANTLRInputStream input(std::string("y = add (x,3);"));
    gbparser::GameboyLanguageLexer lexer(&input);
    antlr4::CommonTokenStream tokenStream(&lexer);
    gbparser::GameboyLanguageParser parser(&tokenStream);
    antlr4::tree::ParseTree *parseTree = parser.program();
    std::cout << parseTree->toStringTree(&parser, true) << std::endl;
    return 0;
}
