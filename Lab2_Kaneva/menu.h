#ifndef MENU_H
#define MENU_H
#include "data.h"
#include "pipe.h"       
#include "cs.h"

class Menu {
private:
    DataManager dataManager; //������ ��� ���� ������
    void showMainMenu();
    void addPipe();
    void addStation();
    void showAllObjects();
    void editPipe();
    void editStation();
    void deletePipe();
    void deleteStation();
    void searchPipes();
    void searchStations();
    void batchEditPipes();
    void saveToFile();
    void loadFromFile();
    void showPipeSearchMenu();
    void showStationSearchMenu();

public:
    void run();
};

#endif