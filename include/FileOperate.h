#ifndef __FILEOPERATE_H__
#define __FILEOPERATE_H__
#include <string>


class FileOperate{
public:
    int open(const std::string& file_name);
    void readAllFromFile(const std::string& file_name, std::string& read_buff);
    void readLineFromFile(const std::string& file_name, std::string& line, std::string& read_buff);
    void writeAllIntoFile(const std::string& file_name, const std::string& write_buff);
    void writeApendIntoFile(const std::string& file_name, const std::string& write_buff);
private:

};





#endif
