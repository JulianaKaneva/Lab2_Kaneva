#include "pipe.h"
#include <string> 

Pipe::Pipe() : id(0), length(0), diameter(0), underRepair(false) {} //труба с нулевыми значениями
Pipe::Pipe(int pipeId, const std::string& pipeName, double pipeLength,
    double pipeDiameter, bool repairStatus)
    : id(pipeId), name(pipeName), length(pipeLength),
    diameter(pipeDiameter), underRepair(repairStatus) {} //Труба с заданными параметрами

int Pipe::getId() const { return id; } // возвращение идентификатора
std::string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
double Pipe::getDiameter() const { return diameter; }
bool Pipe::isUnderRepair() const { return underRepair; }

void Pipe::setRepairStatus(bool status) {
    underRepair = status;
} //изменить статус трубы

std::string Pipe::toString() const {
    std::string result; //пустая строка результата

    result = "Труба ID: " + std::to_string(id) + "\n" +
        "  Километровая отметка: " + name + "\n" +
        "  Длина: " + std::to_string(length) + " км\n" +
        "  Диаметр: " + std::to_string(diameter) + " мм\n" +
        "  Статус: " + (underRepair ? "В ремонте" : "Работает");

    return result;
}