#include <iostream>
#include "../include/TimerTick.h"
#include "../include/Logger.h"

TimerTick::TimerTick(EventPoll* my_epoll, int ms, TIMER_MODE timer_mode) {
    epoll_ = my_epoll;
    set_ms_ = ms;
    timer_mode_ = timer_mode;
    timerInit();
}

int TimerTick::timerInit(void) {
    struct timeval start = {0};
    if (0 > gettimeofday(&start, NULL)) {
        LogError() << "TimerTick:gettimeofday";
        return -1;
    }
    end_time_ms_ = start.tv_sec * 1000 + start.tv_usec / 1000 + set_ms_;
    epoll_->addTimer(this, timerRun);
    return 0;
}

int TimerTick::timerRun(void* usr_data, long int now_time_ms) {
    TimerTick* timertick = (TimerTick*)usr_data;
    int time_duration = timertick->end_time_ms_ - now_time_ms;
    LogDebug() << "addr: " << usr_data << " end: " << timertick->end_time_ms_ << " now: " << now_time_ms << " diff: " << time_duration;
    if (time_duration <= 0) {
        timertick->tim_callback_(usr_data, now_time_ms);
        if(TIMER_ONCE == timertick->timer_mode_) {
            timertick->epoll_->addTimer(timertick, NULL);
        } else {
            timertick->end_time_ms_ += timertick->set_ms_;
            time_duration = timertick->end_time_ms_ - now_time_ms;
        }
    }

    return time_duration;
}

void TimerTick::addCallBack(TIMCALLBACK timcallback) {
    tim_callback_ = timcallback;
}

void TimerTick::timerStop(void) {
    epoll_->addTimer(this, NULL);
}

void TimerTick::timerStart(void) {
    timerInit();
}