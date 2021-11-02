#include "../include/RpcClient.h"

RpcClient::RpcClient(EventPoll* my_epoll, const int my_port, const std::string& my_addr): TcpClient(my_epoll, my_port, my_addr){
    
}

RpcClient::RpcClient(EventPoll *my_epoll, const int fd): TcpClient(my_epoll, fd) {

}
