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
        std::cout << "\nМеню\n";
        std::cout << "1. Добавить трубу\n";
        std::cout << "2. Добавить КС\n";
        std::cout << "3. Просмотр всех объектов\n";
        std::cout << "4. Редактировать трубу\n";
        std::cout << "5. Редактировать КС\n";
        std::cout << "6. Удалить трубу\n";
        std::cout << "7. Удалить КС\n";
        std::cout << "8. Поиск труб\n";
        std::cout << "9. Поиск КС\n";
        std::cout << "10. Пакетное редактирование труб\n";
        std::cout << "11. Сохранить в файл\n";
        std::cout << "12. Загрузить из файла\n";
        std::cout << "13. Соединить КС\n";
        std::cout << "14. Разъединить КС\n";
        std::cout << "15. Показать сеть\n";
        std::cout << "16. Топологическая сортировка\n";
        std::cout << "17. Расчет максимального потока\n";     
        std::cout << "18. Поиск кратчайшего пути\n";          
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";

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
        case 13: connectStations(); break; 
        case 14: disconnectStations(); break; 
        case 15: showNetwork(); break;      
        case 16: topologicalSort(); break;
        case 17: calculateMaxFlow(); break;  
        case 18: findShortestPath(); break;  
        case 0:
            std::cout << "Выход из программы.\n";
            return;
        default:
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

void Menu::addPipe() { //новая труба
    std::string name = utils::inputString("Километровая отметка трубы: ");
    double length = utils::inputPositiveDouble("Длина (км): ");
    double diameter = utils::inputPositiveDouble("Диаметр (мм): ");
    bool underRepair = utils::inputBool("Статус ремонта (0 - работает, 1 - в ремонте): ");

    int id = dataManager.addPipe(name, length, diameter, underRepair);
    std::cout << "Труба добавлена с ID: " << id << std::endl;
}

void Menu::addStation() { //новая кс
    std::string name = utils::inputString("Название КС: ");
    int totalWorkshops = utils::inputPositiveInt("Количество цехов: ");

    int workingWorkshops;
    do {
        workingWorkshops = utils::inputPositiveInt("Количество работающих цехов: ");
        if (workingWorkshops > totalWorkshops) {
            std::cout << "Количество работающих цехов не может превышать общее количество!\n";
        }
    } while (workingWorkshops > totalWorkshops);

    double classCS = utils::inputPositiveDouble("Класс станции: ");

    int id = dataManager.addStation(name, totalWorkshops, workingWorkshops, classCS);
    std::cout << "КС добавлена с ID: " << id << std::endl;
}

void Menu::showAllObjects() { //все объекты
    std::cout << "\nВсе трубы\n";
    auto pipeIds = dataManager.getPipeIds();
    if (pipeIds.empty()) {
        std::cout << "Трубы не добавлены\n";
    }
    else {
        for (int id : pipeIds) { //проход по всем объектам
            Pipe* pipe = dataManager.getPipe(id); //указатель на трубу если она есть
            if (pipe) {
                std::cout << pipe->toString() << "\n\n"; //вывод строкового представления 
            }
        }
    }

    std::cout << "Все компрессорные станции\n";
    auto stationIds = dataManager.getStationIds();
    if (stationIds.empty()) {
        std::cout << "КС не добавлены\n";
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

void Menu::editPipe() { //редактирование трубы
    int id = utils::inputPositiveInt("Введите ID трубы для редактирования: ");

    if (!dataManager.pipeExists(id)) {
        std::cout << "Труба с ID " << id << " не найдена.\n";
        return;
    }

    bool newStatus = utils::inputBool("Новый статус ремонта (0 - работает, 1 - в ремонте): ");
    dataManager.editPipe(id, newStatus);
    std::cout << "Труба отредактирована.\n";
}

void Menu::editStation() { //редактирование кс
    int id = utils::inputPositiveInt("Введите ID КС для редактирования: ");

    if (!dataManager.stationExists(id)) {
        std::cout << "КС с ID " << id << " не найдена.\n";
        return;
    }

    CompressorStation* station = dataManager.getStation(id);
    if (!station) return;

    int newWorking;
    do {
        newWorking = utils::inputPositiveInt("Новое количество работающих цехов: ");
        if (newWorking > station->getTotalWorkshops()) {
            std::cout << "Количество работающих цехов не может превышать общее количество!\n";
        }
    } while (newWorking > station->getTotalWorkshops());

    dataManager.editStation(id, newWorking);
    std::cout << "КС отредактирована.\n";
}

void Menu::deletePipe() { //удаление трубы
    int id = utils::inputPositiveInt("Введите ID трубы для удаления: ");

    if (!dataManager.pipeExists(id)) {
        std::cout << "Труба с ID " << id << " не найдена.\n";
        return;
    }

    dataManager.deletePipe(id);
    std::cout << "Труба удалена.\n";
}

void Menu::deleteStation() { //удаление кс
    int id = utils::inputPositiveInt("Введите ID КС для удаления: ");

    if (!dataManager.stationExists(id)) {
        std::cout << "КС с ID " << id << " не найдена.\n";
        return;
    }

    dataManager.deleteStation(id);
    std::cout << "КС удалена.\n";
}

void Menu::searchPipes() { //меню поиска труб
    showPipeSearchMenu();
}

void Menu::searchStations() { //меню поиска кс
    showStationSearchMenu();
}

void Menu::showPipeSearchMenu() {
    int choice;
    std::cout << "\nПоиск труб\n";
    std::cout << "1. По названию\n";
    std::cout << "2. По статусу ремонта\n";
    std::cout << "Выберите тип поиска: ";

    std::cin >> choice;
    utils::clearInputBuffer();

    std::vector<int> results;

    switch (choice) {
    case 1: {
        std::string name = utils::inputString("Введите часть названия для поиска: ");
        results = dataManager.findPipesByName(name);
        break;
    }
    case 2: {
        bool status = utils::inputBool("Введите статус (0 - работает, 1 - в ремонте): ");
        results = dataManager.findPipesByRepairStatus(status);
        break;
    }
    default:
        std::cout << "Неверный выбор.\n";
        return;
    }

    std::cout << "\nРезультаты поиска (" << results.size() << " найдено):\n";
    for (int id : results) {
        Pipe* pipe = dataManager.getPipe(id);
        if (pipe) {
            std::cout << pipe->toString() << "\n\n";
        }
    }
}

void Menu::showStationSearchMenu() {
    int choice;
    std::cout << "\nПоиск кс\n";
    std::cout << "1. По названию\n";
    std::cout << "2. По проценту незадействованных цехов\n";
    std::cout << "Выберите тип поиска: ";

    std::cin >> choice;
    utils::clearInputBuffer();

    std::vector<int> results;

    switch (choice) {
    case 1: {
        std::string name = utils::inputString("Введите часть названия для поиска: ");
        results = dataManager.findStationsByName(name);
        break;
    }
    case 2: {
        double percentage = utils::inputPositiveDouble("Введите минимальный процент незадействованных цехов: ");
        results = dataManager.findStationsByUnusedPercentage(percentage);
        break;
    }
    default:
        std::cout << "Неверный выбор.\n";
        return;
    }

    std::cout << "\nРезультаты поиска (" << results.size() << " найдено):\n";
    for (int id : results) {
        CompressorStation* station = dataManager.getStation(id);
        if (station) {
            std::cout << station->toString() << "\n\n";
        }
    }
}

void Menu::batchEditPipes() {
    std::cout << "\nПакетное редактирование труб\n";

    //Поиск труб для редактирования
    showPipeSearchMenu();

    std::vector<int> pipeIds;
    std::string input;

    std::cout << "Введите ID труб для редактирования (через пробел): ";
    std::getline(std::cin, input);

    std::stringstream ss(input); //поток из строки
    int id;
    while (ss >> id) { //чтение чисел из потока
        if (dataManager.pipeExists(id)) { //если существует
            pipeIds.push_back(id); //добавляем в вектор
        }
        else {
            std::cout << "Труба с ID " << id << " не существует.\n";
        }
    }

    if (pipeIds.empty()) {
        std::cout << "Нет валидных ID для редактирования.\n";
        return;
    }

    bool newStatus = utils::inputBool("Новый статус ремонта для выбранных труб (0 - работает, 1 - в ремонте): ");

    dataManager.batchEditPipes(pipeIds, newStatus);
    std::cout << "Статус " << pipeIds.size() << " труб изменен.\n";
}

void Menu::saveToFile() { //сохранение в файл
    std::string filename = utils::inputString("Введите имя файла для сохранения: ");
    if (dataManager.saveToFile(filename)) {
        std::cout << "Данные успешно сохранены в файл: " << filename << std::endl;
    }
    else {
        std::cout << "Ошибка сохранения в файл: " << filename << std::endl;
    }
}

void Menu::loadFromFile() { //загрузка из файла
    std::string filename = utils::inputString("Введите имя файла для загрузки: ");
    if (dataManager.loadFromFile(filename)) {
        std::cout << "Данные успешно загружены из файла: " << filename << std::endl;
    }
    else {
        std::cout << "Ошибка загрузки из файла: " << filename << std::endl;
    }
}

void Menu::connectStations() {
    std::cout << "\nСоединение КС\n";

    // Показываем доступные КС
    auto stationIds = dataManager.getStationIds();
    if (stationIds.size() < 2) {
        std::cout << "Для соединения нужно как минимум 2 КС\n";
        return;
    }

    std::cout << "Доступные КС:\n";
    for (int id : stationIds) {
        CompressorStation* station = dataManager.getStation(id);
        if (station) {
            std::cout << "ID " << station->getId() << ": " << station->getName();
            if (dataManager.getNetwork().isCSInConnection(id)) {
                std::cout << " (уже в соединении)";
            }
            std::cout << "\n";
        }
    }

    int startId = utils::inputPositiveInt("ID КС входа: ");
    int endId = utils::inputPositiveInt("ID КС выхода: ");

    if (startId == endId) {
        std::cout << "КС входа и выхода не могут быть одинаковыми\n";
        return;
    }

    // Выбор диаметра из допустимых значений
    std::cout << "Доступные диаметры: 500, 700, 1000, 1400 мм\n";
    double diameter = utils::inputPositiveDouble("Диаметр трубы (мм): ");

    if (dataManager.connectStations(startId, endId, diameter)) {
        std::cout << "КС успешно соединены!\n";
    }
    else {
        std::cout << "Не удалось соединить КС. Проверьте введенные данные.\n";
    }
}

void Menu::disconnectStations() {
    std::cout << "\nРазъединение КС\n";

    dataManager.showNetwork();

    int pipeId = utils::inputPositiveInt("Введите ID трубы для разъединения: ");
    dataManager.disconnectStations(pipeId);
}

void Menu::showNetwork() {
    dataManager.showNetwork();
}

void Menu::topologicalSort() {
    dataManager.topologicalSort();
}

// Расчет максимального потока
void Menu::calculateMaxFlow() {
    dataManager.calculateMaxFlow();
}

// Поиск кратчайшего пути
void Menu::findShortestPath() {
    dataManager.findShortestPath();
}