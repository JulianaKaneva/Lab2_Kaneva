#ifndef PIPE_H //������ �� ���������� ��������� �����
#define PIPE_H //���������� ������ PIPE_H

#include <string>

class Pipe {
private: //������ �������� ������ ������ ������
    int id;
    std::string name;
    double length;
    double diameter;
    bool underRepair;
public: //������ �������� ����� ������
    Pipe(); //�������� �����
    Pipe(int pipeId, const std::string& pipeName, double pipeLength, double pipeDiameter, bool repairStatus); 
    // ������
    int getId() const;
    std::string getName() const;
    double getLength() const;
    double getDiameter() const;
    bool isUnderRepair() const;
    // ���������
    void setRepairStatus(bool status);  // ��������� �������
    std::string toString() const; //��������� ������������� ����� ��� ������
};

#endif