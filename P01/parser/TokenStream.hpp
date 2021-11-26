#ifndef P01_TOKENSTREAM_HPP
#define P01_TOKENSTREAM_HPP

#include <filesystem>

#include "Token.hpp"

class LexingError : public std::exception {
    public:
        explicit LexingError(int pos);
        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

    private:
        std::string errorMessage;
};

class TokenStream {
    public:
        explicit TokenStream(const std::string &input);

        Token getNext();

    private:
        std::string remainingInput;
};


#endif //P01_TOKENSTREAM_HPP
