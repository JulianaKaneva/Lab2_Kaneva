#ifndef PIPE_H //Защита от повторного включения файла
#define PIPE_H //Определяет символ PIPE_H

#include <string>

class Pipe {
private: //Данные доступны только внутри класса
    int id;
    std::string name;
    double length;
    double diameter;
    bool underRepair;
public: //Методы доступны извне класса
    Pipe(); //Создание трубы
    Pipe(int pipeId, const std::string& pipeName, double pipeLength, double pipeDiameter, bool repairStatus); 
    // чтение
    int getId() const;
    std::string getName() const;
    double getLength() const;
    double getDiameter() const;
    bool isUnderRepair() const;
    // изменение
    void setRepairStatus(bool status);  // Изменение статуса
    std::string toString() const; //текстовое представление трубы для вывода
};

#endif