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
#include "TcpClient.h"
#include "StdInput.h"

TcpClient::TcpClient(EventPoll *my_epoll, int my_port, string my_addr) {
    epoll_ = my_epoll;
    port_ = my_port;
    addr_ = my_addr;
    establish();
}

TcpClient::TcpClient(EventPoll *my_epoll, int fd) {
    epoll_ = my_epoll;
    connect_fd_ = fd;
    if (0 > (epoll_ -> addEvent(this, connect_fd_, EPOLLIN | EPOLLET, receive))) {
        perror("TCP:add receive Event");
    }
}

int TcpClient::establish(void) {
    if (0 > (connect_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket");
        return -1;
    }
    cout << "1: socket OK" << endl;

    memset(&cli_addr_, 0, sizeof(cli_addr_));
    cli_addr_.sin_family = AF_INET;
    cli_addr_.sin_addr.s_addr = inet_addr(addr_.c_str());
    cli_addr_.sin_port = htons(port_);
    if (0 > connect(connect_fd_, (sockaddr *)&cli_addr_, sizeof(cli_addr_))) {
        perror("connect");
        return -1;
    }
    cout << "2: connect OK" << endl;

    if (0 > (epoll_ -> addEvent(this, connect_fd_, EPOLLIN | EPOLLET, receive))) {
        perror("TCP:add receive Event");
    }
    cout << "3: add receive Event OK" << endl;


    return 0;
}

int TcpClient::receive(void* server, int fd) {
    TcpClient* Server = (TcpClient*)server;

    memset(Server -> buff_, 0, sizeof(Server -> buff_));
    recv(fd, Server -> buff_, sizeof(Server -> buff_), 0);
    Server -> callback_(Server -> usr_data_, Server -> buff_, sizeof(Server -> buff_));
    printf("recv from server:%s", Server -> buff_);
    return 0;
}

int TcpClient::sendData(string data) {
    return send(connect_fd_, data.c_str(), data.size(), 0);
}

void TcpClient::addRecv(void* usr_data, CLICALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}

int TcpClient::closeConnet(void) {
    return close(connect_fd_);
}
