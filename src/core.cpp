#include "core.h"
#include <stdexcept>
#include "logger.h"
#include <string>

namespace raftelGraphicEngine
{
    void init()
    {
        std::string pathToLogs =  "./logs/";
        std::string projectName =  "raftelGraphicEngine";
        logger::init(pathToLogs, projectName);
    }

    void close()
    {
        logger::close();
    }
};

