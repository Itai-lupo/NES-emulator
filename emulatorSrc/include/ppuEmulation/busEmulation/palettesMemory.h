#pragma once

class palettesMemory: public busDevice<uint8_t, uint16_t>
{
    private:
	    uint8_t paletteData[0x20] = {0};

    public:
        palettesMemory(){}

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            
            addr &= 0x001F;
            if (addr == 0x0010) addr = 0x0000;
            if (addr == 0x0014) addr = 0x0004;
            if (addr == 0x0018) addr = 0x0008;
            if (addr == 0x001C) addr = 0x000C;
            return paletteData[addr];
        }
        
        virtual void write(uint16_t addr, uint8_t data) override
        {
            addr &= 0x001F;
            if (addr == 0x0010) addr = 0x0000;
            if (addr == 0x0014) addr = 0x0004;
            if (addr == 0x0018) addr = 0x0008;
            if (addr == 0x001C) addr = 0x000C;
            paletteData[addr ] = data;
        }

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x3F00 && addr <= 0x3FFF;
        }
};
