#include <iostream>
#include "TimerTick.h"


TimerTick::TimerTick(EventPoll* my_epoll, int ms) {
    epoll_ = my_epoll;
    set_ms_ = ms;
    timerInit();
}

int TimerTick::timerInit(void) {
    struct timespec start = {0};
    if(0 > clock_gettime(CLOCK_REALTIME_COARSE, &start)) {
        perror("TimerInit");
        return -1;
    }
    start_time_ms_ = start.tv_sec * 1000 + start.tv_nsec / (1000 * 1000);
    end_time_ms_ = start_time_ms_ + set_ms_;
    epoll_ -> addTimer(this, timerRun);
    return 0;
}

int TimerTick::timerRun(void* usr_data) {
    TimerTick* timertick = (TimerTick*)usr_data;
    struct timespec now = {0};
    long int now_time_ms = 0;
    clock_gettime(CLOCK_REALTIME_COARSE, &now);
    now_time_ms = now.tv_sec * 1000 + now.tv_nsec / (1000 * 1000);
    if (timertick -> end_time_ms_ > now_time_ms) {
        return timertick -> end_time_ms_ - now_time_ms;
    } else {
        timertick -> tim_callback_(usr_data);
        cout << "now time ms:" << now_time_ms << endl;
        timertick -> end_time_ms_ += timertick -> set_ms_;
    }

    return timertick -> end_time_ms_ - now_time_ms;
}

void TimerTick::addCallback(TIMCALLBACK timcallback) {
    tim_callback_ = timcallback;
}