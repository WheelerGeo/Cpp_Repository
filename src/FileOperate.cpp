#include "../include/FileOperate.h"
#include "../include/Logger.h"
#include "../include/ThreadPool.h"

FileOperate::FileOperate(const std::string& file_name):file_name_(file_name) {
    pthread_mutex_init(&file_lock_, NULL);
}

FileOperate::FileOperate(const std::string& file_name, const std::string& write_buff):file_name_(file_name), write_buff_(write_buff) {
    pthread_mutex_init(&file_lock_, NULL);
}

OPERATE_RET FileOperate::syncReadAllFromFile(const std::string& file_name, std::string& read_buff) {
    std::ifstream readFrom;
    readFrom.open(file_name, std::ifstream::in);
    if (!readFrom.is_open()) {
        LogError() << file_name << ": Open Failed";
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
        LogError() << "No such file";
        return OPRT_FILE_INEXIST_ERROR;
    }
    if (line_id <= 0) {
        LogError() << "Wrong line id";
        return OPRT_INVALID_PARM;
    }
    if (line_id > line_num) {
        LogError() << "Line id too big";
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

OPERATE_RET FileOperate::syncWriteLineIntoFile(const std::string& file_name, const int line_id, const std::string& write_buff) {
    std::ifstream readFrom;
    readFrom.open(file_name, std::ifstream::in);
    if (!readFrom.is_open()) {
        LogError() << file_name << ": Open Failed";
        return OPRT_FILE_OPEN_ERROR;
    }
    std::string wirte_file_data = "";
    int line = 1;
    std::string tmp = "";
    while (getline(readFrom, tmp)) {
        if (line == line_id) {
            wirte_file_data += std::string(write_buff);
            wirte_file_data += "\n";
        } else {
            wirte_file_data += std::string(tmp);
            wirte_file_data += "\n";
        }
        line++;
    }

    readFrom.close();

    syncWriteAllIntoFile(file_name, wirte_file_data);
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
    std::string tmp = "";
    if (readFrom.fail()) {
        LogError() << "No such file";
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
        LogError() << "No such file";
        return OPRT_FILE_INEXIST_ERROR;
    }
    std::string str((std::istreambuf_iterator<char>(readFrom)), std::istreambuf_iterator<char>(0));
    file_size = str.size();
    readFrom.close();
    return OPRT_OK;
}

OPERATE_RET FileOperate::asyncReadAllFromFile(void) {
    if (file_name_ == "") {
        LogError() << "Missing instance parameters";
        return OPRT_INSTANCE_LACK_PARAM;
    }
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        FileOperate* file_operate = (FileOperate*)usr_data;
        std::string read_buff = "";
        pthread_mutex_lock(&file_operate->file_lock_);
        FileOperate::syncReadAllFromFile(file_operate->file_name_, read_buff);
        pthread_mutex_unlock(&file_operate->file_lock_);
        file_operate->readCallBack(read_buff);
        return OPRT_OK;   
    });
    return OPRT_OK;
}

OPERATE_RET FileOperate::asyncWriteAllIntoFile(void) {
    if (file_name_ == "" || write_buff_ == "") {
        LogError() << "Missing instance parameters";
        return OPRT_INSTANCE_LACK_PARAM;
    }
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        FileOperate* file_operate = (FileOperate*)usr_data;
        pthread_mutex_lock(&file_operate->file_lock_);
        FileOperate::syncWriteAllIntoFile(file_operate->file_name_, file_operate->write_buff_);
        pthread_mutex_unlock(&file_operate->file_lock_);
        file_operate->writeCallBack();
        return OPRT_OK;
    });
    return OPRT_OK;
}

OPERATE_RET FileOperate::asyncWriteApendIntoFile(void) { 
    if (file_name_ == "" || write_buff_ == "") {
        LogError() << "Missing instance parameters";
        return OPRT_INSTANCE_LACK_PARAM;
    }
    ThreadPool::getInstance().addThreadPoolTask(this, [] (void* usr_data) {
        FileOperate* file_operate = (FileOperate*)usr_data;
        pthread_mutex_lock(&file_operate->file_lock_);
        FileOperate::syncWriteApendIntoFile(file_operate->file_name_, file_operate->write_buff_);
        pthread_mutex_unlock(&file_operate->file_lock_);
        file_operate->writeCallBack();
        return OPRT_OK;
    });
    return OPRT_OK;
}

OPERATE_RET FileOperate::parseJsonFromStr(const std::string& json_str, Json::Value& read_root) {
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    
    if (!reader->parse(json_str.c_str(), json_str.c_str() + json_str.length(), &read_root, nullptr)) {
		LogError() << "parse failed";
		return OPRT_JSON_PARSE_ERROR;
	}

    return OPRT_OK;
}

OPERATE_RET FileOperate::parseJsonFromFile(const std::string& file_name, Json::Value& read_root) {
    std::string read_buff = "";
    syncReadAllFromFile(file_name, read_buff);
    OPERATE_RET ret = OPRT_OK;
    ret = parseJsonFromStr(read_buff, read_root);

    return ret;
}

OPERATE_RET FileOperate::formatJsonIntoStr(std::string& save_str, const Json::Value& format_root) {
    Json::StreamWriterBuilder writerBuilder;
    std::ostringstream os;
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());

    writer->write(format_root, &os);
    save_str = os.str();

    return OPRT_OK;
}

OPERATE_RET FileOperate::writeJsonIntoFile(const std::string& file_name, const Json::Value& write_root) {
    std::string write_str = "";
    OPERATE_RET ret = OPRT_OK;
    ret = formatJsonIntoStr(write_str, write_root);
    ret = syncWriteAllIntoFile(file_name, write_str);

    return ret;
}