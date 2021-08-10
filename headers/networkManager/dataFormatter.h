#pragma once 
#include "packet.h"


namespace LTE::GMNM
{
    class dataFormatter
    {
        public:
            virtual uint32_t getHeaderSize() = 0;
            virtual void formatRecivedHeader(byteStream& data, packet& formatedHeader) = 0;
            virtual void formatRecivedBody(byteStream& data, packet& formatedData) = 0;
            
            virtual void formatHeaderToSend(byteStream& buffer, packet& dataToSend) = 0;
            virtual void formatBodyToSend(byteStream& buffer, packet& dataToSend) = 0;
    };
    
}