#include "../include/HttpServer.h"
#include "../include/NetworkTool.h"
#include "../include/Logger.h"

HttpServer::HttpServer(EventPoll* my_epoll, const int my_port): TcpServer(my_epoll, my_port){
}