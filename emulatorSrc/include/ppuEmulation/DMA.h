#pragma once
#include "busDevice.h"
#include "2c02.h"

#include <cstdint>
#include <array>


template<typename dataSize, typename addrSize>
class bus;

class DMA: public busDevice<uint8_t, uint16_t>, public LTE::component
{
    private:
        uint8_t dmaPage;
        uint8_t dmaAddr = 0;

        uint8_t dmaData = 0;

        bool dmaActive = false;
        bool readFromPPU = true;

        ppu2c02 *ppu;
        bus<uint8_t, uint16_t> *sysBus;
        

        std::string datatoHexString(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }

    public:
        DMA(ppu2c02 *ppu, bus<uint8_t, uint16_t> *sysBus): ppu(ppu), sysBus(sysBus){}

        ~DMA(){}

        virtual void init(LTE::gameObject *parent) override {}

        virtual void end() override {}


        virtual bool isInRange(uint16_t addr) override
        {
        	return addr == 0x4014;
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
            dmaPage = data;
            dmaActive = true;
        }

        void clock()
        {
            if(readFromPPU)
                dmaData = sysBus->read((uint16_t)dmaPage << 8 | dmaAddr);
            else
            {
                ppu->pOAM[dmaAddr] = dmaData;
                dmaAddr++;

                if(dmaAddr == 0)
                    dmaActive = false;
            }

            readFromPPU = !readFromPPU;
            
        }

        bool isOn()
        {
            return dmaActive;
        }

};