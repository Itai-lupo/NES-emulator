#include "connection.h"
#include "mesh.h"
#include <functional>
#include "logger.h"
#include "asioNetworkInterface.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    connection::connection(const std::string& ip, uint32_t port, dataFormatter *messageFormat, dataCryptographer *dataEncryption): 
        ip(ip), port(port), messageFormat(messageFormat), dataEncryption(dataEncryption)
    {
        listeningThread = new std::thread(&connection::connectAndListen, this);
    }
    
    connection::~connection()
    {
        shouldListen = false;
        // listeningThread->join();
        // delete listeningThread;
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
        id = networkConnction->getPort();
        connectionData *serverData = new connectionData(id, ip, port, networkConnction);
        if(networkConnction->isConnected())
            eventManger::trigerEvent(events::serverConnection, serverData);
        delete serverData;
    }

    void connection::listen()
    {
        while(networkConnction->isConnected())
        {
            packet data;
            byteStream buffer;

            buffer.body.resize(messageFormat->getHeaderSize());
            networkConnction->reciveData(buffer);
            dataEncryption->decodeHeader(buffer);
            messageFormat->formatRecivedHeader(buffer, data);
            
            buffer.body.resize(data.header->getContentLength());
            networkConnction->reciveData(buffer);
            dataEncryption->decodeBody(buffer);
            messageFormat->formatRecivedBody(buffer, data);

            LAUGHTALE_ENGINR_LOG_INFO(data);
            connectionReadData *recivedData = new connectionReadData(data, id, ip, port, networkConnction);
            eventManger::trigerEvent(events::messageReceived, recivedData);
            delete recivedData;
        }
    }

}