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
#include "TcpServer.h"


TcpServer::TcpServer(EventPoll* my_epoll, const int my_port, const string my_addr) {
    epoll = my_epoll;
    port = my_port;
    addr = my_addr;
    establish();
}

int TcpServer::establish(void) {
    /* server address information */
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    int on = 1;

    /* 1: create socket */
    if (0 > (listenfd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("TCP:socket");
        return -1;
    }
    // reuse io
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    cout << "1: socket OK" << endl;

    /* 2: bind serveraddr and socket */
    if (0 > bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr))) {
        perror("TCP:bind");
        return -1;
    }
    cout << "2: bind OK" << endl;

    /* 3: set listen config */
    if (0 > listen(listenfd, 100)) {
        perror("TCP:listen");
        return -1;
    }
    cout << "3: listen OK" << endl;

    /* 4: add listen Event */
    if (0 > (epoll -> addEvent(this, listenfd, EPOLLIN | EPOLLET, this -> listenCli))) {
        perror("TCP:add listen Event");
    }
    cout << "4: add listen Event OK" << endl;

    return listenfd;
}

int TcpServer::listenCli(void* server, int fd) {
    TcpServer* Server = (TcpServer*)server;
    if (fd != Server -> listenfd) {
        return -1;
    }
    int socklen = sizeof(struct sockaddr);
    if (0 > (Server -> acceptfd = accept(Server -> listenfd, (struct sockaddr *)&(Server -> clientAddr), (socklen_t *)&socklen))) {
        return -1;
    }
    Server -> epoll -> addEvent(Server, Server -> acceptfd, EPOLLIN | EPOLLET, Server -> receive);
    printf("8: accpet OK\n");
    return 0;
}

int TcpServer::receive(void* server, int fd) {
    TcpServer* Server = (TcpServer*)server;
    if (Server -> acceptfd == fd){
        memset(Server -> buff, 0, sizeof(Server -> buff));
        recv(fd, Server -> buff, sizeof(Server -> buff), 0);
        printf("s:%s", Server -> buff);
    }
    return 0;
}

