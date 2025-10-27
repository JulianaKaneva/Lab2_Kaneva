#include "utils.h"

namespace utils {
    double inputPositiveDouble(const std::string& prompt) { //������� ����� 
        double number;
        while (true) {
            std::cout << prompt; //��������� ��� �����
            std::cin >> number; //��������� ������ �� ������������
            if (std::cin.fail() || number <= 0) { //��������� ������������
                std::cout << "������� ������������� �����: ";
                std::cin.clear();
                clearInputBuffer(); //������� ������������ ������ �� ������
            }
            else {
                clearInputBuffer(); //������ ����� �����
                return number;
            }
        }
    }
    int inputPositiveInt(const std::string& prompt) {
        int number;
        while (true) {
            std::cout << prompt;
            std::cin >> number;
            if (std::cin.fail() || number <= 0) {
                std::cout << "������� ������������� ����� �����: ";
                std::cin.clear();
                clearInputBuffer();
            }
            else {
                clearInputBuffer();
                return number;
            }
        }
    }
    bool inputBool(const std::string& prompt) {
        int status;
        while (true) {
            std::cout << prompt;
            std::cin >> status;
            if (std::cin.fail() || (status != 0 && status != 1)) {
                std::cout << "������� 0 (false) ��� 1 (true): ";
                std::cin.clear();
                clearInputBuffer();
            }
            else {
                clearInputBuffer();
                return status == 1;
            }
        }
    }
    std::string inputString(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin >> std::ws, input); //��������� ������ � �������������� ��������
        return input;
    }
    void clearInputBuffer() { // �������� ������ ����� �� ������� ����� ������
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

}