#include <iostream> //библиотека для ввода/вывода данных
#include <clocale> //правильно отображать русские символы в консоли
#include "menu.h" 

int main() {
    setlocale(LC_ALL, "Russian");
    Menu menu; //создание объекта класса меню
    menu.run();
    return 0;
}