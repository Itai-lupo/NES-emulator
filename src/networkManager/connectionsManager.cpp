#include "connectionsManager.h"
#include <string>
#include <algorithm>
#include "logger.h"
#include "mesh.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{

    std::vector<connection*> *connectionsManager::connections;
    
    void connectionsManager::init()
    {
        connections = new std::vector<connection *>();
    }

    void connectionsManager::close()
    {
        std::for_each(
            connections->begin(),
            connections->end(),
            [](connection *con){ delete con; }
        );

        connections->clear();
        delete connections;
    }

    connectionId connectionsManager::addConnection(const std::string& ip, uint32_t port)
    {
        connection *newCon = new connection(ip, port);
        connections->push_back(newCon);
        return newCon->getId();
    }

    void connectionsManager::removeConnection(connectionId id)
    {
        connection *toRemove = getConnection(id);
        delete toRemove;
        connections->erase(std::remove_if(
            connections->begin(),
            connections->end(), 
            [=] (connection *con) -> bool { return con->getId() == id; }
        ), connections->end());
    }

    connection *connectionsManager::getConnection(connectionId id)
    {
        std::vector<connection*>::iterator found = std::find_if(
            connections->begin(),
            connections->end(),
            [=](connection *con) -> bool { return con->getId() == id; }
        );

        if(found != connections->end())
            return *found;
        LAUGHTALE_ENGINR_LOG_WARNING("connection with id: " << id << " was'nt found");
        return nullptr;
    }

    void connectionsManager::sendData(connectionId id, packet *data)
    {
        connection* con = getConnection(id);
        if(con == nullptr){
            LAUGHTALE_ENGINR_LOG_ERROR("could not send packet: " << data->header.id << ", connection with id: " << id <<  " was'nt found");
            return;
        }
        con->send(data);
    }


}