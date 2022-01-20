/**
 * @file AssemblyOutput.cpp
 * @author ottojo
 * @date 1/8/22
 * Description here TODO
 */

#include <fmt/format.h>
#include <gbc/AssemblyOutput.hpp>
#include <cassert>

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
    print(fmt::format("add HL, {} ; HL += {}\n", reg, reg));
}

void AssemblyOutput::pop16BitReg(Reg16 reg) {
    print(fmt::format("POP {} ; {} <- stack\n", reg, reg));

}

void AssemblyOutput::push16BitConst(int16_t i) {
    print(fmt::format("; stack <- {}\n", i));
    load16BitConst(Reg16::BC, i);
    push16BitReg(Reg16::BC);
}

void AssemblyOutput::push16BitReg(Reg16 reg) {
    print(fmt::format("PUSH {} ; stack <- {}\n", reg, reg));
}

void AssemblyOutput::load16BitConst(Reg16 reg, uint16_t i) {
    print(fmt::format("LD {}, ${:X}; {} <- ${:X}\n", reg, i, reg, i));
}

void AssemblyOutput::builtin_printWord() {
    call("printWord");
}

void AssemblyOutput::call(const std::string &name) {
    print(fmt::format("call {}\n", name));
}

void AssemblyOutput::comment(const std::string &comment) {
    print(fmt::format("; {}\n", comment));

}

void AssemblyOutput::preamble() {
    print(".include \"../framework.asm\"\n");
}

void AssemblyOutput::pop16ToMemory(Address a) {
    if (std::holds_alternative<NumericAddress>(a)) {
        comment(fmt::format("*{:#x} <- stack", std::get<NumericAddress>(a).a));
        pop16BitReg(Reg16::BC);
        ldReg(Reg::A, Reg::B);
        load16BitConst(Reg16::HL, std::get<NumericAddress>(a).a);
    } else if (std::holds_alternative<std::string>(a)) {
        comment(fmt::format("{} <- stack", std::get<std::string>(a)));
        pop16BitReg(Reg16::BC);
        ldReg(Reg::A, Reg::B);
        loadAddressLabel(Reg16::HL, get<std::string>(a));
    } else {
        throw std::runtime_error{"implement the new addressing format!!!"};
    }
    print(fmt::format("LDI (HL), A\n"));
    ldReg(Reg::A, Reg::C);
    print(fmt::format("LDI (HL), A\n"));
}

void AssemblyOutput::ldReg(Reg target, Reg src) {
    print(fmt::format("LD {}, {}; {} <- {}\n", target, src, target, src));
}

void AssemblyOutput::push16FromMemory(Address a) {
    if (std::holds_alternative<NumericAddress>(a)) {
        comment(fmt::format("stack <- *{:#x}", get<NumericAddress>(a).a));
        load16BitConst(Reg16::HL, get<NumericAddress>(a).a);
    } else if (std::holds_alternative<std::string>(a)) {
        comment(fmt::format("stack <- {}", get<std::string>(a)));
        loadAddressLabel(Reg16::HL, get<std::string>(a));
    } else {
        assert(false);
    }
    print(fmt::format("LDI A, (HL); A = *HL; HL++\n"));
    ldReg(Reg::B, Reg::A);
    print(fmt::format("LDI A, (HL); A = *HL; HL++\n"));
    ldReg(Reg::C, Reg::A);
    push16BitReg(Reg16::BC);
}

void AssemblyOutput::print(std::string_view m) {
    fmt::print(outputFile, "{:{}s}{:s}", "", ind, m);
}

void AssemblyOutput::indent() {
    ind += 4;
}

void AssemblyOutput::unindent() {
    ind -= 4;
}

void AssemblyOutput::sectionWithLabel(const std::string &name) {
    print(fmt::format(".section \"{}\"\n", name));
    print(fmt::format("{}:\n", name));
}

void AssemblyOutput::sectionEnd() {
    print(".ends\n");
}

void AssemblyOutput::ramSection(const std::string &name) {
    print(fmt::format(".ramsection \"{}\" slot 1\n", name));
}

void AssemblyOutput::defineVariable(const std::string &name, int size_bytes) {
    print(fmt::format("{} dsb {}; Variable \"{}\" size {}B\n", name, size_bytes, name, size_bytes));
}

void AssemblyOutput::loadAddressLabel(Reg16 target, const std::string &name) {
    print(fmt::format("LD {}, {}; {} <- &{}\n", target, name, target, name));

}

void AssemblyOutput::ret() {
    print("ret\n");
}


