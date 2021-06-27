#pragma once
#include "core.h"
#include "networkInterface.h"
#include <string>
#include <thread>
#include <vector>
#include "packet.h"
#include "connection.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    class connectionsManager
    {
        private:
            static std::vector<connection*> *connections;
            

        public:
            static void init();
            static void close();

            static connectionId addConnection(const std::string& ip, uint32_t port, dataFormatter *messageFormat, dataCryptographer *dataEncryption = new noEncrption());
            static void removeConnection(connectionId id);
            static connection *getConnection(connectionId id);
            
            static void sendData(connectionId id, packet& data);
    };    
}