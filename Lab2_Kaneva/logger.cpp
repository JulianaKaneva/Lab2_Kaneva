#include "logger.h"
#include <ctime>  // библиотека для работы со временем

Logger::Logger() {
    logFile.open("time.log", std::ios::app); //открывает файл для записи в конец
}
Logger::~Logger() { //закрытие файла
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) { //запись сообщений в лог
    auto now = std::chrono::system_clock::now(); //текущее время
    auto time_t = std::chrono::system_clock::to_time_t(now); //преобразование в стандартный числовой формат
    if (logFile.is_open()) { //проверяет открыт ли файл
        std::tm timeInfo; //структура с полями (год, месяц, день, час, минута, секунда)
        localtime_s(&timeInfo, &time_t); //разложение на поля
        char timeBuffer[20]; //временное хранилище для даты
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo); //запись результата формата: "2025-10-15 14:30:25"
        logFile << timeBuffer << " - " << message << std::endl; //запись в файл и сброс буфера
    }
}

Logger& Logger::getInstance() {
    static Logger instance; //проверка создан ли объект (нет-создает новый, да-использует существующий)
    return instance; //возвращается ссылка на объект без создания копии
}