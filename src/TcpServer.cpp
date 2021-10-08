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


TcpServer::TcpServer(EventPoll* my_epoll, const int my_port, const string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    establish();
}

int TcpServer::establish(void) {
    /* server address information */
    memset(&serverAddr_, 0, sizeof(struct sockaddr));
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(port_);
    serverAddr_.sin_addr.s_addr = inet_addr(addr_.c_str());
    int on = 1;

    if (0 > (listen_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("TCP:socket");
        return -1;
    }
    // reuse io
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    cout << "1: socket OK" << endl;

    if (0 > bind(listen_fd_, (struct sockaddr *)&serverAddr_, sizeof(struct sockaddr))) {
        perror("TCP:bind");
        return -1;
    }
    cout << "2: bind OK" << endl;

    if (0 > listen(listen_fd_, 100)) {
        perror("TCP:listen");
        return -1;
    }
    cout << "3: listen OK" << endl;

    if (0 > (epoll_ -> addEvent(this, listen_fd_, EPOLLIN | EPOLLET, this -> listenCli))) {
        perror("TCP:add listen Event");
    }
    cout << "4: add listen Event OK" << endl;

    return listen_fd_;
}

int TcpServer::listenCli(void* server, int fd) {
    TcpServer* Server = (TcpServer*)server;
    if (fd != Server -> listen_fd_) {
        return -1;
    }

    int socklen = sizeof(struct sockaddr);
    if (0 > (Server -> connet_fd_ = accept(Server -> listen_fd_, (struct sockaddr *)&(Server -> clientAddr_), (socklen_t *)&socklen))) {
        return -1;
    }
    cout << "accept" <<  Server -> connet_fd_ << endl;
    Server -> callback_(Server -> usr_data_, Server -> connet_fd_);

    return 0;
}

void TcpServer::addConnect(void* usr_data, SERCALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}
