#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include <string.h>
#include "StdInput.h"


StdInput::StdInput(EventPoll* my_epoll) {
    epoll = my_epoll;
    stdinInit();
}

int StdInput::receive(void* server, int fd) {
    StdInput* Server = (StdInput*)server;
    memset(Server -> buff, 0, sizeof(Server -> buff));
    read(fd, Server -> buff, sizeof(Server -> buff));
    std::cout << "input:" << Server -> buff;
    return 0;
}

int StdInput::stdinInit(void) {
    if (0 > (epoll -> addEvent(this, STDIN_FILENO, EPOLLIN | EPOLLET, receive))) {
        perror("stdin init");
        return -1;
    }
    return 0;
}