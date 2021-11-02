#include "../include/RpcServer.h"

RpcServer::RpcServer(EventPoll* my_epoll, const int my_port): TcpServer(my_epoll, my_port){
    
}
