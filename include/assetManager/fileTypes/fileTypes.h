#include <string>
#include <map>

#include "mp3File.h"
#include "glslFile.h"
#include "pngFile.h"

namespace LTE
{
    enum fileTypes {
        mp3, glsl, png
    };

    fileTypes stringToFileType(const std::string& str)
    {
         std::map<std::string, fileTypes> stringTofileTypesMap = {
            {"mp3", mp3},
            {"glsl", glsl},
            {"png", png}
        };

        return stringTofileTypesMap[str];
    }
}