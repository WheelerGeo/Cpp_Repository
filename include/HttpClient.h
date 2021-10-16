#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__
#include "HttpClient.h"
#include "TcpClient.h"

class HttpClient: public TcpClient{
public:
    HttpClient(EventPoll* my_epoll, const int my_port, const std::string my_addr);
    HttpClient(EventPoll *my_epoll, int fd);
    int analysisUrl(char* buf);
    int responseSuccess(char* http_file_name);
    int responseSuccess(void);
    int responseFailed(void);
private:
    char* http_method_ = nullptr;
    char* http_file_name_ = nullptr;
    char* http_content_type_ = nullptr;
};

#endif