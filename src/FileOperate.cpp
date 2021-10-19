#include "../include/FileOperate.h"
#include <fstream>

void FileOperate::readAllFromFile(const std::string& file_name, std::string& read_buff) {
    std::ifstream readFrom(file_name, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(readFrom)), std::istreambuf_iterator<char>(0));
    read_buff = std::move(str);
    readFrom.close();
}

void FileOperate::readLineFromFile(const std::string& file_name, std::string& line, std::string& read_buff) {
    std::ifstream readFrom(file_name, std::ifstream::in);
    readFrom.close();
}

void writeIntoFile(const std::string& file_name, const std::string& write_buff) {
    std::ofstream writeIn(file_name, std::ifstream::out);
    writeIn << write_buff;
    writeIn.close();
}

void writeApendIntoFile(const std::string& file_name, const std::string& write_buff) {
    std::ofstream writeIn(file_name, std::ifstream::app);
    writeIn << write_buff;
    writeIn.close();
}

