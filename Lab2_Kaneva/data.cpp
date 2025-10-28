#include "data.h"
#include "logger.h"
#include <fstream>

DataManager::DataManager() : nextPipeId(1), nextStationId(1) {} //������������� �������� � 1

const std::unordered_map<int, Pipe>& DataManager::getPipes() const { return pipes; } //������ ������ ��� ���������
const std::unordered_map<int, CompressorStation>& DataManager::getStations() const { return stations; }

std::vector<int> DataManager::getPipeIds() const { //id ���� � ������
    std::vector<int> ids; //������ ������
    for (const auto& pair : pipes) { //��� �������� �������
        ids.push_back(pair.first); //��������� id � ������
    }
    return ids;
}

std::vector<int> DataManager::getStationIds() const {
    std::vector<int> ids;
    for (const auto& pair : stations) {
        ids.push_back(pair.first);
    }
    return ids;
}

int DataManager::addPipe(const std::string& name, double length, double diameter, bool underRepair) { //�������, ��������� � �������� �����
    Pipe newPipe(nextPipeId, name, length, diameter, underRepair);
    pipes[newPipe.getId()] = newPipe; //���������� � ������� �� id

    Logger::getInstance().log("��������� ����� ID: " + std::to_string(newPipe.getId()));
    return nextPipeId++;
}

bool DataManager::editPipe(int id, bool underRepair) { //������� ����� � ������ ������
    auto it = pipes.find(id);
    if (it != pipes.end()) {
        it->second.setRepairStatus(underRepair); //������ �� ����� �� ��������
        Logger::getInstance().log("�������� ����� ID: " + std::to_string(id) +
            ", ����� ������: " + (underRepair ? "� �������" : "��������"));
        return true;
    }
    return false;
}

bool DataManager::deletePipe(int id) { //�������� �����
    if (pipes.erase(id) > 0) { //������� ������ 0 ����
        Logger::getInstance().log("������� ����� ID: " + std::to_string(id));
        return true;
    }
    return false;
}

Pipe* DataManager::getPipe(int id) { //���� ����� � ���������� ���������
    auto it = pipes.find(id);
    return it != pipes.end() ? &it->second : nullptr;
}

int DataManager::addStation(const std::string& name, int totalWorkshops, int workingWorkshops, double classCS) {
    CompressorStation newStation(nextStationId, name, totalWorkshops, workingWorkshops, classCS);
    stations[newStation.getId()] = newStation;

    Logger::getInstance().log("��������� �� ID: " + std::to_string(newStation.getId()));
    return nextStationId++;
}

bool DataManager::editStation(int id, int workingWorkshops) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        it->second.setWorkingWorkshops(workingWorkshops);
        Logger::getInstance().log("�������� �� ID: " + std::to_string(id) +
            ", ���������� �����: " + std::to_string(workingWorkshops));
        return true;
    }
    return false;
}

bool DataManager::deleteStation(int id) {
    if (stations.erase(id) > 0) {
        Logger::getInstance().log("������� �� ID: " + std::to_string(id));
        return true;
    }
    return false;
}

CompressorStation* DataManager::getStation(int id) {
    auto it = stations.find(id);
    return it != stations.end() ? &it->second : nullptr; // ���� ������, �� ����� ���������  
}

std::vector<int> DataManager::findPipesByName(const std::string& name) const { //����� �� ���������� ���������� �����
    std::vector<int> result;
    for (const auto& pair : pipes) { // �� ���� ��������� � ��������� ����� ���� �� �������
        if (pair.second.getName().find(name) != std::string::npos) { //���� ������ ���������� � �����
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> DataManager::findPipesByRepairStatus(bool underRepair) const {
    std::vector<int> result;
    for (const auto& pair : pipes) {
        if (pair.second.isUnderRepair() == underRepair) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> DataManager::findStationsByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result.push_back(pair.first);
        }
    }
    return result;
}

void DataManager::batchEditPipes(const std::vector<int>& pipeIds, bool underRepair) { //��������� � ������ ����
    for (int id : pipeIds) {
        editPipe(id, underRepair);
    }
    Logger::getInstance().log("�������� �������������� " + std::to_string(pipeIds.size()) +
        " ����, ������: " + (underRepair ? "� �������" : "��������"));
}

bool DataManager::saveToFile(const std::string& filename) const { //���������� � ��������� ����
    std::ofstream file(filename); //����� ��� ������
    if (!file.is_open()) return false;

    file << "Pipes " << pipes.size() << "\n";
    for (const auto& pair : pipes) {
        const Pipe& p = pair.second;
        file << p.getId() << "\n" << p.getName() << "\n" << p.getLength() << "\n"
            << p.getDiameter() << "\n" << p.isUnderRepair() << "\n";
    }

    file << "Stations " << stations.size() << "\n";
    for (const auto& pair : stations) {
        const CompressorStation& cs = pair.second;
        file << cs.getId() << "\n" << cs.getName() << "\n" << cs.getTotalWorkshops() << "\n"
            << cs.getWorkingWorkshops() << "\n" << cs.getClassCS() << "\n";
    }

    file << "Next " << nextPipeId << " " << nextStationId << "\n";

    file.close();
    Logger::getInstance().log("���������� ������ � ����: " + filename);
    return true;
}

bool DataManager::loadFromFile(const std::string& filename) { //��������������� ������ �� �����
    std::ifstream file(filename);
    if (!file.is_open()) return false; 

    pipes.clear();
    stations.clear();

    std::string section; //�������� �������� ������
    int count;

    while (file >> section >> count) { //���� ���� ������
        if (section == "Pipes") {
            for (int i = 0; i < count; ++i) {
                int id;
                std::string name;
                double length, diameter;
                bool underRepair;

                file >> id;
                file.ignore();
                std::getline(file, name);
                file >> length >> diameter >> underRepair;

                pipes[id] = Pipe(id, name, length, diameter, underRepair);
            }
        }
        else if (section == "Stations") {
            for (int i = 0; i < count; ++i) {
                int id, totalWorkshops, workingWorkshops;
                std::string name;
                double classCS;

                file >> id;
                file.ignore();
                std::getline(file, name);
                file >> totalWorkshops >> workingWorkshops >> classCS;

                stations[id] = CompressorStation(id, name, totalWorkshops, workingWorkshops, classCS);
            }
        }
        else if (section == "Next") { //������ �� ��������� id
            file >> nextPipeId >> nextStationId; //���������
        }
    }

    file.close();
    Logger::getInstance().log("�������� ������ �� �����: " + filename);
    return true;
}

bool DataManager::pipeExists(int id) const { //������� ����� �� id
    return pipes.find(id) != pipes.end();
}

bool DataManager::stationExists(int id) const {
    return stations.find(id) != stations.end();
}

std::vector<int> DataManager::findStationsByUnusedPercentage(double minPercentage) const {
    std::vector<int> result; //�������� ��������� ID
    for (const auto& pair : stations) {
        const CompressorStation& cs = pair.second; //������ �� ������
        if (cs.getTotalWorkshops() > 0) {
            double unusedPercentage = 100.0 * (cs.getTotalWorkshops() - cs.getWorkingWorkshops()) / cs.getTotalWorkshops();
            if (unusedPercentage >= minPercentage) {
                result.push_back(pair.first);
            }
        }
    }
    return result;
}