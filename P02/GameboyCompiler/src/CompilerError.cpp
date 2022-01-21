/**
 * @file CompilerError.cpp
 * @author ottojo
 * @date 1/12/22
 */

#include <gbc/CompilerError.hpp>


CompilerError::CompilerError(const std::string &m, const SourceLocation &loc) :
        message(fmt::format("{}: {}", loc, m)) {}

const char *CompilerError::what() const noexcept {
    return message.c_str();
}
