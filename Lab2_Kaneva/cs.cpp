#include "cs.h"
#include <iostream>
#include <string>

CompressorStation::CompressorStation()
    : id(0), totalWorkshops(0), workingWorkshops(0), classCS(0) {
} //������� � �������� ����������

CompressorStation::CompressorStation(int stationId, const std::string& stationName,
    int workshops, int working, double classCS)
    : id(stationId), name(stationName), totalWorkshops(workshops),
    workingWorkshops(working), classCS(classCS) {
} //������� � ��������� �����������

int CompressorStation::getId() const { return id; }
std::string CompressorStation::getName() const { return name; }
int CompressorStation::getTotalWorkshops() const { return totalWorkshops; }
int CompressorStation::getWorkingWorkshops() const { return workingWorkshops; }
double CompressorStation::getClassCS() const { return classCS; }

void CompressorStation::setWorkingWorkshops(int count) {
    workingWorkshops = count;
} //��������� ���������� ���������� �������

double CompressorStation::getUnusedPercentage() const {
    if (totalWorkshops == 0) return 0.0;
    return 100.0 * (totalWorkshops - workingWorkshops) / totalWorkshops;
}

std::string CompressorStation::toString() const { //��������� ������������� �������
    std::string result;

    result = "�� ID: " + std::to_string(id) + "\n" +
        "  ��������: " + name + "\n" +
        "  ����� �����: " + std::to_string(totalWorkshops) + "\n" +
        "  ���������� �����: " + std::to_string(workingWorkshops) + "\n" +
        "  ������� ����������������� �����: " + std::to_string(getUnusedPercentage()) + "%\n" +
        "  ����� �������: " + std::to_string(classCS);

    return result;
}