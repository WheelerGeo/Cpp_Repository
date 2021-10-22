#include "../include/FileOperate.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"


OPERATE_RET FileOperate::syncReadAllFromFile(const std::string& file_name, std::string& read_buff) {
    std::ifstream readFrom;
    readFrom.open(file_name, std::ifstream::in);
    if (!readFrom.is_open()) {
        LogError() << "Open Failed";
        return OPRT_FILE_OPEN_ERROR;
    }
    std::string str((std::istreambuf_iterator<char>(readFrom)), std::istreambuf_iterator<char>(0));
    read_buff = std::move(str);
    readFrom.close();
    return OPRT_OK;
}

OPERATE_RET FileOperate::syncReadLineFromFile(const std::string& file_name, const int line_id, std::string& read_buff) {
    std::ifstream readFrom(file_name, std::ifstream::in);
    int line_num,i = 0;
    OPERATE_RET ret = OPRT_OK;
    if (0 > (ret = countLineOfFile(file_name, line_num))) {
        LogError() << "readLineFromFile:countLine";
        return ret;
    }

    if (readFrom.fail()) {
        LogError() << "readLineFromFile:no such file";
        return OPRT_FILE_INEXIST_ERROR;
    }
    if (line_id <= 0) {
        LogError() << "readLineFromFile:wrong line_id";
        return OPRT_INVALID_PARM;
    }
    if (line_id > line_num) {
        LogError() << "readLineFromFile:line_id too big";
        return OPRT_INVALID_PARM;
    }

    while (getline(readFrom, read_buff) && i < line_id - 1) {
        i++;
    }
    readFrom.close();
    return OPRT_OK;
}

OPERATE_RET FileOperate::syncWriteAllIntoFile(const std::string& file_name, const std::string& write_buff) {
    std::ofstream writeIn(file_name, std::ifstream::out);
    writeIn << write_buff;
    writeIn.close();
    return OPRT_OK;
}

OPERATE_RET FileOperate::syncWriteApendIntoFile(const std::string& file_name, const std::string& write_buff) {
    std::ofstream writeIn(file_name, std::ifstream::app);
    writeIn << write_buff;
    writeIn.close();
    return OPRT_OK;
}

OPERATE_RET FileOperate::countLineOfFile(const std::string& file_name, int& line_number) {
    std::ifstream readFrom(file_name, std::ifstream::in);
    int n=0;
    std::string tmp;
    if (readFrom.fail()) {
        LogError() << "countLineOfFile:no such file";
        return OPRT_FILE_INEXIST_ERROR;
    } else {
        while (getline(readFrom, tmp)) {
            line_number++;
        }
    }
    readFrom.close();
    return OPRT_OK;
}

OPERATE_RET FileOperate::countSizeOfFile(const std::string& file_name, int& file_size) {
    std::ifstream readFrom(file_name, std::ifstream::in);
    if(readFrom.fail()) {
        LogError() << "countSizeOfFile:no such file";
        return OPRT_FILE_INEXIST_ERROR;
    }
    std::string str((std::istreambuf_iterator<char>(readFrom)), std::istreambuf_iterator<char>(0));
    file_size = str.size();
    return OPRT_OK;
}

OPERATE_RET FileOperate::asyncReadAllFromFile(void) {
    if (!file_name_.compare("")) {
        LogError() << "Missing instance parameters";
        return OPRT_INSTANCE_LACK_PARAM;
    }
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        FileOperate* file_operate = (FileOperate*)usr_data;
        std::string read_buff = "";
        FileOperate::syncReadAllFromFile(file_operate->file_name_, read_buff);
        file_operate->readCallBack(read_buff);
        return OPRT_OK;   
    });
    return OPRT_OK;
}
OPERATE_RET FileOperate::asyncWriteAllIntoFile(void) {
    if (!file_name_.compare("") || !write_buff_.compare("")) {
        LogError() << "Missing instance parameters";
        return OPRT_INSTANCE_LACK_PARAM;
    }
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        FileOperate* file_operate = (FileOperate*)usr_data;
        FileOperate::syncWriteAllIntoFile(file_operate->file_name_, file_operate->write_buff_);
        // file_operate->readCallBack(file_operate->write_buff_);
        return OPRT_OK;
    });
    return OPRT_OK;
}
OPERATE_RET FileOperate::asyncWriteApendIntoFile(void) { 
    LogInfo() << "start1";
    if (!file_name_.compare("") || !write_buff_.compare("")) {
        LogError() << "Missing instance parameters";
        return OPRT_INSTANCE_LACK_PARAM;
    }
    LogInfo() << "start2";
    LogDebug() << &ThreadPool::getInstance();
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        FileOperate* file_operate = (FileOperate*)usr_data;
        LogInfo() << "start3";
        FileOperate::syncWriteApendIntoFile(file_operate->file_name_, file_operate->write_buff_);
        LogInfo() << "finish1";
        return OPRT_OK;
    });
    LogInfo() << "finish2";
    return OPRT_OK;

}
