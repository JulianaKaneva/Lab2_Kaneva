#include "network.h"
#include <iostream>
#include <algorithm>

GasNetwork::GasNetwork() : nextConnectionId(1) {}

bool GasNetwork::addConnection(int pipeId, int startCSId, int endCSId) {
    // Проверка не используется ли уже труба
    if (usedPipes.find(pipeId) != usedPipes.end()) {
        return false;
    }

    // Проверка не участвуют ли КС уже в других соединениях
    for (const auto& conn : connections) {
        if (conn.second.startCSId == startCSId || conn.second.endCSId == endCSId ||
            conn.second.startCSId == endCSId || conn.second.endCSId == startCSId) {
            return false;
        }
    }

    Connection conn(pipeId, startCSId, endCSId);
    connections[nextConnectionId] = conn;
    usedPipes.insert(pipeId);

    // Обновление списока смежности
    adjacencyList[startCSId].push_back(endCSId);

    nextConnectionId++;
    return true;
}

bool GasNetwork::removeConnection(int pipeId) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->second.pipeId == pipeId) {
            // Удаляение из списка смежности
            int startCSId = it->second.startCSId;
            int endCSId = it->second.endCSId;

            auto& neighbors = adjacencyList[startCSId];
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), endCSId), neighbors.end());

            // Если у КС больше нет исходящих соединений, удаляем ее из списка смежности
            if (neighbors.empty()) {
                adjacencyList.erase(startCSId);
            }

            connections.erase(it);
            usedPipes.erase(pipeId);
            return true;
        }
    }
    return false;
}

bool GasNetwork::removeConnectionsWithCS(int csId) {
    bool removed = false;
    for (auto it = connections.begin(); it != connections.end();) {
        if (it->second.startCSId == csId || it->second.endCSId == csId) {
            // Освобождение трубы
            usedPipes.erase(it->second.pipeId);

            // Удаление из списка смежности
            int startCSId = it->second.startCSId;
            int endCSId = it->second.endCSId;

            auto& neighbors = adjacencyList[startCSId];
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), endCSId), neighbors.end());

            if (neighbors.empty()) {
                adjacencyList.erase(startCSId);
            }

            it = connections.erase(it);
            removed = true;
        }
        else {
            ++it;
        }
    }
    return removed;
}

bool GasNetwork::isPipeUsed(int pipeId) const {
    return usedPipes.find(pipeId) != usedPipes.end();
}

bool GasNetwork::isCSInConnection(int csId) const {
    for (const auto& conn : connections) {
        if (conn.second.startCSId == csId || conn.second.endCSId == csId) {
            return true;
        }
    }
    return false;
}

void GasNetwork::markPipeAsUsed(int pipeId) {
    usedPipes.insert(pipeId);
}

void GasNetwork::markPipeAsFree(int pipeId) {
    usedPipes.erase(pipeId);
}

const std::unordered_map<int, Connection>& GasNetwork::getConnections() const {
    return connections;
}

std::vector<int> GasNetwork::topologicalSort() const {
    if (hasCycle()) {
        std::cout << "Граф содержит циклы, топологическая сортировка невозможна!\n";
        return {};
    }

    std::vector<int> result;
    std::vector<bool> visited(1000, false);

    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            topologicalSortUtil(pair.first, visited, result);
        }
    }

    // Добавление изолированных вершины
    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            result.push_back(pair.first);
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}

void GasNetwork::topologicalSortUtil(int v, std::vector<bool>& visited, std::vector<int>& stack) const {
    visited[v] = true;

    auto it = adjacencyList.find(v);
    if (it != adjacencyList.end()) {
        for (int neighbor : it->second) {
            if (!visited[neighbor]) {
                topologicalSortUtil(neighbor, visited, stack);
            }
        }
    }

    stack.push_back(v);
}

bool GasNetwork::hasCycle() const {
    std::vector<bool> visited(1000, false);
    std::vector<bool> recStack(1000, false);

    for (const auto& pair : adjacencyList) {
        if (!visited[pair.first]) {
            if (hasCycleUtil(pair.first, visited, recStack)) {
                return true;
            }
        }
    }
    return false;
}

bool GasNetwork::hasCycleUtil(int v, std::vector<bool>& visited, std::vector<bool>& recStack) const {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        auto it = adjacencyList.find(v);
        if (it != adjacencyList.end()) {
            for (int neighbor : it->second) {
                if (!visited[neighbor] && hasCycleUtil(neighbor, visited, recStack)) {
                    return true;
                }
                else if (recStack[neighbor]) {
                    return true;
                }
            }
        }
    }
    recStack[v] = false;
    return false;
}

void GasNetwork::printNetwork() const {
    std::cout << "\nГазотранспортная сеть:\n";
    if (connections.empty()) {
        std::cout << "Нет соединений\n";
        return;
    }

    for (const auto& pair : connections) {
        const Connection& conn = pair.second;
        std::cout << "Соединение ID " << pair.first << ": КС " << conn.startCSId
            << " -> КС " << conn.endCSId << " (труба ID " << conn.pipeId << ")\n";
    }
}

void GasNetwork::clear() {
    connections.clear();
    adjacencyList.clear();
    usedPipes.clear();
    nextConnectionId = 1;
}