#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <limits> //библиотека ограничений

namespace utils { //группировка утилит
    double inputPositiveDouble(const std::string& prompt); //функция для ввода положительных дробных чисел
    int inputPositiveInt(const std::string& prompt); //ввод целого положительного числа
    bool inputBool(const std::string& prompt); //ввод булевого знаечния
    std::string inputString(const std::string& prompt); //ввод текста
    void clearInputBuffer(); //очистка буфера для обработки некорректного ввода
}

#endif