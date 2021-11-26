#include <cmath>
#include <iostream>
#include "CalculatorParser.hpp"
#include "ParserException.hpp"

CalculatorParser::CalculatorParser(const std::string &input) : ts(input) {}

std::optional<double> CalculatorParser::parse() {
    try {
        currToken = ts.getNext();
        return s();
    } catch (ParserException &e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    } catch (LexingError &e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }
}

double CalculatorParser::s() {
    auto e = expression();
    if (currToken.tokenClass != TokenClass::END_OF_STREAM) {
        throw ParserException("Unexpected token");
    }
    return e;
}

double CalculatorParser::expression() {
    double left = term();
    if (currToken.tokenClass == TokenClass::PLUS) {
        readToken();
        return left + expression();
    } else if (currToken.tokenClass == TokenClass::MINUS) {
        readToken();
        return left - expression();
    } else {
        return left;
    }
}

void CalculatorParser::readToken() {
    currToken = ts.getNext();
}

double CalculatorParser::term() {
    double left = factor();
    if (currToken.tokenClass == TokenClass::TIMES) {
        readToken();
        return left * term();
    } else {
        return left;
    }
}

double CalculatorParser::factor() {
    double left = unit();
    if (currToken.tokenClass == TokenClass::POW) {
        readToken();
        return std::pow(left, unit());
    } else if (currToken.tokenClass == TokenClass::DIV) {
        readToken();
        return left / unit();
    } else {
        return left;
    }
}

double CalculatorParser::unit() {
    if (currToken.tokenClass == TokenClass::LPAREN) {
        readToken();
        double e = expression();
        if (currToken.tokenClass == TokenClass::RPAREN) {
            readToken();
            return e;
        } else {
            throw ParserException("Expected RPAREN");
        }
    } else {
        return literal();
    }
}

double CalculatorParser::literal() {
    if (currToken.tokenClass == TokenClass::MINUS) {
        readToken();
        if (currToken.tokenClass == TokenClass::NUM) {
            double val = currToken.value;
            readToken();
            return -1 * val;
        } else {
            throw ParserException("Expected NUM");
        }
    } else if (currToken.tokenClass == TokenClass::NUM) {
        double val = currToken.value;
        readToken();
        return val;
    } else {
        throw ParserException("Expected NUM");
    }
}
