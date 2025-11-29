#ifndef NETWORK_H
#define NETWORK_H

#include <unordered_map>
#include <vector>
#include <set>
#include <stack>
#include "pipe.h"
#include "cs.h"

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
    std::unordered_map<int, Connection> connections;
    std::unordered_map<int, std::vector<int>> adjacencyList;
    std::set<int> usedPipes;
    int nextConnectionId = 1;

public:
    GasNetwork();

    bool addConnection(int pipeId, int startCSId, int endCSId);
    bool removeConnection(int pipeId);
    bool removeConnectionsWithCS(int csId);
    bool isPipeUsed(int pipeId) const;
    bool isCSInConnection(int csId) const;
    void markPipeAsUsed(int pipeId);
    void markPipeAsFree(int pipeId);
    const std::unordered_map<int, Connection>& getConnections() const;
    std::vector<int> topologicalSort() const;
    void printNetwork() const;
    void clear();

private:
    bool hasCycle() const;
    bool hasCycleUtil(int v, std::vector<bool>& visited, std::vector<bool>& recStack) const;
    void topologicalSortUtil(int v, std::vector<bool>& visited, std::vector<int>& stack) const;
};

#endif