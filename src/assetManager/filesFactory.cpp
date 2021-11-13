#include "filesFactory.h"
#include "logger.h"

#include "fileTypes.h"

namespace LTE
{   
    file* filesFactory::createFile(const std::string& filePath, std::string fileType)
    {
        switch (stringToFileType(fileType))
        {
            case fileTypes::glsl:
                return new glslFile(filePath);
                break;
            case fileTypes::mp3:
                return new mp3File(filePath);
                break;
            case fileTypes::png:
                return new pngFile(filePath);
                break;
            
            default:
                LAUGHTALE_ENGINR_LOG_ERROR("file type: " << fileType << " not registry in the system.");
                return NULL;
                break;
        }
    }
}