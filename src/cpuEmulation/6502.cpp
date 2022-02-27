#include "LaughTaleEngine.h"
#include "6502.h"


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
    int8_t temp = (int8_t)systemBus->read(pc);
    pc++;
    addr = pc + temp;
    LAUGHTALE_ENGINR_LOG_INFO("REL(" << hex(addr, 4) << ")" << hex(temp, 2) << ", " << (int) temp);
    return 2;
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
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc) << 8;
    pc++;

    addr = systemBus->read(hi | lo);
    addr |= systemBus->read(hi | ((lo + 1) & 0x00FF)) << 8;
    
    LAUGHTALE_ENGINR_LOG_INFO("IND(" << hex(addr, 4) << ")")
    return 0;
}	

uint8_t cpu6502::IZX()
{
    uint8_t temp = systemBus->read(pc);
    pc++;

    addr = systemBus->read((temp + x) & 0x00FF);
    addr |= systemBus->read((temp + x + 1) & 0x00FF) << 8;
    
    LAUGHTALE_ENGINR_LOG_INFO("IZX(" << hex(addr, 4) << "):" << (int)x)
    return 0;
}
	
uint8_t cpu6502::IZY()
{
    
    uint8_t temp = systemBus->read(pc);
    pc++;

    uint8_t lo = systemBus->read(temp);
    uint8_t hi = systemBus->read((temp + 1) & 0x00FF) << 8;

    addr = hi | lo;
    addr += y;

    LAUGHTALE_ENGINR_LOG_INFO("IZY(" << hex(addr, 4) << "): " << (int)y)
    return  ((addr & 0xFF00) != (hi << 8));
}



// instructions -------------------------------------
uint8_t cpu6502::ADC()
{
    uint16_t temp = systemBus->read(addr);
    uint16_t res = (uint16_t)temp + (uint16_t)a + (uint16_t)status.C;
    
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    status.V = (  ~((uint16_t)a ^ temp) & ((uint16_t)a ^ res) & 0x0080);
    status.C = res > 255;

    a = res & 0x00FF;
    LAUGHTALE_ENGINR_LOG_INFO("ADC(" << hex(pc, 4) << ") = " << (int)a);
    return 1;
}
	
uint8_t cpu6502::AND()
{
    uint8_t temp = systemBus->read(addr);
    a = temp & a;
    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080);

    LAUGHTALE_ENGINR_LOG_INFO("AND(" << hex(pc, 4) << ") = " << (int)a);
    return 0;
}
	
uint8_t cpu6502::ASL()
{
    uint16_t res = systemBus->read(addr);
    res = res << 1;
    
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    status.C = res > 255;
    
    systemBus->write(addr, res & 0x00FF);

    LAUGHTALE_ENGINR_LOG_INFO("ASL(" << hex(pc, 4) << ") = " << (int)res);
    return 0;
}

uint8_t cpu6502::ASL_A()
{
    status.C = (uint16_t)a << 1 & 0xFF00;
    a = a << 1;
    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080);

    LAUGHTALE_ENGINR_LOG_INFO("ASL(" << hex(pc, 4) << ") = " << (int)a);
    return 0;
}
	
uint8_t cpu6502::BCC()
{
    if(status.C != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BCC(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}

uint8_t cpu6502::BCS()
{

    if(status.C != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BCS(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BEQ()
{
    if(status.Z != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BEQ(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BIT()
{
    uint16_t temp = systemBus->read(addr);

    status.Z = ((temp & a & 0x00FF) == 0);
    status.N = (temp & (1 << 7));
    status.V = (temp & (1 << 6));


    LAUGHTALE_ENGINR_LOG_INFO("BIT(" << hex(pc, 4) << ")");
}
	
uint8_t cpu6502::BMI()
{
    if(status.N != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BMI(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}

uint8_t cpu6502::BNE()
{
    if(status.Z != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BNE(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BPL()
{
    if(status.N != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BPL(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BRK()
{
    pc++;
    status.I = 1;

    systemBus->write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    systemBus->write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    status.B = 1;
    systemBus->write(0x0100 + stkp, statusData);
    stkp--;
    status.B = 0;

    pc = (uint16_t)systemBus->read(0xFFFE) | ((uint16_t)systemBus->read(0xFFFF) << 8);
    return 0;
}
	
uint8_t cpu6502::BVC()
{
    if(status.V != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BVC(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}

uint8_t cpu6502::BVS()
{
    if(status.V != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    LAUGHTALE_ENGINR_LOG_INFO("BVS(" << hex(temp, 4) << ") " << hex(pc, 4));
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
    return 0;
}
	
uint8_t cpu6502::CLC()
{
    status.C = 0;
    LAUGHTALE_ENGINR_LOG_INFO("CLC")
    return 0;
}
	
uint8_t cpu6502::CLD()
{
    status.D = 0;
    LAUGHTALE_ENGINR_LOG_INFO("CLD")
    return 0;
}
	
uint8_t cpu6502::CLI()
{
    status.I = 0;
    LAUGHTALE_ENGINR_LOG_INFO("CLI")
    return 0;
}

uint8_t cpu6502::CLV()
{
    status.V = 0;
    LAUGHTALE_ENGINR_LOG_INFO("CLV")
    return 0;
}
	
uint8_t cpu6502::CMP()
{
    uint8_t temp = systemBus->read(addr);
    status.C = a >= temp;
    temp = a - temp;
    status.Z = (temp & 0x00FF) == 0;
    status.N = (temp & 0x0080);

    LAUGHTALE_ENGINR_LOG_INFO("CMP")
    return 1;
}
	
uint8_t cpu6502::CPX()
{
    uint8_t temp = systemBus->read(addr);
    status.C = x >= temp;
    temp = x - temp;
    status.Z = (temp & 0x00FF) == 0;
    status.N = (temp & 0x0080);

    LAUGHTALE_ENGINR_LOG_INFO("CPX")
    return 0;
}
	
uint8_t cpu6502::CPY()
{
    uint8_t temp = systemBus->read(addr);
    status.C = y >= temp;
    temp = y - temp;
    status.Z = (temp & 0x00FF) == 0;
    status.N = (temp & 0x0080);

    LAUGHTALE_ENGINR_LOG_INFO("CPY")
    return 0;
}

uint8_t cpu6502::DEC()
{
    uint8_t temp = systemBus->read(addr);
    temp--;
    systemBus->write(addr, temp);

    status.Z = (temp & 0x00FF) == 0; 
    status.N = (temp & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("DEC(" << hex(addr, 4) << ") = " << hex(systemBus->read(addr), 2));
    return 0;
}
	
uint8_t cpu6502::DEX()
{
    x--;
    
    status.Z = (x & 0x00FF) == 0; 
    status.N = (x & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("DEX = " << (int)x);
    return 0;
}
	
uint8_t cpu6502::DEY()
{
    y--;
    
    status.Z = (y & 0x00FF) == 0; 
    status.N = (y & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("DEY = " << (int)x);
    return 0;
}
	
uint8_t cpu6502::EOR()
{
    uint8_t temp = systemBus->read(addr);
    a = temp ^ a;
    
    status.Z = (a & 0x00FF) == 0; 
    status.N = (a & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("EOR")
    return 1;
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
    
    status.Z = (x & 0x00FF) == 0; 
    status.N = (x & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("INX = " << (int)x);
    return 0;
}
	
uint8_t cpu6502::INY()
{
    y++;
    
    status.Z = (y & 0x00FF) == 0; 
    status.N = (y & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("INY = " << (int)y);
    return 0;
}
	
uint8_t cpu6502::JMP()
{
    pc = addr;

    LAUGHTALE_ENGINR_LOG_INFO("JMP")
    return 0;
}

uint8_t cpu6502::JSR()
{
    pc--;
    systemBus->write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    systemBus->write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    pc = addr;

    LAUGHTALE_ENGINR_LOG_INFO("JSR")
    return 0;
}
	
uint8_t cpu6502::LDA()
{
    a = systemBus->read(addr);
    
    status.Z = (a & 0x00FF) == 0; 
    status.N = (a & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("LDA(" << hex(addr, 4) << "} " << (int)a);
    return 1;
}
	
uint8_t cpu6502::LDX()
{
    x = systemBus->read(addr);
    
    status.Z = (x & 0x00FF) == 0; 
    status.N = (x & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("LDX(" << hex(addr, 4) << "} " << (int)x);
    return 1;
}
	
uint8_t cpu6502::LDY()
{
    y = systemBus->read(addr);
    
    status.Z = (y & 0x00FF) == 0; 
    status.N = (y & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("LDY(" << hex(addr, 4) << "} " << (int)y);
    return 1;
}
uint8_t cpu6502::LSR()
{
    uint8_t res = systemBus->read(addr);
    status.C = res & 0x01;
    res = res >> 1;
    
    status.Z = (res & 0x00FF) == 0;
    status.N = 0;
    
    systemBus->write(addr, res & 0x00FF);

    LAUGHTALE_ENGINR_LOG_INFO("LSR(" << hex(pc, 4) << ") = " << (int)res);
    return 0;
}

uint8_t cpu6502::LSR_A()
{
    status.C = a & 0x01;
    a = a >> 1;
    status.Z = (a & 0x00FF) == 0;
    status.N = 0;

    LAUGHTALE_ENGINR_LOG_INFO("LSR(" << hex(pc, 4) << ") = " << (int)a);
    return 0;
}
	
uint8_t cpu6502::NOP()
{
    switch (systemBus->read(pc - 1)) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
	}
    LAUGHTALE_ENGINR_LOG_INFO("NOP")
	return 0;
}
	
uint8_t cpu6502::ORA()
{
    uint8_t temp = systemBus->read(addr);
    a = temp | a;
    
    status.Z = (a & 0x00FF) == 0; 
    status.N = (a & 0x0080) == 0; 

    LAUGHTALE_ENGINR_LOG_INFO("ORA")
    return 1;
}
	
uint8_t cpu6502::PHA()
{
    systemBus->write(0x0100 + stkp, a);
    stkp--;
    LAUGHTALE_ENGINR_LOG_INFO("PHA")
    return 0;
}

uint8_t cpu6502::PHP()
{
    
    systemBus->write(0x0100 + stkp, statusData);
    stkp--;
    LAUGHTALE_ENGINR_LOG_INFO("PHA")
    return 0;
}
	
uint8_t cpu6502::PLA()
{
    a = systemBus->read(0x0100 + stkp);
    stkp++;
    LAUGHTALE_ENGINR_LOG_INFO("PLA")
    return 0;
}
	
uint8_t cpu6502::PLP()
{
    statusData = systemBus->read(0x0100 + stkp);
    stkp++;
    LAUGHTALE_ENGINR_LOG_INFO("PLP")
    return 0;
}
	
uint8_t cpu6502::ROL()
{
    uint16_t res = systemBus->read(addr);
    res = res << 1 | status.C;
    
    status.C = res & 0xFF00;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    
    systemBus->write(addr, res & 0x00FF);

    LAUGHTALE_ENGINR_LOG_INFO("ROL(" << hex(pc, 4) << ") = " << (int)res);
    return 0;
}

uint8_t cpu6502::ROL_A()
{
    uint16_t res = a;
    res = res << 1 | status.C;
    
    status.C = res & 0xFF00;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    
    a = res & 0x00FF;

    LAUGHTALE_ENGINR_LOG_INFO("ROL(" << hex(pc, 4) << ") = " << (int)res);
    return 0;
}

uint8_t cpu6502::ROR()
{
    uint16_t res = systemBus->read(addr);
    res = (status.C << 7) | (res >> 1);
    
    status.C = res & 0x0001;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    
    systemBus->write(addr, res & 0x00FF);

    LAUGHTALE_ENGINR_LOG_INFO("ROR(" << hex(pc, 4) << ") = " << (int)res);
    return 0;
}

uint8_t cpu6502::ROR_A()
{
    uint16_t res = a;
    res = (status.C << 7) | (res >> 1);
    
    status.C = res & 0x0001;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    
    a = res & 0x00FF;

    LAUGHTALE_ENGINR_LOG_INFO("ROR(" << hex(pc, 4) << ") = " << (int)res);
    return 0;
}
	
uint8_t cpu6502::RTI()
{
    statusData = systemBus->read(0x0100 + stkp);
    stkp++;

    pc = systemBus->read(0x0100 + stkp);
    stkp++;
    
    pc |= systemBus->read(0x0100 + stkp) << 8;
    stkp++;
    LAUGHTALE_ENGINR_LOG_INFO("RTI")
    return 0;
}
	
uint8_t cpu6502::RTS()
{
    pc = systemBus->read(0x0100 + stkp);
    stkp++;
    
    pc |= systemBus->read(0x0100 + stkp) << 8;
    stkp++;
    LAUGHTALE_ENGINR_LOG_INFO("RTS")
    return 0;
}
	
uint8_t cpu6502::SBC()
{
    uint16_t temp = systemBus->read(addr) ^ 0x00FF;
    uint16_t res = (uint16_t)a + (uint16_t)temp + (uint16_t)status.C;
    
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080);
    status.V = (  ~((uint16_t)a ^ temp) & ((uint16_t)a ^ res) & 0x0080);
    status.C = res > 255;

    a = res & 0x00FF;
    LAUGHTALE_ENGINR_LOG_INFO("SBC(" << hex(pc, 4) << ") = " << (int)a);
    return 1;
}

uint8_t cpu6502::SEC()
{
    status.C = 1;

    LAUGHTALE_ENGINR_LOG_INFO("SEC")
    return 0;
}
	
uint8_t cpu6502::SED()
{
    status.D = 1;

    LAUGHTALE_ENGINR_LOG_INFO("SED")
    return 0;
}
	
uint8_t cpu6502::SEI()
{
    status.I = 1;

    LAUGHTALE_ENGINR_LOG_INFO("SEI")
    return 0;
}
	
uint8_t cpu6502::STA()
{
    systemBus->write(addr, a);
    LAUGHTALE_ENGINR_LOG_INFO("STA(" << hex(addr, 4) << ") = " << hex(systemBus->read(addr), 2))
    return 0;
}

uint8_t cpu6502::STX()
{
    systemBus->write(addr, x);
    LAUGHTALE_ENGINR_LOG_INFO("STX(" << hex(addr, 4) << ") = " << hex(systemBus->read(addr), 2))
    return 0;
}
	
uint8_t cpu6502::STY()
{
    systemBus->write(addr, y);
    LAUGHTALE_ENGINR_LOG_INFO("STY(" << hex(addr, 4) << ") = " << hex(systemBus->read(addr), 2))
    return 0;
}
	
uint8_t cpu6502::TAX()
{
    x = a;
    status.Z = (x & 0x00FF) == 0;
    status.N = (x & 0x0080);
    LAUGHTALE_ENGINR_LOG_INFO("TAX")
    return 0;
}
	
uint8_t cpu6502::TAY()
{
    y = a;
    status.Z = (y & 0x00FF) == 0;
    status.N = (y & 0x0080);
    LAUGHTALE_ENGINR_LOG_INFO("TAY")
    return 0;
}

uint8_t cpu6502::TSX()
{
    x = stkp;
    status.Z = (x & 0x00FF) == 0;
    status.N = (x & 0x0080);
    LAUGHTALE_ENGINR_LOG_INFO("TSX")
    return 0;
}
	
uint8_t cpu6502::TXA()
{
    a = x;

    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080);

    LAUGHTALE_ENGINR_LOG_INFO("TXA")
    return 0;
}
	
uint8_t cpu6502::TXS()
{
    stkp = x;
    status.Z = (stkp & 0x00FF) == 0;
    status.N = (stkp & 0x0080);
    LAUGHTALE_ENGINR_LOG_INFO("TXS")
    return 0;
}
	
uint8_t cpu6502::TYA()
{
    a = y;
    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080);
    LAUGHTALE_ENGINR_LOG_INFO("TYA")
    return 0;
}


uint8_t cpu6502::XXX()
{
    LAUGHTALE_ENGINR_LOG_INFO("XXX")
    return 0;
}
