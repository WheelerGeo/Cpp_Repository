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
#include "../include/ErrorCode.h"
#include "../include/FileOperate.h"
using namespace std;


// StdInput* stdInput;
unique_ptr<StdInput> stdInput;
static OPERATE_RET timCallBack(void* usr_data);
static void threadCallBack(void* usr_data);
static OPERATE_RET tcpServerCallBack(void* usr_data, int fd);
static OPERATE_RET tcpClientCallBack(void* usr_data, const std::string& buff);
static OPERATE_RET stdInTcpCallBack(void* usr_data, const std::string& buff);
static OPERATE_RET udpRecvCallBack(void* usr_data, char* buff, string des_ip, int des_port);
static OPERATE_RET stdInUdpCallBack(void* usr_data, const std::string& buff);
static OPERATE_RET httpServerCallBack(void* usr_data, int fd);
static OPERATE_RET httpRequestCallBack(void* usr_data, const std::string& buff);
static OPERATE_RET httpClientCallBack(void* usr_data, const std::string& buff);
static OPERATE_RET stdInHttpCallBack(void* usr_data, const std::string& buff);

int main(int argc, char **argv)
{    
    /* Log init */
    Logger::getInstance().addLoggerToFile("/tmp", "main", FNLog::PRIORITY_INFO, 1024, 1);
    Logger::getInstance().addLoggerToScreen(FNLog::PRIORITY_INFO);
    Logger::getInstance().setLoggerSync();
    Logger::getInstance().loggerStart();

    /* Event poll init */
    EventPoll eventPoll;

    /* Timer init */
    TimerTick* timerTick = new TimerTick(&eventPoll, 100, TimerTick::TIMER_ONCE);
    timerTick->addCallBack(timCallBack);

    /* Thread pool init */
    // if(!memcmp(argv[1], "threadpool", strlen("threadpool"))) {
    //     ThreadPool threadPool = ThreadPool::getInstance();
    //     threadPool.threadPoolStart();
    //     for(int i = 1; i < 51; ++i) {
    //         int* ptr = new int(i);
    //         threadPool.addThreadPoolTask(ptr, threadCallBack);
    //     }
    // }

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

static OPERATE_RET timCallBack(void* usr_data) {
    LogInfo() << usr_data << "timer finish";
    return OPRT_OK;
}

static void threadCallBack(void* usr_data) {
    int i = *(int*)usr_data;
    LogInfo() << "number:" << i;
    sleep(10);
}

static OPERATE_RET tcpServerCallBack(void* usr_data, int fd) {
    TcpClient* tcpClient = new TcpClient((EventPoll*)usr_data, fd);
    tcpClient->addCallBack(tcpClient, tcpClientCallBack);
    stdInput->addCallBack(tcpClient, stdInTcpCallBack);
    return OPRT_OK;
}


static OPERATE_RET tcpClientCallBack(void* usr_data, const std::string& buff) {
    TcpClient* cli_usr = (TcpClient*)usr_data;
    if (0 == buff.size()) {
        cli_usr->closeConnect();
    } else {
        LogInfo() << buff;
    }
    return OPRT_OK;
}

static OPERATE_RET stdInTcpCallBack(void* usr_data, const std::string& buff) {
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

static OPERATE_RET udpRecvCallBack(void* usr_data, char* buff, string des_ip, int des_port) {
    UdpServer* cli_usr = (UdpServer*)usr_data;

    return OPRT_OK;
}

static OPERATE_RET stdInUdpCallBack(void* usr_data, const std::string& buff) {
    UdpServer* ser_usr = (UdpServer*)usr_data;
    LogInfo() << buff;
    if (!buff.compare("close")) {
        ser_usr->closeConnect();
        LogInfo() << "client closed";
    } else {
        ser_usr->sendData(buff, "192.168.1.98", 9000);
    }
    return OPRT_OK;
}

static OPERATE_RET httpServerCallBack(void* usr_data, int fd) {
    HttpClient* httpClient = new HttpClient((EventPoll*)usr_data, fd);
    httpClient->addCallBack(httpClient, httpRequestCallBack);
    return OPRT_OK;
}

static OPERATE_RET httpRequestCallBack(void* usr_data, const std::string& buff) {
    HttpClient* cli_usr = (HttpClient*)usr_data;
    if(0 == buff.size()) {
        cli_usr->closeConnect();
        return 0;
    }
    int ret = cli_usr->analysisUrl(buff);
    
    if (0 == ret) {
        cli_usr->responseSuccess();
    } else if (-1 == ret) {
        // cli_usr->responseFailed();
    } else if (-2 == ret) {
        // cli_usr->responseFailed();
    }
    return OPRT_OK;
}

static OPERATE_RET httpClientCallBack(void* usr_data, const std::string& buff) {
    HttpClient* cli_usr = (HttpClient*)usr_data;
    if (0 == buff.size()) {
        cli_usr->closeConnect();
    } else {
        LogInfo() << buff;
    }
    return OPRT_OK;
}

static OPERATE_RET stdInHttpCallBack(void* usr_data, const std::string& buff) {
    HttpClient* cli_usr = (HttpClient*)usr_data;
    LogInfo() << buff;
    if (!buff.compare("close")) {
        cli_usr->closeConnect();
        LogInfo() << "client closed";
    } else {
        cli_usr->sendData(buff);
    }
    return OPRT_OK;
}
