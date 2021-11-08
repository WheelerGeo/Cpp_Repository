#include <iostream>
#include "../include/CmdOperate.h"
#include "../include/Logger.h"
#include "../include/Ipc.h"
#include "../include/Struct.h"

int main(int argc, char **argv) {
    std::string* argv_string  = new std::string(cmdOperate(argc, argv)['s']);
    if ("" == *argv_string) {
        delete(argv_string);
    } else {
        LogInfo() << "argv string: " << *argv_string;
    }

    ShareMemory shareMemory;
    shareMemory.shmCreate(1, 128);
    shm_param* write1 = (shm_param*)shareMemory.shm_addr_;
    write1->data = 1;
    write1->c = 'a';
    shm_param* read = (shm_param*)shareMemory.shm_addr_;
    LogInfo() << read->data << read->c;
    shareMemory.shmDestroy();

    return 0;
}