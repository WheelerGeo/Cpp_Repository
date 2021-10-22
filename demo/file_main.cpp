#include "../include/FileOperate.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"
#include "../include/EventPoll.h"
#include "../include/cmdOperate.h"
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
        LogInfo() << "argv string is empty";
    }
    LogInfo() << "argv string: " << *argv_string;

    /* Event poll init */
    EventPoll eventPoll;

    ThreadPool& threadPool = ThreadPool::getInstance();
    threadPool.threadPoolStart();

    File fileOperate("../html/index.html");
    fileOperate.asyncReadAllFromFile();
    


    return eventPoll.loop();
}

