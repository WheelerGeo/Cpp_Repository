#include <iostream>
#include "../include/EventPoll.h"
#include "../include/Logger.h"


EventPoll::EventPoll(void) {
    create();
}

int EventPoll::create(void) {
    if (0 > (epoll_fd_ = epoll_create1(0))) {
        perror("epoll_create");
        return -1;
    }
    return 0;
}

int EventPoll::addEvent(void* usr_data, int fd, int flags, CALLBACK callback) {
    struct info infor = {
        .callback_ = callback,
        .usr_data_ = usr_data
    };
    epoll_event *ev = (epoll_event *)malloc(sizeof(epoll_event));
    ev->events = flags;
    ev->data.fd = fd;
    
    map_[fd] = infor; 

    if (0 > epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, ev)) {
        perror("epoll_ctl");
        return -1;
    }
    
    return 0;
}

void EventPoll::addTimer(void* usr_data, TIMCALLBACK tim_callback) {
    struct info infor = {
        .tim_callback_ = tim_callback,
        .usr_data_ = usr_data
    };
    vector_.push_back(infor);
}


int EventPoll::loop() {
    int nfds = 0;
    struct timeval now = {0};
    while (1) {
        epoll_time_out_ = EPOLL_TIMEOUT;
        gettimeofday(&now, NULL);
        now_time_ms_ = now.tv_sec * 1000 + now.tv_usec / 1000;
        for(auto it : vector_) {
            if(it.tim_callback_) {
                int time_duration = it.tim_callback_(it.usr_data_, now_time_ms_);
                if(time_duration < epoll_time_out_ && time_duration > 0) {
                    epoll_time_out_ =  time_duration - 1;
                }
            }
        }

        nfds = epoll_wait(epoll_fd_, events_, EVENTS_MAX, epoll_time_out_);
        LogDebug() << "time_out: " << epoll_time_out_;

        
        
        if (0 > nfds) {
            perror("epoll_wait");
            return -1;
        }
        
        for (int n = 0; n < nfds; ++n) {
            auto it = map_.find(events_[n].data.fd);
            if (it != map_.end()) {
                it->second.callback_(it->second.usr_data_, events_[n].data.fd);
            }
        }
    }
    return 0;
}