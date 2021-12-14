#include <iostream>
#include <antlr4-runtime.h>
#include "GameboyLanguageLexer.h"

int main() {
    antlr4::ANTLRInputStream input(std::string("y = add (x,3)"));
    gbparser::GameboyLanguageLexer l(&input);
    for (auto token = l.nextToken();
         token != nullptr and token->getType() != gbparser::GameboyLanguageLexer::EOF;
         token = l.nextToken()) {
        std::cout << token->getText();
    }
    std::cout << std::endl;
    return 0;
}
