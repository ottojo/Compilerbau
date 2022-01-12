/**
 * @file CompilerError.hpp
 * @author ottojo
 * @date 1/12/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_COMPILERERROR_HPP
#define GAMEBOYCOMPILER_COMPILERERROR_HPP

#include <exception>
#include <string>
#include "SourceLocation.hpp"

class CompilerError : public std::exception {
    public:
        CompilerError(const std::string &m, const SourceLocation &loc);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        std::string message;
};

#endif //GAMEBOYCOMPILER_COMPILERERROR_HPP
