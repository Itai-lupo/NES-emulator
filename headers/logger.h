#include <glog/logging.h>
#include <string>


namespace raftelGraphicEngine
{
    class logger
    {
        public:
            static void init(const std::string& pathToLogs, const std::string& projectName);
            static void close();

            static void LogInfo(const std::string& toLog);
            static void LogWarning(const std::string& toLog);
            static void LogError(const std::string& toLog);
            static void LogFatal(const std::string& toLog);

            static void condtionLogInfo(const std::string& toLog, bool condition);
            static void condtionLogWarning(const std::string& toLog, bool condition);
            static void condtionLogError(const std::string& toLog, bool condition);
            static void condtionLogFatal(const std::string& toLog, bool condition);
    };

}

