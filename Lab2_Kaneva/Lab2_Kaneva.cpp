#include <iostream> //библиотека для ввода/вывода данных
#include <clocale> //правильно отображать русские символы в консоли
#include "menu.h" 

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Система управления трубопровода\n";
    Menu menu; //создание объекта класса меню
    menu.run();
    return 0;
}