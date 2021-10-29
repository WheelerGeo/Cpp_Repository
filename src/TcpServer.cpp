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

TcpServer::TcpServer(EventPoll* my_epoll, const int my_port) {
    epoll_ = my_epoll;
    port_ = my_port;
    establish();
}

OPERATE_RET TcpServer::establish(void) {
    struct sockaddr_in server_addr_ = {0};
    /* server address information */
    memset(&server_addr_, 0, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
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
    struct sockaddr_in client_addr_ = {0};
    int connet_fd_ = -1;
    TcpServer* Server = (TcpServer*)server;
    if (fd != Server->listen_fd_) {
        return OPRT_INVALID_PARM;
    }

    int socklen = sizeof(struct sockaddr);
    if (0 > (connet_fd_ = accept(Server->listen_fd_, 
                                    (struct sockaddr *)&(client_addr_), 
                                    (socklen_t *)&socklen))) {
        return OPRT_SOCK_ACCEPT_ERROR;
    }
    LogInfo() << "TcpServer:accept:" << connet_fd_;
    Server->listenCallBack(connet_fd_);

    return OPRT_OK;
}

OPERATE_RET TcpServer::listenCallBack(int fd) {
    return OPRT_OK;
}