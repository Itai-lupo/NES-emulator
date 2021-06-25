#include "connection.h"
#include "mesh.h"
#include <functional>
#include "logger.h"
#include "asioNetworkInterface.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    connection::connection(const std::string& ip, uint32_t port): ip(ip), port(port)
    {
        listeningThread = new std::thread(&connection::connectAndListen, this);
    }
    
    connection::~connection()
    {
        shouldListen = false;
        listeningThread->join();
        delete listeningThread;
    }


    void connection::send(packet *data)
    {

    }

    void connection::connectAndListen()
    {
        connect();
        listen();

    }

    void connection::connect()
    {
        networkConnction = new asioNetworkInterface(ip, port);
        connectionData *serverData = new connectionData(ip, port, networkConnction);
        if(networkConnction->isConnected())
            eventManger::trigerEvent(events::serverConnection, serverData);
    }

    void connection::listen()
    {
        while(networkConnction->isConnected())
        {
            // LAUGHTALE_ENGINR_LOG_INFO("connected");
        }
    }

}