#ifndef LOGGER_H //�������� �� ��������� �� ��� LOGGER_H
#define LOGGER_H //���� �������
#include <string>
#include <fstream>
#include <chrono> //���������� ������ �� ��������


class Logger {
private:
    std::ofstream logFile; //���������� � ����
public:
    Logger(); //�������� �����
    ~Logger(); //�������� �����
    void log(const std::string& message); //������ ��������� � ����
    static Logger& getInstance(); //���������� ������ �� ��������� ������
};

#endif