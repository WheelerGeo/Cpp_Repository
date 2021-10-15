/*****************************************************************************
*  Riechan's Basic TCPServer library                                         *
*  @file     TcpServer.h                                                     *
*  @brief    TCPServer connect API                                           *
*  @author   Riechan.ye                                                      *
*  @email    Riechan.ye@outlook.com                                          *
*  @version  1.0.0                                                           *
*  @date     2021.9.23 16:00:00                                              *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/09/23 | 1.0.0     | Riechan.ye     | Create file                     *
*----------------------------------------------------------------------------*
*  2021/10/15 | 1.0.1     | Riechan.ye     | delete using namspace std       *
*****************************************************************************/
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include <string>
#include <netinet/in.h>
#include <functional>
#include <map>
#include "EventPoll.h"
#include "TcpClient.h"
using SERCALLBACK = std::function<int(void* usr_data, int fd)>;

// create TCP server class
class TcpServer{
public:
    TcpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr);
    int establish(void);
    static int listenCli(void* server, int fd);
    void addCallBack(void* usr_data, SERCALLBACK callback);
private:
    EventPoll* epoll_ = nullptr;
    int port_ = 0;
    std::string addr_ = "";
    int listen_fd_ = -1;
    int connet_fd_ = -1;
    struct sockaddr_in server_addr_ = {0};
    struct sockaddr_in client_addr_ = {0};
    char buff_[1024] = {0};
    SERCALLBACK callback_ = nullptr;
    void* usr_data_ = nullptr;
    
};




#endif