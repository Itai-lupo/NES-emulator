#include "core.h"
#include "events.h"
#include "logger.h"
#include <stdexcept>
#include <string>

namespace raftelGraphicEngine
{
    void init()
    {
        std::string pathToLogs =  "./logs/";
        std::string projectName =  "raftelGraphicEngine";
        logger::init(pathToLogs, projectName);
        eventManger::init();
    }

    void close()
    {
        logger::close();
        eventManger::close();

    }
};

