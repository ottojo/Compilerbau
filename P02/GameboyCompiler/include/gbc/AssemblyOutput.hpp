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
#include <gbc/Reg.hpp>

struct Address {
    uint16_t a;
};


class AssemblyOutput {
    public:

        explicit AssemblyOutput(const std::string &filename);

        void preamble();

        void finalize();

        void comment(const std::string &comment);

        void load16BitConst(Reg16 reg, uint16_t i);

        /**
         * Loads 16bit const into BC and pushes it to stack
         */
        void push16BitConst(int16_t i);

        void push16BitReg(Reg16 reg);

        void pop16BitReg(Reg16 reg);

        void ldReg(Reg target, Reg src);

        void pop16ToAddr(Address a);

        void push16FromAddr(Address a);

        //void

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

        void sectionWithLabel(const std::string &name);

        void sectionEnd();

        void indent();

        void unindent();

    private:

        int ind = 0; ///< current indent (for easily readable output)

        std::FILE *outputFile;

        void print(std::string_view m);

        /**
         * HL += reg
         */
        void add16HL(Reg16 reg);
};

#endif //GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP
