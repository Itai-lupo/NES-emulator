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
    return 0;
}
	
uint8_t cpu6502::IMM()
{
    addr = pc++;
    return 0;
}	

uint8_t cpu6502::ZP0()
{
    addr = systemBus->read(pc);
    pc++;
    
    return 0;
}
	
uint8_t cpu6502::ZPX()
{
    addr = systemBus->read(pc);
    pc++;

    addr += x;
    addr &= 0x00FF;
    
    return 0;
}	

uint8_t cpu6502::ZPY()
{
    addr = systemBus->read(pc);
    pc++;

    addr += y;
    addr &= 0x00FF;
    
    return 0;
}
	
uint8_t cpu6502::REL()
{
    int8_t temp = (int8_t)systemBus->read(pc);
    pc++;
    addr = pc + temp;
    
    return 2;
}

uint8_t cpu6502::ABS()
{
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc);
    pc++;

    addr = (hi << 8) | lo;
    
    return 0;
}
	
uint8_t cpu6502::ABX()
{
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc);
    pc++;

    addr = (hi << 8) | lo;
    addr += x;
    
    return ((addr & 0xFF00) != (hi << 8));
}	

uint8_t cpu6502::ABY()
{
    uint16_t lo = systemBus->read(pc);
    pc++;
    uint16_t hi = systemBus->read(pc);
    pc++;

    addr = (hi << 8) | lo;
    addr += y;
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
    
    return 0;
}	

uint8_t cpu6502::IZX()
{
    uint8_t temp = systemBus->read(pc);
    pc++;

    addr = systemBus->read((temp + x) & 0x00FF);
    addr |= systemBus->read((temp + x + 1) & 0x00FF) << 8;
    
    return 0;
}
	
uint8_t cpu6502::IZY()
{
    uint16_t temp = systemBus->read(pc);
    pc++;

    uint16_t lo = systemBus->read(temp & 0x00FF);
    uint16_t hi = systemBus->read((temp + 1) & 0x00FF) << 8;

    addr = hi | lo;
    addr += y;

    return  ((addr & 0xFF00) != (hi << 8));
}



// instructions -------------------------------------
uint8_t cpu6502::ADC()
{
    uint16_t temp = systemBus->read(addr);
    uint16_t res = (uint16_t)temp + (uint16_t)a + (uint16_t)status.C;
    
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    status.V = (  ~((uint16_t)a ^ temp) & ((uint16_t)a ^ res) & 0x0080) != 0;
    status.C = res > 255;

    a = res & 0x00FF;
    return 1;
}
	
uint8_t cpu6502::AND()
{
    uint8_t temp = systemBus->read(addr);
    a = temp & a;
    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080) != 0;

    return 0;
}
	
uint8_t cpu6502::ASL()
{
    uint16_t res = systemBus->read(addr);
    res = res << 1;
    
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    status.C = (res & 0xFF00) != 0;
    
    systemBus->write(addr, res & 0x00FF);
    return 0;
}

uint8_t cpu6502::ASL_A()
{
    uint16_t res = a << 1;

    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    status.C = (res & 0xFF00) != 0;

	a = res & 0x00FF;
    return 0;
}
	
uint8_t cpu6502::BCC()
{
    if(status.C != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}

uint8_t cpu6502::BCS()
{

    if(status.C != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BEQ()
{
    //LAUGHTALE_ENGINR_LOG_INFO("BEQ from " << hex(pc, 4) << " to " << hex(addr, 4) << " if " << (int)status.Z)

    if(status.Z != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BIT()
{
    uint8_t temp = systemBus->read(addr);
    status.Z = ((a & temp) == 0);
    status.N = ((temp & (1 << 7)) == 0) ? 0: 1;
    status.V = ((temp & (1 << 6)) == 0) ? 0: 1;

    // LAUGHTALE_ENGINR_LOG_INFO("BIT " << hex(addr, 4) << ", " << hex(temp, 2) << ", " << hex(a, 2))

    return 0;
}
	
uint8_t cpu6502::BMI()
{
    if(status.N != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}

uint8_t cpu6502::BNE()
{
    //LAUGHTALE_ENGINR_LOG_INFO("BEQ from " << hex(pc, 4) << " to " << hex(addr, 4) << " if " << 1 - (int)status.Z)

    if(status.Z != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BPL()
{
    //LAUGHTALE_ENGINR_LOG_INFO("BPL from " << hex(pc, 4) << " to " << hex(addr, 4) << " if " << (bool)status.N)
    if(status.N != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}
	
uint8_t cpu6502::BRK()
{
    pc++;
    status.I = 1;

    //LAUGHTALE_ENGINR_LOG_INFO("BRK(" << hex(pc - 2, 4) << ") push in to stack " << hex(pc, 4))

    systemBus->write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    systemBus->write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    status.B = 1;
    systemBus->write(0x0100 + stkp, statusData);
    stkp--;
    status.B = 0;

    pc = (uint16_t)systemBus->read(0xFFFE) | ((uint16_t)systemBus->read(0xFFFF) << 8);
    //LAUGHTALE_ENGINR_LOG_INFO("BRK read from 0xFFFE and FFFF " << hex(pc, 4))
    return 0;
}
	
uint8_t cpu6502::BVC()
{
    if(status.V != 0)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
}

uint8_t cpu6502::BVS()
{
    if(status.V != 1)
        return 0;
    uint8_t temp = pc;
    pc = addr;
    return 1 + ((pc & 0xFF00) != (temp & 0xFF00));
    return 0;
}
	
uint8_t cpu6502::CLC()
{
    status.C = 0;
    return 0;
}
	
uint8_t cpu6502::CLD()
{
    status.D = 0;
    return 0;
}
	
uint8_t cpu6502::CLI()
{
    status.I = 0;
    return 0;
}

uint8_t cpu6502::CLV()
{
    status.V = 0;
    return 0;
}
	
uint8_t cpu6502::CMP()
{
    uint16_t temp = systemBus->read(addr);
    //LAUGHTALE_ENGINR_LOG_INFO("CMP " << hex(addr, 4) << ", " << hex(temp, 2) << ", " << hex(a, 2))
    status.C = a >= temp;
    temp = (uint16_t)a - temp;
    status.Z = (temp & 0x00FF) == 0;
    status.N = (temp & 0x0080) != 0;

    return 1;
}
	
uint8_t cpu6502::CPX()
{
    uint8_t temp = systemBus->read(addr);
    status.C = x >= temp;
    temp = (uint16_t)x - temp;
    status.Z = (temp & 0x00FF) == 0;
    status.N = (temp & 0x0080) != 0;

    return 0;
}
	
uint8_t cpu6502::CPY()
{
    uint8_t temp = systemBus->read(addr);
    status.C = y >= temp;
    temp = (uint16_t)y - temp;
    status.Z = (temp & 0x00FF) == 0;
    status.N = (temp & 0x0080) != 0;

    return 0;
}

uint8_t cpu6502::DEC()
{
    uint8_t temp = systemBus->read(addr);
    temp--;
    systemBus->write(addr, temp);

    status.Z = temp == 0; 
    status.N = (temp & 0x80) != 0; 

    return 0;
}
	
uint8_t cpu6502::DEX()
{
    x--;
    
    status.Z = (x & 0x00FF) == 0; 
    status.N = (x & 0x0080) != 0; 

    return 0;
}
	
uint8_t cpu6502::DEY()
{
    y--;
    
    status.Z = (y & 0x00FF) == 0; 
    status.N = (y & 0x0080) != 0; 

    return 0;
}
	
uint8_t cpu6502::EOR()
{
    uint8_t temp = systemBus->read(addr);
    a = temp ^ a;
    
    status.Z = a == 0; 
    status.N = (a & 0x0080) != 0; 

    return 1;
}

uint8_t cpu6502::INC()
{
    uint16_t temp = systemBus->read(addr);
    temp++;
    systemBus->write(addr, temp & 0x00FF);

    status.Z = (temp & 0x00FF) == 0; 
    status.N = (temp & 0x0080) != 0; 

    return 0;
}
	
uint8_t cpu6502::INX()
{
    x++;
    
    status.Z = (x & 0x00FF) == 0; 
    status.N = (x & 0x0080) != 0; 

    return 0;
}
	
uint8_t cpu6502::INY()
{
    y++;
    
    status.Z = (y & 0x00FF) == 0; 
    status.N = (y & 0x0080) != 0; 

    return 0;
}
	
uint8_t cpu6502::JMP()
{
    // LAUGHTALE_ENGINR_LOG_INFO("jump from " << pc << ", " << addr);
    pc = addr;

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

    return 0;
}
	
uint8_t cpu6502::LDA()
{
    a = systemBus->read(addr);
    
    status.Z = (a & 0x00FF) == 0; 
    status.N = (a & 0x0080) != 0; 

    return 1;
}
	
uint8_t cpu6502::LDX()
{
    x = systemBus->read(addr);
    
    status.Z = x == 0; 
    status.N = (x & 0x80) != 0; 

    return 1;
}
	
uint8_t cpu6502::LDY()
{
    y = systemBus->read(addr);
    
    status.Z = y == 0; 
    status.N = (y & 0x80) != 0; 

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

    return 0;
}

uint8_t cpu6502::LSR_A()
{
    status.C = a & 0x01;
    a = a >> 1;
    status.Z = (a & 0x00FF) == 0;
    status.N = 0;

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
    return 0;
}
	
uint8_t cpu6502::ORA()
{
    uint8_t temp = systemBus->read(addr);
    a = temp | a;
    
    status.Z = a == 0; 
    status.N = (a & 0x80) != 0; 

    return 1;
}
	
uint8_t cpu6502::PHA()
{
    systemBus->write(0x0100 + stkp, a);
    stkp--;
    return 0;
}

uint8_t cpu6502::PHP()
{
    
	status.B = 1;
	status.U = 1;
    systemBus->write(0x0100 + stkp, statusData);
    stkp--;

	status.B = 0;
	status.U = 0;

    return 0;
}
	
uint8_t cpu6502::PLA()
{
    stkp++;
    a = systemBus->read(0x0100 + stkp);
    
    status.Z = (a == 0);
    status.N = (a & 0x80) != 0;
    
    return 0;
}
	
uint8_t cpu6502::PLP()
{
    stkp++;
    statusData = systemBus->read(0x0100 + stkp);
    status.U = 1;
    return 0;
}
	
uint8_t cpu6502::ROL()
{
    uint16_t res = systemBus->read(addr);
    res = (res << 1) | status.C;
    
    status.C = (res & 0xFF00) != 0;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    
    systemBus->write(addr, res & 0x00FF);

    return 0;
}

uint8_t cpu6502::ROL_A()
{
    uint16_t res = a;
    res = (res << 1) | status.C;
    
    status.C = (res & 0xFF00) != 0;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    
    a = res & 0x00FF;
	// LAUGHTALE_ENGINR_LOG_INFO((int)a)

    return 0;
}

uint8_t cpu6502::ROR()
{
    uint16_t temp = systemBus->read(addr);
    uint16_t res = ((uint16_t)status.C << 7) | (temp >> 1);
    
    status.C = temp & 0x0001;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    
    systemBus->write(addr, res & 0x00FF);

    return 0;
}

uint8_t cpu6502::ROR_A()
{
    uint16_t res = a;

    res >>= 1;
	res |= status.C << 7;

    status.C = a & 0x0001;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    
    a = res & 0x00FF;
	
    return 0;
}
	
uint8_t cpu6502::RTI()
{
    stkp++;
    statusData = systemBus->read(0x0100 + stkp);
	status.B = ~status.B;
	status.U = ~status.U;

    //LAUGHTALE_ENGINR_LOG_INFO("RTI before pop stack " << hex(pc, 4))
    stkp++;
    pc = (uint16_t)systemBus->read(0x0100 + stkp);
    stkp++;
    pc |= ((uint16_t)systemBus->read(0x0100 + stkp)) << 8;

    //LAUGHTALE_ENGINR_LOG_INFO("RTI after pop stack " << hex(pc, 4))
    return 0;
}
	
uint8_t cpu6502::RTS()
{
    //LAUGHTALE_ENGINR_LOG_INFO("RTS before pop stack " << hex(pc, 4))

    stkp++;
    pc = systemBus->read(0x0100 + stkp);
    
    stkp++;
    pc |= systemBus->read(0x0100 + stkp) << 8;
    pc++;
    //LAUGHTALE_ENGINR_LOG_INFO("RTS after pop stack " << hex(pc, 4))
    
    return 0;
}
	
uint8_t cpu6502::SBC()
{
    uint16_t value = ((uint16_t)systemBus->read(addr)) ^ 0xFF;

    uint16_t res = (uint16_t)a + value + (uint16_t)status.C;
    
    status.C = (res & 0xFF00) != 0;
    status.Z = (res & 0x00FF) == 0;
    status.N = (res & 0x0080) != 0;
    status.V = ((res ^ (uint16_t)a) & (res ^ value) & 0x0080) != 0;

    a = res & 0x00FF;
    return 1;
}

uint8_t cpu6502::SEC()
{
    status.C = 1;

    return 0;
}
	
uint8_t cpu6502::SED()
{
    status.D = 1;

    return 0;
}
	
uint8_t cpu6502::SEI()
{
    status.I = 1;

    return 0;
}
	
uint8_t cpu6502::STA()
{
    systemBus->write(addr, a);
    return 0;
}

uint8_t cpu6502::STX()
{
    systemBus->write(addr, x);
    return 0;
}
	
uint8_t cpu6502::STY()
{
    systemBus->write(addr, y);
    return 0;
}
	
uint8_t cpu6502::TAX()
{
    x = a;
    status.Z = (x & 0x00FF) == 0;
    status.N = (x & 0x0080) != 0;
    return 0;
}
	
uint8_t cpu6502::TAY()
{
    y = a;
    status.Z = y == 0;
    status.N = (y & 0x80) != 0;
    return 0;
}

uint8_t cpu6502::TSX()
{
    x = stkp;
    status.Z = x == 0;
    status.N = (x & 0x80) != 0;
    return 0;
}
	
uint8_t cpu6502::TXA()
{
    a = x;

    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080) != 0;

    return 0;
}
	
uint8_t cpu6502::TXS()
{
    stkp = x;
    return 0;
}
	
uint8_t cpu6502::TYA()
{
    a = y;
    status.Z = (a & 0x00FF) == 0;
    status.N = (a & 0x0080) != 0;
    return 0;
}


uint8_t cpu6502::XXX()
{
    return 0;
}
