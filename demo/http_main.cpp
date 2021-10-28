#include "../include/HttpServer.h"
#include "../include/HttpClient.h"
#include "../include/CmdOperate.h"
#include "../include/ThreadPool.h"
#include "../include/StdInput.h"
#include "../include/ErrorCode.h"
#include "../include/yaml/yaml.h"
#include <memory>


class UsrHttpClient: public HttpClient {
public:
    UsrHttpClient(EventPoll* my_epoll, int fd): HttpClient(my_epoll, fd){}
    OPERATE_RET recvCallBack(void* usr_data, const std::string& recv_buff) override {
        if (0 == recv_buff.size()) {
            closeConnect();
            
            return OPRT_OK;
        } else {
            OPERATE_RET ret = analysisUrl(recv_buff);
            switch (ret) {
                case OPRT_OK: {
                    responseSuccess();
                }
                break;
                case OPRT_HTTP_PROTO_ERROR: {
                    responseFailed("404.html");
                }
                break;
                case OPRT_HTTP_METHOD_ERROR: {
                    responseFailed("404.html");
                }
                break;
                default: {

                }
                break;
            }
        }
        return OPRT_OK;
    }
};

class UsrHttpServer: public HttpServer {
public:
    UsrHttpServer(EventPoll* my_epoll, const std::string eth_type, const int my_port): HttpServer(my_epoll, eth_type, my_port){}
    UsrHttpServer(EventPoll* my_epoll, const int my_port, const std::string my_addr): HttpServer(my_epoll, my_port, my_addr){}
    OPERATE_RET listenCallBack(int fd) override {
        UsrHttpClient* httpClient = new UsrHttpClient(epoll_, fd);
        return OPRT_OK;
    }
};

int main(int argc, char **argv) {
    std::string* argv_string = new std::string(cmdOperate(argc, argv)['s']);
    if ("" == *argv_string) {
        delete(argv_string);
        LogInfo() << "argv string is empty";
    } else {
        LogInfo() << "argv string: " << *argv_string;
    }

    /* Event poll init */
    EventPoll eventPoll;
    YAML::Node config = YAML::LoadFile("../config.yml");
    if (config["Server"]["ip"].as<std::string>() != "null") {
        UsrHttpServer httpServer(&eventPoll, config["Server"]["port"].as<int>(), config["Server"]["ip"].as<std::string>());
    } else {
        UsrHttpServer httpServer(&eventPoll, config["Server"]["eth"].as<std::string>(), config["Server"]["port"].as<int>());
    }


    return eventPoll.loop();
}