#pragma once
#include "core.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

namespace LaughTaleEngine::goingMarryNetworkManger
{
    class byteStream
    {
        private:
            std::vector<uint8_t> body;

        public:
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
        packetId id;
        uint32_t content_length;
        std::string route;
        
    } ;

    struct packetBody
    {
        
    };

    struct packet
    {
        packetHeader header;
        packetBody body;

        packet(const packetHeader& header): header(header)
        {
            
        }
            
            
        friend std::ostream& operator << (std::ostream& os, const packet& msg)
        {
            os << "ID:" << msg.header.id << " Size:" << msg.header.content_length << " route: " << msg.header.route;
            return os;
        }
    };
} 