/**
 * @file SourceLocation.hpp
 * @author ottojo
 * @date 1/9/22
 */

#ifndef GAMEBOYCOMPILER_SOURCELOCATION_HPP
#define GAMEBOYCOMPILER_SOURCELOCATION_HPP

#include <string>
#include <Token.h>
#include <fmt/format.h>

class SourceLocation {
    public:
        SourceLocation() = default;

        explicit SourceLocation(antlr4::Token *token);

        std::string file;
        std::size_t line = 0;
        std::size_t column = 0;
};


template<>
struct fmt::formatter<SourceLocation> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(SourceLocation c, FormatContext &ctx) {
        std::string s = fmt::format("{}:{}:{}", c.file, c.line, c.column);
        return formatter<string_view>::format(s, ctx);
    }
};


#endif //GAMEBOYCOMPILER_SOURCELOCATION_HPP
