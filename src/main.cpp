#include <iostream>
#include "TcpServer.h"
#include "TcpClient.h"
#include "EventPoll.h"
#include "StdInput.h"
#include "TimerTick.h"
#include <string.h>
using namespace std;


StdInput* stdInput;
static int tcpRecv(void* usr_data, char* buff, size_t len) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    if (len == 0) {
        cli_usr -> closeConnet();
    } else {
        cout << buff << endl;
    }
    return 0;
}

static int stdinRecv(void* usr_data, char* buff, size_t len) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    cout << buff << endl;
    if (!memcmp(buff, "close", 5)) {
        cli_usr -> closeConnet();
        cout << "client closed" << endl;
    } else {
        cli_usr -> sendData(buff);
    }
    return 0;
}

static int tcpConnect(void* usr_data, int fd) {
    TcpClient* tcpClient = new TcpClient((EventPoll*)usr_data, fd);
    tcpClient -> addRecv(tcpClient, tcpRecv);
    stdInput -> addRecv(tcpClient, stdinRecv);
    return 0;
}

static int timCallback(void *usr_data) {
    return 0;
}



int main(int argc, char **argv)
{    
    EventPoll eventPoll;
    TimerTick timerTick(&eventPoll, 100);
    timerTick.addCallback(timCallback);
    
    if (! memcmp(argv[1], "server", 6)) {
        TcpServer tcpServer(&eventPoll, 8000, "192.168.1.98");
        tcpServer.addConnect(&eventPoll, tcpConnect);
        stdInput = new StdInput(&eventPoll);
    } else if (! memcmp(argv[1], "client", 6)) {
        stdInput = new StdInput(&eventPoll);
        TcpClient tcpClient(&eventPoll, 8000, "192.168.1.98");
        tcpClient.addRecv(&tcpClient, tcpRecv);
        stdInput -> addRecv(&tcpClient, stdinRecv);
    }



    
    return eventPoll.loop();
}