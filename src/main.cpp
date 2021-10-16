#include <iostream>
#include <string.h>
#include <memory>
#include "../include/TcpServer.h"
#include "../include/TcpClient.h"
#include "../include/UdpServer.h"
#include "../include/EventPoll.h"
#include "../include/StdInput.h"
#include "../include/TimerTick.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"
#include "../include/NetworkTool.h"
#include "../include/HttpServer.h"
#include "../include/HttpClient.h"

using namespace std;


// StdInput* stdInput;
unique_ptr<StdInput> stdInput;
static int timCallBack(void* usr_data, long int now_time_ms);
static void threadCallBack(void* usr_data);
static int tcpServerCallBack(void* usr_data, int fd);
static int tcpClientCallBack(void* usr_data, char* buff, size_t len);
static int stdInTcpCallBack(void* usr_data, char* buff, size_t len);
static int udpRecvCallBack(void* usr_data, char* buff, string des_ip, int des_port);
static int stdInUdpCallBack(void* usr_data, char* buff, size_t len);
static int httpServerCallBack(void* usr_data, int fd);
static int httpClientCallBack(void* usr_data, char* buff, size_t len);
static int stdInHttpCallBack(void* usr_data, char* buff, size_t len);
static int httpRequestCallBack(void* usr_data, char* buff, size_t len);

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
    timerTick->addCallBack(timCallBack);

    /* Thread pool init */
    if(!memcmp(argv[1], "threadpool", strlen("threadpool"))) {
        ThreadPool* threadPool = new ThreadPool(10, 5, 50);
        for(int i = 1; i < 51; ++i) {
            int* ptr = new int(i);
            threadPool->addThreadPoolTask(ptr, threadCallBack);
        }
    }

    /* TCP server or client init */
    if (!memcmp(argv[1], "tcpserver", strlen("tcpserver"))) {
        TcpServer* tcpServer = new TcpServer(&eventPoll, 8000);
        tcpServer->addCallBack(&eventPoll, tcpServerCallBack);
        stdInput.reset(new StdInput(&eventPoll));
    } else if (!memcmp(argv[1], "tcpclient", strlen("tcpclient"))) {
        stdInput.reset(new StdInput(&eventPoll));
        TcpClient* tcpClient = new TcpClient(&eventPoll, 8000, "192.168.1.98");
        tcpClient->addCallBack(&tcpClient, tcpClientCallBack);
        stdInput->addCallBack(&tcpClient, stdInTcpCallBack);
    }

    /* UDP server init */
    if (!memcmp(argv[1], "udpserver", strlen("udpserver"))) {
        UdpServer* udpServer = new UdpServer(&eventPoll, 9000);
        udpServer->addCallBack(&udpServer, udpRecvCallBack);
        stdInput.reset(new StdInput(&eventPoll));
        stdInput->addCallBack(&udpServer, stdInUdpCallBack);
    }

    /* HTTP server or client init */
    if (!memcmp(argv[1], "httpserver", strlen("httpserver"))) {
        HttpServer* httpServer = new HttpServer(&eventPoll, 1080);
        httpServer->addCallBack(&eventPoll, httpServerCallBack);

    } else if (!memcmp(argv[1], "httpclient", strlen("httpclient"))) {
        HttpClient* httpClient = new HttpClient(&eventPoll, 1080, "192.168.1.98");
        httpClient->addCallBack(&eventPoll, httpClientCallBack);
        stdInput.reset(new StdInput(&eventPoll));
        stdInput->addCallBack(&httpClient, stdInHttpCallBack);
    }

    
    return eventPoll.loop();
}

static int timCallBack(void* usr_data, long int now_time_ms) {
    LogInfo() << usr_data << "timer finish";
    return 0;
}

static void threadCallBack(void* usr_data) {
    int i = *(int*)usr_data;
    LogInfo() << "number:" << i;
    sleep(10);
}

static int tcpServerCallBack(void* usr_data, int fd) {
    TcpClient* tcpClient = new TcpClient((EventPoll*)usr_data, fd);
    tcpClient->addCallBack(tcpClient, tcpClientCallBack);
    stdInput->addCallBack(tcpClient, stdInTcpCallBack);
    return 0;
}

static int tcpClientCallBack(void* usr_data, char* buff, size_t len) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    if (len == 0) {
        cli_usr->closeConnect();
    } else {
        LogInfo() << buff;
    }
    return 0;
}

static int stdInTcpCallBack(void* usr_data, char* buff, size_t len) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    cout << buff << endl;
    if (!memcmp(buff, "close", 5)) {
        cli_usr->closeConnect();
        LogInfo() << "client closed";
    } else {
        cli_usr->sendData(buff);
    }
    return 0;
}

static int udpRecvCallBack(void* usr_data, char* buff, string des_ip, int des_port) {
    UdpServer* cli_usr = (UdpServer*)usr_data;

    return 0;
}

static int stdInUdpCallBack(void* usr_data, char* buff, size_t len) {
    UdpServer* ser_usr = (UdpServer*)usr_data;
    LogInfo() << buff;
    if (!memcmp(buff, "close", 5)) {
        ser_usr->closeConnect();
        LogInfo() << "client closed";
    } else {
        ser_usr->sendData(buff, "192.168.1.98", 9000);
    }
    return 0;
}

static int httpServerCallBack(void* usr_data, int fd) {
    HttpClient* httpClient = new HttpClient((EventPoll*)usr_data, fd);
    httpClient->addCallBack(httpClient, httpRequestCallBack);
    return 0;
}

static int httpRequestCallBack(void* usr_data, char* buff, size_t len) {
    HttpClient* cli_usr = (HttpClient*)usr_data;
    int ret = cli_usr->analysisUrl(buff);
    if (0 == ret) {
        cli_usr->responseSuccess();
    } else if (-1 == ret) {

    } else if (-2 == ret) {

    }
    return 0;
}

static int httpClientCallBack(void* usr_data, char* buff, size_t len) {
    HttpClient* cli_usr = (HttpClient*)usr_data;
    if (len == 0) {
        cli_usr->closeConnect();
    } else {
        LogInfo() << buff;
    }
    return 0;
}

static int stdInHttpCallBack(void* usr_data, char* buff, size_t len) {
    HttpClient* cli_usr = (HttpClient*)usr_data;
    cout << buff << endl;
    if (!memcmp(buff, "close", 5)) {
        cli_usr->closeConnect();
        LogInfo() << "client closed";
    } else {
        cli_usr->sendData(buff);
    }
    return 0;
}
