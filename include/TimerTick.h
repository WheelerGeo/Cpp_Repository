/*****************************************************************************
*  Riechan's Basic Timer library                                             *
*  @file     TimerTick.h                                                     *
*  @brief    Timer create API                                                *
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
#ifndef __TIMER_TICK_H__
#define __TIMER_TICK_H__
#include <sys/time.h>
#include "EventPoll.h"
#include "ErrorCode.h"

class TimerTick {
public:
    using EXTCALLBACK = std::function<OPERATE_RET(void* usr_data)>;
    enum TIMER_MODE {
        TIMER_FOREVER = 0,
        TIMER_ONCE
    };
    TimerTick(EventPoll* my_epoll, int ms, TIMER_MODE timer_mode);
    OPERATE_RET timerInit(void);
    static OPERATE_RET timerRun(void* usr_data, long int now_time_ms, int* time_duration);
    void addCallBack(EXTCALLBACK timcallback);
    void timerStop(void);
    void timerStart(void);
private:
    EventPoll* epoll_ = NULL;
    int set_ms_ = -1;
    long int end_time_ms_ = -1;
    EXTCALLBACK tim_callback_ = NULL;
    TIMER_MODE timer_mode_ = TIMER_FOREVER;
};


#endif 
