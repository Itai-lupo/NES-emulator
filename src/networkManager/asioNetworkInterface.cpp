#include "asioNetworkInterface.h"
#include "logger.h"

namespace LaughTaleEngine::goingMarryNetworkManger
{
    bool asioNetworkInterface::contextInitialized = false;
    asio::io_context  asioNetworkInterface::asioContext;
    std::thread asioNetworkInterface::contextThread;

    asioNetworkInterface::asioNetworkInterface(const std::string& ip, uint32_t port): networkInterface()
    {
        try
        {
            if(contextInitialized)
                initContext(); 

            asio::ip::tcp::resolver resolver(asioContext);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip, std::to_string(port));
            socketConnction = new asio::ip::tcp::socket(asioContext);

            asio::connect(*socketConnction, endpoints);
            connected = true;
        }
        catch(const std::exception& e)
        {
            LAUGHTALE_ENGINR_LOG_ERROR(e.what());
            connected = false;
        }
    }

    asioNetworkInterface::~asioNetworkInterface()
    {

    }

    void asioNetworkInterface::reciveHeader(packetHeader *buffer)
    {

    }

    void asioNetworkInterface::reciveBody(packet *buffer)
    {

    }

    void asioNetworkInterface::sendData(packet *data)
    {

    }    

    void asioNetworkInterface::sendData(const byteStream& data)
    {
        LAUGHTALE_ENGINR_LOG_INFO(data);
        LAUGHTALE_ENGINR_LOG_INFO(asio::write(*socketConnction, asio::buffer(data.getData(), data.getSize() - 1)));
    }

    void asioNetworkInterface::initContext()
    {
        contextThread = std::thread([&]() { asioContext.run(); });
        contextInitialized = true;        
    }

}