#pragma once
#include "busDevice.h"
#include <cstdint>

#include <array>

class ram: public busDevice<uint8_t, uint16_t>
{
    private:
        std::array<uint8_t, 2048> ramData;
        std::string datatoHexString(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }


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
            // LAUGHTALE_ENGINR_LOG_INFO("ram read: " << datatoHexString(addr, 4) << ", " << datatoHexString(ramData[addr & 0x07FF], 2));
            return ramData[addr & 0x07FF];
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
            // LAUGHTALE_ENGINR_LOG_INFO("ram write: " << datatoHexString(addr, 4) << ", " << datatoHexString(data, 2));
            ramData[addr & 0x07FF] = data;
        }

};