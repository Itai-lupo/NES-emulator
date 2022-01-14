#pragma once

#include <glog/logging.h>
#include <string>

#define LAUGHTALE_ENGINR_LOG_INFO(toLog) LOG(INFO) << toLog;
#define LAUGHTALE_ENGINR_LOG_WARNING(toLog) LOG(WARNING) << toLog;
#define LAUGHTALE_ENGINR_LOG_ERROR(toLog) LOG(ERROR) << toLog;
#define LAUGHTALE_ENGINR_LOG_FATAL(toLog) LOG(FATAL) << toLog;
#define LAUGHTALE_ENGINR_CONDTION_LOG_INFO(toLog, condition) LOG_IF(INFO, condition) << toLog;
#define LAUGHTALE_ENGINR_CONDTION_LOG_WARNING(toLog, condition) LOG_IF(WARNING, condition) << toLog;
#define LAUGHTALE_ENGINR_CONDTION_LOG_ERROR(toLog, condition) LOG_IF(ERROR, condition) << toLog;
#define LAUGHTALE_ENGINR_CONDTION_LOG_FATAL(toLog, condition) LOG_IF(FATAL, condition) << toLog;


namespace LTE
{
    class logger
    { 
        public:
            static void init(const std::string& pathToLogs, const std::string& projectName);
            static void close();
    };

}

