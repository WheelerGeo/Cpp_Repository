#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include <string.h>
#include "StdInput.h"


StdInput::StdInput(EventPoll* my_epoll) {
    epoll_ = my_epoll;
    stdinInit();
}

int StdInput::stdinInit(void) {
    if (0 > (epoll_ -> addEvent(this, STDIN_FILENO, EPOLLIN | EPOLLET, receive))) {
        perror("stdin init");
        return -1;
    }
    return 0;
}


int StdInput::receive(void* server, int fd) {
    StdInput* Server = (StdInput*)server;
    memset(Server -> buff_, 0, sizeof(Server -> buff_));
    read(fd, Server -> buff_, sizeof(Server -> buff_));
    Server -> callback_(Server -> usr_data_, Server -> buff_, sizeof(Server -> buff_));
    std::cout << "input:" << Server -> buff_;
    return 0;
}

void StdInput::addRecv(void* usr_data, STDCALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}

