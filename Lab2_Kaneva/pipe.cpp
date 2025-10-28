#include "pipe.h"
#include <string> 

Pipe::Pipe() : id(0), length(0), diameter(0), underRepair(false) {} //����� � �������� ����������
Pipe::Pipe(int pipeId, const std::string& pipeName, double pipeLength,
    double pipeDiameter, bool repairStatus)
    : id(pipeId), name(pipeName), length(pipeLength),
    diameter(pipeDiameter), underRepair(repairStatus) {} //����� � ��������� �����������

int Pipe::getId() const { return id; } // ����������� ��������������
std::string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
double Pipe::getDiameter() const { return diameter; }
bool Pipe::isUnderRepair() const { return underRepair; }

void Pipe::setRepairStatus(bool status) {
    underRepair = status;
} //�������� ������ �����

std::string Pipe::toString() const {
    std::string result; //������ ������ ����������

    result = "����� ID: " + std::to_string(id) + "\n" +
        "  ������������ �������: " + name + "\n" +
        "  �����: " + std::to_string(length) + " ��\n" +
        "  �������: " + std::to_string(diameter) + " ��\n" +
        "  ������: " + (underRepair ? "� �������" : "��������");

    return result;
}