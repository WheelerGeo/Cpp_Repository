#ifndef __STDINPUT_H__
#define __STDINPUT_H__
#include "EventPoll.h"
using STDCALLBACK = function<int(void* usr_data, char* buff, size_t len)>;


class StdInput{
public:
    StdInput(EventPoll* my_epoll);
    int stdinInit(void);
    static int receive(void* server, int fd);
    void addRecv(void* usr_data, STDCALLBACK callback);
private:
    EventPoll* epoll_ = NULL;
    char buff_[1024] = {0};
    void* usr_data_ = NULL;
    STDCALLBACK callback_ = NULL;
};



#endif