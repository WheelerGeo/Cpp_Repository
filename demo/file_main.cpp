#include "../include/FileOperate.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"
#include "../include/EventPoll.h"
#include "../include/CmdOperate.h"
#include "../include/yaml/yaml.h"
class File: public FileOperate {
public:
    File(const std::string& file_name): FileOperate(file_name) {}
    File(const std::string& file_name, const std::string& write_buff): FileOperate(file_name, write_buff) {}
    void readCallBack(const std::string& read_buff) override {
        LogInfo() << read_buff;
    }
};

int main(int argc, char **argv) {
    std::string* argv_string  = new std::string(cmdOperate(argc, argv)['s']);
    if ("" == *argv_string) {
        delete(argv_string);
    } else {
        LogInfo() << "argv string: " << *argv_string;
    }

    /* Event poll init */
    EventPoll eventPoll;

    ThreadPool& threadPool = ThreadPool::getInstance();
    threadPool.threadPoolStart();

    File fileOperate("../fodder/index.html");
    fileOperate.asyncReadAllFromFile();

    YAML::Node config = YAML::LoadFile("../config.yml");
    LogInfo() << "ip:" << config["Client"]["ip"].as<std::string>();
    LogInfo() << "port:" << config["Client"]["port"].as<int>();

    Json::Value json_write_buff, server, client;
    server["port"] = 8000;
    client["ip"] = "192.168.1.98";
    client["port"] = 8000;
    json_write_buff["server"] = server;
    json_write_buff["client"] = client;
    FileOperate::writeJsonIntoFile("../config.json", json_write_buff);

    std::unique_ptr<Json::Value> json_read_buff(new Json::Value);
    FileOperate::parseJsonFromFile("../config.json", *json_read_buff);
    LogInfo() << "server port:" << (*json_read_buff)["server"]["port"].asInt();
    LogInfo() << "client ip:" << (*json_read_buff)["client"]["ip"].asString();
    LogInfo() << "client port:" << (*json_read_buff)["client"]["port"].asInt();



    return eventPoll.loop();
}

