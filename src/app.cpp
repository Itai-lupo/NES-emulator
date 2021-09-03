#include "app.h"

#include "core.h"
#include "events.h"
#include "entity.h"
#include "logger.h"
#include "windowManger.h"
#include "indexBuffer.h"
#include "soundEngine.h"
#include "soundSynthesizer.h"
#include "materialsManger.h"
#include "connectionsManager.h"

#include <chrono>
#include <string>


namespace LTE
{
    bool app::keepRunning = true;
    
    void app::init()
    {
        keepRunning = true;
        std::string pathToLogs =  "./logs/";
        std::string projectName =  "LTE";
        logger::init(pathToLogs, projectName);
        eventManger::init();
        entityManger::init();
        windowManger::init();
        soundEngine::init();
        materialsManger::init();
        GMNM::connectionsManager::init();

        osFactory = OSAbstractFactory::init();
        os = osFactory->createOsApi();
    }

    void app::close()
    {
        keepRunning = false;
        eventManger::close();
        entityManger::close();
        soundEngine::close();
        windowManger::close();
        materialsManger::close();
        GMNM::connectionsManager::close();
        logger::close();

        delete osFactory;
        delete os;
    }


    uint64_t getTime(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void app::run()
    {
        uint64_t startTime = getTime();
        uint64_t now = getTime();

        onUpdateData *updateData = new onUpdateData(startTime, startTime, 0);
        while (keepRunning)
        {
            os->pollEvents();
            now = getTime();
            updateData->DeltaTime = now - updateData->currentTime;
            updateData->currentTime = now;
            eventManger::trigerEvent(updateData);
        }   
    }
};

