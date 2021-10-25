#ifndef __CMDOPERATE_H__
#define __CMDOPERATE_H__
#include <unistd.h>
#include <iostream>
#include <map>
#include "../include/Logger.h"

void helpMsg(char* argv) {
    std::string name = argv;
    std::cout << "Usage: version 0.0.1" << std::endl;
    std::cout << "\033[31mLog priority: -i > -d > -t\033[0m" << std::endl;
    std::cout << "  -h   view help message" << std::endl;
    std::cout << "  -i   Log info message into \033[33m/tmp" << name.substr(name.rfind("/")) << ".log*\033[0m" << std::endl;
    std::cout << "  -d   Log debug message into \033[33m/tmp" << name.substr(name.rfind("/")) << ".log*\033[0m" << std::endl;
    std::cout << "  -t   Log trace message into \033[33m/tmp" << name.substr(name.rfind("/")) << ".log*\033[0m" << std::endl;
    std::cout << "  -s   Input the string, such as " << "\033[33m\"" <<  name << " -s 192.168.1.1" << "\"\033[0m" << std::endl;
}

void errorMsg1(char* argv) {
    std::string name = argv;
    std::cout << "Usage: version 0.0.1" << std::endl;
    std::cout << "\033[31mPlease use " << "\"" << name << " -h" << "\"" << " view valid option\033[0m" << std::endl;
}

std::map<char, std::string> cmdOperate(int argc, char **argv) {
    int param = 0;
    std::map<char, std::string> cmd;
    std::string name = argv[0];
    while ((param = getopt(argc, argv, "hidts:")) != -1) {
        switch (param) {
            case 'h': {
                helpMsg(argv[0]);
                exit(0);
            }
            break;
            case 'i': {
                cmd['i'] = "info";
            }
            break;
            case 'd': {
                cmd['d'] = "debug";
            }
            break;
            case 't': {
                cmd['t'] = "trace";
            }
            break;
            case 's': {
                cmd['s'] = optarg;
            }
            break;
            default: {
                errorMsg1(argv[0]);
                exit(0);
            }
            break;
        }
    }
    FNLog::LogPriority level = FNLog::PRIORITY_ERROR;
    if (cmd.find('t') != cmd.end()) {
        level = FNLog::PRIORITY_TRACE;
    }
    if (cmd.find('d') != cmd.end()) {
        level = FNLog::PRIORITY_DEBUG;
    }
    if (cmd.find('i') != cmd.end()) {
        level = FNLog::PRIORITY_INFO;
    }

    /* Log init */
    Logger::getInstance().addLoggerToFile("/tmp", name.substr(name.rfind("/") + 1), level, 1024, 4);
    Logger::getInstance().addLoggerToScreen(level);
    Logger::getInstance().setLoggerSync();
    Logger::getInstance().loggerStart();

    return cmd;
}



#endif