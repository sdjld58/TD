#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <climits>
#include <TGUI/TGUI.hpp>

GameManager::GameManager() : playerLife(10), gold(100), isPreparation(true)
{
    // UI �ʱ�ȭ
    ui.initialize(map);

    ui.onTowerButtonClicked = [this]() {
        this->handleTowerButtonClicked();
        };
}

void GameManager::run()
{
    loadMap("Map1.csv");
    parsePath();
    loadUnitTypes("UnitTypes.csv");
    loadTowerData("TowerData.csv");
    loadWaves("Stage1.csv");

    ui.setMap(map);
    ui.setPath(path);
    ui.setTowers(towers);
    ui.setUnitTypes(unitTypes);

    int currentTick = 0;

    for (const auto& wave : waves)
    {
        int waveID = wave.getWaveID();
        currentwaveType = wave.getIsDefence();

        if (wave.getIsDefence())
        {
            gold += wave.getGold();
            startPreparationPhase(); // ���� ���̺� �غ�
        }

        if (wave.getIsDefence())
        {
            if (wave.getLife() > 0)
            {
                playerLife = wave.getLife();
            }

            std::queue<Unit> unitQueue;

            // ���̺꿡 ���Ե� ���� ����
            for (int unitID : wave.getUnits())
            {
                auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                    [unitID](const UnitType& ut) { return ut.getId() == unitID; });
                if (it != unitTypes.end())
                {
                    UnitType& unitType = *it;
                    Unit unit(unitType.getId(), unitType.getUnitName(), unitType.getHp(),
                        unitType.getTimePerMove(), unitType.getArmor(), unitType.getResist(),
                        unitType.getKillReward(), path);
                    unitQueue.push(unit);
                }
            }

            std::vector<Unit> activeUnits;

            // **���� ���� ���� �κ� ����**
            sf::Clock clock;
            sf::Time lastLogicUpdateTime = sf::Time::Zero;
            sf::Time logicUpdateInterval = sf::milliseconds(500); // �� ������Ʈ ���� (500ms)
            bool waveOver = false;

            while (!waveOver && ui.getWindow().isOpen())
            {
                sf::Time elapsedTime = clock.restart();

                // **�̺�Ʈ ó��**
                sf::Event event;
                while (ui.getWindow().pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        ui.getWindow().close();
                        return;
                    }
                    // ��Ÿ �̺�Ʈ ó�� �ʿ� �� �߰�
                }

                // **�� ������Ʈ �ð� üũ**
                lastLogicUpdateTime += elapsedTime;
                if (lastLogicUpdateTime >= logicUpdateInterval)
                {
                    lastLogicUpdateTime -= logicUpdateInterval;
                    currentTick++;

                    // **�� ������Ʈ ����**

                    // ���� ����
                    spawnUnits(activeUnits, unitQueue);

                    // ���� �̵�
                    updateUnits(activeUnits);

                    // ���� ó��
                    attackUnits(activeUnits, currentTick, currentwaveType);

                    // �ܼ� ���
                    updateAndPrintMap(activeUnits);

                    // ���� ���� ���� üũ
                    if (playerLife <= 0)
                    {
                        std::cout << "Game Over!\n";
                        ui.getWindow().close();
                        return;
                    }

                    if (activeUnits.empty() && unitQueue.empty())
                    {
                        std::cout << "���̺� " << waveID << " Ŭ����!\n";
                        waveOver = true;
                    }
                }

                // **����ü ������Ʈ (�� ������)**
                updateProjectiles(elapsedTime);

                // **ȭ�� �׸���**
                updateGameState(activeUnits);

                // **FPS ���� (���� ����)**
                // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // �� 60 FPS
            }
            // **���� ���� ���� �κ� ��**
        }
        else
        {
            // ���� ���̺� ó��
            attackGold = wave.getGold();
            startAttackWave(wave, currentTick);
        }
    }

    std::cout << "���α׷��� �����մϴ�.\n";
}

void GameManager::spawnUnits(std::vector<Unit>& activeUnits, std::queue<Unit>& unitQueue)
{
    // S ��ġ�� ��������� ���� ����
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
}

void GameManager::updateUnits(std::vector<Unit>& activeUnits)
{
    // ���� ������Ʈ
    for (auto it = activeUnits.begin(); it != activeUnits.end();)
    {
        bool arrived = it->update();
        if (arrived)
        {
            playerLife -= 1;
            it = activeUnits.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void GameManager::updateAndPrintMap(const std::vector<Unit>& activeUnits)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::vector<std::vector<std::string>> mapWithUnits = map;

    // ���� ��ġ�� �ʿ� ǥ��
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
    std::cout << "=== �� ���� ===\n";
    for (const auto& row : mapWithUnits)
    {
        for (const auto& cell : row)
        {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }

    // �÷��̾� ���� ���
    std::cout << "\n�÷��̾� ������: " << playerLife << "\n";
    std::cout << "���: " << gold << "\n";

    // ���� ���̺� ��� ���
    std::cout << "���� ���(AttackGold): " << attackGold << "\n";

    // ���� ���� ���
    std::cout << "\n=== ���� ���� ===\n";
    for (const auto& unit : activeUnits)
    {
        std::cout << "����: " << unit.getName()
            << ", ��ġ: (" << unit.getX() << ", " << unit.getY() << ")"
            << ", ü��: " << unit.getHp() << "\n";
    }
    // ���� ���� ��⿭ ��� �߰�
    if (!unitProductionQueue.empty())
    {
        std::cout << "\n=== ���� ���� ��⿭ ===\n";
        std::queue<int> tempQueue = unitProductionQueue; // ��⿭ ���纻 ����
        while (!tempQueue.empty())
        {
            int unitId = tempQueue.front();
            tempQueue.pop();

            // ���� ID�� ���� �̸� ã��
            auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                [unitId](const UnitType& ut) { return ut.getId() == unitId; });
            if (it != unitTypes.end())
            {
                std::cout << "����: " << it->getUnitName() << "\n";
            }
            else
            {
                std::cout << "���� ID: " << unitId << "\n";
            }
        }
    }
}





void GameManager::updateGameState(std::vector<Unit>& activeUnits)
{
    // UI�� ���� ���� ���¸� ������Ʈ�ϰ� ȭ���� �׸��ϴ�.
    ui.update(activeUnits, placedTowers, playerLife, gold, attackGold, selectedX, projectiles);
}

void GameManager::loadMap(const std::string& filename)
{
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

void GameManager::parsePath() {
    // BFS �˰����� ����Ͽ� S���� D������ ��θ� ã���ϴ�.
    size_t rows = map.size();
    size_t cols = map[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<size_t, size_t>> q;
    std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, { -1, -1 }));

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
        auto current = q.front();
        q.pop();
        size_t y = current.first;
        size_t x = current.second;

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
        path.clear();
        int y = destY;
        int x = destX;
        while (!(x == (int)startX && y == (int)startY)) {
            path.push_back({ x, y });
            auto parentPos = parent[y][x];
            y = parentPos.first;
            x = parentPos.second;
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
            isHeader = false;
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

void GameManager::loadTowerData(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Ÿ�� ������ ������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ','))
        {
            tokens.push_back(item);
        }

        if (tokens.size() == 12) // ����� ������ ũ�� �ݿ�
        {
            int id = std::stoi(tokens[0]);
            std::string towerName = tokens[1];
            int nextTowerID = std::stoi(tokens[2]);
            int buildCost = std::stoi(tokens[3]);
            int attackRange = std::stoi(tokens[4]);
            int damage = std::stoi(tokens[5]);
            bool isMagic = std::stoi(tokens[6]) == 1;
            int timePerAttack = std::stoi(tokens[7]);
            int targetAmount = std::stoi(tokens[8]);
            int isNodamage = std::stoi(tokens[9]); // �߰��� isNodamage ó��
            std::string tool = tokens[10];        // �߰��� tool ó��
            std::string tool2 = tokens[11];

            Tower tower(id, towerName, nextTowerID, buildCost, attackRange, damage,
                isMagic, timePerAttack, targetAmount, isNodamage, tool ,tool2);
            towers.push_back(tower);

            tower.displayInfo();
        }
        else
        {
            std::cout << "�߸��� ������ ������ �ֽ��ϴ�: " << line << std::endl;
        }
    }
    file.close();
}


void GameManager::loadWaves(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "���̺� ������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line))
    {
        if (!line.empty() && (unsigned char)line[0] == 0xEF)
        {
            line.erase(0, 3);
        }

        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ','))
        {
            tokens.push_back(item);
        }

        if (tokens.size() >= 5)
        {
            int waveID = std::stoi(tokens[0]);
            int life = std::stoi(tokens[1]);
            bool isDefence = (tokens[2] == "1");
            int gold = std::stoi(tokens[3]);

            std::vector<int> units;
            for (size_t i = 4; i < tokens.size(); ++i)
            {
                if (tokens[i].empty()) continue;
                units.push_back(std::stoi(tokens[i]));
            }

            Wave wave(waveID, life, isDefence, gold, units);
            waves.push_back(wave);
        }
        else
        {
            std::cout << "���̺� �����͸� �Ľ��ϴ� �� ���� �߻�: " << line << std::endl;
        }
    }
    file.close();
}

void GameManager::startPreparationPhase()
{
    std::vector<std::vector<std::string>> mapWithUnits = map;
    isPreparation = true;
    isTowerPlacementMode = false;  // Ÿ�� ��ġ ��� ���� ����
    bool isTowerSelected = false;

    selectedX = mapWithUnits[0].size() / 2;
    selectedY = mapWithUnits.size() / 2;
    selectedTowerIndex = 1;

    int originalX = selectedX;
    int originalY = selectedY;
    int towerX = -1;
    int towerY = -1;

    while (isPreparation)
    {
        ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY);

        sf::Event event;
        while (ui.getWindow().pollEvent(event))
        {
            isTowerPlacementMode = true;

            // **TGUI �̺�Ʈ ó�� �߰�**
            ui.gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                ui.getWindow().close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::S)
                {
                    isPreparation = false;
                    break;
                }
                
                else if (isTowerPlacementMode)
                {
                    if (event.key.code == sf::Keyboard::Up && selectedY > 0)
                        selectedY--;
                    else if (event.key.code == sf::Keyboard::Down && selectedY < mapWithUnits.size() - 1)
                        selectedY++;
                    else if (event.key.code == sf::Keyboard::Left && selectedX > 0)
                        selectedX--;
                    else if (event.key.code == sf::Keyboard::Right && selectedX < mapWithUnits[0].size() - 1)
                        selectedX++;

                    else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9)
                    {
                        int towerID = event.key.code - sf::Keyboard::Num0;
                        auto it = std::find_if(towers.begin(), towers.end(),
                            [towerID](const Tower& tower) { return tower.getId() == towerID; });

                        if (it != towers.end())
                        {
                            Tower& selectedTower = *it;
                            selectedTowerIndex = std::distance(towers.begin(), it);
                            std::cout << "Ÿ�� " << selectedTower.getTowerName() << " �� ���õǾ����ϴ�.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        else
                        {
                            std::cout << "�ش� Ÿ���� �������� �ʽ��ϴ�.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                    }

                    else if (event.key.code == sf::Keyboard::U)
                    {
                        auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
                            [this](const PlacedTower& tower) {
                                return tower.getX() == selectedX && tower.getY() == selectedY;
                            });

                        if (towerIt != placedTowers.end())
                        {
                            bool inTowerSelectionMode = true;
                            towerX = selectedX;
                            towerY = selectedY;

                            std::cout << "Ÿ���� ���õǾ����ϴ�. 1 Ű�� ���� ���׷��̵��ϰų� S Ű�� ���� �Ǹ��ϼ���. U Ű�� �ٽ� ���� ������ �����մϴ�.\n";

                            while (inTowerSelectionMode)
                            {
                                sf::Event towerEvent;
                                while (ui.getWindow().pollEvent(towerEvent))
                                {
                                    // **TGUI �̺�Ʈ ó�� �߰�**
                                    ui.gui.handleEvent(towerEvent);

                                    if (towerEvent.type == sf::Event::KeyPressed)
                                    {
                                        if (towerEvent.key.code == sf::Keyboard::Num1)
                                        {
                                            towerIt->upgrade(gold, map, towers,1);
                                            std::cout << "Ÿ���� ���׷��̵�Ǿ����ϴ�.\n";
                                            inTowerSelectionMode = false;
                                        }
                                        else if (towerEvent.key.code == sf::Keyboard::Num2)
                                        {
                                            towerIt->upgrade(gold, map, towers,2);
                                            std::cout << "Ÿ���� ���׷��̵�Ǿ����ϴ�.\n";
                                            inTowerSelectionMode = false;
                                        }
                                        else if (towerEvent.key.code == sf::Keyboard::S)
                                        {
                                            int refundAmount = static_cast<int>(towerIt->getBuildCost() * 0.3);
                                            gold += refundAmount;
                                            std::cout << "Ÿ���� �ǸŵǾ����ϴ�. ��ȯ�� ���: " << refundAmount << "\n";

                                            map[towerY][towerX] = "O";
                                            placedTowers.erase(towerIt);
                                            inTowerSelectionMode = false;
                                        }
                                        else if (towerEvent.key.code == sf::Keyboard::U)
                                        {
                                            inTowerSelectionMode = false;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            std::cout << "���� ��ġ�� Ÿ���� �����ϴ�.\n";
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter && selectedTowerIndex >= 0)
                    {
                        attemptPlaceTower();
                    }
                   
                }
            }

            // **���콺 Ŭ�� �̺�Ʈ ó��**
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // ���콺 ���� ��ư Ŭ�� �� ó��
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(ui.getWindow());
                    float mouseX = static_cast<float>(mousePosition.x);
                    float mouseY = static_cast<float>(mousePosition.y);

                    // �̼Ҹ�Ʈ�� ��ǥ ��ȯ�� ���� ������
                    float mapWidth = static_cast<float>(mapWithUnits[0].size());
                    float mapHeight = static_cast<float>(mapWithUnits.size());

                    // ȭ�� �߽� ��ǥ
                    float centerX = ui.getWindow().getSize().x / 2.0f;
                    float centerY = 0.0f; // ���� ȭ�� ��ܿ��� �����Ѵٰ� ����

                    // ȭ�� ��ǥ�� Ÿ�� ��ǥ�� ��ȯ
                    float tempX = (mouseX - centerX) / (ui.tileWidth / 2.0f);
                    float tempY = mouseY / (ui.tileHeight / 2.0f);

                    float tileX = (tempX + tempY) / 2.0f - 2;
                    float tileY = (tempY - tempX) / 2.0f - 3.5;

                    // Ÿ�� ��ǥ�� ������ ��ȯ
                    int clickedTileX = static_cast<int>(std::floor(tileX));
                    int clickedTileY = static_cast<int>(std::floor(tileY));

                    // Ÿ�� ��ǥ�� �� ���� ���� �ִ��� Ȯ��
                    if (clickedTileX >= 0 && clickedTileX < static_cast<int>(mapWidth) &&
                        clickedTileY >= 0 && clickedTileY < static_cast<int>(mapHeight))
                    {
                        selectedX = clickedTileX;
                        selectedY = clickedTileY;
                        std::cout << "Ÿ�� ���õ�: (" << selectedX << ", " << selectedY << ")\n";
                    }
                }
            }
        }
    }
}

void GameManager::attackUnits(std::vector<Unit>& activeUnits, int currentTick, bool currentWaveType) {
    // ��� Ÿ�� ó��
    for (auto& tower : placedTowers) {
        if (tower.getIsNoDamage() == 1) continue; // ���� Ÿ���� �������� ����

        if (currentTick % tower.getTimePerAttack() != 0) continue; // ���� ƽ�� �ƴϸ� �Ѿ

        int range = tower.getAttackRange();
        int damage = tower.getDamage();
        int targetAmount = tower.getTargetAmount();
        int targetsAttacked = 0;

        // **�⺻ ���� ó��**
        std::vector<Unit*> aoeTargets; // ���� ���� ��� ����
        for (auto it = activeUnits.begin(); it != activeUnits.end() && targetsAttacked < targetAmount;) {
            int unitX = it->getX();
            int unitY = it->getY();
            int towerX = tower.getX();
            int towerY = tower.getY();

            int distanceSquared = (towerX - unitX) * (towerX - unitX) + (towerY - unitY) * (towerY - unitY);

            if (distanceSquared <= range * range) {
                // �⺻ ������ ó��
                int newHp = calculateDamage(tower.getIsMagic(), damage, *it);
                it->reduceHp(newHp);

                // ���� ���� ó��
                if (it->getHp() <= 0) {
                    if (currentWaveType) {
                        gold += it->getKillReward();
                    }
                    it = activeUnits.erase(it);
                    continue;
                }

                aoeTargets.push_back(&(*it)); // ���� ���� ����� ����
                targetsAttacked++;
            }
            ++it;
        }

        // **���� ���� ó��**
        if (tower.getIsNoDamage() == 2) {
            for (Unit* target : aoeTargets) { // �⺻ ���� ������κ��� ���� ���� ����
                for (auto aoeIt = activeUnits.begin(); aoeIt != activeUnits.end(); ) {
                    if (&(*aoeIt) == target) {
                        ++aoeIt;
                        continue; // �⺻ ���� ����� ����
                    }

                    int aoeDistanceSquared = (target->getX() - aoeIt->getX()) * (target->getX() - aoeIt->getX()) +
                        (target->getY() - aoeIt->getY()) * (target->getY() - aoeIt->getY());

                    if (aoeDistanceSquared <= 1 * 1) { // ���� ���� �Ÿ� 1ĭ
                        const int reducedDamage = damage - 1;
                        int finalAoeDamage = calculateDamage(tower.getIsMagic(), reducedDamage, *aoeIt);
                        aoeIt->reduceHp(finalAoeDamage);

                        if (aoeIt->getHp() <= 0) {
                            if (currentWaveType) {
                                gold += aoeIt->getKillReward();
                            }
                            aoeIt = activeUnits.erase(aoeIt);
                        }
                        else {
                            ++aoeIt;
                        }
                    }
                    else {
                        ++aoeIt;
                    }
                }
            }
        }
    }
}



void GameManager::createProjectile(const PlacedTower& tower, const Unit& targetUnit)
{
    float projectileOffsetX = 0;
    float projectileOffsetY = 50;
    int towerX = tower.getX();
    int towerY = tower.getY();
    int unitX = targetUnit.getX();
    int unitY = targetUnit.getY();

    // ȭ�� ��ǥ�� ��ȯ (�̼Ҹ�Ʈ�� ��ȯ ����)
    float startX = (towerX - towerY) * (ui.tileWidth / 2.0f) + ui.getWindow().getSize().x / 2.0f;
    float startY = (towerX + towerY) * (ui.tileHeight / 2.0f);

    float targetX = (unitX - unitY) * (ui.tileWidth / 2.0f) + ui.getWindow().getSize().x / 2.0f;
    float targetY = (unitX + unitY) * (ui.tileHeight / 2.0f);

    startX += projectileOffsetX;
    targetX += projectileOffsetX;
    startY += projectileOffsetY;
    targetY += projectileOffsetY;

    // ����ü ����
    float speed = 400.0f; // ����ü �ӵ� (ũ�� �����Ͽ� ������ �̵�)
    Projectile projectile(startX, startY, targetX, targetY, speed, ui.getProjectileTexture());

    projectiles.push_back(projectile);
}

void GameManager::updateProjectiles(sf::Time deltaTime)
{
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        it->update(deltaTime);

        if (it->hasReachedTarget())
        {
            it = projectiles.erase(it); // ����ü ����
        }
        else
        {
            ++it;
        }
    }
}

int GameManager::calculateDamage(bool damagetype, int damage, const Unit& unit)
{

    int actualDamage = 1;


    if (damagetype == 0) //���� ������
    {
        actualDamage = damage - unit.getArmor();
    }
    else if (damagetype == 1) //���� ������
    {
        actualDamage = damage - unit.getResist();
    }

    actualDamage = actualDamage > 0 ? actualDamage : 1;  // �ּ� ������ ����

    // ������ �پ�� ü�� ��ȯ
    return unit.getHp() - actualDamage;

}

void GameManager::startAttackWave(const Wave& wave, int& currentTick)
{
    std::vector<Unit> activeUnits;
    bool waveOver = false;
    int previousPlayerLife = playerLife;

    while (!waveOver)
    {
        handleAttackInput();

        updateAttackUnits(activeUnits);

        // Ÿ���� ������ ����
        attackUnits(activeUnits, currentTick, currentwaveType);

        // UI ������Ʈ �� �ܼ� ���
        updateGameState(activeUnits);
        updateAndPrintMap(activeUnits); // ���� ���̺꿡���� �� ���¸� ���

        waveOver = isAttackWaveOver(activeUnits);
        if (previousPlayerLife > playerLife) waveOver = true;// ���� ���� �� ���� ���̺� �ٷ� ����

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        currentTick++;
    }

    std::cout << "���� ���̺� ����!\n";
    if (previousPlayerLife > playerLife)
    {
        gold = static_cast<int>(gold * 1.2);
        std::cout << "���� ����! ���� ��ȭ�� �����߽��ϴ�. ���� ���: " << gold << "\n";
    }
}


void GameManager::handleAttackInput()
{
    sf::Event event;
    while (ui.getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            ui.getWindow().close();
            exit(0); // ���α׷� ����
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            int unitId = 0;
            if (event.key.code == sf::Keyboard::Num1)
                unitId = 1;
            else if (event.key.code == sf::Keyboard::Num2)
                unitId = 2;
            else if (event.key.code == sf::Keyboard::Num3)
                unitId = 3;

            if (unitId != 0)
            {
                auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                    [unitId](const UnitType& ut) { return ut.getId() == unitId; });

                if (it != unitTypes.end())
                {
                    const UnitType& unitType = *it;
                    if (attackGold >= unitType.getProductionCost())
                    {
                        attackGold -= unitType.getProductionCost();
                        unitProductionQueue.push(unitId);
                        std::cout << unitType.getUnitName() << " ������ ���� ��⿭�� �߰��Ǿ����ϴ�.\n";
                    }
                    else
                    {
                        std::cout << "��ȭ�� �����Ͽ� ������ ������ �� �����ϴ�.\n";
                    }
                }
            }
        }
    }
}

void GameManager::updateAttackUnits(std::vector<Unit>& activeUnits)
{
    // ���� ������ ��� �ִ��� Ȯ��
    bool sOccupied = false;
    for (const auto& unit : activeUnits)
    {
        if (unit.getX() == path[0].first && unit.getY() == path[0].second)
        {
            sOccupied = true;
            break;
        }
    }

    // ���� ������ ��� �ְ�, ��⿭�� ������ ���� ��쿡�� ������ ����
    if (!sOccupied && !unitProductionQueue.empty())
    {
        int unitId = unitProductionQueue.front();
        unitProductionQueue.pop();

        auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
            [unitId](const UnitType& ut) { return ut.getId() == unitId; });

        if (it != unitTypes.end())
        {
            const UnitType& unitType = *it;
            Unit unit(unitType.getId(), unitType.getUnitName(), unitType.getHp(),
                unitType.getTimePerMove(), unitType.getArmor(), unitType.getResist(),
                unitType.getKillReward(), path);
            activeUnits.push_back(unit);
        }
    }

    // ���� ������Ʈ
    for (auto it = activeUnits.begin(); it != activeUnits.end();)
    {
        bool arrived = it->update();
        if (arrived)
        {
            std::cout << it->getName() << " ������ �������� �����߽��ϴ�!\n";

            // �÷��̾� ������ ���� �߰�**
            playerLife -= 1;
            std::cout << "�÷��̾� �������� 1 �����߽��ϴ�. ���� ������: " << playerLife << "\n";

            // �������� 0 �����̸� ���� ����**
            if (playerLife <= 0)
            {
                std::cout << "Game Over!\n";
                ui.getWindow().close();
                exit(0); // ���α׷� ����
            }

            it = activeUnits.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool GameManager::isAttackWaveOver(const std::vector<Unit>& activeUnits)
{
    int minProductionCost = INT_MAX;
    for (const auto& unitType : unitTypes)
    {
        if (unitType.getProductionCost() < minProductionCost)
        {
            minProductionCost = unitType.getProductionCost();
        }
    }

    if (attackGold < minProductionCost && activeUnits.empty() && unitProductionQueue.empty())
    {
        return true;
    }

    return false;
}

void GameManager::handleTowerButtonClicked()
{
    if (isTowerPlacementMode)
    {
        attemptPlaceTower();
    }
    else
    {
        std::cout << "Ÿ�� ��ġ ��尡 �ƴմϴ�.\n";
    }
}

bool GameManager::isTileSelectable(int x, int y)
{
    // �� ���� ������ Ȯ��
    if (x < 0 || x >= static_cast<int>(map[0].size()) || y < 0 || y >= static_cast<int>(map.size()))
        return false;

    // �ش� Ÿ���� �� ��������
    std::string tileValue = map[y][x];

    // Ÿ���� ��ġ ������ Ÿ������ �Ǵ� �̹� Ÿ���� ��ġ�� Ÿ������ Ȯ��
    if (tileValue == "O")
    {
        // ��ġ ������ Ÿ��
        return true;
    }
    else
    {
        // �̹� ��ġ�� Ÿ���� �ִ��� Ȯ��
        auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
            [x, y](const PlacedTower& tower) {
                return tower.getX() == x && tower.getY() == y;
            });
        if (towerIt != placedTowers.end())
        {
            // Ÿ���� ��ġ�� Ÿ��
            return true;
        }
    }

    // Ÿ�� ��ġ �Ұ����� Ÿ��
    return false;
}

void GameManager::attemptPlaceTower()
{
    if (selectedTowerIndex >= 0)
    {
        if (map[selectedY][selectedX] == "O")
        {
            Tower selectedTower = towers[selectedTowerIndex];

            if (gold - selectedTower.getBuildCost() >= 0)
            {
                PlacedTower newTower(selectedTower, selectedX, selectedY);
                gold -= selectedTower.getBuildCost();

                map[selectedY][selectedX] = newTower.getTowerName();
                placedTowers.push_back(newTower);
                std::cout << newTower.getTowerName() << " Ÿ���� ��ġ�Ǿ����ϴ�!\n";
            }
            else
            {
                std::cout << "Ÿ���� ��ġ�� ��尡 �����մϴ�.\n";
            }
        }
        else
        {
            std::cout << "�ش� ��ġ�� Ÿ���� ��ġ�� �� �����ϴ�.\n";
        }
    }
    else
    {
        std::cout << "Ÿ���� ���õ��� �ʾҽ��ϴ�.\n";
    }
}