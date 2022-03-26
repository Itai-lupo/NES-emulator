#pragma once
#include "cartridge.h"


class nameTable: public busDevice<uint8_t, uint16_t>
{
    private:
	    uint8_t tableData[2][1024];
    	static std::string hex(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }
    public:
        ppuBusCartridge *cart;
        
        nameTable()
        {
            for (size_t i = 0; i < 1024; i++)
            {
                tableData[0][i] = 0;
                tableData[1][i] = 0;
            }
            
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            addr &= 0x0FFF;
            if (cart->getMirror() == MIRROR::VERTICAL)
            {
                // Vertical
                if (addr >= 0x0000 && addr <= 0x03FF)
                    return tableData[0][addr & 0x03FF];
                if (addr >= 0x0400 && addr <= 0x07FF)
                    return tableData[1][addr & 0x03FF];
                if (addr >= 0x0800 && addr <= 0x0BFF)
                    return tableData[0][addr & 0x03FF];
                if (addr >= 0x0C00 && addr <= 0x0FFF)
                    return tableData[1][addr & 0x03FF];
            }
            else if (cart->getMirror() == MIRROR::HORIZONTAL)
            {
                // Horizontal
                if (addr >= 0x0000 && addr <= 0x03FF)
                    return tableData[0][addr & 0x03FF];
                if (addr >= 0x0400 && addr <= 0x07FF)
                    return tableData[0][addr & 0x03FF];
                if (addr >= 0x0800 && addr <= 0x0BFF)
                    return tableData[1][addr & 0x03FF];
                if (addr >= 0x0C00 && addr <= 0x0FFF)
                    return tableData[1][addr & 0x03FF];
            }
            return 0;
        }
        
        virtual void write(uint16_t addr, uint8_t data) override
        {
            addr &= 0x0FFF;
            // LAUGHTALE_ENGINR_CONDTION_LOG_INFO(data, data != 0 && data != 32)
            
            if (cart->getMirror() == MIRROR::VERTICAL)
            {
                // Vertical
                if (addr >= 0x0000 && addr <= 0x03FF)
                    tableData[0][addr & 0x03FF] = data;
                if (addr >= 0x0400 && addr <= 0x07FF)
                    tableData[1][addr & 0x03FF] = data;
                if (addr >= 0x0800 && addr <= 0x0BFF)
                    tableData[0][addr & 0x03FF] = data;
                if (addr >= 0x0C00 && addr <= 0x0FFF)
                    tableData[1][addr & 0x03FF] = data;
            }
            else if (cart->getMirror() == MIRROR::HORIZONTAL)
            {
                // Horizontal
                if (addr >= 0x0000 && addr <= 0x03FF)
                    tableData[0][addr & 0x03FF] = data;
                if (addr >= 0x0400 && addr <= 0x07FF)
                    tableData[0][addr & 0x03FF] = data;
                if (addr >= 0x0800 && addr <= 0x0BFF)
                    tableData[1][addr & 0x03FF] = data;
                if (addr >= 0x0C00 && addr <= 0x0FFF)
                    tableData[1][addr & 0x03FF] = data;
            }
        }

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x2000 && addr <= 0x3EFF;
        }
};
