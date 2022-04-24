#pragma once
#include "cartridge.h"

class ppuBusCartridge: public busDevice<uint8_t, uint16_t>
{
    private:
        cartridge *cart;

    public:
        ppuBusCartridge(cartridge *cart): cart(cart){}

        MIRROR getMirror()
        {
            return cart->getMirror();
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            return cart->readCHRMemory(addr);
        }
        
        virtual void write(uint16_t addr, uint8_t data) override
        {
            cart->writeCHRMemory(addr, data);
        }

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x0000 && addr <= 0x1FFF;
        }
};
