/*****************************************************************************
*  Riechan's Basic TCP library                                               *
*  @file     tcp.h                                                           *
*  @brief    TCP connect API                                                 *
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
*                                                                            *
*****************************************************************************/
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include <string>
#include <netinet/in.h>
#include <functional>
#include <map>
#include "EventPoll.h"

using namespace std;


// create TCP server class
class TcpServer{
public:
    TcpServer(EventPoll* my_epoll, const int my_port, const string my_addr);
    int establish(void);
    static int listenCli(void* server, int fd);
    static int receive(void* server, int fd);
private:
    int port = 0;
    string addr = "";
    int listenfd = -1;
    int acceptfd = -1;
    struct sockaddr_in serverAddr = {0};
    struct sockaddr_in clientAddr = {0};
    char buff[1024] = {0};
    EventPoll* epoll = NULL;
};




#endif