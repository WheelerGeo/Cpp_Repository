/*****************************************************************************
*  Riechan's Basic Log library                                               *
*  @file     Logger.h                                                        *
*  @brief    Log print API                                                   *
*  @author   Riechan.ye                                                      *
*  @email    Riechan.ye@outlook.com                                          *
*  @version  1.0.0                                                           *
*  @date     2021.10.15 20:00:00                                             *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/10/15 | 1.0.1     | Riechan.ye     | Create file                     *
*****************************************************************************/
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
        FNLog::LogPriority level, int limit_size = 1024, int rollback = 4);
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