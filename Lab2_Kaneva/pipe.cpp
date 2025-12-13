#include "pipe.h"
#include <string> 
#include <cmath>
#include <limits>

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
// Расчет пропускной способности по формуле sqrt(d^5 / l)
double Pipe::getCapacity() const {
    if (underRepair) {
        return 0.0;
    }

    double d_m = diameter / 1000.0;     // перевод диаметра в метры
    double l_m = length * 1000.0;       // перевод длины в метры

    double d_pow_5 = pow(d_m, 5);
    double capacity = sqrt(d_pow_5 / l_m);
    capacity *= 1000.0;                 // поправочный коэффициент

    return capacity;
}

// Вес для расчета пути (длина или бесконечность при ремонте)
double Pipe::getWeight() const {
    if (underRepair) {
        return std::numeric_limits<double>::infinity();
    }
    return length;  // простейший вариант - длина трубы
}
