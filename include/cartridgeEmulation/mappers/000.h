#pragma once
#include "mapper.h"

class mapper_000 : public mapper
{
    public:
        mapper_000(uint8_t prgBanks, uint8_t chrBanks): mapper(prgBanks, chrBanks){}
        ~mapper_000(){}

        virtual bool readPRGMemory(uint32_t &addr, uint8_t& data) override
        {
            if(!(addr >= 0x8000 && addr <= 0xFFFF))
                return false;

            addr = addr & (prgBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;            
        }

        virtual bool writePRGMemory(uint32_t &addr, uint8_t data) override
        {
            if(!(addr >= 0x8000 && addr <= 0xFFFF))
                return false;    

            addr = addr & (prgBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }


        virtual bool readCHRMemory(uint32_t &addr) override
        {
            return true;
        }

        virtual bool writeCHRMemory(uint32_t &addr) override
        {
            return true;
        }
};