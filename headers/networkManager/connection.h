#pragma once
#include "core.h"
#include "packet.h"
#include "networkInterface.h"
#include <string>
#include <thread>
#include "events.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    struct connectionData: public IEventData
    {
        std::string ip;
        uint32_t port;
        networkInterface *networkConnction;

       connectionData(const std::string& ip, uint32_t port, networkInterface *networkConnction): ip(ip), port(port), networkConnction(networkConnction){}
    };
    
    class connection
    {
        private:
            connectionId id;
            std::thread *listeningThread;
            networkInterface *networkConnction;
            bool shouldListen = true;
            
            void connectAndListen();
            void connect();
            void listen();

            std::string ip;
            uint32_t port;

        public:
            connection(const std::string& ip, uint32_t port);
            ~connection();

            connectionId getId(){ return id; }

            void send(packet *data);
    };    
}