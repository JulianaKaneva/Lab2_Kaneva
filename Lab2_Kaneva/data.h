#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <unordered_map> //���������� ���-������ ��� �������� ������ id
#include <vector> //���������� �������
#include <string>
#include "pipe.h"
#include "cs.h"

class DataManager {
private:
    std::unordered_map<int, Pipe> pipes; //������� id-�����
    std::unordered_map<int, CompressorStation> stations; //������� id-�������
    int nextPipeId; //������� ��� ���������� id �����
    int nextStationId; //������� ��� ���������� id �������

public:
    DataManager();
    const std::unordered_map<int, Pipe>& getPipes() const; //������ �� ���� ������ 
    const std::unordered_map<int, CompressorStation>& getStations() const; //������ �� ���� ��������
    std::vector<int> getPipeIds() const; //���������� ������ id ����
    std::vector<int> getStationIds() const; //���������� ������ id �������
    int addPipe(const std::string& name, double length, double diameter, bool underRepair); //���������� �����
    bool editPipe(int id, bool underRepair); //����������� ������ �������
    bool deletePipe(int id); // �������� ����� �� id
    Pipe* getPipe(int id); // ���������� ��������� � ���������� �������
    int addStation(const std::string& name, int totalWorkshops, int workingWorkshops, double classCS); 
    bool editStation(int id, int workingWorkshops);
    bool deleteStation(int id);
    CompressorStation* getStation(int id);
    std::vector<int> findPipesByName(const std::string& name) const; //����� ���� �� ��������
    std::vector<int> findPipesByRepairStatus(bool underRepair) const; //����� ���� �� ������� �������
    std::vector<int> findStationsByName(const std::string& name) const; 
    std::vector<int> findStationsByUnusedPercentage(double minPercentage) const;
    void batchEditPipes(const std::vector<int>& pipeIds, bool underRepair); //�������� �������������� ����
    bool saveToFile(const std::string& filename) const; //���������� ������ � ����
    bool loadFromFile(const std::string& filename); //�������� ������ �� �����
    bool pipeExists(int id) const; //�������� ������������� �����
    bool stationExists(int id) const; //�������� ������������� �������
};

#endif
