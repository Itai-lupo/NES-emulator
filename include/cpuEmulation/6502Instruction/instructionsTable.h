#pragma once
#include "instrction.h"


#define CPU_6502_INSTRUCTION_SET_TABLE \
{   \
    { "BRK", &cpu6502::BRK, &cpu6502::IMM, 7 }, { "ORA", &cpu6502::ORA, &cpu6502::IZX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 3 }, { "ORA", &cpu6502::ORA, &cpu6502::ZP0, 3 }, { "ASL", &cpu6502::ASL, &cpu6502::ZP0, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "PHP", &cpu6502::PHP, &cpu6502::IMP, 3 }, { "ORA", &cpu6502::ORA, &cpu6502::IMM, 2 }, { "ASL", &cpu6502::ASL, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "ORA", &cpu6502::ORA, &cpu6502::ABS, 4 }, { "ASL", &cpu6502::ASL, &cpu6502::ABS, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, \
    { "BPL", &cpu6502::BPL, &cpu6502::REL, 2 }, { "ORA", &cpu6502::ORA, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "ORA", &cpu6502::ORA, &cpu6502::ZPX, 4 }, { "ASL", &cpu6502::ASL, &cpu6502::ZPX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "CLC", &cpu6502::CLC, &cpu6502::IMP, 2 }, { "ORA", &cpu6502::ORA, &cpu6502::ABY, 4 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "ORA", &cpu6502::ORA, &cpu6502::ABX, 4 }, { "ASL", &cpu6502::ASL, &cpu6502::ABX, 7 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, \
    { "JSR", &cpu6502::JSR, &cpu6502::ABS, 6 }, { "AND", &cpu6502::AND, &cpu6502::IZX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "BIT", &cpu6502::BIT, &cpu6502::ZP0, 3 }, { "AND", &cpu6502::AND, &cpu6502::ZP0, 3 }, { "ROL", &cpu6502::ROL, &cpu6502::ZP0, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "PLP", &cpu6502::PLP, &cpu6502::IMP, 4 }, { "AND", &cpu6502::AND, &cpu6502::IMM, 2 }, { "ROL", &cpu6502::ROL, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "BIT", &cpu6502::BIT, &cpu6502::ABS, 4 }, { "AND", &cpu6502::AND, &cpu6502::ABS, 4 }, { "ROL", &cpu6502::ROL, &cpu6502::ABS, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, \
    { "BMI", &cpu6502::BMI, &cpu6502::REL, 2 }, { "AND", &cpu6502::AND, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "AND", &cpu6502::AND, &cpu6502::ZPX, 4 }, { "ROL", &cpu6502::ROL, &cpu6502::ZPX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "SEC", &cpu6502::SEC, &cpu6502::IMP, 2 }, { "AND", &cpu6502::AND, &cpu6502::ABY, 4 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "AND", &cpu6502::AND, &cpu6502::ABX, 4 }, { "ROL", &cpu6502::ROL, &cpu6502::ABX, 7 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, \
    { "RTI", &cpu6502::RTI, &cpu6502::IMP, 6 }, { "EOR", &cpu6502::EOR, &cpu6502::IZX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 3 }, { "EOR", &cpu6502::EOR, &cpu6502::ZP0, 3 }, { "LSR", &cpu6502::LSR, &cpu6502::ZP0, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "PHA", &cpu6502::PHA, &cpu6502::IMP, 3 }, { "EOR", &cpu6502::EOR, &cpu6502::IMM, 2 }, { "LSR", &cpu6502::LSR, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "JMP", &cpu6502::JMP, &cpu6502::ABS, 3 }, { "EOR", &cpu6502::EOR, &cpu6502::ABS, 4 }, { "LSR", &cpu6502::LSR, &cpu6502::ABS, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, \
    { "BVC", &cpu6502::BVC, &cpu6502::REL, 2 }, { "EOR", &cpu6502::EOR, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "EOR", &cpu6502::EOR, &cpu6502::ZPX, 4 }, { "LSR", &cpu6502::LSR, &cpu6502::ZPX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "CLI", &cpu6502::CLI, &cpu6502::IMP, 2 }, { "EOR", &cpu6502::EOR, &cpu6502::ABY, 4 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "EOR", &cpu6502::EOR, &cpu6502::ABX, 4 }, { "LSR", &cpu6502::LSR, &cpu6502::ABX, 7 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, \
    { "RTS", &cpu6502::RTS, &cpu6502::IMP, 6 }, { "ADC", &cpu6502::ADC, &cpu6502::IZX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 3 }, { "ADC", &cpu6502::ADC, &cpu6502::ZP0, 3 }, { "ROR", &cpu6502::ROR, &cpu6502::ZP0, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "PLA", &cpu6502::PLA, &cpu6502::IMP, 4 }, { "ADC", &cpu6502::ADC, &cpu6502::IMM, 2 }, { "ROR", &cpu6502::ROR, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "JMP", &cpu6502::JMP, &cpu6502::IND, 5 }, { "ADC", &cpu6502::ADC, &cpu6502::ABS, 4 }, { "ROR", &cpu6502::ROR, &cpu6502::ABS, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, \
    { "BVS", &cpu6502::BVS, &cpu6502::REL, 2 }, { "ADC", &cpu6502::ADC, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "ADC", &cpu6502::ADC, &cpu6502::ZPX, 4 }, { "ROR", &cpu6502::ROR, &cpu6502::ZPX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "SEI", &cpu6502::SEI, &cpu6502::IMP, 2 }, { "ADC", &cpu6502::ADC, &cpu6502::ABY, 4 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "ADC", &cpu6502::ADC, &cpu6502::ABX, 4 }, { "ROR", &cpu6502::ROR, &cpu6502::ABX, 7 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, \
    { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "STA", &cpu6502::STA, &cpu6502::IZX, 6 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "STY", &cpu6502::STY, &cpu6502::ZP0, 3 }, { "STA", &cpu6502::STA, &cpu6502::ZP0, 3 }, { "STX", &cpu6502::STX, &cpu6502::ZP0, 3 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 3 }, { "DEY", &cpu6502::DEY, &cpu6502::IMP, 2 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "TXA", &cpu6502::TXA, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "STY", &cpu6502::STY, &cpu6502::ABS, 4 }, { "STA", &cpu6502::STA, &cpu6502::ABS, 4 }, { "STX", &cpu6502::STX, &cpu6502::ABS, 4 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 4 }, \
    { "BCC", &cpu6502::BCC, &cpu6502::REL, 2 }, { "STA", &cpu6502::STA, &cpu6502::IZY, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "STY", &cpu6502::STY, &cpu6502::ZPX, 4 }, { "STA", &cpu6502::STA, &cpu6502::ZPX, 4 }, { "STX", &cpu6502::STX, &cpu6502::ZPY, 4 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 4 }, { "TYA", &cpu6502::TYA, &cpu6502::IMP, 2 }, { "STA", &cpu6502::STA, &cpu6502::ABY, 5 }, { "TXS", &cpu6502::TXS, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 5 }, { "STA", &cpu6502::STA, &cpu6502::ABX, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, \
    { "LDY", &cpu6502::LDY, &cpu6502::IMM, 2 }, { "LDA", &cpu6502::LDA, &cpu6502::IZX, 6 }, { "LDX", &cpu6502::LDX, &cpu6502::IMM, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "LDY", &cpu6502::LDY, &cpu6502::ZP0, 3 }, { "LDA", &cpu6502::LDA, &cpu6502::ZP0, 3 }, { "LDX", &cpu6502::LDX, &cpu6502::ZP0, 3 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 3 }, { "TAY", &cpu6502::TAY, &cpu6502::IMP, 2 }, { "LDA", &cpu6502::LDA, &cpu6502::IMM, 2 }, { "TAX", &cpu6502::TAX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "LDY", &cpu6502::LDY, &cpu6502::ABS, 4 }, { "LDA", &cpu6502::LDA, &cpu6502::ABS, 4 }, { "LDX", &cpu6502::LDX, &cpu6502::ABS, 4 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 4 }, \
    { "BCS", &cpu6502::BCS, &cpu6502::REL, 2 }, { "LDA", &cpu6502::LDA, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "LDY", &cpu6502::LDY, &cpu6502::ZPX, 4 }, { "LDA", &cpu6502::LDA, &cpu6502::ZPX, 4 }, { "LDX", &cpu6502::LDX, &cpu6502::ZPY, 4 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 4 }, { "CLV", &cpu6502::CLV, &cpu6502::IMP, 2 }, { "LDA", &cpu6502::LDA, &cpu6502::ABY, 4 }, { "TSX", &cpu6502::TSX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 4 }, { "LDY", &cpu6502::LDY, &cpu6502::ABX, 4 }, { "LDA", &cpu6502::LDA, &cpu6502::ABX, 4 }, { "LDX", &cpu6502::LDX, &cpu6502::ABY, 4 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 4 }, \
    { "CPY", &cpu6502::CPY, &cpu6502::IMM, 2 }, { "CMP", &cpu6502::CMP, &cpu6502::IZX, 6 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "CPY", &cpu6502::CPY, &cpu6502::ZP0, 3 }, { "CMP", &cpu6502::CMP, &cpu6502::ZP0, 3 }, { "DEC", &cpu6502::DEC, &cpu6502::ZP0, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "INY", &cpu6502::INY, &cpu6502::IMP, 2 }, { "CMP", &cpu6502::CMP, &cpu6502::IMM, 2 }, { "DEX", &cpu6502::DEX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "CPY", &cpu6502::CPY, &cpu6502::ABS, 4 }, { "CMP", &cpu6502::CMP, &cpu6502::ABS, 4 }, { "DEC", &cpu6502::DEC, &cpu6502::ABS, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, \
    { "BNE", &cpu6502::BNE, &cpu6502::REL, 2 }, { "CMP", &cpu6502::CMP, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "CMP", &cpu6502::CMP, &cpu6502::ZPX, 4 }, { "DEC", &cpu6502::DEC, &cpu6502::ZPX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "CLD", &cpu6502::CLD, &cpu6502::IMP, 2 }, { "CMP", &cpu6502::CMP, &cpu6502::ABY, 4 }, { "NOP", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "CMP", &cpu6502::CMP, &cpu6502::ABX, 4 }, { "DEC", &cpu6502::DEC, &cpu6502::ABX, 7 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, \
    { "CPX", &cpu6502::CPX, &cpu6502::IMM, 2 }, { "SBC", &cpu6502::SBC, &cpu6502::IZX, 6 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "CPX", &cpu6502::CPX, &cpu6502::ZP0, 3 }, { "SBC", &cpu6502::SBC, &cpu6502::ZP0, 3 }, { "INC", &cpu6502::INC, &cpu6502::ZP0, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 5 }, { "INX", &cpu6502::INX, &cpu6502::IMP, 2 }, { "SBC", &cpu6502::SBC, &cpu6502::IMM, 2 }, { "NOP", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::SBC, &cpu6502::IMP, 2 }, { "CPX", &cpu6502::CPX, &cpu6502::ABS, 4 }, { "SBC", &cpu6502::SBC, &cpu6502::ABS, 4 }, { "INC", &cpu6502::INC, &cpu6502::ABS, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, \
    { "BEQ", &cpu6502::BEQ, &cpu6502::REL, 2 }, { "SBC", &cpu6502::SBC, &cpu6502::IZY, 5 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 8 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "SBC", &cpu6502::SBC, &cpu6502::ZPX, 4 }, { "INC", &cpu6502::INC, &cpu6502::ZPX, 6 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 6 }, { "SED", &cpu6502::SED, &cpu6502::IMP, 2 }, { "SBC", &cpu6502::SBC, &cpu6502::ABY, 4 }, { "NOP", &cpu6502::NOP, &cpu6502::IMP, 2 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }, { "???", &cpu6502::NOP, &cpu6502::IMP, 4 }, { "SBC", &cpu6502::SBC, &cpu6502::ABX, 4 }, { "INC", &cpu6502::INC, &cpu6502::ABX, 7 }, { "???", &cpu6502::XXX, &cpu6502::IMP, 7 }  \
}