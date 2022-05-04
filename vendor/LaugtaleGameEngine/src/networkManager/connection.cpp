#include "connection.h"
#include "mesh.h"
#include <functional>
#include "logger.h"
#include "asioNetworkInterface.h"
#include <sys/prctl.h>

namespace LTE::GMNM
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
        byteStream headerBuffer;
        while (!canSend){}
        
        canSend = false;
        messageFormat->formatHeaderToSend(headerBuffer, data);
        dataEncryption->encodeHeader(headerBuffer);

        networkConnction->sendData(headerBuffer);
        
        byteStream bodyBuffer;
        messageFormat->formatBodyToSend(bodyBuffer, data);
        dataEncryption->encodeBody(bodyBuffer);
        if(bodyBuffer.getSize() != 0)
            networkConnction->sendData(bodyBuffer);
        canSend = true;

    }

    void connection::connect()
    {
        networkConnction = new asioNetworkInterface(ip, port);
        if(!networkConnction->isConnected())
            return;
        
        id = networkConnction->getPort();
        connectionData *serverData = new connectionData(id, ip, port, [&, this](packet& data){ this->send(data); } );
        canSend = true;
        serverData->route = "server connection/";
        if(networkConnction->isConnected())
            eventManger::trigerEvent(serverData);
        delete serverData;
    }

    void connection::listen()
    {
        std::string thradName = "con " + std::to_string(id);
        prctl(PR_SET_NAME, thradName.c_str(), 0, 0, 0);
        while(networkConnction->isConnected() && shouldListen)
        {
            packet data;
            byteStream buffer;
            int headerSize = 0;

            buffer.body.resize(messageFormat->getHeaderSize());
            networkConnction->reciveData(buffer);
            headerSize = buffer.getSize();
            
            dataEncryption->decodeHeader(buffer);
            messageFormat->formatRecivedHeader(buffer, data);
            
            buffer.body.resize(data.header->getContentLength());
            networkConnction->reciveData(buffer);
            dataEncryption->decodeBody(buffer);
            messageFormat->formatRecivedBody(buffer, data);

            connectionReadData *recivedData = new connectionReadData(data, id, ip, port,
             [&, this](packet& data){ this->send(data); } 
             );
            recivedData->route = "message received/";
            if (headerSize != 0)
                eventManger::trigerEvent(recivedData);
            delete recivedData;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}