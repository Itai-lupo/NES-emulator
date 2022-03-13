#pragma once

class nameTable: public busDevice<uint8_t, uint16_t>
{
    private:
	    uint8_t tableData[2][1024];

    public:
        nameTable(){}

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            
        }
        
        virtual void write(uint16_t addr, uint8_t data) override
        {
            
        }

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x2000 && addr <= 0x2FFF;
        }
};
