#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__
#include <string>
#include <map>
#include "EventPoll.h"
using CLICALLBACK = function<int(void* usr_data, char* buff, size_t len)>;


class TcpClient{
public:
    TcpClient(EventPoll *my_epoll, int my_port, string my_addr);
    TcpClient(EventPoll *my_epoll, int fd);
    int establish(void);
    static int receive(void* server, int fd);
    int sendData(string data);
    void addRecv(void* usr_data, CLICALLBACK callback);
    int closeConnet(void);
private:
    EventPoll* epoll_ = NULL;
    int port_ = 0;
    string addr_ = {0};
    int connect_fd_ = -1;
    struct sockaddr_in cli_addr_ = {0};
    char buff_[1024] = {0};
    CLICALLBACK callback_ = NULL;
    void* usr_data_ = NULL;
};




#endif