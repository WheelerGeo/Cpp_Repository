#ifndef __RPCCLIENT_H__
#define __RPCCLIENT_H__
#include "TcpClient.h"

class RpcClient: public TcpClient {
public:
    RpcClient(EventPoll* my_epoll, const int my_port, const std::string& my_addr);
    RpcClient(EventPoll* my_epoll, const int fd);


};





#endif