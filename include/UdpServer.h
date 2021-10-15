#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__
#include "EventPoll.h"

using UDPCALLBACK = function<int(void*, char*, string, int)>;

class UdpServer{
public:
    UdpServer(EventPoll* my_epoll, const int my_port, const string my_addr);
    int udpServerStart(void);
    void addCallBack(void* usr_data, UDPCALLBACK callback);
    static int receive(void* server, int fd);
    int closeConnect(void);
    int sendData(string data, string des_addr, int des_port);
private:
    EventPoll* epoll_ = nullptr;
    int port_ = 0;
    string addr_ = "";
    int sock_fd_ = 0;
    struct sockaddr_in server_addr_ = {0};
    void* usr_data_ = nullptr;
    UDPCALLBACK callback_ = nullptr;
    char buff_[1024] = {0};


};



#endif 