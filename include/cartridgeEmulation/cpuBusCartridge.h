#pragma once
#include "cartridge.h"

class cpuBusCartridge: public busDevice<uint8_t, uint16_t>
{
    private:
        cartridge *cart;
        std::string datatoHexString(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }


    public:
        cpuBusCartridge(cartridge *cart): cart(cart){}

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            return cart->readPRGMemory(addr);
        }
        
        virtual void write(uint16_t addr, uint8_t data) override
        {
            LAUGHTALE_ENGINR_LOG_INFO("cpuBusCartridge write:" << datatoHexString(addr, 4) << ", " << datatoHexString(data, 2));

            cart->writePRGMemory(addr, data);
        }

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x4020 && addr <= 0xFFFF;
        }      
};