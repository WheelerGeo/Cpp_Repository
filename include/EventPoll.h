#ifndef __EVENTPOLL_H__
#define __EVENTPOLL_H__
#include <functional>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <sys/time.h>
#define EVENTS_MAX      10
#define EPOLL_TIMEOUT   1000
using namespace std;
using CALLBACK = function<int(void* usr_data, int fd)>;
using TIMCALLBACK = function<int(void* usr_data, long int now_time_ms)>;


class EventPoll{
public:
    EventPoll(void);
    int create(void);
    int addEvent(void* usr_data, int fd, int flags, CALLBACK callback);
    void addTimer(void* usr_data, TIMCALLBACK tim_callback);
    int loop();
private:
    int epoll_fd_ = -1;
    int events_cnt = 0;
    struct info {
        TIMCALLBACK tim_callback_ = NULL;
        CALLBACK callback_ = NULL;
        void* usr_data_;
    };
    map<int, struct info> map_;
    vector<struct info> vector_;
    epoll_event events_[EVENTS_MAX] ={0};
    // TIMCALLBACK tim_callback_ = NULL;
    // void* usr_data_;
    int epoll_time_out_ = EPOLL_TIMEOUT;
    long int now_time_ms_ = 0;
};



#endif 