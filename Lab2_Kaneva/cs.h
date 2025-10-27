#ifndef COMPRESSOR_STATION_H //проверка не был ли определен ранее
#define COMPRESSOR_STATION_H //файл включен 

#include <string>

class CompressorStation {
private: //данные только внутри класса
    int id;
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    double classCS;

public: //данные доступны извне класса
    CompressorStation();
    CompressorStation(int stationId, const std::string& stationName,
        int workshops, int working, double ccs);
    int getId() const;
    std::string getName() const;
    int getTotalWorkshops() const;
    int getWorkingWorkshops() const;
    double getClassCS() const;
    void setWorkingWorkshops(int count); //изменение количество работающих цехов
    double getUnusedPercentage() const;
    std::string toString() const; //текстовое представление для вывода
};

#endif