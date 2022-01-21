/**
 * @file AssemblyOutput.cpp
 * @author ottojo
 * @date 1/8/22
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

void AssemblyOutput::pop16ToMemory(VarAccess::Location a) {
    if (std::holds_alternative<VarAccess::FPRelative>(a)) {
        auto offset = std::get<VarAccess::FPRelative>(a).offset;
        comment(fmt::format("*(FP{:+}) <- stack", offset));
        indent();
        // Address to HL
        load16BitConst(Reg16::HL, offset);
        add16HL(Reg16::FP);
    } else if (std::holds_alternative<VarAccess::ByGlobalName>(a)) {
        comment(fmt::format("{} <- stack", std::get<VarAccess::ByGlobalName>(a).name));
        indent();
        // Address to HL
        loadAddressLabel(Reg16::HL, get<VarAccess::ByGlobalName>(a).name);
    } else {
        throw std::runtime_error{"implement the new addressing format!!!"};
    }
    pop16BitReg(Reg16::BC);
    comment("*HL <- BC");
    ldReg(Reg::A, Reg::B);
    print(fmt::format("LDI (HL), A; A = *HL; HL++\n"));
    ldReg(Reg::A, Reg::C);
    print(fmt::format("LDI (HL), A; A = *HL; HL++\n"));
    unindent();
}

void AssemblyOutput::ldReg(Reg target, Reg src) {
    if (target == src) {
        return;
    }
    print(fmt::format("LD {}, {}; {} <- {}\n", target, src, target, src));
}

void AssemblyOutput::ldReg16(Reg16 target, Reg16 src) {
    ldReg(lowReg(target), lowReg(src));
    ldReg(highReg(target), highReg(src));
}

void AssemblyOutput::push16FromMemory(VarAccess::Location a) {
    if (std::holds_alternative<VarAccess::FPRelative>(a)) {
        auto offset = std::get<VarAccess::FPRelative>(a).offset;
        comment(fmt::format("stack <- *(FP{:+})", offset));
        load16BitConst(Reg16::HL, offset);
        add16HL(Reg16::FP);
    } else if (std::holds_alternative<VarAccess::ByGlobalName>(a)) {
        comment(fmt::format("stack <- {}", get<VarAccess::ByGlobalName>(a).name));
        loadAddressLabel(Reg16::HL, get<VarAccess::ByGlobalName>(a).name);
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

void AssemblyOutput::saveSPtoFP() {
    comment("FP <- SP");
    print("LD HL, SP+0\n");
    ldReg16(Reg16::FP, Reg16::HL);
}

void AssemblyOutput::restoreSPfromFP() {
    comment("SP <- FP");
    ldReg16(Reg16::HL, Reg16::FP);
    print("LD SP, HL\n");

}

void AssemblyOutput::addSP(std::int16_t val) {
    print(fmt::format("ADD SP, {:+}\n", val));
}


