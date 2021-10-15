#include <sys/types.h> 
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <memory>
#include <utility>
#include "../include/TcpServer.h"
#include "../include/Logger.h"

TcpServer::TcpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    establish();
}

int TcpServer::establish(void) {
    /* server address information */
    memset(&server_addr_, 0, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());
    int on = 1;

    if (0 > (listen_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
        LogError() << "TCP:socket";
        return -1;
    }
    // reuse io
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    LogInfo() << "TCP:1: socket OK";

    if (0 > bind(listen_fd_, (struct sockaddr *)&server_addr_, sizeof(struct sockaddr))) {
        LogError() << "TCP:bind";
        return -1;
    }
    LogInfo() << "TCP:2: bind OK";

    if (0 > listen(listen_fd_, 100)) {
        LogError() << "TCP:listen";
        return -1;
    }
    LogInfo() << "TCP:3: listen OK";

    if (0 > (epoll_ -> addEvent(this, listen_fd_, EPOLLIN | EPOLLET, this->listenCli))) {
        LogError() << "TCP:add listen Even";
        return -1;
    }
    LogInfo() << "TCP:4: add listen Event OK";

    return listen_fd_;
}

int TcpServer::listenCli(void* server, int fd) {
    TcpServer* Server = (TcpServer*)server;
    if (fd != Server->listen_fd_) {
        return -1;
    }

    int socklen = sizeof(struct sockaddr);
    if (0 > (Server->connet_fd_ = accept(Server->listen_fd_, 
                                         (struct sockaddr *)&(Server->client_addr_), 
                                         (socklen_t *)&socklen))) {
        return -1;
    }
    LogInfo() << "TCP:accept:" << Server->connet_fd_;
    Server -> callback_(Server->usr_data_, Server->connet_fd_);

    return 0;
}

void TcpServer::addCallBack(void* usr_data, SERCALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}
