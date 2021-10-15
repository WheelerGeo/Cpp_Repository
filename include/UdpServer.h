/*****************************************************************************
*  Riechan's Basic UDP library                                               *
*  @file     UdpServer.h                                                     *
*  @brief    UDP connect API                                                 *
*  @author   Riechan.ye                                                      *
*  @email    Riechan.ye@outlook.com                                          *
*  @version  1.0.0                                                           *
*  @date     2021.10.15 20:00:00                                             *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/10/15 | 1.0.1     | Riechan.ye     | Create file                     *
*****************************************************************************/
#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__
#include "EventPoll.h"

using UDPCALLBACK = std::function<int(void*, char*, std::string, int)>;

class UdpServer{
public:
    UdpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr);
    int udpServerStart(void);
    void addCallBack(void* usr_data, UDPCALLBACK callback);
    static int receive(void* server, int fd);
    int closeConnect(void);
    int sendData(std::string data, std::string des_addr, int des_port);
private:
    EventPoll* epoll_ = nullptr;
    int port_ = 0;
    std::string addr_ = "";
    int sock_fd_ = 0;
    struct sockaddr_in server_addr_ = {0};
    void* usr_data_ = nullptr;
    UDPCALLBACK callback_ = nullptr;
    char buff_[1024] = {0};


};



#endif 