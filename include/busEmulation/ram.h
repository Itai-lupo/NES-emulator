#include "busDevice.h"
#include <cstdint>

#include <array>

class ram: public busDevice<uint8_t, uint16_t>
{
    private:
    	std::array<uint8_t, 2048> ramData;

    public:
        ram()
        {
            for (auto &i : ramData) i = 0x00;
        }

        ~ram()
        {

        }

        virtual bool isInRange(uint16_t addr) override
        {
        	return addr >= 0x0000 && addr <= 0x1FFF;
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            return ramData[addr & 0x07FF];
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
            ramData[addr & 0x07FF] = data;
        }

};