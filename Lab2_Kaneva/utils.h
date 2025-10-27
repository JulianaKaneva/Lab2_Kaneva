#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <limits> //���������� �����������

namespace utils { //����������� ������
    double inputPositiveDouble(const std::string& prompt); //������� ��� ����� ������������� ������� �����
    int inputPositiveInt(const std::string& prompt); //���� ������ �������������� �����
    bool inputBool(const std::string& prompt); //���� �������� ��������
    std::string inputString(const std::string& prompt); //���� ������
    void clearInputBuffer(); //������� ������ ��� ��������� ������������� �����
}

#endif