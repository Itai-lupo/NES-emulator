#include "app.h"

#include "core.h"
#include "events.h"
#include "entity.h"
#include "logger.h"
#include "window.h"
#include "VertexBufferManger.h"
#include "indexBufferManger.h"
#include "vertexArrayManger.h"
#include "shaderManger.h"
#include "soundEngine.h"
#include "soundSynthesizer.h"

#include <chrono>
#include <string>


namespace LaughTaleEngine
{
    bool app::keepRunning = true;
    
    void app::init()
    {
        keepRunning = true;
        std::string pathToLogs =  "./logs/";
        std::string projectName =  "LaughTaleEngine";
        logger::init(pathToLogs, projectName);
        eventManger::init();
        entityManger::init();
        windowManger::init();
        soundEngine::init();
        soundSynthesizer::init();
    }

    void app::close()
    {
        keepRunning = false;
        logger::close();
        eventManger::close();
        entityManger::close();
        windowManger::close();
        soundEngine::close();
        soundSynthesizer::close();
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
            now = getTime();
            updateData->DeltaTime = now - updateData->currentTime;
            updateData->currentTime = now;
            eventManger::trigerEvent(events::AppUpdate, updateData);
        }
        
    }
};

