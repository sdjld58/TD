#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <thread>
#include <chrono>
#include <conio.h>
#include <Windows.h>

GameManager::GameManager() : playerLife(10), gold(0), isPreparation(true) // ���� �Ŵ��� �ʱ�ȭ
{
    erasecursol(); 
}

void GameManager::run()
{
    loadMap("Map1.csv");
    parsePath();
    loadUnitTypes("UnitTypes.csv");
    loadWaves("Stage1.csv");

    for (const auto& wave : waves) 
    {
        int waveID = wave.getWaveID();

        // ���̺� ���� �� �غ� �ܰ� ����
        startPreparationPhase();

        // 1, 3, 5 ���̺길 ���� ����� ó��
        if (waveID == 1 || waveID == 3 || waveID == 5)
        {
            std::cout << "\n=== ���̺� " << waveID << " ���� ===\n";

            if (wave.getLife() > 0)
            {
                playerLife = wave.getLife();
            }
            gold = wave.getGold();

            if (wave.getIsDefence())
            {
                // ���� ���̺� ó��
                std::cout << "���� ���̺��Դϴ�. ���: " << gold << ", ������: " << playerLife << "\n";

                // ���� ��⿭ ����
                std::queue<Unit> unitQueue;
                for (int unitID : wave.getUnits()) 
                {
                    // unitID�� �ش��ϴ� UnitType ã��
                    auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                        [unitID](const UnitType& ut) { return ut.getId() == unitID; });
                    if (it != unitTypes.end())
                    {
                        UnitType& unitType = *it;
                        Unit unit(unitType.getId(), unitType.getUnitName(), unitType.getHp(),
                            unitType.getTimePerMove(), path);
                        unitQueue.push(unit);
                    }
                }

                std::vector<Unit> activeUnits;

                const int maxTicks = 100;
                for (int tick = 0; tick < maxTicks; ++tick)
                {
                    // S�� ��������� ���� ���� ����
                    bool sOccupied = false;
                    for (const auto& unit : activeUnits)
                    {
                        if (unit.getX() == path[0].first && unit.getY() == path[0].second)
                        {
                            sOccupied = true;
                            break;
                        }
                    }
                    if (!sOccupied && !unitQueue.empty())
                    {
                        Unit unit = unitQueue.front();
                        unitQueue.pop();
                        activeUnits.push_back(unit);
                    }

                    // ���� ������Ʈ
                    for (auto it = activeUnits.begin(); it != activeUnits.end();) 
                    {
                        bool arrived = it->update();
                        if (arrived) 
                        {
                            // ������ �������� ����
                            std::cout << it->getName() << " ������ �������� �����߽��ϴ�!\n";
                            playerLife -= 1;
                            it = activeUnits.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }

                    // �� ������Ʈ �� ���
                    updateAndPrintMap(activeUnits);

                    // �÷��̾� ������ ���
                    //std::cout << "Player Life: " << playerLife << "\n";

                    if (playerLife <= 0) 
                    {
                        std::cout << "Game Over!\n";
                        return;
                    }

                    if (activeUnits.empty() && unitQueue.empty()) 
                    {
                        std::cout << "���̺� " << waveID << " Ŭ����!\n";
                        break;
                    }

                    // ƽ ���̿� �ð� ���� �߰� (��: 500�и���)
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
        }
        else {
            std::cout << "=== ���̺� " << waveID << "�� ���� ���̺갡 �ƴմϴ�. �ǳʶݴϴ�. ===\n";
        }
    }

    std::cout << "���α׷��� �����մϴ�.\n";
}


void GameManager::updateAndPrintMap(const std::vector<Unit>& activeUnits)
{
    // ȭ���� ����� ���� �ٽ� ����մϴ�.
    // Windows�� Linux���� ȭ���� ����� ����� �ٸ��Ƿ�, �ý��ۿ� �°� �����մϴ�.
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // ���� �����Ͽ� ������ ǥ���� �� �ֵ��� �մϴ�.
    std::vector<std::vector<std::string>> mapWithUnits = map;

    // ���ֵ��� ��ġ�� ������ �̸��� ǥ���մϴ�.
    for (const auto& unit : activeUnits) 
    {
        int x = unit.getX();
        int y = unit.getY();
        if (y >= 0 && y < (int)mapWithUnits.size() && x >= 0 && x < (int)mapWithUnits[0].size())
        {
            mapWithUnits[y][x] = unit.getName();
        }
    }

    // �� ���
    for (const auto& row : mapWithUnits) 
    {
        for (const auto& cell : row) 
        {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Player Life: " << playerLife << std::endl;
}

void GameManager::loadMap(const std::string& filename) 
{
    // ���� �ڵ�� ����
    std::ifstream file(filename);

    if (!file.is_open()) 
    {
        std::cout << "������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) 
    {
        if (firstLine) {
            // UTF-8 BOM ����
            const std::string utf8_bom = "\xEF\xBB\xBF";
            if (line.compare(0, utf8_bom.size(), utf8_bom) == 0) 
            {
                line.erase(0, utf8_bom.size());
            }
            firstLine = false;
        }

        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, ',')) 
        {
            if (!item.empty()) 
            {
                row.push_back(item);
            }
        }
        map.push_back(row);
    }
    file.close();
}

void GameManager::printMap()
{
    std::cout << "=== �� ��� ===" << std::endl;
    for (const auto& row : map) 
    {

        for (const auto& cell : row) 
        {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}

void GameManager::parsePath() {
    // BFS �˰����� ����Ͽ� S���� D������ ��θ� ã���ϴ�.
    size_t rows = map.size();
    size_t cols = map[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<size_t, size_t>> q;
    std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, { -1, -1 }));

    // ���� ���� ã��
    size_t startX = 0, startY = 0;
    bool foundStart = false;
    for (size_t y = 0; y < rows; ++y) {
        for (size_t x = 0; x < cols; ++x) {
            if (map[y][x] == "S") {
                startX = x;
                startY = y;
                foundStart = true;
                break;
            }
        }
        if (foundStart) break;
    }

    q.push({ startY, startX });
    visited[startY][startX] = true;

    std::vector<int> dx = { 0, 1, 0, -1 };
    std::vector<int> dy = { -1, 0, 1, 0 };

    bool foundDest = false;
    size_t destX = 0, destY = 0;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        if (map[y][x] == "D") {
            destX = x;
            destY = y;
            foundDest = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (ny >= 0 && ny < (int)rows && nx >= 0 && nx < (int)cols) {
                if (!visited[ny][nx] && (map[ny][nx] == "P" || map[ny][nx] == "D")) {
                    visited[ny][nx] = true;
                    q.push({ ny, nx });
                    parent[ny][nx] = { y, x };
                }
            }
        }
    }

    if (foundDest) {
        // ��� ����
        path.clear();
        int y = destY;
        int x = destX;
        while (!(x == (int)startX && y == (int)startY)) {
            path.push_back({ x, y });
            auto [py, px] = parent[y][x];
            y = py;
            x = px;
        }
        path.push_back({ (int)startX, (int)startY });
        std::reverse(path.begin(), path.end());
    }
    else {
        std::cout << "��θ� ã�� �� �����ϴ�." << std::endl;
    }
}

void GameManager::loadUnitTypes(const std::string& filename) 
{
    // ���� �ڵ�� ����
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "���� Ÿ�� ������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line)) 
    {
        if (isHeader) 
        {
            isHeader = false; // ����� �ǳʶݴϴ�.
            continue;
        }

        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ','))
        {
            tokens.push_back(item);
        }

        if (tokens.size() == 8) 
        {
            int id = std::stoi(tokens[0]);
            std::string unitName = tokens[1];
            int hp = std::stoi(tokens[2]);
            int timePerMove = std::stoi(tokens[3]);
            int armor = std::stoi(tokens[4]);
            int resist = std::stoi(tokens[5]);
            int productionCost = std::stoi(tokens[6]);
            int killReward = std::stoi(tokens[7]);

            UnitType unitType(id, unitName, hp, timePerMove, armor, resist, productionCost, killReward);
            unitTypes.push_back(unitType);
        }
        else
        {
            std::cout << "���� Ÿ�� �����͸� �Ľ��ϴ� �� ���� �߻�: " << line << std::endl;
        }
    }
    file.close();
}

void GameManager::printUnitTypes() 
{
    std::cout << "=== ���� Ÿ�� ��� ===" << std::endl;
    for (const auto& unitType : unitTypes)
    {
        unitType.printInfo();
    }
}

void GameManager::loadWaves(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "���̺� ������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line)) {
        // UTF-8 BOM ����
        if (!line.empty() && (unsigned char)line[0] == 0xEF) {
            line.erase(0, 3);
        }

        if (isHeader) {
            isHeader = false; // ����� �ǳʶݴϴ�.
            continue;
        }

        // ������ ��������� �ǳʶݴϴ�.
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 5) {
            int waveID = std::stoi(tokens[0]);
            int life = std::stoi(tokens[1]);
            bool isDefence = (tokens[2] == "1");
            int gold = std::stoi(tokens[3]);

            // units�� tokens[4]���� ����
            std::vector<int> units;
            for (size_t i = 4; i < tokens.size(); ++i) {
                if (tokens[i].empty()) continue;
                units.push_back(std::stoi(tokens[i]));
            }

            Wave wave(waveID, life, isDefence, gold, units);
            waves.push_back(wave);
        }
        else {
            std::cout << "���̺� �����͸� �Ľ��ϴ� �� ���� �߻�: " << line << std::endl;
        }
    }
    file.close();
}

void GameManager::printWaves() {
    std::cout << "=== ���̺� ��� ===" << std::endl;
    for (const auto& wave : waves) {
        wave.printInfo();
    }
}

void GameManager::startPreparationPhase()
{
    std::vector<std::vector<std::string>> mapWithUnits = map;
    isPreparation = true;
    bool isTowerPlacementMode = false;  // Ÿ�� ��ġ ��� ���� ����

    // Ŀ�� �ʱ� ��ġ�� �� �߾����� ����
    int selectedX = mapWithUnits[0].size() / 2;
    int selectedY = mapWithUnits.size() / 2;

    // Ÿ�� ��ġ ��� ���� ��ġ�� ������ ����
    int originalX = selectedX;
    int originalY = selectedY;

    while (isPreparation)
    {
        // ȭ���� ����� �ʰ� Ŀ�� ��ġ�� �ٽ� ���
        updateAndPrintMap({});

        if (isTowerPlacementMode)
        {
            COORD cursorPos;
            cursorPos.X = static_cast<SHORT>(selectedX * 2); // �ܼ��� ���� ���� ��� (2�� ����)
            cursorPos.Y = static_cast<SHORT>(selectedY);
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
            std::cout << " ";                                //���� Ŀ�� ��ġ�� ��ĭ���� ǥ��
        }
        
        

        // �غ� �ܰ� �ȳ� �޽��� ���
        COORD messagePos = { 0, static_cast<SHORT>(mapWithUnits.size() + 2) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), messagePos);
        std::cout << "�غ� �ܰ��Դϴ�. 'S'�� ���� �����ϰų� 'B'�� ���� Ÿ���� ��ġ�Ͻʽÿ�.\n";

        char input = _getch(); // Ű �Է� ���
        if (input == 's' || input == 'S')
        {
            isPreparation = false;
            break; // �غ� �ܰ踦 ����
        }
        else if (input == 'b' || input == 'B')     // Ÿ�� ��ġ ��� ���
        {
           
            if (isTowerPlacementMode)
            {
                // Ÿ�� ��ġ ��� ��Ȱ��ȭ - Ŀ���� ���� ��ġ�� �ǵ���
                selectedX = originalX;
                selectedY = originalY;
                isTowerPlacementMode = false;
                
            }
            else
            {
                // Ÿ�� ��ġ ��� Ȱ��ȭ - ���� ��ġ ����
                originalX = selectedX;
                originalY = selectedY;
                isTowerPlacementMode = true;
                
            }
        }
        else if (isTowerPlacementMode)      // ����Ű �Է����� Ŀ�� �̵� (Ÿ�� ��ġ ����� ����)
        {

            if (input == 224 || input == -32)
            {
                input = _getch();
                if (input == 72 && selectedY > 0)                  // ���� ����Ű
                    selectedY--;
                else if (input == 80 && selectedY < mapWithUnits.size() - 1) // �Ʒ��� ����Ű
                    selectedY++;
                else if (input == 75 && selectedX > 0)             // ���� ����Ű
                    selectedX--;
                else if (input == 77 && selectedX < mapWithUnits[0].size() - 1) // ������ ����Ű
                    selectedX++;


                updateAndPrintMap({});
            }
            else if (input == 13) // ���ͷ� Ÿ�� ��ġ
            {

            }
        }
    }
}


void GameManager::erasecursol()
{
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curConsoleinfo;

    GetConsoleCursorInfo(hConsoleOut, &curConsoleinfo);
    curConsoleinfo.bVisible = 0;

    SetConsoleCursorInfo(hConsoleOut, &curConsoleinfo);
}