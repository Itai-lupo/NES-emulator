#pragma once
#include "file.h"
#include <string>

namespace LTE
{
    class filesFactory
    {
        public:
            static file* createFile(const std::string& filePath, std::string fileType);
    };
}