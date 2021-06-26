#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <stdlib.h>
#include <cstring>
using namespace LaughTaleEngine;
using namespace goingMarryNetworkManger;


struct basicHeader: public packetHeader
{
    uint32_t contentLength;
    packetId id;

    virtual uint32_t getContentLength() override
    {
        return contentLength;
    }

    virtual packetId getContentId() override
    {
        return id;
    }

    virtual std::string toString() override
    {
        std::ostringstream str;
        str << "ID: " << id << ", content length: " << contentLength;
        return str.str();
    }

};

struct basicBody: public packetBody
{
    byteStream data;
    virtual void *getData() override
    {
        return (void*)&data;
    }

    virtual std::string toString() override
    {
        std::ostringstream str;
        str << data;
        return str.str();
    }

};

class basicDataFormat: public dataFormatter
{
    virtual uint32_t getHeaderSize() override { return 8; }

    virtual void formatRecivedHeader(byteStream& data, packet& formatedHeader) override
    {
        basicHeader *temp = new basicHeader();
        data >> temp->id;
        data >> temp->contentLength;

        formatedHeader.header = temp;
    }

    virtual void formatRecivedBody(byteStream& data, packet& formatedData) override
    {
        basicBody *temp = new basicBody();
        temp->data = data;
        formatedData.body = temp;
    }

    
    virtual void formatHeaderToSend(byteStream& buffer, packet& dataToSend) override
    {

    }

    virtual void formatBodyToSend(byteStream& buffer, packet& dataToSend) override
    {

    }

};

using namespace LaughTaleEngine;
float birdPostions[6 * 5] = 
{
        0.00f,     0.10f,  0.0f,   0.5f,   1.0f,
        0.085f,    0.05f,  0.0f,   1.0f,   0.75f,
        0.085f,   -0.05f,  0.0f,   1.0f,   0.25f,
        0.00f,    -0.10f,  0.0f,   0.5f,   0.0f,
        -0.085f,   -0.05f,  0.0f,   0.0f,   0.25f,
        -0.085f,    0.05f,  0.0f,   0.0f,   0.75f
};


unsigned int birdIndices[12] = 
{
    0, 1, 5,
    1, 2, 4,
    2, 3, 4,
    1, 4, 5
};

void onWindowClose(__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
{
    app::keepRunning = false;
}

void onServerConnect(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
{
    connectionData *eventData = 
        dynamic_cast<connectionData *>(sendor);

    LAUGHTALE_ENGINR_LOG_INFO("Server Connected");

    byteStream headerData;
    byteStream bodyData;
    basicHeader head;

    bodyData << "hello world";
    bodyData.body.resize(bodyData.getSize() - 1);

    head.id = 1;
    head.contentLength = bodyData.getSize();
    headerData << head.contentLength << head.id;

    eventData->networkConnction->sendData(headerData);
    eventData->networkConnction->sendData(bodyData);
}

void onServerMessage(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
{
    connectionReadData *eventData = 
        dynamic_cast<connectionReadData *>(sendor);

    LAUGHTALE_ENGINR_LOG_INFO("Server Message " << eventData->data);
    packetHeader *head = eventData->data.header;
    
    byteStream headerData;
    byteStream bodyData = *(byteStream *)eventData->data.body->getData();
    bodyData << '!' << '!';
    headerData << head->getContentLength() + 2 << head->getContentId();

    eventData->networkConnction->sendData(headerData);
    eventData->networkConnction->sendData(bodyData);
}

TEST(networkManger, decoder)
{
    app::init();
    eventManger::addEvent(events::WindowClose, onWindowClose);
    
    windowPieceId gameWindowId =  windowManger::addWindow("flappyBird");
    windowManger::setCamera(gameWindowId, new orthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
    windowManger::bindContext(gameWindowId);

    mesh birdMesh(gameWindowId);
    birdMesh.setShader("res/flappyBird/bird.shader");

    birdMesh.setVertexBuffer(birdPostions, 6 * 5 * sizeof(float));
    birdMesh.setIndexBuffer(birdIndices, 12);
    birdMesh.getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
    birdMesh.getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});
    birdMesh.setVertexArray();

    birdMesh.setMaterial("res/textures/5_star.png", {0.0f, 0.0f, 0.0f, 0.0f});
    birdMesh.setTransform(glm::translate(glm::mat4(1.0f), { 0.0f,  0.0f, 0.0f}));

    eventManger::addEvent(events::serverConnection, onServerConnect);
    eventManger::addEvent(events::messageReceived, onServerMessage);
    
    connectionId testConId = connectionsManager::addConnection("localhost", 80, new basicDataFormat());
    // packet *dataToSend = new packet(0, &birdMesh);


    // connectionsManager::sendData(testConId, dataToSend);

    app::run();

    app::close();
    exit(0);
}