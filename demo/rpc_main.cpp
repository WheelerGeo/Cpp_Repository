#include "../include/CmdOperate.h"
#include "../include/RpcClient.h"
#include "../include/RpcServer.h"
#include "../include/FileOperate.h"
#include "../include/TimerTick.h"



class UsrRpcClient: public RpcClient {
public:
    UsrRpcClient(EventPoll* my_epoll, const int my_port, const std::string& my_address): RpcClient(my_epoll, my_port, my_address){}
    UsrRpcClient(EventPoll* my_epoll, const int my_fd): RpcClient(my_epoll, my_fd){}
    OPERATE_RET recvCallBack(void* usr_data, const std::string& recv_buff) override {
        Json::Value rcv_json_root;
        std::string send_buff = "";
        Json::Value send_json_root;
        FileOperate::parseJsonFromStr(recv_buff, rcv_json_root);
        if (rcv_json_root["function"]["name"].asString() == "sayHello") {
            send_json_root["result"] = sayHello(rcv_json_root["function"]["param1"].asString());
            FileOperate::formatJsonIntoStr(send_buff, send_json_root);
            sendData(send_buff);
        } else {
            send_json_root["result"] = "Error";
            FileOperate::formatJsonIntoStr(send_buff, send_json_root);
            sendData(send_buff);
        }
        return OPRT_OK;
    }

    std::string sayHello(const std::string& name) {
        std::string ret = "Hello " + name;
        return ret;
    }

};

class UsrRpcServer: public RpcServer {
public:
    UsrRpcServer(EventPoll* my_epoll, const int my_port): RpcServer(my_epoll, my_port){}
    OPERATE_RET listenCallBack(int fd) override {
        UsrRpcClient* client = new UsrRpcClient(epoll_, fd);
        return OPRT_OK;
    }
};

class UsrClient: public RpcClient {
public:
    UsrClient(EventPoll* my_epoll, const int my_port, const std::string& my_address): RpcClient(my_epoll, my_port, my_address){}
    UsrClient(EventPoll* my_epoll, const int my_fd): RpcClient(my_epoll, my_fd){}
    OPERATE_RET recvCallBack(void* usr_data, const std::string& recv_buff) override {
        Json::Value rcv_json_root;
        FileOperate::parseJsonFromStr(recv_buff, rcv_json_root);
        if (rcv_json_root["result"].asString() != "Error") {
            LogInfo() << rcv_json_root["result"].asString();
        } else {
            LogError() << "function error";
        }
        return OPRT_OK;
    }

    OPERATE_RET sayHello(const std::string& name) {
        Json::Value send_json_root, function;
        std::string send_buff = "";
        function["name"] = "sayHello";
        function["param1"] = name;
        send_json_root["function"] = function;
        FileOperate::formatJsonIntoStr(send_buff, send_json_root);
        return sendData(send_buff);
    }

};

std::unique_ptr<UsrClient> usrClient;
std::unique_ptr<UsrRpcServer> usrRpcServer;

OPERATE_RET timCallBack(void* usr_data) {
    if (usrClient != nullptr) {
        usrClient->sayHello("Arien");
    }
    return OPRT_OK;
}

int main(int argc, char** argv) {
    std::unique_ptr<std::string> argv_string(new std::string(cmdOperate(argc, argv)['s']));
    if ("" == *argv_string) {
    } else {
        LogInfo() << "argv string:" << *argv_string;
    }

    EventPoll eventPoll;

    if(*argv_string == "client") {
        Json::Value read_file_json;
        FileOperate::parseJsonFromFile("../config.json", read_file_json);
        usrClient.reset(new UsrClient(&eventPoll, read_file_json["client"]["port"].asInt(), read_file_json["client"]["ip"].asString()));
    } else if (*argv_string == "server") {
        Json::Value read_file_json;
        FileOperate::parseJsonFromFile("../config.json", read_file_json);
        usrRpcServer.reset(new UsrRpcServer(&eventPoll, read_file_json["server"]["port"].asInt()));
    }

    TimerTick* timerTick = new TimerTick(&eventPoll, 100, TimerTick::TIMER_FOREVER);
    timerTick->addCallBack(timCallBack);



    return eventPoll.loop();
}