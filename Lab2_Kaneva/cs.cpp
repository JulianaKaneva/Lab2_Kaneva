#include "cs.h"
#include <iostream>
#include <string>

CompressorStation::CompressorStation()
    : id(0), totalWorkshops(0), workingWorkshops(0), classCS(0) {
} //станция с нулевыми значениями

CompressorStation::CompressorStation(int stationId, const std::string& stationName,
    int workshops, int working, double classCS)
    : id(stationId), name(stationName), totalWorkshops(workshops),
    workingWorkshops(working), classCS(classCS) {
} //станция с заданными параметрами

int CompressorStation::getId() const { return id; }
std::string CompressorStation::getName() const { return name; }
int CompressorStation::getTotalWorkshops() const { return totalWorkshops; }
int CompressorStation::getWorkingWorkshops() const { return workingWorkshops; }
double CompressorStation::getClassCS() const { return classCS; }

void CompressorStation::setWorkingWorkshops(int count) {
    workingWorkshops = count;
} //изменение количества работающих станций

double CompressorStation::getUnusedPercentage() const {
    if (totalWorkshops == 0) return 0.0;
    return 100.0 * (totalWorkshops - workingWorkshops) / totalWorkshops;
}

std::string CompressorStation::toString() const { //текстовое представление станции
    std::string result;

    result = "КС ID: " + std::to_string(id) + "\n" +
        "  Название: " + name + "\n" +
        "  Всего цехов: " + std::to_string(totalWorkshops) + "\n" +
        "  Работающих цехов: " + std::to_string(workingWorkshops) + "\n" +
        "  Процент незадействованных цехов: " + std::to_string(getUnusedPercentage()) + "%\n" +
        "  Класс станции: " + std::to_string(classCS);

    return result;
}