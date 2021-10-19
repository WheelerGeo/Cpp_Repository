#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include <string.h>
#include "../include/StdInput.h"
#include "../include/Logger.h"

StdInput::StdInput(EventPoll* my_epoll) {
    epoll_ = my_epoll;
    stdinInit();
}

OPERATE_RET StdInput::stdinInit(void) {
    if (0 > (epoll_->addEvent(this, STDIN_FILENO, EPOLLIN | EPOLLET, receive))) {
        LogError() << "StdInput:addEvent";
        return OPRT_EPOLL_ADDEVENT_ERROR;
    }
    return OPRT_OK;
}

/* 存在读标准输入大于1025的bug */
OPERATE_RET StdInput::receive(void* server, int fd) {
    StdInput* Server = (StdInput*)server;
    char buff[1024] = "";
    int buff_size = 0;
    Server->buff_.clear();
    if (0 > (buff_size = read(fd, buff, sizeof(buff)))) {
        LogError() << "StdInput:receive";
        return OPRT_FILE_READ_ERROR;
    }
    Server->buff_ = std::string(buff, buff_size);
   
    Server->callback_(Server->usr_data_, Server->buff_);
    LogInfo() << "Stdinput:" << Server->buff_;
    return OPRT_OK;
}

void StdInput::addCallBack(void* usr_data, STDCALLBACK callback) {
    usr_data_ = usr_data;
    callback_ = callback;
}

