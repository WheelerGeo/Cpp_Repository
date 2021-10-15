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
#include "../include/UdpClient.h"
#include "../include/Logger.h"




UdpClient::UdpClient(EventPoll* my_epoll, int my_port, string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    memset(&cli_addr_, 0, sizeof(struct sockaddr_in));
    cli_addr_.sin_family = AF_INET;
    cli_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());
    cli_addr_.sin_port = htons(port_);
    udpClientStart();
}

UdpClient::UdpClient(EventPoll* my_epoll, int my_fd) {
    epoll_ = my_epoll;
    sock_fd_ = my_fd;
    epoll_ -> addEvent(this, sock_fd_, EPOLLIN | EPOLLET, receive);
}

int UdpClient::udpClientStart(void) {
    if (0 > (sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0))) {
        LogError() << "UDP:socket";
        return -1;
    } 
    return 0;
}

void UdpClient::addCallBack(void* usr_data, CLICALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}


int UdpClient::receive(void* server, int fd) {
    UdpClient* Server = (UdpClient*)server;
    socklen_t len = sizeof(struct sockaddr_in);

    memset(Server->buff_, 0, sizeof(Server->buff_));
    recvfrom(fd, Server->buff_, sizeof(Server->buff_), 0, (struct sockaddr *)&(Server->ser_addr_), &len);
    Server -> callback_(Server->usr_data_, Server->buff_, sizeof(Server->buff_));
    LogInfo() << "recv from server:" << Server -> buff_;

    return 0;
}