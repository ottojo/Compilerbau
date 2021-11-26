#include <string>
#include <iostream>
#include "CalculatorParser.hpp"

int main(int argc, char *argv[]) {
    std::string expression;
    for (int i = 1; i < argc; i++) {
        expression += std::string(argv[i]) + " ";
    }
    CalculatorParser p(expression);
    auto result = p.parse();
    if (result.has_value()) {
        std::cout << result.value() << std::endl;
        return 0;
    }
    return 1;
}
