#ifndef __IPC_H__ 
#define __IPC_H__
#include <iostream>
#include <string>
#include "ErrorCode.h"


class ShareMemory {
public:
    OPERATE_RET shmCreate(key_t key, size_t size);
    OPERATE_RET shmDetach(void);
    OPERATE_RET shmDestroy(void);
    void* shm_addr_ = nullptr;
private:
    int shm_id_ = -1;
    size_t shm_size_ = -1;
};

class NamedPipe {
public:
    NamedPipe(const std::string& read_fifo, const std::string& write_fifo): read_fifo_(read_fifo), write_fifo_(write_fifo) {}
    OPERATE_RET fifoCreate(void);
    OPERATE_RET asyncFifoRead(void);
    OPERATE_RET asyncFifoWrite(const std::string& write_buff);
    virtual void readCallBack(const std::string& read_buff) {}
    virtual void writeCallBack(void) {}
protected:
    std::string write_buff_ = "";
    std::string read_fifo_ = "";
    std::string write_fifo_ = "";
    
    
};




#endif 