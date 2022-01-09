/**
 * @file AssemblyOutput.cpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#include <fmt/format.h>
#include "AssemblyOutput.hpp"

AssemblyOutput::AssemblyOutput(const std::string &filename) : outputFile(std::fopen(filename.c_str(), "w")) {
    if (!outputFile) {
        throw fmt::system_error(errno, "cannot open file '{}'", filename);
    }
}

void AssemblyOutput::stackAdd16() {
    pop16BitReg(Reg16::HL);
    pop16BitReg(Reg16::BC);
    add16HL(Reg16::BC);
    push16BitReg(Reg16::HL);
}

void AssemblyOutput::add16HL(Reg16 reg) {
    fmt::print(outputFile, "add HL, {} ; HL += {}\n", reg, reg);
}

void AssemblyOutput::pop16BitReg(Reg16 reg) {
    fmt::print(outputFile, "POP {} ; {} <- stack\n", reg, reg);

}

void AssemblyOutput::push16BitConst(int16_t i) {
    fmt::print(outputFile, "; stack <- {}\n", i);
    load16BitConst(Reg16::BC, i);
    push16BitReg(Reg16::BC);
}

void AssemblyOutput::push16BitReg(Reg16 reg) {
    fmt::print(outputFile, "PUSH {} ; stack <- {}\n", reg, reg);
}

void AssemblyOutput::load16BitConst(Reg16 reg, uint16_t i) {
    fmt::print(outputFile, "LD {}, ${:X}; {} <- ${:X}\n", reg, i, reg, i);
}

void AssemblyOutput::builtin_printWord() {
    call("printWord");
}

void AssemblyOutput::call(const std::string &name) {
    fmt::print(outputFile, "call {}\n", name);
}

void AssemblyOutput::comment(const std::string &comment) {
    fmt::print(outputFile, "; {}\n", comment);

}

void AssemblyOutput::preamble() {
    fmt::print(outputFile, ".include \"../framework.asm\"\n");
    fmt::print(outputFile, ".section \"main\"\n");
    fmt::print(outputFile, "main:\n");
}

void AssemblyOutput::finalize() {
    fmt::print(outputFile, "ret\n");
    fmt::print(outputFile, ".ends\n");
}

void AssemblyOutput::pop16ToAddr(Address a) {
    comment(fmt::format("*{:#x} <- stack", a.a));
    pop16BitReg(Reg16::BC);
    ldReg(Reg::A, Reg::B);
    load16BitConst(Reg16::HL, a.a);
    fmt::print(outputFile, "LDI (HL), A\n");
    ldReg(Reg::A, Reg::C);
    fmt::print(outputFile, "LDI (HL), A\n");
}

void AssemblyOutput::ldReg(Reg target, Reg src) {
    fmt::print(outputFile, "LD {}, {}; {} <- {}\n", target, src, target, src);
}

void AssemblyOutput::push16FromAddr(Address a) {
    comment(fmt::format("stack <- *{:#x}", a.a));
    load16BitConst(Reg16::HL, a.a);
    fmt::print(outputFile, "LDI A, (HL)\n");
    ldReg(Reg::B, Reg::A);
    fmt::print(outputFile, "LDI A, (HL)\n");
    ldReg(Reg::C, Reg::A);
    push16BitReg(Reg16::BC);
}


