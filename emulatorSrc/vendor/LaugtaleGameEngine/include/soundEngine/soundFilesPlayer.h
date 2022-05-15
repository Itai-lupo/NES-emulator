#pragma once
#include <string>
#include "envelope.h"
#include "soundSynthesizer.h"

namespace LTE
{
    class soundFile : public envelope
    {
    private:
        uint len;
        std::vector <short *> channels;
        long i = 0;
        
    public:
        soundFile(std::string filePath);
        ~soundFile();

        virtual double getSampelAmp(double time, int16_t micAmp);
    };        

    class soundFilesPlayer
    {
        private:
            static inline soundFile *file;
        public:
            static void playFile(std::string filePath)
            {
                file = new soundFile(filePath);
                soundSynthesizer::addEnvelope(file);
            }

            static void stopPlayingFile()
            {
                soundSynthesizer::removeEnvelope(file->getId());
                // delete file;
            }
    };
}