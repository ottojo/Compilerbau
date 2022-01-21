/**
 * @file Reg.hpp
 * @author ottojo
 * @date 1/9/22
 */

#ifndef GAMEBOYCOMPILER_REG_HPP
#define GAMEBOYCOMPILER_REG_HPP

#include <fmt/format.h>

enum class Reg {
    A, B, C, D, E, H, L
};

enum class Reg16 {
    BC, DE_FP, HL, FP = DE_FP
};

Reg lowReg(Reg16 r);
Reg highReg(Reg16 r);

template<>
struct fmt::formatter<Reg> : formatter<std::string_view> {
    // parse is inherited from formatter<string_view>.
    template<typename FormatContext>
    auto format(Reg c, FormatContext &ctx) {
        string_view name = "unknown";
        switch (c) {
            case Reg::A:
                name = "A";
                break;
            case Reg::B:
                name = "B";
                break;
            case Reg::C:
                name = "C";
                break;
            case Reg::D:
                name = "D";
                break;
            case Reg::E:
                name = "E";
                break;
            case Reg::H:
                name = "H";
                break;
            case Reg::L:
                name = "L";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

template<>
struct fmt::formatter<Reg16> : formatter<std::string_view> {
    // parse is inherited from formatter<string_view>.
    template<typename FormatContext>
    auto format(Reg16 c, FormatContext &ctx) {
        string_view name = "unknown";
        switch (c) {
            case Reg16::BC:
                name = "BC";
                break;
            case Reg16::DE_FP:
                name = "DE";
                break;
            case Reg16::HL:
                name = "HL";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

#endif //GAMEBOYCOMPILER_REG_HPP
