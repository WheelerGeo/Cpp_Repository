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
#include "../include/TcpClient.h"
#include "../include/StdInput.h"
#include "../include/Logger.h"

TcpClient::TcpClient(EventPoll *my_epoll, int my_port, std::string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    establish();
}

TcpClient::TcpClient(EventPoll *my_epoll, int fd) {
    epoll_ = my_epoll;
    connect_fd_ = fd;
    if (0 > (epoll_->addEvent(this, connect_fd_, EPOLLIN | EPOLLET, receive))) {
        LogError() << "TcpClient:addEvent";
    }
}

OPERATE_RET TcpClient::establish(void) {
    if (0 > (connect_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
        LogError() << "TcpClient:socket";
        return OPRT_SOCK_CREATE_ERROR;
    }
    LogInfo() << "TcpClient:1: socket OK";

    memset(&cli_addr_, 0, sizeof(cli_addr_));
    cli_addr_.sin_family = AF_INET;
    cli_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());
    cli_addr_.sin_port = htons(port_);
    if (0 > connect(connect_fd_, (sockaddr *)&cli_addr_, sizeof(cli_addr_))) {
        LogError() << "TcpClient:connect";
        return OPRT_SOCK_CONNECT_ERROR;
    }
    LogInfo() << "TcpClient:2: connect OK";

    if (0 > (epoll_->addEvent(this, connect_fd_, EPOLLIN | EPOLLET, receive))) {
        LogError() << "TcpClient:addEvent";
        return OPRT_EPOLL_ADDEVENT_ERROR;
    }
    LogInfo() << "TcpClient:3: add receive Event OK";

    return OPRT_OK;
}

OPERATE_RET TcpClient::receive(void* server, int fd) {
    int buff_size = 0;
    char buff[1024] = "";
    TcpClient* Server = (TcpClient*)server;

    Server->buff_.clear();
    buff_size = recv(fd, buff, sizeof(buff), 0);
    if (0 > buff_size) {
        LogError() << "TcpClient:receive";
        return OPRT_SOCK_RECV_ERROR;
    }
    Server->buff_ = std::string(buff, buff_size);
    LogDebug() << "recv from server[" << buff_size << "]:" << Server->buff_;
    Server->recvCallBack(Server->usr_data_, Server->buff_);
    
    return OPRT_OK;
}

OPERATE_RET TcpClient::sendData(std::string data) {
    if (0 > send(connect_fd_, data.c_str(), data.size(), 0)) {
        LogError() << "TcpClient:send";
        return OPRT_SOCK_SEND_ERROR;
    }
    return OPRT_OK;
}

OPERATE_RET TcpClient::closeConnect(void) {
    if (0 > close(connect_fd_)) {
        LogError() << "TcpClient:close";
        return OPRT_SOCK_CLOSE_ERROR;
    }
    LogInfo() << "TcpClient:close fd:" << connect_fd_;
    return OPRT_OK;
}

OPERATE_RET TcpClient::addUsrData(void* usr_data) {
    usr_data = usr_data;
    return OPRT_OK;
}

OPERATE_RET TcpClient::recvCallBack(void* usr_data, const std::string& recv_buff) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    if (0 == recv_buff.size()) {
        cli_usr->closeConnect();
    } else {
        LogDebug() << recv_buff;
    }
    return OPRT_OK;
}