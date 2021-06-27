#pragma once
#include "core.h"
#include "packet.h"
#include "networkInterface.h"
#include <string>
#include <thread>
#include "events.h"
#include "dataFormatter.h"
#include "dataCryptographer.h"
#include <functional>

namespace LaughTaleEngine::goingMarryNetworkManger
{
    struct connectionData: public IEventData
    {
        connectionId conId;
        std::string ip;
        uint32_t port;
        std::function<void(packet& data)> send;

       connectionData(connectionId conId, const std::string& ip, uint32_t port, std::function<void(packet& data)> send): 
        conId(conId), ip(ip), port(port), send(send){}
    };

    struct connectionReadData: public connectionData
    {
        packet data;

       connectionReadData(packet data, connectionId conId, const std::string& ip, uint32_t port, std::function<void(packet& data)> send): 
        connectionData(conId, ip, port, send), data(data){}
    };
    
    class connection
    {
        private:
            bool canSend = false;
            connectionId id = 0;
            std::thread *listeningThread;
            networkInterface *networkConnction;
            dataFormatter *messageFormat;
            dataCryptographer *dataEncryption;
            bool shouldListen = true;
            
            void connect();
            void listen();

            std::string ip;
            uint32_t port;

        public:
            connection(const std::string& ip, uint32_t port, dataFormatter *messageFormat, dataCryptographer *dataEncryption);
            ~connection();

            connectionId getId(){ return id; }
            void fullClose(){ networkConnction->fullClose(); }

            void send(packet& data);
    };    
}