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
#include "../include/NetworkTool.h"

TcpServer::TcpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    establish();
}

TcpServer::TcpServer(EventPoll* my_epoll, const int my_port) {
    epoll_ = my_epoll;
    port_ = my_port;
    char ip[16];
    NetworkTool::GetLocalIp("ens33", ip);
    addr_ = ip;
    LogInfo() << "GetLocalIp:" << addr_;
    establish();
}

OPERATE_RET TcpServer::establish(void) {
    /* server address information */
    memset(&server_addr_, 0, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());
    int on = 1;

    if (0 > (listen_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
        LogError() << "TcpServer:socket";
        return OPRT_SOCK_CREATE_ERROR;
    }
    // reuse io
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    LogInfo() << "TcpServer:1: socket OK";

    if (0 > bind(listen_fd_, (struct sockaddr *)&server_addr_, sizeof(struct sockaddr))) {
        LogError() << "TcpServer:bind";
        return OPRT_SOCK_BIND_ERROR;
    }
    LogInfo() << "TcpServer:2: bind OK";

    if (0 > listen(listen_fd_, 100)) {
        LogError() << "TcpServer:listen";
        return OPRT_SOCK_LISTEN_ERROR;
    }
    LogInfo() << "TcpServer:3: listen OK";

    if (0 > (epoll_->addEvent(this, listen_fd_, EPOLLIN | EPOLLET, this->listenCli))) {
        LogError() << "TcpServer:addEvent";
        return OPRT_EPOLL_ADDEVENT_ERROR;
    }
    LogInfo() << "TcpServer:4: add listen Event OK";

    return OPRT_OK;
}

OPERATE_RET TcpServer::listenCli(void* server, int fd) {
    TcpServer* Server = (TcpServer*)server;
    if (fd != Server->listen_fd_) {
        return OPRT_INVALID_PARM;
    }

    int socklen = sizeof(struct sockaddr);
    if (0 > (Server->connet_fd_ = accept(Server->listen_fd_, 
                                         (struct sockaddr *)&(Server->client_addr_), 
                                         (socklen_t *)&socklen))) {
        return OPRT_SOCK_ACCEPT_ERROR;
    }
    LogInfo() << "TcpServer:accept:" << Server->connet_fd_;
    Server->listenCallBack(Server->connet_fd_);

    return OPRT_OK;
}

OPERATE_RET TcpServer::listenCallBack(int fd) {
    return OPRT_OK;
}