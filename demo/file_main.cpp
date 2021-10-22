#include "../include/FileOperate.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"
class File: public FileOperate {
public:
    File(const std::string& file_name, const std::string& write_buff): FileOperate(file_name, write_buff) {}
    void readCallBack(const std::string& read_buff) override {
    }
};

int main(int argc, char **argv) {
    /* Log init */
    Logger::getInstance().addLoggerToFile("/tmp", "file_main", FNLog::PRIORITY_DEBUG, 1024, 1);
    Logger::getInstance().addLoggerToScreen(FNLog::PRIORITY_DEBUG);
    Logger::getInstance().setLoggerSync();
    Logger::getInstance().loggerStart();

    ThreadPool threadPool = ThreadPool::getInstance();
    LogDebug() << &ThreadPool::getInstance();
    threadPool.threadPoolStart();

    std::string write_buff = "21312321321321";
    File fileOperate("../a.txt", write_buff);
    LogInfo() << "Hello World!";
    fileOperate.asyncWriteApendIntoFile();
    return 0;
}

