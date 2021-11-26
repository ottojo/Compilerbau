#ifndef P01_CALCULATORPARSER_HPP
#define P01_CALCULATORPARSER_HPP

#include <string>
#include <optional>

#include "TokenStream.hpp"

class CalculatorParser {
    public:
        explicit CalculatorParser(const std::string &input);

        std::optional<double> parse();

    private:

        double s();

        double expression();

        double term();

        double factor();

        double unit();

        double literal();

        void readToken();

        TokenStream ts;
        Token currToken{};
};

#endif //P01_CALCULATORPARSER_HPP
