/**
 * @file AssemblyOutput.hpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#ifndef GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP
#define GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP

#include <cstdint>
#include <cstdio>
#include <string>
#include <fmt/format.h>

enum class Reg {
    A, B, C, D, E, H, L
};


enum class Reg16 {
    BC, DE, HL
};


class AssemblyOutput {
    public:

        explicit AssemblyOutput(const std::string &filename);

        void preamble();
        void finalize();

        void comment(const std::string &comment);

        void load16BitConst(Reg16 reg, int16_t i);

        /**
         * Loads 16bit const into BC and pushes it to stack
         */
        void push16BitConst(int16_t i);

        void push16BitReg(Reg16 reg);

        void pop16BitReg(Reg16 reg);

        /**
         * adds 2 numbers from stack, pushes result to stack.
         * Modifies HL, BC
         */
        void stackAdd16();

        /**
         * Prints word in HL
         */
        void builtin_printWord();

        void call(const std::string &name);

    private:

        std::FILE *outputFile;

        /**
         * HL += reg
         */
        void add16HL(Reg16 reg);
};

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
            case Reg16::DE:
                name = "DE";
                break;
            case Reg16::HL:
                name = "HL";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

#endif //GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP
