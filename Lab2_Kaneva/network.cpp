#include "network.h"
#include <iostream>
#include <algorithm>

GasNetwork::GasNetwork() : nextConnectionId(1) {}

bool GasNetwork::addConnection(int pipeId, int startCSId, int endCSId) {
    // Проверка не используется ли уже труба
    if (usedPipes.find(pipeId) != usedPipes.end()) {
        return false;
    }

    // Проверяем, что не соединяем КС саму с собой
    if (startCSId == endCSId) {
        std::cout << "Ошибка: Нельзя соединить КС саму с собой!\n";
        return false;
    }

    // Проверка на дублирование соединения
    for (const auto& conn : connections) {
        if (conn.second.startCSId == startCSId && conn.second.endCSId == endCSId) {
            std::cout << "Ошибка: Соединение КС " << startCSId << " - КС " << endCSId
                << " уже существует!\n";
            return false;
        }
    }

    Connection conn(pipeId, startCSId, endCSId);
    connections[nextConnectionId] = conn;
    usedPipes.insert(pipeId);

    // Обновление списока смежности
    adjacencyList[startCSId].push_back(endCSId);
    std::cout << "Соединение создано: КС " << startCSId << " - КС " << endCSId
        << " (труба ID " << pipeId << ")\n";
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
            std::cout << "Соединение разорвано: труба ID " << pipeId
                << " освобождена (КС " << startCSId << " - КС " << endCSId << ")\n";
            return true;
        }
    }
    return false;
}

bool GasNetwork::removeConnectionsWithCS(int csId) {
    bool removed = false;
    std::vector<std::tuple<int, int, int>> removedConnections;
    for (auto it = connections.begin(); it != connections.end();) {
        if (it->second.startCSId == csId || it->second.endCSId == csId) {
            // Сохранениее информацию об удаляемом соединении
            removedConnections.push_back({
                it->first,
                it->second.startCSId,
                it->second.endCSId
            });
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
    if (!removedConnections.empty()) {
        std::cout << "Разорваны соединения с КС ID " << csId << ":\n";
        for (const auto& conn : removedConnections) {
            std::cout << "Соединение " << std::get<0>(conn)
                << ": КС " << std::get<1>(conn)
                << " → КС " << std::get<2>(conn) << "\n";
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
    if (adjacencyList.empty()) {
        return {};
    }

    // Подсчет входящих степеней
    std::unordered_map<int, int> inDegree;
    for (const auto& pair : adjacencyList) {
        inDegree[pair.first]; // Инициализация
        for (int neighbor : pair.second) {
            inDegree[neighbor]++;
        }
    }

    // Алгоритм Кана
    std::queue<int> q;
    for (const auto& pair : inDegree) {
        if (pair.second == 0) {
            q.push(pair.first);
        }
    }

    std::vector<int> result;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);

        if (adjacencyList.find(node) != adjacencyList.end()) {
            for (int neighbor : adjacencyList.at(node)) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
    }

    // Проверка на циклы
    if (result.size() != inDegree.size()) {
        std::cout << "Граф содержит циклы, топологическая сортировка невозможна!\n";
        return {};
    }

    return result;
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