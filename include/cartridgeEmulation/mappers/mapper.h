#pragma once
#include <cstdint>

class mapper
{
    protected:
        uint8_t prgBanks, chrBanks;
    public:
        mapper(uint8_t prgBanks, uint8_t chrBanks): prgBanks(prgBanks), chrBanks(chrBanks){}
        
        virtual ~mapper() = default;

        virtual bool readPRGMemory(uint16_t& addr) = 0;
        virtual bool writePRGMemory(uint16_t& addr) = 0;

        virtual bool readCHRMemory(uint16_t& addr) = 0;
        virtual bool writeCHRMemory(uint16_t& addr) = 0;
}; 
