#pragma once
#include "mapper.h"
#include <array>
#include "LaughTaleEngine.h"

class mapper_001 : public mapper
{
    private:
        uint8_t chrBankSelect4Lo = 0x00;
        uint8_t chrBankSelect4Hi = 0x00;
        uint8_t chrBankSelect8 = 0x00;

        uint8_t prgBankSelect16Lo = 0x00;
        uint8_t prgBankSelect16Hi = 0x00;
        uint8_t prgBankSelect32 = 0x00;

        uint8_t loadRegister = 0x00;
        uint8_t loadRegisterCount = 0x00;
        uint8_t controlRegister = 0x1C;

        MIRROR mirrorMode = MIRROR::HORIZONTAL;

        std::array<uint8_t, 0x8000> ramStatic;
        
    public:
        mapper_001(uint8_t prgBanks, uint8_t chrBanks): mapper(prgBanks, chrBanks)
        {
            prgBankSelect16Hi = prgBanks - 1;
        }

        ~mapper_001(){}

        virtual bool readPRGMemory(uint32_t &addr, uint8_t& data) override
        {
            if(addr >= 0x6000 && addr <= 0x7FFF)
            {
                data = ramStatic[addr & 0x1FFF];
                return false;
            }
            else if(addr < 0x8000)
                return false;


            if (controlRegister & 0b01000)
            {
                if (addr >= 0x8000 && addr <= 0xBFFF)
                    addr = prgBankSelect16Lo * 0x4000 + (addr & 0x3FFF);

                else if (addr >= 0xC000 && addr <= 0xFFFF)
                    addr = prgBankSelect16Hi * 0x4000 + (addr & 0x3FFF);
                
            }
            else
                addr = prgBankSelect32 * 0x8000 + (addr & 0x7FFF);
            

            return true;      
        }

        virtual bool writePRGMemory(uint32_t &addr, uint8_t data) override
        {
            if(addr >= 0x6000 && addr <= 0x7FFF)
            {
                ramStatic[addr & 0x1FFF] = data;
                return false;
            }
            else if(addr < 0x8000)
                return false;
            
            if(data & 0x80)
            {
                loadRegister = 0;
                loadRegisterCount = 0;
                controlRegister = controlRegister | 0x0C;
                return false;
            }

            loadRegister >>= 1;
            loadRegister |= (data & 0x01) << 4;
            loadRegisterCount++;

            if(loadRegisterCount != 5)
                return false;
            
            
            uint8_t targetRegister = (addr >> 13) & 0x03;
            
            if(targetRegister == 0)
            {
                controlRegister = loadRegister & 0x1F;
                switch (controlRegister & 0x03)
                {
                    case 0: mirrorMode = MIRROR::ONESCREEN_LO;  break;
                    case 1: mirrorMode = MIRROR::ONESCREEN_HI;  break;
                    case 2: mirrorMode = MIRROR::VERTICAL;      break;
                    case 3: mirrorMode = MIRROR::HORIZONTAL;    break;
                }
            }
            else if(targetRegister == 1)
            {
                if (controlRegister & 0b10000)
                    chrBankSelect4Lo = loadRegister & 0x1F;
                else 
                    chrBankSelect8 = loadRegister & 0x1E;

            }
            else if(targetRegister == 2)
            {
                if (controlRegister & 0b10000)
                    chrBankSelect4Hi = loadRegister & 0x1F;
            }
            else if(targetRegister == 3)
            {
                uint8_t prgMode = (controlRegister >> 2) & 0x03;
                if(prgMode == 0 || prgMode == 1)
                    prgBankSelect32 = loadRegister & 0xf;
                if(prgMode == 2)
                {
                    prgBankSelect16Lo = 0;
                    prgBankSelect16Hi = loadRegister & 0x0f;
                }
                if(prgMode == 3)
                {
                    prgBankSelect16Lo = loadRegister & 0x0f;
                    prgBankSelect16Hi = prgBanks - 1;
                }
            }

            loadRegister = 0;
            loadRegisterCount = 0;
            return false;
        }


        virtual bool readCHRMemory(uint32_t &addr) override
        {
            if(addr >= 0x2000)
                return false;
            
            if(chrBanks == 0)
                return true;
            
            if (controlRegister & 0b10000)
            {
                if (addr >= 0x0000 && addr <= 0x0FFF)
                    addr = chrBankSelect4Lo * 0x1000 + (addr & 0x0FFF);

                else if (addr >= 0x1000 && addr <= 0x1FFF)
                    addr = chrBankSelect4Hi * 0x1000 + (addr & 0x0FFF);
                
            }
            else
                addr = chrBankSelect8 * 0x2000 + (addr & 0x1FFF);

            return true;
        }

        virtual bool writeCHRMemory(uint32_t &addr) override
        {
            if(addr < 0x2000)
            {
                addr = addr * (chrBanks == 0);
                return true;
            }

            return false;
        }

        virtual MIRROR getMirror() override
        {
            return mirrorMode;
        }
};