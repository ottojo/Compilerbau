/**
 * @file AssemblyOutput.hpp
 * @author ottojo
 * @date 1/8/22
 */

#ifndef GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP
#define GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP

#include <cstdint>
#include <cstdio>
#include <string>
#include <fmt/format.h>
#include <gbc/Reg.hpp>
#include <variant>

struct NumericAddress {
    uint16_t a;
};

using Address = std::variant<NumericAddress, std::string>;


class AssemblyOutput {
    public:

        explicit AssemblyOutput(const std::string &filename);

        void preamble();

        void comment(const std::string &comment);

        void load16BitConst(Reg16 reg, uint16_t i);

        /**
         * Loads 16bit const into BC and pushes it to stack
         */
        void push16BitConst(int16_t i);

        void push16BitReg(Reg16 reg);

        void pop16BitReg(Reg16 reg);

        void ldReg(Reg target, Reg src);

        void ldReg16(Reg16 target, Reg16 src);

        /**
         * Modifies HL
         */
        void saveSPtoFP();

        void pop16ToMemory(Address a);

        void push16FromMemory(Address a);

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

        void ramSection(const std::string &name);

        void defineVariable(const std::string &name, int size_bytes);

        void sectionEnd();

        void indent();

        void unindent();

        void ret();

        /**
         * Modifies HL!
         */
        void restoreSPfromFP();

    private:

        int ind = 0; ///< current indent (for easily readable output)

        std::FILE *outputFile;

        void print(std::string_view m);

        /**
         * HL += reg
         */
        void add16HL(Reg16 reg);

        void loadAddressLabel(Reg16 target, const std::string &name);
};

#endif //GAMEBOYCOMPILER_ASSEMBLYOUTPUT_HPP
