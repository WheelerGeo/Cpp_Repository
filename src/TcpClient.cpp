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

TcpClient::TcpClient(EventPoll *my_epoll, int my_port, string my_addr) {
    epoll = my_epoll;
    port = my_port;
    addr = my_addr;
}

int TcpClient::establish(void) {
    if (0 > (connect_fd_ = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket");
        return -1;
    }
    memset(&cli_addr_, 0, sizeof(cli_addr_));
    cli_addr_.sin_family = AF_INET;
    cli_addr_.sin_addr.s_addr = inet_addr(addr.c_str());
    cli_addr_.sin_port = htons(port);

    if (0 > connect(connect_fd_, (sockaddr *)&cli_addr_, sizeof(cli_addr_))) {
        perror("connect");
        return -1;
    }

    return 0;
}