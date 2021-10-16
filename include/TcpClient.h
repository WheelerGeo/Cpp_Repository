/*****************************************************************************
*  Riechan's Basic TcpClient library                                         *
*  @file     TcpClient.h                                                     *
*  @brief    TcpClient connect API                                           *
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
#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__
#include <string>
#include <map>
#include <netinet/in.h>
#include "EventPoll.h"
using CLICALLBACK = std::function<int(void* usr_data, char* buff, size_t len)>;


class TcpClient{
public:
    TcpClient(EventPoll *my_epoll, int my_port, std::string my_addr);
    TcpClient(EventPoll *my_epoll, int fd);
    int establish(void);
    static int receive(void* server, int fd);
    int sendData(std::string data);
    void addCallBack(void* usr_data, CLICALLBACK callback);
    int closeConnect(void);
protected:
    char buff_[1024] = {0};
private:
    EventPoll* epoll_ = nullptr;
    int port_ = 0;
    std::string addr_ = {0};
    int connect_fd_ = -1;
    struct sockaddr_in cli_addr_ = {0};
    CLICALLBACK callback_ = nullptr;
    void* usr_data_ = nullptr;
};




#endif