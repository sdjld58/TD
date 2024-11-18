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
    // UI 초기화
    ui.initialize(map);
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

    for (const auto& wave : waves)
    {
        int waveID = wave.getWaveID();
        gold += wave.getGold();

        // 웨이브 시작 전 준비 단계 시작
        startPreparationPhase();

        if (wave.getIsDefence())
        {
            if (wave.getLife() > 0)
            {
                playerLife = wave.getLife();
            }
            gold = wave.getGold();

            // 수비 웨이브 처리
            std::queue<Unit> unitQueue;
            for (int unitID : wave.getUnits())
            {
                auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                    [unitID](const UnitType& ut) { return ut.getId() == unitID; });
                if (it != unitTypes.end())
                {
                    UnitType& unitType = *it;
                    Unit unit(unitType.getId(), unitType.getUnitName(), unitType.getHp(),
                        unitType.getTimePerMove(), unitType.getArmor(), unitType.getResist(), unitType.getKillReward(), path);
                    unitQueue.push(unit);
                }
            }

            std::vector<Unit> activeUnits;

            const int maxTicks = 100;
            for (int tick = 0; tick < maxTicks; ++tick)
            {
                // 유닛 생성 로직
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

                // 유닛 업데이트
                for (auto it = activeUnits.begin(); it != activeUnits.end();)
                {
                    bool arrived = it->update();
                    if (arrived)
                    {
                        playerLife -= 1;
                        it = activeUnits.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // 타워가 유닛을 공격
                attackUnits(activeUnits);

                // 게임 상태 업데이트 및 화면 출력
                updateGameState(activeUnits);

                if (playerLife <= 0)
                {
                    std::cout << "Game Over!\n";
                    return;
                }

                if (activeUnits.empty() && unitQueue.empty())
                {
                    std::cout << "웨이브 " << waveID << " 클리어!\n";
                    break;
                }

                // 틱 사이에 시간 지연 추가
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        else {
            // 공격 웨이브 처리
            attackGold = wave.getGold();
            startAttackWave(wave);
        }
    }

    std::cout << "프로그램을 종료합니다.\n";
}

void GameManager::updateGameState(std::vector<Unit>& activeUnits)
{
    // UI를 통해 게임 상태를 업데이트하고 화면을 그립니다.
    ui.update(activeUnits, placedTowers, playerLife, gold);
}

void GameManager::loadMap(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "파일을 열 수 없습니다: " << filename << std::endl;
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
    // BFS 알고리즘을 사용하여 S에서 D까지의 경로를 찾습니다.
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
        std::cout << "경로를 찾을 수 없습니다." << std::endl;
    }
}

void GameManager::loadUnitTypes(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "유닛 타입 파일을 열 수 없습니다: " << filename << std::endl;
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
            std::cout << "유닛 타입 데이터를 파싱하는 중 오류 발생: " << line << std::endl;
        }
    }
    file.close();
}

void GameManager::loadTowerData(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "타워 데이터 파일을 열 수 없습니다: " << filename << std::endl;
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

        if (tokens.size() == 9)
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

            Tower tower(id, towerName, nextTowerID, buildCost, attackRange, damage, isMagic, timePerAttack, targetAmount);
            towers.push_back(tower);
        }
    }
    file.close();
}

void GameManager::loadWaves(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "웨이브 파일을 열 수 없습니다: " << filename << std::endl;
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
            std::cout << "웨이브 데이터를 파싱하는 중 오류 발생: " << line << std::endl;
        }
    }
    file.close();
}

void GameManager::startPreparationPhase()
{
    isPreparation = true;
    bool isTowerPlacementMode = false;

    int selectedX = map[0].size() / 2;
    int selectedY = map.size() / 2;
    int selectedTowerIndex = 0;

    while (isPreparation)
    {
        // UI를 통해 화면을 업데이트 (selectedX, selectedY 전달)
        ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY);

        sf::Event event;
        while (ui.getWindow().pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                ui.getWindow().close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::S)
                {
                    isPreparation = false;
                    break;
                }
                else if (event.key.code == sf::Keyboard::B)
                {
                    isTowerPlacementMode = !isTowerPlacementMode;
                }
                else if (isTowerPlacementMode)
                {
                    if (event.key.code == sf::Keyboard::Up && selectedY > 0)
                        selectedY--;
                    else if (event.key.code == sf::Keyboard::Down && selectedY < map.size() - 1)
                        selectedY++;
                    else if (event.key.code == sf::Keyboard::Left && selectedX > 0)
                        selectedX--;
                    else if (event.key.code == sf::Keyboard::Right && selectedX < map[0].size() - 1)
                        selectedX++;
                    else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9)
                    {
                        int towerID = event.key.code - sf::Keyboard::Num0;
                        auto it = std::find_if(towers.begin(), towers.end(),
                            [towerID](const Tower& tower) { return tower.getId() == towerID; });

                        if (it != towers.end())
                        {
                            selectedTowerIndex = std::distance(towers.begin(), it);
                            std::cout << "타워 " << it->getTowerName() << " 이 선택되었습니다.\n";
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter && selectedTowerIndex >= 0)
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
                                std::cout << newTower.getTowerName() << " 타워가 설치되었습니다!\n";
                            }
                            else
                            {
                                std::cout << "타워를 설치할 골드가 부족합니다.\n";
                            }
                        }
                    }
                }
            }

            // **마우스 클릭 이벤트 처리**
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // 마우스 왼쪽 버튼 클릭 시 처리
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(ui.getWindow());
                    float mouseX = static_cast<float>(mousePosition.x);
                    float mouseY = static_cast<float>(mousePosition.y);

                    // 이소메트릭 좌표 변환을 위한 보정값
                    float mapWidth = static_cast<float>(mapWithUnits[0].size());
                    float mapHeight = static_cast<float>(mapWithUnits.size());

                    // 화면 중심 좌표
                    float centerX = ui.getWindow().getSize().x / 2.0f;
                    float centerY = 0.0f; // 맵이 화면 상단에서 시작한다고 가정

                    // 화면 좌표를 타일 좌표로 변환
                    float tempX = (mouseX - centerX) / (ui.tileWidth / 2.0f);
                    float tempY = mouseY / (ui.tileHeight / 2.0f);

                    float tileX = (tempX + tempY) / 2.0f - 2;
                    float tileY = (tempY - tempX) / 2.0f - 3.5;

                    // 타일 좌표를 정수로 변환
                    int clickedTileX = static_cast<int>(std::floor(tileX));
                    int clickedTileY = static_cast<int>(std::floor(tileY));

                    // 타일 좌표가 맵 범위 내에 있는지 확인
                    if (clickedTileX >= 0 && clickedTileX < static_cast<int>(mapWidth) &&
                        clickedTileY >= 0 && clickedTileY < static_cast<int>(mapHeight))
                    {
                        selectedX = clickedTileX;
                        selectedY = clickedTileY;
                        std::cout << "타일 선택됨: (" << selectedX << ", " << selectedY << ")\n";
                    }
                }
            }
        }
    }
}

void GameManager::attackUnits(std::vector<Unit>& activeUnits)
{
    for (const auto& tower : placedTowers)
    {
        int range = tower.getAttackRange();
        int damage = tower.getDamage();
        int targetAmount = tower.getTargetAmount();
        int targetsAttacked = 0;

        for (auto it = activeUnits.begin(); it != activeUnits.end() && targetsAttacked < targetAmount;)
        {
            int unitX = it->getX();
            int unitY = it->getY();
            int towerX = tower.getX();
            int towerY = tower.getY();

            int distanceSquared = (towerX - unitX) * (towerX - unitX) + (towerY - unitY) * (towerY - unitY);

            if (distanceSquared <= range * range)
            {
                it->reduceHp(damage);

                if (it->getHp() <= 0)
                {
                    std::cout << it->getName() << " 유닛이 제거되었습니다!\n";
                    gold += it->getKillReward();
                    it = activeUnits.erase(it);
                    continue;
                }

                targetsAttacked++;
            }
            ++it;
        }
    }
}

void GameManager::startAttackWave(const Wave& wave)
{
    std::vector<Unit> activeUnits;
    bool waveOver = false;

    while (!waveOver)
    {
        handleAttackInput();

        updateAttackUnits(activeUnits);

        attackUnits(activeUnits);

        updateGameState(activeUnits);

        waveOver = isAttackWaveOver(activeUnits);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "공격 웨이브 종료!\n";
    if (playerLife > 0)
    {
        gold = static_cast<int>(gold * 1.2);
        std::cout << "공격 성공! 수비 재화가 증가했습니다. 현재 골드: " << gold << "\n";
    }
}

void GameManager::handleAttackInput()
{
    // SFML 이벤트 처리로 대체하거나 별도의 입력 방법을 구현해야 합니다.
    // 이 예제에서는 간단히 구현하지 않고 넘어갑니다.
}

void GameManager::updateAttackUnits(std::vector<Unit>& activeUnits)
{
    if (!unitProductionQueue.empty())
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

    for (auto it = activeUnits.begin(); it != activeUnits.end();)
    {
        bool arrived = it->update();
        if (arrived)
        {
            std::cout << it->getName() << " 유닛이 목적지에 도달했습니다!\n";
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
