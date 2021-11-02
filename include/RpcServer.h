#ifndef __RPCSERVER_H__
#define __RPCSERVER_H__
#include "TcpServer.h"

class RpcServer: public TcpServer {
public:
    RpcServer(EventPoll* my_epoll, const int my_port);


};





#endif