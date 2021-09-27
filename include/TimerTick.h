#ifndef __TIMER_TICK_H__
#define __TIMER_TICK_H__
#include <time.h>
#include "EventPoll.h"

class TimerTick {
public:
    TimerTick(EventPoll* my_epoll, int ms);
    int timerInit(void);
    static int timerRun(void* usr_data);
    void addCallback(TIMCALLBACK timcallback);
private:
    int set_ms_ = -1;
    EventPoll* epoll_ = NULL;
    long int start_time_ms_ = 0;
    long int end_time_ms_ = 0;
    TIMCALLBACK tim_callback_ = NULL;

};


#endif 
