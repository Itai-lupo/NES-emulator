#include "logger.h"
#include <stdio.h>

namespace raftelGraphicEngine
{
    void logger::init(std::string& pathToLogs, std::string& projectName)
    {
        google::InitGoogleLogging(projectName.c_str());
        FLAGS_stderrthreshold = 0;
        FLAGS_minloglevel = 1;
        google::SetLogDestination(google::WARNING, (pathToLogs + "warninig/" + projectName + "_").c_str());
        google::SetLogDestination(google::ERROR, (pathToLogs + "error/" + projectName + "_").c_str());
        google::SetLogDestination(google::FATAL, (pathToLogs + "fatal/" + projectName + "_").c_str());
        LOG(INFO) << "init glog successfully";
    }

    void logger::close()
    {
        google::ShutdownGoogleLogging();
    }

    void LogInfo(std::string& toLog)
    {
        LOG(INFO) << toLog;
    }

    void LogWarning(std::string& toLog)
    {
        LOG(WARNING) << toLog;
    }
    
    void LogError(std::string& toLog)
    {
        LOG(ERROR) << toLog;
    }
    
    void LogFatal(std::string& toLog)
    {
        LOG(FATAL) << toLog;
    }
    

    void condtionLogInfo(std::string& toLog, bool condition)
    {
        LOG_IF(INFO, condition) << toLog;
    }
    
    void condtionLogWarning(std::string& toLog, bool condition)
    {
        LOG_IF(WARNING, condition) << toLog;
    }
    
    void condtionLogError(std::string& toLog, bool condition)
    {
        LOG_IF(ERROR, condition) << toLog;
    }
    
    void condtionLogFatal(std::string& toLog, bool condition)
    {
        LOG_IF(FATAL, condition) << toLog;
    }
    
}