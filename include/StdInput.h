#ifndef __STDINPUT_H__
#define __STDINPUT_H__
#include "EventPoll.h"

class StdInput{
public:
    StdInput(EventPoll* my_epoll);
    int stdinInit(void);
    static int receive(void* server, int fd);
private:
    int stdinfd = -1;
    char buff[1024] = {0};
    EventPoll* epoll = NULL;
};



#endif