#pragma once
#include "file.h"

namespace LTE
{
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    class glslFile: public file
    {
        private:
            ShaderProgramSource source;

            void ParseShaders();

        public:
            glslFile(const std::string& filePath);

            virtual ~glslFile();
            virtual asset *genrateAssetForWindow(windowPieceId  winId);

            virtual void loadFileData();

    };
}