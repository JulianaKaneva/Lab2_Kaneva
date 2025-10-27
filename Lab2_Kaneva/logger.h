#ifndef LOGGER_H //проверка не определен ли уже LOGGER_H
#define LOGGER_H //файл включен
#include <string>
#include <fstream>
#include <chrono> //библиотека работы со временем


class Logger {
private:
    std::ofstream logFile; //сохранение в файл
public:
    Logger(); //открытие файла
    ~Logger(); //закрытие файла
    void log(const std::string& message); //запись сообщений в файл
    static Logger& getInstance(); //возвращает ссылку на экземпл€р класса
};

#endif