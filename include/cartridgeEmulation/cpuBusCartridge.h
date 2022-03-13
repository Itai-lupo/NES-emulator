#pragma once
#include "cartridge.h"

class cpuBusCartridge: public busDevice<uint8_t, uint16_t>
{
    private:
        cartridge *cart;

    public:
        cpuBusCartridge(cartridge *cart): cart(cart){}

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            return cart->readPRGMemory(addr);
        }
        
        virtual void write(uint16_t addr, uint8_t data) override
        {
            cart->writePRGMemory(addr, data);
        }

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x4020 && addr <= 0xFFFF;
        }      
};