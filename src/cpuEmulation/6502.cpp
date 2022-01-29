#include "6502.h"
#include "LaughTaleEngine.h"


std::string hex(uint32_t n, uint8_t d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
}

// memory mods -------------------------------------
uint8_t cpu6502::IMP()
{
    addr = 0x0000;
    LAUGHTALE_ENGINR_LOG_INFO("IMP")
    return 0;
}
	
uint8_t cpu6502::IMM()
{
    addr = pc++;
    LAUGHTALE_ENGINR_LOG_INFO("IMM")
    return 0;
}	

uint8_t cpu6502::ZP0()
{
    addr = systemBus->read(pc);
    pc++;
    
    LAUGHTALE_ENGINR_LOG_INFO("ZP0(" << hex(addr, 4) << ")")
    return 0;
}
	
uint8_t cpu6502::ZPX()
{
    
    addr = systemBus->read(pc);
    pc++;

    addr += x;
    addr &= 0x00FF;
    
    LAUGHTALE_ENGINR_LOG_INFO("ZPX(" << hex(addr, 4) << ")")
    return 0;
}	

uint8_t cpu6502::ZPY()
{
    addr = systemBus->read(pc);
    pc++;

    addr += y;
    addr &= 0x00FF;
    
    LAUGHTALE_ENGINR_LOG_INFO("ZPY(" << hex(addr, 4) << ")")
    return 0;
}
	
uint8_t cpu6502::REL()
{
    LAUGHTALE_ENGINR_LOG_INFO("REL")
    return 0;
}

uint8_t cpu6502::ABS()
{
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc);
    pc++;

    addr = (hi << 8) | lo;
    
    LAUGHTALE_ENGINR_LOG_INFO("ABS(" << hex(addr, 4) << ")")
    return 0;
}
	
uint8_t cpu6502::ABX()
{
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc);
    pc++;

    addr = ((hi << 8) | lo) + x;
    
    LAUGHTALE_ENGINR_LOG_INFO("ABX(" << hex(addr, 4) << ")")
    return ((addr & 0xFF00) != (hi << 8));
}	

uint8_t cpu6502::ABY()
{
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc);
    pc++;

    addr = ((hi << 8) | lo) + y;
    
    LAUGHTALE_ENGINR_LOG_INFO("ABY(" << hex(addr, 4) << ")")
    return ((addr & 0xFF00) != (hi << 8));
}
	
uint8_t cpu6502::IND()
{
    LAUGHTALE_ENGINR_LOG_INFO("IND")
    return 0;
}	

uint8_t cpu6502::IZX()
{
    LAUGHTALE_ENGINR_LOG_INFO("IZX")
    return 0;
}
	
uint8_t cpu6502::IZY()
{
    LAUGHTALE_ENGINR_LOG_INFO("IZY")
    return 0;
}



// instructions -------------------------------------
uint8_t cpu6502::ADC()
{
    LAUGHTALE_ENGINR_LOG_INFO("ADC")
    return 0;
}
	
uint8_t cpu6502::AND()
{
    LAUGHTALE_ENGINR_LOG_INFO("AND")
    return 0;
}
	
uint8_t cpu6502::ASL()
{
    LAUGHTALE_ENGINR_LOG_INFO("ASL")
    return 0;
}
	
uint8_t cpu6502::BCC()
{
    LAUGHTALE_ENGINR_LOG_INFO("BCC")
    return 0;
}

uint8_t cpu6502::BCS()
{
    LAUGHTALE_ENGINR_LOG_INFO("BCS")
    return 0;
}
	
uint8_t cpu6502::BEQ()
{
    LAUGHTALE_ENGINR_LOG_INFO("BEQ")
    return 0;
}
	
uint8_t cpu6502::BIT()
{
    LAUGHTALE_ENGINR_LOG_INFO("BIT")
    return 0;
}
	
uint8_t cpu6502::BMI()
{
    LAUGHTALE_ENGINR_LOG_INFO("BMI")
    return 0;
}

uint8_t cpu6502::BNE()
{
    LAUGHTALE_ENGINR_LOG_INFO("BNE")
    return 0;
}
	
uint8_t cpu6502::BPL()
{
    LAUGHTALE_ENGINR_LOG_INFO("BPL")
    return 0;
}
	
uint8_t cpu6502::BRK()
{
    LAUGHTALE_ENGINR_LOG_INFO("BRK")
    return 0;
}
	
uint8_t cpu6502::BVC()
{
    LAUGHTALE_ENGINR_LOG_INFO("BVC")
    return 0;
}

uint8_t cpu6502::BVS()
{
    LAUGHTALE_ENGINR_LOG_INFO("BVS")
    return 0;
}
	
uint8_t cpu6502::CLC()
{
    LAUGHTALE_ENGINR_LOG_INFO("CLC")
    return 0;
}
	
uint8_t cpu6502::CLD()
{
    LAUGHTALE_ENGINR_LOG_INFO("CLD")
    return 0;
}
	
uint8_t cpu6502::CLI()
{
    LAUGHTALE_ENGINR_LOG_INFO("CLI")
    return 0;
}

uint8_t cpu6502::CLV()
{
    LAUGHTALE_ENGINR_LOG_INFO("CLV")
    return 0;
}
	
uint8_t cpu6502::CMP()
{
    LAUGHTALE_ENGINR_LOG_INFO("CMP")
    return 0;
}
	
uint8_t cpu6502::CPX()
{
    LAUGHTALE_ENGINR_LOG_INFO("CPX")
    return 0;
}
	
uint8_t cpu6502::CPY()
{
    LAUGHTALE_ENGINR_LOG_INFO("CPY")
    return 0;
}

uint8_t cpu6502::DEC()
{
    LAUGHTALE_ENGINR_LOG_INFO("DEC")
    return 0;
}
	
uint8_t cpu6502::DEX()
{
    LAUGHTALE_ENGINR_LOG_INFO("DEX")
    return 0;
}
	
uint8_t cpu6502::DEY()
{
    LAUGHTALE_ENGINR_LOG_INFO("DEY")
    return 0;
}
	
uint8_t cpu6502::EOR()
{
    LAUGHTALE_ENGINR_LOG_INFO("EOR")
    return 0;
}

uint8_t cpu6502::INC()
{
    uint8_t temp = systemBus->read(addr);
    temp++;
    systemBus->write(addr, temp);

    status.Z = (temp & 0x00FF) == 0; 
    status.N = (temp & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("INC(" << hex(addr, 4) << ") = " << hex(systemBus->read(addr), 2));
    return 0;
}
	
uint8_t cpu6502::INX()
{
    x++;
    LAUGHTALE_ENGINR_LOG_INFO("INX = " << (int)x);
    return 0;
}
	
uint8_t cpu6502::INY()
{
    LAUGHTALE_ENGINR_LOG_INFO("INY")
    return 0;
}
	
uint8_t cpu6502::JMP()
{
    LAUGHTALE_ENGINR_LOG_INFO("JMP")
    return 0;
}

uint8_t cpu6502::JSR()
{
    LAUGHTALE_ENGINR_LOG_INFO("JSR")
    return 0;
}
	
uint8_t cpu6502::LDA()
{
    LAUGHTALE_ENGINR_LOG_INFO("LDA")
    return 0;
}
	
uint8_t cpu6502::LDX()
{
    LAUGHTALE_ENGINR_LOG_INFO("LDX")
    return 0;
}
	
uint8_t cpu6502::LDY()
{
    LAUGHTALE_ENGINR_LOG_INFO("LDY")
    return 0;
}

uint8_t cpu6502::LSR()
{
    LAUGHTALE_ENGINR_LOG_INFO("LSR")
    return 0;
}
	
uint8_t cpu6502::NOP()
{
    LAUGHTALE_ENGINR_LOG_INFO("NOP")
    return 0;
}
	
uint8_t cpu6502::ORA()
{
    LAUGHTALE_ENGINR_LOG_INFO("ORA")
    return 0;
}
	
uint8_t cpu6502::PHA()
{
    LAUGHTALE_ENGINR_LOG_INFO("PHA")
    return 0;
}

uint8_t cpu6502::PHP()
{
    LAUGHTALE_ENGINR_LOG_INFO("PHP")
    return 0;
}
	
uint8_t cpu6502::PLA()
{
    LAUGHTALE_ENGINR_LOG_INFO("PLA")
    return 0;
}
	
uint8_t cpu6502::PLP()
{
    LAUGHTALE_ENGINR_LOG_INFO("PLP")
    return 0;
}
	
uint8_t cpu6502::ROL()
{
    LAUGHTALE_ENGINR_LOG_INFO("ROL")
    return 0;
}

uint8_t cpu6502::ROR()
{
    LAUGHTALE_ENGINR_LOG_INFO("ROR")
    return 0;
}
	
uint8_t cpu6502::RTI()
{
    LAUGHTALE_ENGINR_LOG_INFO("RTI")
    return 0;
}
	
uint8_t cpu6502::RTS()
{
    LAUGHTALE_ENGINR_LOG_INFO("RTS")
    return 0;
}
	
uint8_t cpu6502::SBC()
{
    LAUGHTALE_ENGINR_LOG_INFO("SBC")
    return 0;
}

uint8_t cpu6502::SEC()
{
    LAUGHTALE_ENGINR_LOG_INFO("SEC")
    return 0;
}
	
uint8_t cpu6502::SED()
{
    LAUGHTALE_ENGINR_LOG_INFO("SED")
    return 0;
}
	
uint8_t cpu6502::SEI()
{
    LAUGHTALE_ENGINR_LOG_INFO("SEI")
    return 0;
}
	
uint8_t cpu6502::STA()
{
    LAUGHTALE_ENGINR_LOG_INFO("STA")
    return 0;
}

uint8_t cpu6502::STX()
{
    LAUGHTALE_ENGINR_LOG_INFO("STX")
    return 0;
}
	
uint8_t cpu6502::STY()
{
    LAUGHTALE_ENGINR_LOG_INFO("STY")
    return 0;
}
	
uint8_t cpu6502::TAX()
{
    LAUGHTALE_ENGINR_LOG_INFO("TAX")
    return 0;
}
	
uint8_t cpu6502::TAY()
{
    LAUGHTALE_ENGINR_LOG_INFO("TAY")
    return 0;
}

uint8_t cpu6502::TSX()
{
    LAUGHTALE_ENGINR_LOG_INFO("TSX")
    return 0;
}
	
uint8_t cpu6502::TXA()
{
    LAUGHTALE_ENGINR_LOG_INFO("TXA")
    return 0;
}
	
uint8_t cpu6502::TXS()
{
    LAUGHTALE_ENGINR_LOG_INFO("TXS")
    return 0;
}
	
uint8_t cpu6502::TYA()
{
    LAUGHTALE_ENGINR_LOG_INFO("TYA")
    return 0;
}


uint8_t cpu6502::XXX()
{
    LAUGHTALE_ENGINR_LOG_INFO("XXX")
    return 0;
}
