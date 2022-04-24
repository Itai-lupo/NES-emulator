#pragma once
#include "mapper.h"
#include "LaughTaleEngine.h"
class mapper_002 : public mapper
{
    private:
        uint8_t PRGBankSelectLo = 0x00;
        uint8_t PRGBankSelectHi = 0x00;

    public:
        mapper_002(uint8_t prgBanks, uint8_t chrBanks): mapper(prgBanks, chrBanks), PRGBankSelectHi(prgBanks - 1){}
        ~mapper_002(){}

        virtual bool readPRGMemory(uint32_t &addr, uint8_t &data) override
        {
            if(addr >= 0x8000 && addr <= 0xBFFF)
            {
                addr = PRGBankSelectLo * 0x4000 + (addr & 0x3FFF);
             
                return true;
            }
            else if(addr >= 0xC000 && addr <= 0xFFFF)
            {
                
                addr = (PRGBankSelectHi * 0x4000 + (addr & 0x3FFF));
                
                
                return true;
            }
            return false;            
        }

        virtual bool writePRGMemory(uint32_t &addr, uint8_t data) override
        {
            if(addr >= 0x8000 && addr <= 0xFFFF)
                PRGBankSelectLo = data & 0x0F; 
                
            return false;
        }


        virtual bool readCHRMemory(uint32_t &addr) override
        {

            if (addr < 0x2000)
            {
                // mapped_addr = addr;
                return true;
            }
            else
                return false;
            return addr < 0x2000;
        }

        virtual bool writeCHRMemory(uint32_t &addr) override
        {

            if (addr < 0x2000)
            {
                if (chrBanks == 0) // Treating as RAM
                {
                    // mapped_addr = addr;
                    return true;
                }
            }
            return false;
            if(addr < 0x2000 && chrBanks == 0)
            {
                return true;
            }
            return false;
        }
};