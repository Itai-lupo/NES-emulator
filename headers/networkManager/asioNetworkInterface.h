#pragma once
#include <string>
#include "packet.h"
#include "networkInterface.h"
#include <thread>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


namespace LaughTaleEngine::goingMarryNetworkManger
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
            virtual void reciveHeader(packetHeader *buffer) override;
            virtual void reciveBody(packet *buffer) override;
            virtual void sendData(packet *data) override;
            virtual void sendData(const byteStream& data) override;

            virtual bool isConnected() override { return connected; }
    };
}