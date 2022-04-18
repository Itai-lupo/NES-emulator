#pragma once
#include "LaughTaleEngine.h"
#include "instrction.h"
#include "instructionsTable.h"
#include "bus.h"

#include <functional>

class cpu6502: public LTE::component
{
    public:
        struct instruction6502
        {
            std::string name;
            uint8_t (cpu6502::*operate)(void) = nullptr;
            uint8_t (cpu6502::*addrmode)(void) = nullptr;
            uint8_t cycles = 0;
            uint8_t size;

        };

        unsigned long clock = 0;
        uint16_t addr = 0x0000;
        uint8_t DATA;
        uint8_t a = 0X00, x = 0X00, y = 0X00;
        uint8_t stkp = 0X00;
        uint16_t pc = 0X0000;
        uint8_t cycles = 0X00;
        uint8_t opcode = 0;

        union 
        {
            struct
            {
                uint8_t C: 1;	// Carry Bit
                uint8_t Z: 1;	// Zero
                uint8_t I: 1;	// Disable Interrupts
                uint8_t D: 1;	// Decimal Mode (unused in this implementation)
                uint8_t B: 1;	// Break
                uint8_t U: 1;	// Unused
                uint8_t V: 1;	// Overflow
                uint8_t N: 1;	// Negative
            } status;

            uint8_t statusData;
        };
        
        bool RW;

        std::vector<instruction6502> lookup;
        



    private:
        uint8_t IMP();	uint8_t IMM();	
        uint8_t ZP0();	uint8_t ZPX();	
        uint8_t ZPY();	uint8_t REL();
        uint8_t ABS();	uint8_t ABX();	
        uint8_t ABY();	uint8_t IND();	
        uint8_t IZX();	uint8_t IZY();

    private: 
        uint8_t ADC();	uint8_t AND();	 uint8_t ASL(); uint8_t ASL_A();    uint8_t BCC();
        uint8_t BCS();	uint8_t BEQ();	 uint8_t BIT();	uint8_t BMI();
        uint8_t BNE();	uint8_t BPL();	 uint8_t BRK();	uint8_t BVC();
        uint8_t BVS();	uint8_t CLC();	 uint8_t CLD();	uint8_t CLI();
        uint8_t CLV();	uint8_t CMP();	 uint8_t CPX();	uint8_t CPY();
        uint8_t DEC();	uint8_t DEX();	 uint8_t DEY();	uint8_t EOR();
        uint8_t INC();	uint8_t INX();	 uint8_t INY();	uint8_t JMP();
        uint8_t JSR();	uint8_t LDA();	 uint8_t LDX();	uint8_t LDY();
        uint8_t LSR();  uint8_t LSR_A(); uint8_t NOP();	uint8_t ORA();      uint8_t PHA();
        uint8_t PHP();	uint8_t PLA();	 uint8_t PLP();	uint8_t ROL();      uint8_t ROL_A();
        uint8_t ROR();  uint8_t ROR_A(); uint8_t RTI();	uint8_t RTS();	    uint8_t SBC();
        uint8_t SEC();	uint8_t SED();	 uint8_t SEI();	uint8_t STA();
        uint8_t STX();	uint8_t STY();	 uint8_t TAX();	uint8_t TAY();
        uint8_t TSX();	uint8_t TXA();	 uint8_t TXS();	uint8_t TYA();

        uint8_t XXX();

        bus<uint8_t, uint16_t> *systemBus;

    public:
        cpu6502()
        {
            lookup = CPU_6502_INSTRUCTION_SET_TABLE;
        }
        
        virtual ~cpu6502() override
        {

        }
        
        virtual void init(LTE::gameObject *parent) override
        {
            systemBus = parent->getComponent<bus<uint8_t, uint16_t>>();
        }

        virtual void end() override
        {

        }

};
