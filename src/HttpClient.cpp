#include <string.h>
#include <fstream>
#include "../include/HttpClient.h"
#include "../include/Logger.h"
#include "../include/FileOperate.h"

HttpClient::HttpClient(EventPoll* my_epoll, const int my_port, const std::string my_addr): TcpClient(my_epoll, my_port, my_addr) {

}

HttpClient::HttpClient(EventPoll *my_epoll, int fd): TcpClient(my_epoll, fd) {

}

OPERATE_RET HttpClient::urlParse(const std::string& buf) {
    std::string str_buf = buf;
    if (std::string::npos == str_buf.find("HTTP/")) {
        LogError() << "urlParser:Url Error";
        return OPRT_HTTP_PROTO_ERROR;
    }
    
    http_method_ = str_buf.substr(0, str_buf.find(" /"));
    http_file_name_ = str_buf.substr(str_buf.find("/") + 1, str_buf.find("HTTP") - 6);
    LogInfo() << "http_method:" << http_method_;
    LogInfo() << "http_file_name:" << http_file_name_;       
    if (0 != http_method_.compare("GET")) {
        LogError() << "analysisUrl:method Error";
		return OPRT_HTTP_METHOD_ERROR;
	}

    return OPRT_OK;
}

OPERATE_RET HttpClient::responseSuccess(const std::string& http_file_name) {
    std::string send_buff = "";
    std::string protocol = "HTTP/1.1 200 OK\r\n";
    std::string server = "Server:Linux Web Server\r\n";
    std::string cnt_len = "Content-length:";
    std::string file_buff = "";
    std::string http_file = "";
    if ("" == http_file_name) {
        http_file = "../fodder/index.html";
    } else {
        http_file = "../fodder/" + http_file_name_;
    }
    
    if (OPRT_FILE_OPEN_ERROR == FileOperate::syncReadAllFromFile(http_file, file_buff)) {
        LogError() << "File empty";
        send_buff = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
        if (0 > sendData(send_buff)) {
            LogError() << "HttpClient:senddata";
            return OPRT_SOCK_SEND_ERROR;
        }
        closeConnect();
        return OPRT_FILE_OPEN_ERROR;
    }

    cnt_len += std::to_string(file_buff.size()) + "\r\n\r\n";
    send_buff += protocol + server + cnt_len + file_buff;
    
    if (0 > sendData(send_buff)) {
        LogError() << "HttpClient:senddata";
        return OPRT_SOCK_SEND_ERROR;
    }

    
    closeConnect();
    return OPRT_OK;
}

OPERATE_RET HttpClient::responseSuccess(void) {
    return HttpClient::responseSuccess(http_file_name_); 
}

OPERATE_RET HttpClient::responseFailed(const std::string& error_file_name) {
    std::string send_buff = "";
    std::string protocol = "HTTP/1.1 400 Bad Request\r\n";
    std::string server = "Server:Linux Web Server\r\n";
    std::string cnt_len = "Content-length:";
    std::string file_buff = "";
    std::string error_file = "../fodder/" + error_file_name;

    FileOperate::syncReadAllFromFile(error_file, file_buff);
    cnt_len += std::to_string(file_buff.size()) + "\r\n\r\n";
    send_buff += protocol + server + cnt_len + file_buff;
    
    if (0 > sendData(send_buff)) {
        LogError() << "HttpClient:senddata";
        return OPRT_SOCK_SEND_ERROR;
    }

    closeConnect();
    return OPRT_OK;
}
