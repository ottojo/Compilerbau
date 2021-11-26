#include "ParserException.hpp"

#include <utility>

ParserException::ParserException(std::string message) :m(std::move(message)){}

const char *ParserException::what() const noexcept {
    return m.c_str();
}
