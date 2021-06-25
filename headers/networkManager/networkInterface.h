#pragma once
#include <string>
#include "packet.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    class networkInterface
    {
        public:
            virtual ~networkInterface() = default;

            virtual void reciveHeader(packetHeader *buffer) = 0;
            virtual void reciveBody(packet *buffer) = 0;
            virtual void sendData(packet *data) = 0;
            virtual void sendData(const byteStream& data) = 0;

            virtual bool isConnected() = 0;
    };
}