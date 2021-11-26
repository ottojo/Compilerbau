#ifndef P01_TOKEN_HPP
#define P01_TOKEN_HPP

enum class TokenClass {
    NUM,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    POW,
    LPAREN,
    RPAREN,
    END_OF_STREAM
};

struct Token {
    TokenClass tokenClass;
    int value;
};

#endif //P01_TOKEN_HPP
