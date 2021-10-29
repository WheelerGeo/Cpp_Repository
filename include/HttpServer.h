#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__
#include "TcpServer.h"


class HttpServer: public TcpServer{
public:
    HttpServer(EventPoll* my_epoll, const int my_port);
};

#endif 