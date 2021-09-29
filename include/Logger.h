#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "fn_log.h"

class Logger{
public:
    Logger();
    int loggerInit(void);
    int loggerStart(void);
    static Logger& getInstance(void);
    void setLoggerFromConfig(const std::string &config_path);
    int setLoggerSync(void);
    int addLoggerToFile(const std::string& fpath, const std::string& fname, 
        FNLog::LogPriority level, int limit_size = 1000, int rollback = 4);
    int addLoggerToScreen(FNLog::LogPriority level);
    int addLoggerToUdp(const std::string& ip, int port, FNLog::LogPriority level);
protected:
    ~Logger(){};

private:
    FNLog::LogPriority level_;
    int device_id_ = 0;
    std::string config_path_ = "";
};



#endif