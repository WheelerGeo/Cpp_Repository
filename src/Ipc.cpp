#include "../include/Ipc.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>


OPERATE_RET ShareMemory::shmCreate(key_t key, size_t size) {
    shm_size_ = size;
    shm_id_ = shmget(key, shm_size_, IPC_CREAT | 0666);
    if (0 > shm_id_) {
        LogError() << "Share memory id create fail";
        return OPRT_SHM_CREATE_ERROR;
    }
    shm_addr_ = shmat(shm_id_, NULL, 0);
    if (-1 == atoi((char*)shm_addr_)) {
        LogError() << "Share memory address alloc fail";
        return OPRT_SHM_ADDRESS_ALLOC_ERROR;
    }
    LogInfo() << "Shm_id:" << shm_id_ << "(" << shm_addr_ << ")" << " create!";
    
    return OPRT_OK;
}

OPERATE_RET ShareMemory::shmDetach(void) {
    if (-1 == atoi((char*)shm_addr_)) {
        LogError() << "Share memory not found";
        return OPRT_SHM_NOT_FOUND;
    }
    shmdt(shm_addr_);
    LogInfo() << "Shm_id:" << shm_id_ << "(" << shm_addr_ << ")" << " detach!";
    return OPRT_OK;
}

OPERATE_RET ShareMemory::shmDestroy(void) {
    shmid_ds shmsds;
    if (-1 == atoi((char*)shm_addr_)) {
        LogError() << "Share memory not found";
        return OPRT_SHM_NOT_FOUND;
    }
    if(0 != shmctl(shm_id_, IPC_RMID, &shmsds)){
        LogError() << "Share memory destroy faild";
        return OPRT_SHM_DESTROY_ERROR;
    }
    LogInfo() << "Shm_id:" << shm_id_ << "(" << shm_addr_ << ")" << " destroy!";
    return OPRT_OK;
}

OPERATE_RET NamedPipe::fifoCreate(void) {
    if (0 > mkfifo(read_fifo_.c_str(), 0777)) {
        LogError() << "Read fifo create failed!";
        return OPRT_FIFO_CREATE_ERROR;
    }
    if (0 > mkfifo(write_fifo_.c_str(), 0777)) {
        LogError() << "Write fifo create failed!";
        return OPRT_FIFO_CREATE_ERROR;
    }
    return OPRT_OK;
}

OPERATE_RET NamedPipe::asyncFifoRead(void) {
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        NamedPipe* namedPipe = (NamedPipe*)usr_data;
        char read_buff[1024] = "";
        int fd = open(namedPipe->read_fifo_.c_str(), O_RDONLY);
        if (0 > fd) {
            LogError() << "Read Fifo open failed!";
            return OPRT_FIFO_OPEN_ERROR;
        }
        if (0 > read(fd, read_buff, sizeof(read_buff))) {
            LogError() << "Read Fifo read failed!";
            return OPRT_FIFO_READ_ERROR;
        }
        namedPipe->readCallBack(read_buff);
        return OPRT_OK;
    });
    return OPRT_OK;
}

OPERATE_RET NamedPipe::asyncFifoWrite(const std::string& write_buff) {
    write_buff_ = write_buff;
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        NamedPipe* namedPipe = (NamedPipe*)usr_data;
        int fd = open(namedPipe->write_fifo_.c_str(), O_WRONLY);
        if (0 > fd) {
            LogError() << "Write Fifo open failed!";
            return OPRT_FIFO_OPEN_ERROR;
        }
        if (0 > write(fd, namedPipe->write_buff_.c_str(), namedPipe->write_buff_.size())) {
            LogError() << "Write Fifo write failed!";
            return OPRT_FIFO_WRITE_ERROR;
        }
        namedPipe->writeCallBack();
        return OPRT_OK;
    });
    return OPRT_OK;
}