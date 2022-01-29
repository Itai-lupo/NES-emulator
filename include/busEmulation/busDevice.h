#pragma once

template<typename dataSize, typename addrSize>
class busDevice
{
    private:
    
    public:
        virtual ~busDevice() = default;


        virtual bool isInRange(addrSize addr) = 0;
        virtual dataSize read(addrSize addr, bool bReadOnly = false) = 0;
        virtual void write(addrSize addr, dataSize data) = 0;
};