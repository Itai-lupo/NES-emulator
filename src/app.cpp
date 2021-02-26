#include "core.h"
#include "events.h"
#include "entity.h"
#include "logger.h"

#include <string>
#include <GLFW/glfw3.h>
namespace raftelGraphicEngine
{
    void init()
    {
        std::string pathToLogs =  "./logs/";
        std::string projectName =  "raftelGraphicEngine";
        logger::init(pathToLogs, projectName);
        eventManger::init();
        entityManger::init();
    }

    void close()
    {
        logger::close();
        eventManger::close();
        entityManger::close();
        
    }
};

