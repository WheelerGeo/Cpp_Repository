#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include <string.h>
#include "../include/StdInput.h"
#include "../include/Logger.h"
#include "../include/TcpClient.h"

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
   
    Server->recvCallBack(Server->usr_data_, Server->buff_);
    LogInfo() << "Stdinput:" << Server->buff_;
    return OPRT_OK;
}

OPERATE_RET StdInput::addUsrData(void* usr_data) {
    usr_data_ = usr_data;
    return OPRT_OK;
}

OPERATE_RET StdInput::recvCallBack(void* usr_data, const std::string& buff) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    LogInfo() << buff;
    if (!buff.compare("close")) {
        cli_usr->closeConnect();
        LogInfo() << "client closed";
    } else {
        cli_usr->sendData(buff);
    }
    return OPRT_OK;
}

