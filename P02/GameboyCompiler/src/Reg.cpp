/**
 * @file Reg.cpp
 * @author ottojo
 * @date 1/21/22
 * Description here TODO
 */

#include <gbc/Reg.hpp>

Reg lowReg(Reg16 r) {
    switch (r) {
        case Reg16::BC:
            return Reg::C;
        case Reg16::DE_FP:
            return Reg::E;
        case Reg16::HL:
            return Reg::L;
    }
    throw std::runtime_error{"Attempted to access nonexistent register!"};
}

Reg highReg(Reg16 r) {
    switch (r) {
        case Reg16::BC:
            return Reg::B;
        case Reg16::DE_FP:
            return Reg::D;
        case Reg16::HL:
            return Reg::H;
    }
    throw std::runtime_error{"Attempted to access nonexistent register!"};
}
