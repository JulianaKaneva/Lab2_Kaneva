#ifndef MENU_H
#define MENU_H
#include "data.h"
#include "pipe.h"       
#include "cs.h"

class Menu {
private:
    DataManager dataManager; //объект как поле класса
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
    void connectStations();
    void disconnectStations();
    void showNetwork();
    void topologicalSort();
    void calculateMaxFlow();
    void findShortestPath();

public:
    void run();
};

#endif