#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__
#include <string>

#include "EventPoll.h"

class TcpClient{
public:
    TcpClient(EventPoll *my_epoll, int my_port, string my_addr);
    int establish(void);
private:
    EventPoll* epoll = NULL;
    int port = 0;
    string addr = {0};
    int connect_fd_ = -1;
    struct sockaddr_in cli_addr_ = {0};
};




#endif