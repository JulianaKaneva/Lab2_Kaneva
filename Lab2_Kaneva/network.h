#ifndef NETWORK_H
#define NETWORK_H

#include <unordered_map>
#include <vector>
#include <set>
#include <stack>
#include "pipe.h"
#include "cs.h"
#include <queue> // Очередь (FIFO) для топологической сортировки
#include <utility>
#include <string>

struct Connection {
    int pipeId;
    int startCSId;
    int endCSId;
    bool isActive;
    // Конструктор по умолчанию
    Connection() : pipeId(0), startCSId(0), endCSId(0), isActive(false) {}
    // Существующий конструктор
    Connection(int pipe, int start, int end)
        : pipeId(pipe), startCSId(start), endCSId(end), isActive(true) {
    }
};

class GasNetwork {
private:
    std::unordered_map<int, Connection> connections; // Хранилище всех соединений
    std::unordered_map<int, std::vector<int>> adjacencyList; // Граф в виде списка смежности (id начала - id конца)
    std::set<int> usedPipes; // Множество ID труб, которые уже используются в соединениях
    int nextConnectionId = 1;
    // Вспомогательные методы для алгоритмов
    std::unordered_map<int, std::vector<std::pair<int, double>>>
        getWeightedGraph(const std::unordered_map<int, Pipe>& pipes,
            bool useCapacity = false) const;

    bool bfsForMaxFlow(int source, int sink,
        const std::unordered_map<int, std::vector<std::pair<int, double>>>& adj,
        std::unordered_map<int, int>& parent) const;

    std::vector<int> dijkstra(int start, int end,
        const std::unordered_map<int, std::vector<std::pair<int, double>>>& weightedAdj,
        std::unordered_map<int, double>& distances,
        std::unordered_map<int, int>& predecessors) const;

public:
    GasNetwork();

    bool addConnection(int pipeId, int startCSId, int endCSId);
    bool removeConnection(int pipeId);
    bool removeConnectionsWithCS(int csId);
    bool isPipeUsed(int pipeId) const; //Проверки
    bool isCSInConnection(int csId) const;
    void markPipeAsUsed(int pipeId); // Управление статусом
    void markPipeAsFree(int pipeId);
    const std::unordered_map<int, Connection>& getConnections() const; // Доступ к данным
    std::vector<int> topologicalSort() const; //Работа с графом
    void printNetwork() const;
    void clear();
    void setNextConnectionId(int id);
    // Новые методы для расчета максимального потока и кратчайшего пути
    double calculateMaxFlow(int sourceKS, int sinkKS,
        const std::unordered_map<int, Pipe>& pipes) const;
    std::vector<int> findShortestPath(int startKS, int endKS,
        const std::unordered_map<int, Pipe>& pipes) const;
    // Методы для отображения информации
    void showMaxFlowInfo(int sourceKS, int sinkKS,
        const std::unordered_map<int, Pipe>& pipes) const;
    void showShortestPathInfo(int startKS, int endKS,
        const std::unordered_map<int, Pipe>& pipes) const;
};

#endif