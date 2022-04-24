#pragma once
#include <cstdint>

enum MIRROR
{
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
    HARDWARE
};

class mapper
{
    protected:
        uint8_t prgBanks, chrBanks;
        
    public:
        mapper(uint8_t prgBanks, uint8_t chrBanks): prgBanks(prgBanks), chrBanks(chrBanks){}
        
        virtual ~mapper() = default;

        virtual bool readPRGMemory(uint32_t& addr,  uint8_t& data) = 0;
        virtual bool writePRGMemory(uint32_t& addr, uint8_t data) = 0;

        virtual bool readCHRMemory(uint32_t& addr) = 0;
        virtual bool writeCHRMemory(uint32_t& addr) = 0;

        virtual MIRROR getMirror(){ return MIRROR::HARDWARE; }
}; 
