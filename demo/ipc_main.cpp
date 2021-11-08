#include "../include/ThreadPool.h"
#include "../include/CmdOperate.h"
#include "../include/Ipc.h"
#include "../include/EventPoll.h"
class UsrNamedPipe: public NamedPipe {
public:
    UsrNamedPipe(const std::string& read_fifo, const std::string& write_fifo): NamedPipe(read_fifo, write_fifo){}
    void readCallBack(const std::string& read_buff) {
        LogInfo() << "fifo read:" << read_buff;
    }
    void writeCallBack(void) {
        LogInfo() << "fifo write:" << write_buff_;
    }
};

int main(int argc, char **argv) {
    std::string* argv_string  = new std::string(cmdOperate(argc, argv)['s']);
    if ("" == *argv_string) {
        delete(argv_string);
    } else {
        LogInfo() << "argv string: " << *argv_string;
    }
    
    EventPoll eventPoll;

    if (*argv_string == "read") {
        UsrNamedPipe* usrNamePipe = new UsrNamedPipe("1111", "2222");
        usrNamePipe->fifoCreate();
        usrNamePipe->asyncFifoRead();
    } else {
        UsrNamedPipe* usrNamePipe = new UsrNamedPipe("2222", "1111");
        usrNamePipe->asyncFifoWrite("Hello Arien");
    }

    return eventPoll.loop();
}