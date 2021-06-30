#include "asioNetworkInterface.h"
#include "logger.h"

namespace LTE::goingMarryNetworkManger
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

    void asioNetworkInterface::fullClose()
    {
        socketConnction->close();
  
        if(!contextInitialized) return;

        asioContext.stop();
        if (contextThread.joinable()) contextThread.join();

        contextInitialized = false;      
    }

    void asioNetworkInterface::close()
    {
        socketConnction->close();
    }

    asioNetworkInterface::~asioNetworkInterface()
    {
        free(socketConnction);
    }

    void asioNetworkInterface::reciveData(const byteStream& data)
    {
        try
        {
            asio::read(*socketConnction, asio::mutable_buffer((void *)&data.body[0], data.body.size()));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void asioNetworkInterface::sendData(const byteStream& data)
    {
        if(!isConnected())
        {
            LAUGHTALE_ENGINR_LOG_WARNING("failed to send data, connecteion close");
            return;
        }

        asio::write(*socketConnction, asio::buffer(data.getData(), data.getSize()));
    }

    void asioNetworkInterface::initContext()
    {
        contextThread = std::thread([&]() { asioContext.run(); });
        contextInitialized = true;        
    }
}