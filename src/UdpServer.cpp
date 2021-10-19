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
#include "../include/UdpServer.h"
#include "../include/Logger.h"
#include "../include/NetworkTool.h"


UdpServer::UdpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    udpServerStart();
}

UdpServer::UdpServer(EventPoll* my_epoll, const int my_port) {
    epoll_ = my_epoll;
    port_ = my_port;
    char ip[16];
    NetworkTool::GetLocalIp("ens33", ip);
    addr_ = ip;
    LogInfo() << "GetLocalIp:" << addr_;
    udpServerStart();
}

OPERATE_RET UdpServer::udpServerStart(void) {
    /* server address information */
    memset(&server_addr_, 0, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());

    if (0 > (sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0))) {
        LogError() << "UDP:socket";
        return OPRT_SOCK_CREATE_ERROR;
    }
    LogInfo() << "UDP:1:socket";

    if (0 > bind(sock_fd_, (struct sockaddr *)&server_addr_, sizeof(struct sockaddr))) {
        LogError() << "UDP:bind";
        return OPRT_SOCK_BIND_ERROR;
    }
    LogInfo() << "UDP:2:bind";

    if (0 > epoll_->addEvent(this, sock_fd_, EPOLLIN | EPOLLET, receive)) {
        LogError() << "UDP:addEvent";
        return OPRT_EPOLL_ADDEVENT_ERROR;
    }
    
    return OPRT_OK;
}

void UdpServer::addCallBack(void* usr_data, EXTCALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}

OPERATE_RET UdpServer::receive(void* server, int fd) {
    UdpServer* Server = (UdpServer*)server;
    struct sockaddr_in usr_addr = {0};
    socklen_t len = sizeof(struct sockaddr_in);

    memset(&usr_addr, 0, sizeof(struct sockaddr_in));
    memset(Server->buff_, 0, sizeof(Server->buff_));

    if (0 > recvfrom(Server->sock_fd_, Server->buff_, sizeof(Server->buff_), 0, (struct sockaddr *)&usr_addr, &len)) {
        LogError() << "UDP:recvfrom";
        return OPRT_SOCK_RECVFROM_ERROR;
    }
    
    struct in_addr in  = usr_addr.sin_addr;
    char des_ip[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, &in, des_ip, sizeof(des_ip));
    
    int des_port = ntohs(usr_addr.sin_port);

    Server->callback_(Server->usr_data_, Server->buff_, des_ip, des_port);
    LogInfo() << "recv from "  << des_ip << "/" << des_port << ": " << Server->buff_;

    return OPRT_OK;
}

OPERATE_RET UdpServer::closeConnect(void) {
    if (0 > close(sock_fd_)) {
        LogError() << "UDP:close";
        return OPRT_SOCK_CLOSE_ERROR;
    }
    return OPRT_OK;
 }

 OPERATE_RET UdpServer::sendData(std::string data, std::string des_ip, int des_port) {
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in des_addr;
    memset(&des_addr, 0, sizeof(struct sockaddr_in));
    des_addr.sin_family = AF_INET;
    des_addr.sin_port = htons(des_port);
    des_addr.sin_addr.s_addr = inet_addr(des_ip.c_str());

    if (0 > (sendto(sock_fd_, data.c_str(), data.size(), 0 , (struct sockaddr *)&des_addr, len))) {
        LogError() << "UDP:sendto";
        return OPRT_SOCK_SENDTO_ERROR;
    }
    return OPRT_OK;
}
