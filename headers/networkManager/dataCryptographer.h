#pragma once 
#include "packet.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    class dataCryptographer
    {
        public:
            virtual void decodeHeader(byteStream& dataToDecode) = 0;
            virtual void decodeBody(byteStream& dataToDecode) = 0;

            virtual void encodeHeader(byteStream& dataToEncode) = 0;
            virtual void encodeBody(byteStream& dataToEncode) = 0;
    };
    
    class noEncrption: public dataCryptographer
    {
        public:
            virtual void decodeHeader(byteStream& dataToDecode) override {}
            virtual void decodeBody(byteStream& dataToDecode) override {}

            virtual void encodeHeader(byteStream& dataToEncode) override {}
            virtual void encodeBody(byteStream& dataToEncode) override {}
    };
}