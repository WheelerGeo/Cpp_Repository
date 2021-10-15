#include <iostream>
#include <string.h>
#include <memory>
#include "../include/TcpServer.h"
#include "../include/TcpClient.h"
#include "../include/EventPoll.h"
#include "../include/StdInput.h"
#include "../include/TimerTick.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"

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

static int timCallback(void* usr_data, long int now_time_ms) {
    LogInfo() << usr_data << "timer finish";
    return 0;
}

static void threadCallback(void* usr_data) {
    int i = *(int*)usr_data;
    LogInfo() << "number:" << i;
    sleep(10);
}




int main(int argc, char **argv)
{    
    /* Log init */
    Logger::getInstance().addLoggerToFile("./log", "main", FNLog::PRIORITY_INFO, 1024, 1);
    Logger::getInstance().addLoggerToScreen(FNLog::PRIORITY_INFO);
    Logger::getInstance().setLoggerSync();
    Logger::getInstance().loggerStart();
    
    /* Event poll init */
    EventPoll eventPoll;

    /* Timer init */
    TimerTick* timerTick = new TimerTick(&eventPoll, 100, TimerTick::TIMER_ONCE);
    timerTick->addCallback(timCallback);

    /* Thread pool init */
    ThreadPool threadPool(10, 5, 50);
    for(int i = 1; i < 51; ++i) {
        int* ptr = new int(i);
        threadPool.addThreadPoolTask(ptr, threadCallback);
    }

    /* TCP server or client init */
    if (!memcmp(argv[1], "server", 6)) {
        TcpServer tcpServer(&eventPoll, 8000, "192.168.1.98");
        tcpServer.addConnect(&eventPoll, tcpConnect);
        stdInput = new StdInput(&eventPoll);
    } else if (!memcmp(argv[1], "client", 6)) {
        stdInput = new StdInput(&eventPoll);
        TcpClient tcpClient(&eventPoll, 8000, "192.168.1.98");
        tcpClient.addRecv(&tcpClient, tcpRecv);
        stdInput -> addRecv(&tcpClient, stdinRecv);
    }



    
    return eventPoll.loop();
}