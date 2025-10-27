#ifndef COMPRESSOR_STATION_H //�������� �� ��� �� ��������� �����
#define COMPRESSOR_STATION_H //���� ������� 

#include <string>

class CompressorStation {
private: //������ ������ ������ ������
    int id;
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    double classCS;

public: //������ �������� ����� ������
    CompressorStation();
    CompressorStation(int stationId, const std::string& stationName,
        int workshops, int working, double ccs);
    int getId() const;
    std::string getName() const;
    int getTotalWorkshops() const;
    int getWorkingWorkshops() const;
    double getClassCS() const;
    void setWorkingWorkshops(int count); //��������� ���������� ���������� �����
    double getUnusedPercentage() const;
    std::string toString() const; //��������� ������������� ��� ������
};

#endif