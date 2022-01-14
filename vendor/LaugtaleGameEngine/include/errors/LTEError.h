#pragma once
#include <stdexcept>
#include <string>

namespace LTE
{
    class LTEException: public std::exception
    {
        protected:
            std::string error;

        public:
            LTEException(const std::string& error): error(error){}

            const char * what () const throw ()
            {
                return error.c_str();
            }

    };

    class GameObjectNotFoundException: public LTEException
    {
        public:
            GameObjectNotFoundException(const std::string& error): LTEException("game object not found exception: "  + error)
            {

            }
    
    };

    class ComponentNotFoundException: public LTEException
    {
        public:
            ComponentNotFoundException(const std::string& error): LTEException("game object not found exception: "  + error)
            {

            }
    };


    class WindowNotFoundException: public LTEException
    {
        public:
            WindowNotFoundException(const std::string& error): LTEException("window not found exception: "  + error)
            {

            }
    };


    class PCMDeviceNotFoundException: public LTEException
    {
        public:
            PCMDeviceNotFoundException(const std::string& error): LTEException("pcm device not found exception: "  + error)
            {

            }
    };
}