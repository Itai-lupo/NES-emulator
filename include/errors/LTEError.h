#include <stdexcept>
#include <string>

namespace LTE
{
    class GameObjectNotFoundException: public std::exception
    {
        private:
            std::string error;
        public:
        GameObjectNotFoundException(const std::string& error):error("game object not found exception: "  + error)
        {

        }
    
        const char * what () const throw ()
        {
            return error.c_str();
        }
    };


    class WindowNotFoundException: public std::exception
    {
        private:
            std::string error;
        public:
        WindowNotFoundException(const std::string& error):error("window not found exception: "  + error)
        {

        }
    
        const char * what () const throw ()
        {
            return error.c_str();
        }
    };
}