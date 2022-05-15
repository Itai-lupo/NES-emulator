#pragma once
#include "LaughTaleEngine.h"
#include <vector>

#include "busDevice.h"

template<typename dataSize, typename addrSize>
class bus: public LTE::component
{
    private:
        std::vector<busDevice<dataSize, addrSize> *> busDevices;

    public:
        virtual void init(LTE::gameObject *parent) override
        {

        }

        virtual void end() override
        {

        }

        bus(/* args */){}
        ~bus(){}

        bus *pushDevice(busDevice<dataSize, addrSize> *toAdd)
        {
            busDevices.push_back(toAdd);
            return this;
        }

        dataSize read(addrSize addr, bool bReadOnly = false)
        {
            for(auto& device: busDevices)
                if(device->isInRange(addr))
                    return device->read(addr, bReadOnly);
            return 0;
        }

        void write(addrSize addr, dataSize data)
        {
            for(auto& device: busDevices)
                if(device->isInRange(addr))
                    return device->write(addr, data);
        }
};