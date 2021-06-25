#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <stdlib.h>
#include <cstring>


using namespace LaughTaleEngine::goingMarryNetworkManger;
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

void onWindowClose(__attribute__((unused)) LaughTaleEngine::IEntity *eventEntity, __attribute__((unused)) LaughTaleEngine::IEventData *sendor)
{
    LaughTaleEngine::app::keepRunning = false;
}

void onServerConnect(__attribute__((unused)) LaughTaleEngine::IEntity *eventEntity, LaughTaleEngine::IEventData *sendor)
{
    LaughTaleEngine::goingMarryNetworkManger::connectionData *eventData = 
        dynamic_cast<LaughTaleEngine::goingMarryNetworkManger::connectionData *>(sendor);

    LAUGHTALE_ENGINR_LOG_INFO("Server Connected");

    byteStream testData;
    testData << "hello world";
    eventData->networkConnction->sendData(testData);
}

TEST(networkManger, decoder)
{
    app::init();
    LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::WindowClose, onWindowClose);
    
    windowPieceId gameWindowId =  LaughTaleEngine::windowManger::addWindow("flappyBird");
    LaughTaleEngine::windowManger::setCamera(gameWindowId, new LaughTaleEngine::orthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
    LaughTaleEngine::windowManger::bindContext(gameWindowId);

    mesh birdMesh(gameWindowId);
    birdMesh.setShader("res/flappyBird/bird.shader");

    birdMesh.setVertexBuffer(birdPostions, 6 * 5 * sizeof(float));
    birdMesh.setIndexBuffer(birdIndices, 12);
    birdMesh.getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
    birdMesh.getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});
    birdMesh.setVertexArray();

    birdMesh.setMaterial("res/textures/5_star.png", {0.0f, 0.0f, 0.0f, 0.0f});
    birdMesh.setTransform(glm::translate(glm::mat4(1.0f), { 0.0f,  0.0f, 0.0f}));

    LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::serverConnection, onServerConnect);
    
    LaughTaleEngine::connectionId testConId = LaughTaleEngine::goingMarryNetworkManger::connectionsManager::addConnection("127.0.0.1", 80);
    // LaughTaleEngine::goingMarryNetworkManger::packet *dataToSend = new LaughTaleEngine::goingMarryNetworkManger::packet(0, &birdMesh);


    // LaughTaleEngine::goingMarryNetworkManger::connectionsManager::sendData(testConId, dataToSend);

    app::run();

    app::close();
    exit(0);
}