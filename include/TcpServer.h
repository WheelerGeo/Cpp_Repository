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
#include "ErrorCode.h"


// create TCP server class
class TcpServer{
public:
    TcpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr);
    TcpServer(EventPoll* my_epoll, const int my_port);
    OPERATE_RET establish(void);
    static OPERATE_RET listenCli(void* server, int fd);
    virtual OPERATE_RET listenCallBack(int fd);
    
protected:
    EventPoll* epoll_ = nullptr;
    int port_ = 0;
private:
    std::string addr_ = "";
    int listen_fd_ = -1;
    int connet_fd_ = -1;
    struct sockaddr_in server_addr_ = {0};
    struct sockaddr_in client_addr_ = {0};
    char buff_[1024] = {0};
    void* usr_data_ = nullptr;
};




#endif