#ifndef __FILEOPERATE_H__
#define __FILEOPERATE_H__
#include <string>
#include <fstream>
#include <functional>
#include <map>
#include "ErrorCode.h"
#include "json/json.h"

class FileOperate{
public:
    FileOperate(const std::string& file_name);
    FileOperate(const std::string& file_name, const std::string& write_buff);
    
    static OPERATE_RET syncReadAllFromFile(const std::string& file_name, std::string& read_buff);
    static OPERATE_RET syncReadLineFromFile(const std::string& file_name, const int line_id, std::string& read_buff);
    static OPERATE_RET syncWriteAllIntoFile(const std::string& file_name, const std::string& write_buff);
    static OPERATE_RET syncWriteLineIntoFile(const std::string& file_name, const int line_id, const std::string& write_buff);
    static OPERATE_RET syncWriteApendIntoFile(const std::string& file_name, const std::string& write_buff);
    static OPERATE_RET countLineOfFile(const std::string& file_name, int& line_number);
    static OPERATE_RET countSizeOfFile(const std::string& file_name, int& file_size);

    OPERATE_RET asyncReadAllFromFile(void);
    OPERATE_RET asyncWriteAllIntoFile(void);
    OPERATE_RET asyncWriteApendIntoFile(void);
    virtual void readCallBack(const std::string& read_buff) {}
    virtual void writeCallBack(void) {}

    static OPERATE_RET parseJsonFromStr(const std::string& json_str, Json::Value& read_root);
    static OPERATE_RET parseJsonFromFile(const std::string& file_name, Json::Value& read_root);
    static OPERATE_RET formatJsonIntoStr(std::string& write_str, const Json::Value& format_root);
    static OPERATE_RET writeJsonIntoFile(const std::string& file_name, const Json::Value& write_root);
private:
    std::string file_name_ = "";
    std::string write_buff_ = "";
    pthread_mutex_t file_lock_;
};





#endif
