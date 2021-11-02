#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__
#include "HttpClient.h"
#include "TcpClient.h"
#include "ErrorCode.h"

class HttpClient: public TcpClient{
public:
    HttpClient(EventPoll* my_epoll, const int my_port, const std::string my_addr);
    HttpClient(EventPoll *my_epoll, const int fd);
    OPERATE_RET urlParse(const std::string& buf);
    OPERATE_RET responseSuccess(const std::string& http_file_name);
    OPERATE_RET responseSuccess(void);
    OPERATE_RET responseFailed(const std::string& error_file_name);
private:
    std::string http_method_ = "";
    std::string http_file_name_ = "";
};

#endif