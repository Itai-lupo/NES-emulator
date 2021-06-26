#pragma once
#include "core.h"
#include "packet.h"
#include "networkInterface.h"
#include <string>
#include <thread>
#include "events.h"
#include "dataFormatter.h"
#include "dataCryptographer.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    struct connectionData: public IEventData
    {
        connectionId conId;
        std::string ip;
        uint32_t port;
        networkInterface *networkConnction;

       connectionData(connectionId conId, const std::string& ip, uint32_t port, networkInterface *networkConnction): 
        conId(conId), ip(ip), port(port), networkConnction(networkConnction){}
    };

    struct connectionReadData: public connectionData
    {
        packet data;

       connectionReadData(packet data, connectionId conId, const std::string& ip, uint32_t port, networkInterface *networkConnction): 
        connectionData(conId, ip, port, networkConnction), data(data){}
    };
    
    class connection
    {
        private:
            connectionId id;
            std::thread *listeningThread;
            networkInterface *networkConnction;
            dataFormatter *messageFormat;
            dataCryptographer *dataEncryption;
            bool shouldListen = true;
            
            void connectAndListen();
            void connect();
            void listen();

            std::string ip;
            uint32_t port;

        public:
            connection(const std::string& ip, uint32_t port, dataFormatter *messageFormat, dataCryptographer *dataEncryption);
            ~connection();

            connectionId getId(){ return id; }

            void send(packet *data);
    };    
}