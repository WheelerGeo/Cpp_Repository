#ifndef __TIMER_TICK_H__
#define __TIMER_TICK_H__
#include <sys/time.h>
#include "EventPoll.h"

class TimerTick {
public:
    enum TIMER_MODE {
        TIMER_FOREVER = 0,
        TIMER_ONCE
    };
    TimerTick(EventPoll* my_epoll, int ms, TIMER_MODE timer_mode);
    int timerInit(void);
    static int timerRun(void* usr_data, long int now_time_ms);
    void addCallback(TIMCALLBACK timcallback);
    void timerStop(void);
    void timerStart(void);
private:
    EventPoll* epoll_ = NULL;
    int set_ms_ = -1;
    long int end_time_ms_ = -1;
    TIMCALLBACK tim_callback_ = NULL;
    TIMER_MODE timer_mode_ = TIMER_FOREVER;
};


#endif 
