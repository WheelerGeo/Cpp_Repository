#include <string.h>
#include <fstream>
#include "../include/HttpClient.h"
#include "../include/Logger.h"

HttpClient::HttpClient(EventPoll* my_epoll, const int my_port, const std::string my_addr): TcpClient(my_epoll, my_port, my_addr) {

}

HttpClient::HttpClient(EventPoll *my_epoll, int fd): TcpClient(my_epoll, fd) {

}

int HttpClient::analysisUrl(char* buf) {
    if (nullptr == strstr(buf, "HTTP/")) {
        LogError() << "analysisUrl:Url Error";
        return -1;
    }
    
    strcpy(http_method_, strtok(buf, " /"));      
	strcpy(http_file_name_, strtok(NULL, " /"));       
    if (0 != strcmp(http_method_, "GET")) {
        LogError() << "analysisUrl:method Error";
		return -2;
	}

    return 0;
}

int HttpClient::responseSuccess(char* http_file_name) {
    char send_buff[] = "";
    char protocol[] = "HTTP/1.0 200 OK\r\n";
	char server[] = "Server:Linux Web Server \r\n";
	char cnt_len[] = "Content-length:2048\r\n";
    std::ifstream ifs(http_file_name, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>(0));
    ifs.close();

    strcat(send_buff, protocol);
    strcat(send_buff, server);
    strcat(send_buff, cnt_len);
    strcat(send_buff, str.c_str());
    
    sendData(send_buff);
    return 0;

}

int HttpClient::responseSuccess(void) {
    char send_buff[] = "";
    char protocol[] = "HTTP/1.0 200 OK\r\n";
	char server[] = "Server:Linux Web Server \r\n";
	char cnt_len[] = "Content-length:2048\r\n";
    std::ifstream ifs(http_file_name_, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>(0));
    ifs.close();

    strcat(send_buff, protocol);
    strcat(send_buff, server);
    strcat(send_buff, cnt_len);
    strcat(send_buff, str.c_str());
    
    sendData(send_buff);
    return 0;
}
