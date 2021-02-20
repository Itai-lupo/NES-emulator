#include <glog/logging.h>
#include <string>


namespace raftelGraphicEngine
{
    class logger
    {
        public:
            static void init(std::string& pathToLogs, std::string& projectName);
            static void close();

            static void LogInfo(std::string& toLog);
            static void LogWarning(std::string& toLog);
            static void LogError(std::string& toLog);
            static void LogFatal(std::string& toLog);

            static void condtionLogInfo(std::string& toLog, bool condition);
            static void condtionLogWarning(std::string& toLog, bool condition);
            static void condtionLogError(std::string& toLog, bool condition);
            static void condtionLogFatal(std::string& toLog, bool condition);
    };

}

