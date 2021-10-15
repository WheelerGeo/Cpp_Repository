#ifndef __UDPCLIENT_H__
#define __UDPCLIENT_H__
#include "../include/EventPoll.h"

using CLICALLBACK = function<int(void* usr_data, char* buff, size_t len)>;

class UdpClient{
public:
    UdpClient(EventPoll* my_epoll, int my_port, string my_addr);
    UdpClient(EventPoll* my_epoll, int my_fd);
    int udpClientStart(void);
    void addCallBack(void* usr_data, CLICALLBACK callback);
    static int receive(void* server, int fd);
private:
    EventPoll* epoll_ = nullptr;
    struct sockaddr_in cli_addr_ = {0};
    struct sockaddr_in ser_addr_ = {0};
    int port_ = 0;
    string addr_ = "";
    int sock_fd_ = 0;
    char buff_[1024] = {0};
    void* usr_data_ = nullptr;
    CLICALLBACK callback_ = nullptr;
};


#endif 