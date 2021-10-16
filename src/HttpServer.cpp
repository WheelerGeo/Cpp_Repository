#include "../include/HttpServer.h"
#include "../include/NetworkTool.h"
#include "../include/Logger.h"

HttpServer::HttpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr): TcpServer(my_epoll, my_port, my_addr){
    
}

HttpServer::HttpServer(EventPoll* my_epoll, const int my_port): TcpServer(my_epoll, my_port){
}