#pragma once
#include <string>
#include "packet.h"

namespace LTE::GMNM
{
    class networkInterface
    {
        public:
            virtual ~networkInterface() = default;
            
            virtual void close() = 0;
            virtual void fullClose() = 0;
            
            virtual void reciveData(byteStream& data) = 0;
            virtual void sendData(const byteStream& data) = 0;

            virtual bool isConnected() = 0;
            virtual uint32_t getPort() = 0;
    };
}