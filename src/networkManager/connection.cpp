#include "connection.h"
#include "mesh.h"
#include <functional>
#include "logger.h"
#include "asioNetworkInterface.h"

namespace LTE::goingMarryNetworkManger
{
    connection::connection(const std::string& ip, uint32_t port, dataFormatter *messageFormat, dataCryptographer *dataEncryption): 
        ip(ip), port(port), messageFormat(messageFormat), dataEncryption(dataEncryption)
    {
        connect();
        listeningThread = new std::thread(&connection::listen, this);
    }
    
    connection::~connection()
    {
        shouldListen = false;
        networkConnction->close();
        listeningThread->join();
        delete networkConnction;
        delete listeningThread;
    }


    void connection::send(packet& data)
    {
        byteStream buffer;
        while (!canSend){}
        
        canSend = false;
        messageFormat->formatHeaderToSend(buffer, data);
        dataEncryption->encodeHeader(buffer);

        networkConnction->sendData(buffer);
        
        messageFormat->formatBodyToSend(buffer, data);
        dataEncryption->encodeBody(buffer);

        networkConnction->sendData(buffer);
        canSend = true;

    }

    void connection::connect()
    {
        networkConnction = new asioNetworkInterface(ip, port);
        id = networkConnction->getPort();
        connectionData *serverData = new connectionData(id, ip, port, [&, this](packet& data){ this->send(data); } );
        canSend = true;
        serverData->eventType = events::serverConnection;
        if(networkConnction->isConnected())
            eventManger::trigerEvent(serverData);
        delete serverData;
    }

    void connection::listen()
    {
        while(networkConnction->isConnected() && shouldListen)
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

            connectionReadData *recivedData = new connectionReadData(data, id, ip, port,
             [&, this](packet& data){ this->send(data); } 
             );
            recivedData->eventType = events::messageReceived;
            eventManger::trigerEvent(recivedData);
            delete recivedData;
        }
    }
}