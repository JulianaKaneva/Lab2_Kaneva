#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <unordered_map> //библиотека хэш-таблиц для быстрого поиска id
#include <vector> //библиотека списков
#include <string>
#include "pipe.h"
#include "cs.h"
#include "network.h"

class DataManager {
private:
    std::unordered_map<int, Pipe> pipes; //таблица id-труба
    std::unordered_map<int, CompressorStation> stations; //таблица id-станция
    GasNetwork network;
    int nextPipeId; //счетчик для следующего id трубы
    int nextStationId; //счетчик для следующего id станции

public:
    DataManager();
    const std::unordered_map<int, Pipe>& getPipes() const; //доступ ко всем трубам 
    const std::unordered_map<int, CompressorStation>& getStations() const; //доступ ко всем станциям
    std::vector<int> getPipeIds() const; //возвращает список id труб
    std::vector<int> getStationIds() const; //возвращает список id станций
    int addPipe(const std::string& name, double length, double diameter, bool underRepair); //добавление трубы
    bool editPipe(int id, bool underRepair); //редактирует статус ремонта
    bool deletePipe(int id); // удаление трубы по id
    Pipe* getPipe(int id); // возвращает указатель к конкретной станции
    int addStation(const std::string& name, int totalWorkshops, int workingWorkshops, double classCS); 
    bool editStation(int id, int workingWorkshops);
    bool deleteStation(int id);
    CompressorStation* getStation(int id);
    std::vector<int> findPipesByName(const std::string& name) const; //поиск труб по названию
    std::vector<int> findPipesByRepairStatus(bool underRepair) const; //поиск труб по статусу ремонта
    std::vector<int> findStationsByName(const std::string& name) const; 
    std::vector<int> findStationsByUnusedPercentage(double minPercentage) const;
    void batchEditPipes(const std::vector<int>& pipeIds, bool underRepair); //пакетное редактирование труб
    bool saveToFile(const std::string& filename) const; //сохранение данных в файл
    bool loadFromFile(const std::string& filename); //загрузка данных из файла
    bool pipeExists(int id) const; //проверка существования трубы
    bool stationExists(int id) const; //проверка существования станции
    GasNetwork& getNetwork();
    const GasNetwork& getNetwork() const;
    std::vector<int> findPipesByDiameter(double diameter, bool onlyUnused = true) const;
    bool connectStations(int startCSId, int endCSId, double diameter);
    void disconnectStations(int pipeId);
    void showNetwork() const;
    void topologicalSort() const;
};

#endif
