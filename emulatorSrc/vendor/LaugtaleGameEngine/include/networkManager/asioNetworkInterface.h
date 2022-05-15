#pragma once
#include <string>
#include "packet.h"
#include "networkInterface.h"
#include <thread>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


namespace LTE::GMNM
{
    class asioNetworkInterface: public networkInterface
    {
        private:
            static bool contextInitialized;
            static asio::io_context asioContext;
            static std::thread contextThread;
            static void initContext();

            asio::ip::tcp::socket *socketConnction;
            bool connected = false;
        public:
            asioNetworkInterface(const std::string& ip, uint32_t port);

            virtual ~asioNetworkInterface() override;
            virtual void reciveData(byteStream& data) override;
            virtual void sendData(const byteStream& data) override;
            virtual void close() override;
            virtual void fullClose() override;
            
            virtual bool isConnected() override { return connected; }
            virtual uint32_t getPort() override { return socketConnction->local_endpoint().port(); };
    };
}