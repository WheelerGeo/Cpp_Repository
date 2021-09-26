#ifndef __EVENTPOLL_H__
#define __EVENTPOLL_H__
#include <functional>
#include <sys/epoll.h>
#include <map>

#define EVENTS_MAX      10
#define EPOLL_TIMEOUT   100
using namespace std;
using CALLBACK = function<int(void* server, int fd)>;

class EventPoll{
public:
    EventPoll(void);
    int create(void);
    int addEvent(void* server, int fd, int flags, CALLBACK callback);
    int loop();
private:
    CALLBACK listen_cb = NULL;
    CALLBACK accept_cb = NULL;
    int epollfd = -1;
    int events_cnt = 0;
    struct info {
        CALLBACK callback = NULL;
        void* usr_data;
    };
    map<int, struct info> map_;
    epoll_event events[EVENTS_MAX] ={0};
};



#endif 