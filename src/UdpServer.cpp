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



UdpServer::UdpServer(EventPoll* my_epoll, const int my_port, const string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    udpServerStart();
}

int UdpServer::udpServerStart(void) {
    /* server address information */
    memset(&server_addr_, 0, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());

    if (0 > (sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0))) {
        LogError() << "UDP:socket";
        return -1;
    }

    if (0 > bind(sock_fd_, (struct sockaddr *)&server_addr_, sizeof(struct sockaddr))) {
        LogError() << "UDP:bind";
        return -1;
    }

    epoll_->addEvent(this, sock_fd_, EPOLLIN | EPOLLET, receive);

    
    return 0;
}

void UdpServer::addCallBack(void* usr_data, UDPCALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
    
}

int UdpServer::receive(void* server, int fd) {
    UdpServer* Server = (UdpServer*)server;
    struct sockaddr_in usr_addr = {0};
    socklen_t len = sizeof(struct sockaddr_in);
    memset(&usr_addr, 0, sizeof(struct sockaddr_in));
    memset(Server->buff_, 0, sizeof(Server->buff_));

    recvfrom(Server->sock_fd_, Server->buff_, sizeof(Server->buff_), 0, (struct sockaddr *)&usr_addr, &len);
    
    struct in_addr in  = usr_addr.sin_addr;
    char des_ip[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, &in, des_ip, sizeof(des_ip));
    
    int des_port = ntohs(usr_addr.sin_port);

    Server->callback_(Server->usr_data_, Server->buff_, des_ip, des_port);
    LogInfo() << "recv from "  << des_ip << "/" << des_port << ": " << Server->buff_;

    return 0;
}

 int UdpServer::closeConnect(void) {
    return close(sock_fd_);
 }

 int UdpServer::sendData(string data, string des_ip, int des_port) {
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in des_addr;
    memset(&des_addr, 0, sizeof(struct sockaddr_in));
    des_addr.sin_family = AF_INET;
    des_addr.sin_port = htons(des_port);
    des_addr.sin_addr.s_addr = inet_addr(des_ip.c_str());

    return sendto(sock_fd_, data.c_str(), data.size(), 0 , (struct sockaddr *)&des_addr, len);
 }
