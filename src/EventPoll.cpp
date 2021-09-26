#include "EventPoll.h"



EventPoll::EventPoll(void) {
    create();
}

int EventPoll::create(void) {
    if (0 > (epollfd = epoll_create1(0))) {
        perror("epoll_create");
        return -1;
    }
    return 0;
}

int EventPoll::addEvent(void* server, int fd, int flags, CALLBACK callback) {
    struct info infor;
    epoll_event *ev = (epoll_event *)malloc(sizeof(epoll_event));
    ev->events = flags;
    ev->data.fd = fd;
    infor.usr_data = server;
    infor.callback = callback;
    map_[fd] = infor; 

    if (0 > epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, ev)) {
        perror("epoll_ctl");
        return -1;
    }
    
    return 0;
}

int EventPoll::loop() {
    int nfds = 0;
    while (1) { 
        nfds = epoll_wait(epollfd, events, EVENTS_MAX, EPOLL_TIMEOUT);
        if (0 > nfds) {
            perror("epoll_wait");
            return -1;
        }
        for (int n = 0; n < nfds; ++n) {
            auto it = map_.find(events[n].data.fd);
            if (it != map_.end()) {
                it->second.callback(it->second.usr_data, events[n].data.fd);
            }
        }
    }
    return 0;
}