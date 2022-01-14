#pragma once
#include "core.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

namespace LTE::GMNM
{
    struct byteStream
    {
        std::vector<uint8_t> body;
        size_t getSize() const
        {
            return body.size();
        }

        std::vector<uint8_t> getData() const
        {
            return body;
        }

        template<typename DataType>
        friend byteStream& operator << (byteStream& bytes, const DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

            size_t i = bytes.body.size();
            bytes.body.resize(bytes.body.size() + sizeof(DataType));
            std::memcpy(bytes.body.data() + i, &data, sizeof(DataType));

            return bytes;
        }

        template<typename DataType>
        friend byteStream& operator >> (byteStream& data, DataType& buffer)
        {
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

            size_t i = data.body.size() - sizeof(DataType);
            std::memcpy(&buffer, data.body.data() + i, sizeof(DataType));
            data.body.resize(i);

            return data;
        }	

        
        friend std::ostream& operator << (std::ostream& os, const byteStream& msg)
        {
            os << "byteStream: " << msg.body.data();
            return os;
        }	

    };

    struct packetHeader
    {
        virtual uint32_t getContentLength() = 0;
        virtual packetId getContentId() = 0;
        virtual std::string toString() = 0;

    };

    struct packetBody
    {
        virtual void *getData() = 0;
        virtual std::string toString() = 0;
    };

    struct packet
    {
        packetHeader *header;
        packetBody *body;
            
        friend std::ostream& operator << (std::ostream& os, const packet& msg)
        {
            os << "Header: \n\t" << msg.header->toString() << std::endl << "Body: \n\t" << msg.body->toString();
            return os;
        }
    };
} 