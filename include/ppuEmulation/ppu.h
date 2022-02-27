#include "busDevice.h"
#include <cstdint>

#include <array>

#include "bus.h"


class ppu: public busDevice<uint8_t, uint16_t>
{
    private:
    	std::array<uint8_t, 0x1FFF> ppuData;
        bus<uint8_t, uint16_t> ppuBus; // pattern(0x0000 - 0x1FFF), name table(0x2000 - 0x2FFF), PALETTES( 0x3F00 - 0x3FFF)
    public:
        ppu()
        {
            for (auto &i : ppuData) i = 0x00;
        }

        ~ppu()
        {

        }

        virtual bool isInRange(uint16_t addr) override
        {
        	return addr >= 0x2000 && addr <= 0x3FFF;
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            addr &= 0x0007;
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
            addr &= 0x0007;
        }

};