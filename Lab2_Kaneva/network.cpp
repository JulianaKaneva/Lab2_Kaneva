#include "network.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <set>

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
    // Создание и сохранение нового соединения
    Connection conn(pipeId, startCSId, endCSId); //создание объекта
    connections[nextConnectionId] = conn; //сохранение в таблицу
    usedPipes.insert(pipeId); //труба в множество использованных

    // Обновление списока смежности
    adjacencyList[startCSId].push_back(endCSId); //добавление конечной кс для начальной
    std::cout << "Соединение создано: КС " << startCSId << " - КС " << endCSId
        << " (труба ID " << pipeId << ")\n";
    nextConnectionId++;
    return true;
}

bool GasNetwork::removeConnection(int pipeId) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->second.pipeId == pipeId) { //если труба совпадает
            //Сохранение данных 
            int startCSId = it->second.startCSId;
            int endCSId = it->second.endCSId;
            //Удаление из списка смежностей
            auto& neighbors = adjacencyList[startCSId]; //Ссылка на вектор смежных кс
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), endCSId), neighbors.end()); //Удаление вхождений (remove перемещает в начало, erase удаляет ненужный конец)

            // Если у КС больше нет исходящих соединений, удаляем ее из списка смежности
            if (neighbors.empty()) {
                adjacencyList.erase(startCSId);
            }

            connections.erase(it); //удаление соединения из таблицы
            usedPipes.erase(pipeId); //освобождение трубы
            std::cout << "Соединение разорвано: труба ID " << pipeId
                << " освобождена (КС " << startCSId << " - КС " << endCSId << ")\n";
            return true;
        }
    }
    return false;
}

bool GasNetwork::removeConnectionsWithCS(int csId) {
    bool removed = false; //флаг на удаление
    std::vector<std::tuple<int, int, int>> removedConnections; //хранение информации об удаленных соединениях
    for (auto it = connections.begin(); it != connections.end();) {
        if (it->second.startCSId == csId || it->second.endCSId == csId) { //кс как начало или конец соединения
            // Сохранение информации об удаляемом соединении
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

            if (neighbors.empty()) { //удаление соединения
                adjacencyList.erase(startCSId);
            }

            it = connections.erase(it); //итератор на следующий после удаленного
            removed = true; //удаление было
        }
        else {
            ++it;
        }
    }
    if (!removedConnections.empty()) { //если кортеж удаления не пустой
        std::cout << "Разорваны соединения с КС ID " << csId << ":\n";
        for (const auto& conn : removedConnections) {
            std::cout << "Соединение " << std::get<0>(conn)
                << ": КС " << std::get<1>(conn)
                << " -> КС " << std::get<2>(conn) << "\n";
        }
    }
    return removed;
}

//Проверка трубы
bool GasNetwork::isPipeUsed(int pipeId) const {
    return usedPipes.find(pipeId) != usedPipes.end();
}
//Проверка кс
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
    std::unordered_map<int, int> inDegree; //количество вхождений у кс
    for (const auto& pair : adjacencyList) {
        inDegree[pair.first]; // Инициализация
        for (int neighbor : pair.second) {
            inDegree[neighbor]++;
        }
    }

    // Алгоритм Кана
    std::queue<int> q; //очередь
    for (const auto& pair : inDegree) {
        if (pair.second == 0) { 
            q.push(pair.first); //добавляем в очередь кс с нулевой входящей степенью 
        }
    }

    std::vector<int> result;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);

        if (adjacencyList.find(node) != adjacencyList.end()) {
            for (int neighbor : adjacencyList.at(node)) { //берем соединенные с ней кс
                inDegree[neighbor]--; //перестаем учитывать удаленную кс
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

// Если id меньше или равно текущему - не меняем (защита от ошибок)
void GasNetwork::setNextConnectionId(int id) {
    if (id > nextConnectionId) {
        nextConnectionId = id;
    }
}
// Вспомогательный метод для создания взвешенного графа
std::unordered_map<int, std::vector<std::pair<int, double>>>
GasNetwork::getWeightedGraph(const std::unordered_map<int, Pipe>& pipes, bool useCapacity) const {
    std::unordered_map<int, std::vector<std::pair<int, double>>> graph;

    for (const auto& conn : connections) {
        int from = conn.second.startCSId;
        int to = conn.second.endCSId;
        int pipeId = conn.second.pipeId;

        // Ищем трубу
        auto pipeIt = pipes.find(pipeId);
        if (pipeIt == pipes.end()) {
            continue;  // Труба не найдена - пропускаем
        }

        const Pipe& pipe = pipeIt->second;
        double weight;

        if (useCapacity) {
            // Для максимального потока используем пропускную способность
            weight = pipe.getCapacity();
        }
        else {
            // Для кратчайшего пути используем вес (длину)
            weight = pipe.getWeight();
        }

        // Добавляем ребро в граф
        graph[from].push_back({ to, weight });
    }

    return graph;
}

// BFS для алгоритма Форда-Фалкерсона
bool GasNetwork::bfsForMaxFlow(int source, int sink,
    const std::unordered_map<int, std::vector<std::pair<int, double>>>& adj,
    std::unordered_map<int, int>& parent) const {
    std::set<int> visited;
    std::queue<int> q;

    q.push(source);
    visited.insert(source);
    parent[source] = -1;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        auto it = adj.find(current);
        if (it == adj.end()) continue;

        for (const auto& neighbor : it->second) {
            int next = neighbor.first;
            double capacity = neighbor.second;

            if (visited.find(next) == visited.end() && capacity > 0.0001) {
                parent[next] = current;
                if (next == sink) return true;

                q.push(next);
                visited.insert(next);
            }
        }
    }

    return false;
}

// Алгоритм Дейкстры
std::vector<int> GasNetwork::dijkstra(int start, int end,
    const std::unordered_map<int, std::vector<std::pair<int, double>>>& weightedAdj,
    std::unordered_map<int, double>& distances,
    std::unordered_map<int, int>& predecessors) const {
    // Приоритетная очередь для эффективного выбора вершины
    using Pair = std::pair<double, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

    // Инициализация
    for (const auto& node : weightedAdj) {
        distances[node.first] = std::numeric_limits<double>::infinity();
        predecessors[node.first] = -1;
    }

    distances[start] = 0.0;
    pq.push({ 0.0, start });

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        if (current == end) break;

        // Пропускаем если нашли лучший путь
        if (currentDist > distances[current] + 0.0001) continue;

        auto it = weightedAdj.find(current);
        if (it == weightedAdj.end()) continue;

        for (const auto& neighbor : it->second) {
            int next = neighbor.first;
            double weight = neighbor.second;

            // Пропускаем ребра с бесконечным весом
            if (std::isinf(weight)) continue;

            double newDist = distances[current] + weight;

            if (newDist < distances[next] - 0.0001) {
                distances[next] = newDist;
                predecessors[next] = current;
                pq.push({ newDist, next });
            }
        }
    }

    // Восстанавливаем путь
    std::vector<int> path;
    if (std::isinf(distances[end])) {
        return path;  // Путь не найден
    }

    for (int v = end; v != -1; v = predecessors[v]) {
        path.push_back(v);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

// Расчет максимального потока (Форд-Фалкерсон)
double GasNetwork::calculateMaxFlow(int sourceKS, int sinkKS,
    const std::unordered_map<int, Pipe>& pipes) const {
    if (sourceKS == sinkKS) return 0.0;

    // Строим остаточную сеть
    auto originalGraph = getWeightedGraph(pipes, true);
    std::unordered_map<int, std::vector<std::pair<int, double>>> residualGraph;

    // Инициализируем остаточную сеть
    for (const auto& node : originalGraph) {
        int u = node.first;
        for (const auto& edge : node.second) {
            int v = edge.first;
            double capacity = edge.second;
            residualGraph[u].push_back({ v, capacity });
            residualGraph[v].push_back({ u, 0.0 });
        }
    }

    std::unordered_map<int, int> parent;
    double maxFlow = 0.0;
    int iteration = 0;
    const int MAX_ITERATIONS = 1000;

    while (bfsForMaxFlow(sourceKS, sinkKS, residualGraph, parent) && iteration < MAX_ITERATIONS) {
        iteration++;

        // Находим минимальную пропускную способность на пути
        double pathFlow = std::numeric_limits<double>::infinity();
        for (int v = sinkKS; v != sourceKS; v = parent[v]) {
            int u = parent[v];

            // Ищем ребро u->v
            bool found = false;
            auto& edges = residualGraph[u];
            for (auto& edge : edges) {
                if (edge.first == v) {
                    pathFlow = std::min(pathFlow, edge.second);
                    found = true;
                    break;
                }
            }

            if (!found) {
                pathFlow = 0.0;
                break;
            }
        }

        if (pathFlow < 0.0001) break;
        // Обновляем остаточные пропускные способности
        for (int v = sinkKS; v != sourceKS; v = parent[v]) {
            int u = parent[v];

            // Уменьшаем пропускную способность на прямом ребре
            auto& forwardEdges = residualGraph[u];
            for (auto& edge : forwardEdges) {
                if (edge.first == v) {
                    edge.second -= pathFlow;
                    break;
                }
            }
            // Увеличиваем пропускную способность на обратном ребре
            auto& backwardEdges = residualGraph[v];
            bool backwardFound = false;
            for (auto& edge : backwardEdges) {
                if (edge.first == u) {
                    edge.second += pathFlow;
                    backwardFound = true;
                    break;
                }
            }
            if (!backwardFound) {
                residualGraph[v].push_back({ u, pathFlow });
            }
        }
        maxFlow += pathFlow;
        parent.clear();
    }

    return maxFlow;
}

// Поиск кратчайшего пути (Дейкстра)
std::vector<int> GasNetwork::findShortestPath(int startKS, int endKS,
    const std::unordered_map<int, Pipe>& pipes) const {
    if (startKS == endKS) {
        return std::vector<int>{startKS};
    }

    // Проверяем, что КС подключены к сети
    bool startExists = false, endExists = false;
    for (const auto& conn : connections) {
        if (conn.second.startCSId == startKS || conn.second.endCSId == startKS) startExists = true;
        if (conn.second.startCSId == endKS || conn.second.endCSId == endKS) endExists = true;
    }

    if (!startExists || !endExists) {
        return std::vector<int>();
    }

    // Строим взвешенный граф
    auto weightedGraph = getWeightedGraph(pipes, false);

    // Добавляем изолированные вершины
    if (weightedGraph.find(startKS) == weightedGraph.end()) {
        weightedGraph[startKS] = std::vector<std::pair<int, double>>();
    }
    if (weightedGraph.find(endKS) == weightedGraph.end()) {
        weightedGraph[endKS] = std::vector<std::pair<int, double>>();
    }

    std::unordered_map<int, double> distances;
    std::unordered_map<int, int> predecessors;

    return dijkstra(startKS, endKS, weightedGraph, distances, predecessors);
}

// Отображение информации о максимальном потоке
void GasNetwork::showMaxFlowInfo(int sourceKS, int sinkKS,
    const std::unordered_map<int, Pipe>& pipes) const {
    std::cout << "Расчет максимального потока\n";
    std::cout << "Источник: КС " << sourceKS << "\n";
    std::cout << "Сток: КС " << sinkKS << "\n";

    if (connections.empty()) {
        std::cout << "В сети нет соединений! Максимальный поток = 0\n";
        return;
    }

    if (sourceKS == sinkKS) {
        std::cout << "Ошибка: источник и сток не могут совпадать!\n";
        return;
    }

    std::cout << "Информация о трубах в сети\n";
    bool hasValidPipes = false;
    for (const auto& conn : connections) {
        int pipeId = conn.second.pipeId;
        auto pipeIt = pipes.find(pipeId);
        if (pipeIt != pipes.end()) {
            const Pipe& pipe = pipeIt->second;
            std::cout << "Труба ID " << pipeId << ": ";
            std::cout << "КС " << conn.second.startCSId << " -> КС " << conn.second.endCSId;
            std::cout << " | Пропускная способность: " << pipe.getCapacity() << " усл. ед.";
            if (pipe.isUnderRepair()) std::cout << " [В РЕМОНТЕ]";
            std::cout << "\n";
            hasValidPipes = true;
        }
    }

    if (!hasValidPipes) {
        std::cout << "Нет доступных труб для транспортировки!\n";
        return;
    }

    std::cout << "Расчет максимального потока\n";
    double maxFlow = calculateMaxFlow(sourceKS, sinkKS, pipes);
    std::cout << "Максимальный поток: " << maxFlow << " усл. ед.\n";

    if (maxFlow < 0.0001) {
        std::cout << "Статус: нет пути - невозможно доставить газ от источника к стоку\n";
    }
    else if (maxFlow < 1.0) {
        std::cout << "Статус: малый поток - ограниченная пропускная способность\n";
    }
    else {
        std::cout << "Статус: путь есть - газ может транспортироваться\n";
    }
}

// Отображение информации о кратчайшем пути
void GasNetwork::showShortestPathInfo(int startKS, int endKS,
    const std::unordered_map<int, Pipe>& pipes) const {
    std::cout << "Поиск кратчайшего пути\n";

    std::vector<int> path = findShortestPath(startKS, endKS, pipes);

    if (path.empty()) {
        std::cout << "Путь не найден\n";
        return;
    }

    // Расчет длины пути
    double totalLength = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        for (const auto& conn : connections) {
            if (conn.second.startCSId == path[i] && conn.second.endCSId == path[i + 1]) {
                auto pipeIt = pipes.find(conn.second.pipeId);
                if (pipeIt != pipes.end() && !pipeIt->second.isUnderRepair()) {
                    totalLength += pipeIt->second.getLength();
                }
                break;
            }
        }
    }

    std::cout << "Длина пути: " << totalLength << " км\n";
    std::cout << "Количество труб в пути: " << (path.size() - 1) << "\n";
    std::cout << "\nМаршрут: ";

    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << "КС" << path[i];
        if (i < path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
}