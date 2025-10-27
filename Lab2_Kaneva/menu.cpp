#include "menu.h"
#include "utils.h"
#include "logger.h"
#include <iostream>
#include <sstream>

void Menu::run() {
    showMainMenu();
}

void Menu::showMainMenu() {
    int choice;
    while (true) {
        std::cout << "\n����\n";
        std::cout << "1. �������� �����\n";
        std::cout << "2. �������� ��\n";
        std::cout << "3. �������� ���� ��������\n";
        std::cout << "4. ������������� �����\n";
        std::cout << "5. ������������� ��\n";
        std::cout << "6. ������� �����\n";
        std::cout << "7. ������� ��\n";
        std::cout << "8. ����� ����\n";
        std::cout << "9. ����� ��\n";
        std::cout << "10. �������� �������������� ����\n";
        std::cout << "11. ��������� � ����\n";
        std::cout << "12. ��������� �� �����\n";
        std::cout << "0. �����\n";
        std::cout << "�������� ��������: ";

        std::cin >> choice;
        utils::clearInputBuffer();

        switch (choice) {
        case 1: addPipe(); break;
        case 2: addStation(); break;
        case 3: showAllObjects(); break;
        case 4: editPipe(); break;
        case 5: editStation(); break;
        case 6: deletePipe(); break;
        case 7: deleteStation(); break;
        case 8: searchPipes(); break;
        case 9: searchStations(); break;
        case 10: batchEditPipes(); break;
        case 11: saveToFile(); break;
        case 12: loadFromFile(); break;
        case 0:
            std::cout << "����� �� ���������.\n";
            return;
        default:
            std::cout << "�������� �����. ���������� �����.\n";
        }
    }
}

void Menu::addPipe() { //����� �����
    std::string name = utils::inputString("������������ ������� �����: ");
    double length = utils::inputPositiveDouble("����� (��): ");
    double diameter = utils::inputPositiveDouble("������� (��): ");
    bool underRepair = utils::inputBool("������ ������� (0 - ��������, 1 - � �������): ");

    int id = dataManager.addPipe(name, length, diameter, underRepair);
    std::cout << "����� ��������� � ID: " << id << std::endl;
}

void Menu::addStation() { //����� ��
    std::string name = utils::inputString("�������� ��: ");
    int totalWorkshops = utils::inputPositiveInt("���������� �����: ");

    int workingWorkshops;
    do {
        workingWorkshops = utils::inputPositiveInt("���������� ���������� �����: ");
        if (workingWorkshops > totalWorkshops) {
            std::cout << "���������� ���������� ����� �� ����� ��������� ����� ����������!\n";
        }
    } while (workingWorkshops > totalWorkshops);

    double classCS = utils::inputPositiveDouble("����� �������: ");

    int id = dataManager.addStation(name, totalWorkshops, workingWorkshops, classCS);
    std::cout << "�� ��������� � ID: " << id << std::endl;
}

void Menu::showAllObjects() { //��� �������
    std::cout << "\n��� �����\n";
    auto pipeIds = dataManager.getPipeIds();
    if (pipeIds.empty()) {
        std::cout << "����� �� ���������\n";
    }
    else {
        for (int id : pipeIds) {
            Pipe* pipe = dataManager.getPipe(id);
            if (pipe) {
                std::cout << pipe->toString() << "\n\n";
            }
        }
    }

    std::cout << "��� ������������� �������\n";
    auto stationIds = dataManager.getStationIds();
    if (stationIds.empty()) {
        std::cout << "�� �� ���������\n";
    }
    else {
        for (int id : stationIds) {
            CompressorStation* station = dataManager.getStation(id);
            if (station) {
                std::cout << station->toString() << "\n\n";
            }
        }
    }
}

void Menu::editPipe() { //�������������� �����
    int id = utils::inputPositiveInt("������� ID ����� ��� ��������������: ");

    if (!dataManager.pipeExists(id)) {
        std::cout << "����� � ID " << id << " �� �������.\n";
        return;
    }

    bool newStatus = utils::inputBool("����� ������ ������� (0 - ��������, 1 - � �������): ");
    dataManager.editPipe(id, newStatus);
    std::cout << "����� ���������������.\n";
}

void Menu::editStation() { //�������������� ��
    int id = utils::inputPositiveInt("������� ID �� ��� ��������������: ");

    if (!dataManager.stationExists(id)) {
        std::cout << "�� � ID " << id << " �� �������.\n";
        return;
    }

    CompressorStation* station = dataManager.getStation(id);
    if (!station) return;

    int newWorking;
    do {
        newWorking = utils::inputPositiveInt("����� ���������� ���������� �����: ");
        if (newWorking > station->getTotalWorkshops()) {
            std::cout << "���������� ���������� ����� �� ����� ��������� ����� ����������!\n";
        }
    } while (newWorking > station->getTotalWorkshops());

    dataManager.editStation(id, newWorking);
    std::cout << "�� ���������������.\n";
}

void Menu::deletePipe() { //�������� �����
    int id = utils::inputPositiveInt("������� ID ����� ��� ��������: ");

    if (!dataManager.pipeExists(id)) {
        std::cout << "����� � ID " << id << " �� �������.\n";
        return;
    }

    dataManager.deletePipe(id);
    std::cout << "����� �������.\n";
}

void Menu::deleteStation() { //�������� ��
    int id = utils::inputPositiveInt("������� ID �� ��� ��������: ");

    if (!dataManager.stationExists(id)) {
        std::cout << "�� � ID " << id << " �� �������.\n";
        return;
    }

    dataManager.deleteStation(id);
    std::cout << "�� �������.\n";
}

void Menu::searchPipes() { //���� ������ ����
    showPipeSearchMenu();
}

void Menu::searchStations() { //���� ������ ��
    showStationSearchMenu();
}

void Menu::showPipeSearchMenu() {
    int choice;
    std::cout << "\n����� ����\n";
    std::cout << "1. �� ��������\n";
    std::cout << "2. �� ������� �������\n";
    std::cout << "�������� ��� ������: ";

    std::cin >> choice;
    utils::clearInputBuffer();

    std::vector<int> results;

    switch (choice) {
    case 1: {
        std::string name = utils::inputString("������� ����� �������� ��� ������: ");
        results = dataManager.findPipesByName(name);
        break;
    }
    case 2: {
        bool status = utils::inputBool("������� ������ (0 - ��������, 1 - � �������): ");
        results = dataManager.findPipesByRepairStatus(status);
        break;
    }
    default:
        std::cout << "�������� �����.\n";
        return;
    }

    std::cout << "\n���������� ������ (" << results.size() << " �������):\n";
    for (int id : results) {
        Pipe* pipe = dataManager.getPipe(id);
        if (pipe) {
            std::cout << pipe->toString() << "\n\n";
        }
    }
}

void Menu::showStationSearchMenu() {
    int choice;
    std::cout << "\n����� ��\n";
    std::cout << "1. �� ��������\n";
    std::cout << "2. �� �������� ����������������� �����\n";
    std::cout << "�������� ��� ������: ";

    std::cin >> choice;
    utils::clearInputBuffer();

    std::vector<int> results;

    switch (choice) {
    case 1: {
        std::string name = utils::inputString("������� ����� �������� ��� ������: ");
        results = dataManager.findStationsByName(name);
        break;
    }
    case 2: {
        double percentage = utils::inputPositiveDouble("������� ����������� ������� ����������������� �����: ");
        results = dataManager.findStationsByUnusedPercentage(percentage);
        break;
    }
    default:
        std::cout << "�������� �����.\n";
        return;
    }

    std::cout << "\n���������� ������ (" << results.size() << " �������):\n";
    for (int id : results) {
        CompressorStation* station = dataManager.getStation(id);
        if (station) {
            std::cout << station->toString() << "\n\n";
        }
    }
}

void Menu::batchEditPipes() {
    std::cout << "\n�������� �������������� ����\n";

    //����� ���� ��� ��������������
    showPipeSearchMenu();

    std::vector<int> pipeIds;
    std::string input;

    std::cout << "������� ID ���� ��� �������������� (����� ������): ";
    std::getline(std::cin, input);

    // ������ �������
    std::stringstream ss(input);
    int id;
    while (ss >> id) {
        if (dataManager.pipeExists(id)) {
            pipeIds.push_back(id);
        }
        else {
            std::cout << "����� � ID " << id << " �� ����������.\n";
        }
    }

    if (pipeIds.empty()) {
        std::cout << "��� �������� ID ��� ��������������.\n";
        return;
    }

    bool newStatus = utils::inputBool("����� ������ ������� ��� ��������� ���� (0 - ��������, 1 - � �������): ");

    dataManager.batchEditPipes(pipeIds, newStatus);
    std::cout << "������ " << pipeIds.size() << " ���� �������.\n";
}

void Menu::saveToFile() { //���������� � ����
    std::string filename = utils::inputString("������� ��� ����� ��� ����������: ");
    if (dataManager.saveToFile(filename)) {
        std::cout << "������ ������� ��������� � ����: " << filename << std::endl;
    }
    else {
        std::cout << "������ ���������� � ����: " << filename << std::endl;
    }
}

void Menu::loadFromFile() { //�������� �� �����
    std::string filename = utils::inputString("������� ��� ����� ��� ��������: ");
    if (dataManager.loadFromFile(filename)) {
        std::cout << "������ ������� ��������� �� �����: " << filename << std::endl;
    }
    else {
        std::cout << "������ �������� �� �����: " << filename << std::endl;
    }
}