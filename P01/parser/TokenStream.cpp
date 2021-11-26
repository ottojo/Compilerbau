#include "TokenStream.hpp"

TokenStream::TokenStream(const std::string &input) {
    for (const auto &c: input) {
        if (c != ' ') {
            remainingInput += c;
        }
    }
}

Token TokenStream::getNext() {
    // End of string
    if (remainingInput.empty()) {
        return {.tokenClass=TokenClass::END_OF_STREAM};
    }

    // Number
    if (std::isdigit(remainingInput[0])) {
        std::string number;
        for (int i = 0; i < remainingInput.length() and std::isdigit(remainingInput[i]); i++) {
            number += remainingInput[i];
        }
        remainingInput.erase(0, number.length());
        int value = std::atoi(number.c_str());
        return {.tokenClass=TokenClass::NUM, .value=value};
    }

    auto in = remainingInput[0];
    remainingInput.erase(0, 1);
    switch (in) {
        case '+':
            return {.tokenClass=TokenClass::PLUS};
        case '-':
            return {.tokenClass=TokenClass::MINUS};
        case '*':
            return {.tokenClass=TokenClass::TIMES};
        case '/':
            return {.tokenClass=TokenClass::DIV};
        case '^':
            return {.tokenClass=TokenClass::POW};
        case '(':
            return {.tokenClass=TokenClass::LPAREN};
        case ')':
            return {.tokenClass=TokenClass::RPAREN};
        default:
            throw LexingError(0);
    }

}

const char *LexingError::what() const noexcept {
    return errorMessage.c_str();
}

LexingError::LexingError(int pos) : errorMessage("Lexing error at position " + std::to_string(pos)) {}
