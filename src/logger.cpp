#include "logger.h"
#include <stdio.h>

namespace raftelGraphicEngine
{
    static bool wasInit = false; 
    void logger::init(std::string& pathToLogs, std::string& projectName)
    {
        if(wasInit) return;
        google::InitGoogleLogging(projectName.c_str());
        FLAGS_stderrthreshold = 0;

        google::SetLogDestination(google::INFO, (pathToLogs + "info/" + projectName + "_").c_str());
        google::SetLogDestination(google::WARNING, (pathToLogs + "warninig/" + projectName + "_").c_str());
        google::SetLogDestination(google::ERROR, (pathToLogs + "error/" + projectName + "_").c_str());
        google::SetLogDestination(google::FATAL, (pathToLogs + "fatal/" + projectName + "_").c_str());
        wasInit = true;
        LOG(INFO) << "init glog successfully";
    }

    void logger::close()
    {
        // google::ShutdownGoogleLogging();
    }

    void logger::LogInfo(std::string& toLog)
    {
        LOG(INFO) << toLog;
    }

    void logger::LogWarning(std::string& toLog)
    {
        LOG(WARNING) << toLog;
    }
    
    void logger::LogError(std::string& toLog)
    {
        LOG(ERROR) << toLog;
    }
    
    void logger::LogFatal(std::string& toLog)
    {
        LOG(FATAL) << toLog;
    }
    

    void logger::condtionLogInfo(std::string& toLog, bool condition)
    {
        LOG_IF(INFO, condition) << toLog;
    }
    
    void logger::condtionLogWarning(std::string& toLog, bool condition)
    {
        LOG_IF(WARNING, condition) << toLog;
    }
    
    void logger::condtionLogError(std::string& toLog, bool condition)
    {
        LOG_IF(ERROR, condition) << toLog;
    }
    
    void logger::condtionLogFatal(std::string& toLog, bool condition)
    {
        LOG_IF(FATAL, condition) << toLog;
    }
    
}