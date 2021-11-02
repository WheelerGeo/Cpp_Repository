#include <iostream>
#include "../include/TimerTick.h"
#include "../include/Logger.h"

TimerTick::TimerTick(EventPoll* my_epoll, int ms, TIMER_MODE timer_mode) {
    epoll_ = my_epoll;
    set_ms_ = ms;
    timer_mode_ = timer_mode;
    timerInit();
}

OPERATE_RET TimerTick::timerInit(void) {
    struct timeval start = {0};
    if (0 > gettimeofday(&start, NULL)) {
        LogError() << "TimerTick:gettimeofday";
        return OPRT_TIME_GET_ERROR;
    }
    end_time_ms_ = start.tv_sec * 1000 + start.tv_usec / 1000 + set_ms_;
    epoll_->addTimer(this, timerRun);
    return OPRT_OK;
}

OPERATE_RET TimerTick::timerRun(void* usr_data, const long int now_time_ms, int* time_duration) {
    TimerTick* timertick = (TimerTick*)usr_data;
    *time_duration = timertick->end_time_ms_ - now_time_ms;
    LogTrace() << "addr: " << usr_data << " end: " << timertick->end_time_ms_ << " now: " << now_time_ms << " diff: " << *time_duration;
    if (*time_duration <= 0) {
        timertick->tim_callback_(usr_data);
        if(TIMER_ONCE == timertick->timer_mode_) {
            timertick->epoll_->addTimer(timertick, NULL);
        } else {
            timertick->end_time_ms_ += timertick->set_ms_;
            *time_duration = timertick->end_time_ms_ - now_time_ms;
        }
    }

    return OPRT_OK;
}

void TimerTick::addCallBack(EXTCALLBACK timcallback) {
    tim_callback_ = timcallback;
}

void TimerTick::timerStop(void) {
    epoll_->addTimer(this, NULL);
}

void TimerTick::timerStart(void) {
    timerInit();
}