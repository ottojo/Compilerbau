//
// Created by jonas on 14.12.21.
//

#include "DeliveryVisitor.hpp"
#include "DeliveryParser.h"
#include "DeliveryLexer.h"

int main() {
    std::ifstream stream("tests/Delivery/invoice1.del");
    if (not stream.is_open()) {
        std::cerr << "File not found" << std::endl;
        return 1;
    }
    antlr4::ANTLRInputStream inputStream(stream);

    delivery::DeliveryLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);

    delivery::DeliveryParser parser(&tokenStream);
    auto mainCtx = parser.delivery();

    DeliveryVisitor visitor;
    auto result = visitor.visit(mainCtx);
    auto f = result.as<float>();

    std::cout << f << std::endl;
}
