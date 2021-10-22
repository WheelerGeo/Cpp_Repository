#include <iostream>
#include "../include/FileOperate.h"
#include "../include/Logger.h"

int main(int argc, char **argv) {
    /* Log init */
    Logger::getInstance().addLoggerToFile("/tmp", "test", FNLog::PRIORITY_INFO, 1024, 1);
    Logger::getInstance().addLoggerToScreen(FNLog::PRIORITY_INFO);
    Logger::getInstance().setLoggerSync();
    Logger::getInstance().loggerStart();

    std::string write_buff = "aaaaaaaaa\nbbbbbbbbb\n";
    // FileOperate::asyncWriteAllIntoFile("a.txt", write_buff);
    LogInfo() << write_buff;




    return 0;
}