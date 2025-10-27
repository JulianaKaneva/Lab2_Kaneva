#include "utils.h"

namespace utils {
    double inputPositiveDouble(const std::string& prompt) { //функци€ ввода 
        double number;
        while (true) {
            std::cout << prompt; //подсказка дл€ ввода
            std::cin >> number; //считывает данные от пользовател€
            if (std::cin.fail() || number <= 0) { //провер€ет корректность
                std::cout << "¬ведите положительное число: ";
                std::cin.clear();
                clearInputBuffer(); //удал€ет некорректные данные из буфера
            }
            else {
                clearInputBuffer(); //чистит буфер ввода
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
                std::cout << "¬ведите положительное целое число: ";
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
                std::cout << "¬ведите 0 (false) или 1 (true): ";
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
        std::getline(std::cin >> std::ws, input); //считывает строку с игнорированием пробелов
        return input;
    }
    void clearInputBuffer() { // очищение буфера ввода до символа новой строки
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

}