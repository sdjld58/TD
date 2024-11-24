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
            startPreparationPhase(); // 수비 웨이브 준비
        }

        if (wave.getIsDefence())
        {
            if (wave.getLife() > 0)
            {
                playerLife = wave.getLife();
            }

            std::queue<Unit> unitQueue;

            // 웨이브에 포함된 유닛 생성
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

            // **게임 루프 수정 부분 시작**
            sf::Clock clock;
            sf::Time lastLogicUpdateTime = sf::Time::Zero;
            sf::Time logicUpdateInterval = sf::milliseconds(500); // 논리 업데이트 간격 (500ms)
            bool waveOver = false;

            while (!waveOver && ui.getWindow().isOpen())
            {
                sf::Time elapsedTime = clock.restart();

                // **이벤트 처리**
                sf::Event event;
                while (ui.getWindow().pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        ui.getWindow().close();
                        return;
                    }
                    // 기타 이벤트 처리 필요 시 추가
                }

                // **논리 업데이트 시간 체크**
                lastLogicUpdateTime += elapsedTime;
                if (lastLogicUpdateTime >= logicUpdateInterval)
                {
                    lastLogicUpdateTime -= logicUpdateInterval;
                    currentTick++;

                    // **논리 업데이트 수행**

                    // 유닛 스폰
                    spawnUnits(activeUnits, unitQueue);

                    // 유닛 이동
                    updateUnits(activeUnits);

                    // 공격 처리
                    attackUnits(activeUnits, currentTick, currentwaveType);

                    // 콘솔 출력
                    updateAndPrintMap(activeUnits);

                    // 게임 종료 조건 체크
                    if (playerLife <= 0)
                    {
                        std::cout << "Game Over!\n";
                        ui.getWindow().close();
                        return;
                    }

                    if (activeUnits.empty() && unitQueue.empty())
                    {
                        std::cout << "웨이브 " << waveID << " 클리어!\n";
                        waveOver = true;
                    }
                }

                // **투사체 업데이트 (매 프레임)**
                updateProjectiles(elapsedTime);

                // **화면 그리기**
                updateGameState(activeUnits);

                // **FPS 제한 (선택 사항)**
                // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // 약 60 FPS
            }
            // **게임 루프 수정 부분 끝**
        }
        else
        {
            // 공격 웨이브 처리
            attackGold = wave.getGold();
            startAttackWave(wave, currentTick);
        }
    }

    std::cout << "프로그램을 종료합니다.\n";
}

void GameManager::spawnUnits(std::vector<Unit>& activeUnits, std::queue<Unit>& unitQueue)
{
    // S 위치가 비어있으면 유닛 스폰
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
    // 유닛 업데이트
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

    // 유닛 위치를 맵에 표시
    for (const auto& unit : activeUnits)
    {
        int x = unit.getX();
        int y = unit.getY();
        if (y >= 0 && y < (int)mapWithUnits.size() && x >= 0 && x < (int)mapWithUnits[0].size())
        {
            mapWithUnits[y][x] = unit.getName();
        }
    }

    // 맵 출력
    std::cout << "=== 맵 상태 ===\n";
    for (const auto& row : mapWithUnits)
    {
        for (const auto& cell : row)
        {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }

    // 플레이어 상태 출력
    std::cout << "\n플레이어 라이프: " << playerLife << "\n";
    std::cout << "골드: " << gold << "\n";

    // 공격 웨이브 골드 출력
    std::cout << "공격 골드(AttackGold): " << attackGold << "\n";

    // 유닛 상태 출력
    std::cout << "\n=== 유닛 상태 ===\n";
    for (const auto& unit : activeUnits)
    {
        std::cout << "유닛: " << unit.getName()
            << ", 위치: (" << unit.getX() << ", " << unit.getY() << ")"
            << ", 체력: " << unit.getHp() << "\n";
    }
    // 유닛 생산 대기열 출력 추가
    if (!unitProductionQueue.empty())
    {
        std::cout << "\n=== 유닛 생산 대기열 ===\n";
        std::queue<int> tempQueue = unitProductionQueue; // 대기열 복사본 생성
        while (!tempQueue.empty())
        {
            int unitId = tempQueue.front();
            tempQueue.pop();

            // 유닛 ID로 유닛 이름 찾기
            auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                [unitId](const UnitType& ut) { return ut.getId() == unitId; });
            if (it != unitTypes.end())
            {
                std::cout << "유닛: " << it->getUnitName() << "\n";
            }
            else
            {
                std::cout << "유닛 ID: " << unitId << "\n";
            }
        }
    }
}





void GameManager::updateGameState(std::vector<Unit>& activeUnits)
{
    // UI를 통해 게임 상태를 업데이트하고 화면을 그립니다.
    ui.update(activeUnits, placedTowers, playerLife, gold, attackGold, selectedX, projectiles);
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

        if (tokens.size() == 12) // 변경된 데이터 크기 반영
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
            int isNodamage = std::stoi(tokens[9]); // 추가된 isNodamage 처리
            std::string tool = tokens[10];        // 추가된 tool 처리
            std::string tool2 = tokens[11];

            Tower tower(id, towerName, nextTowerID, buildCost, attackRange, damage,
                isMagic, timePerAttack, targetAmount, isNodamage, tool ,tool2);
            towers.push_back(tower);

            tower.displayInfo();
        }
        else
        {
            std::cout << "잘못된 데이터 형식이 있습니다: " << line << std::endl;
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
    std::vector<std::vector<std::string>> mapWithUnits = map;
    isPreparation = true;
    isTowerPlacementMode = false;  // 타워 설치 모드 상태 변수
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

            // **TGUI 이벤트 처리 추가**
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
                            std::cout << "타워 " << selectedTower.getTowerName() << " 이 선택되었습니다.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        else
                        {
                            std::cout << "해당 타워가 존재하지 않습니다.\n";
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

                            std::cout << "타워가 선택되었습니다. 1 키를 눌러 업그레이드하거나 S 키를 눌러 판매하세요. U 키를 다시 눌러 선택을 해제합니다.\n";

                            while (inTowerSelectionMode)
                            {
                                sf::Event towerEvent;
                                while (ui.getWindow().pollEvent(towerEvent))
                                {
                                    // **TGUI 이벤트 처리 추가**
                                    ui.gui.handleEvent(towerEvent);

                                    if (towerEvent.type == sf::Event::KeyPressed)
                                    {
                                        if (towerEvent.key.code == sf::Keyboard::Num1)
                                        {
                                            towerIt->upgrade(gold, map, towers,1);
                                            std::cout << "타워가 업그레이드되었습니다.\n";
                                            inTowerSelectionMode = false;
                                        }
                                        else if (towerEvent.key.code == sf::Keyboard::Num2)
                                        {
                                            towerIt->upgrade(gold, map, towers,2);
                                            std::cout << "타워가 업그레이드되었습니다.\n";
                                            inTowerSelectionMode = false;
                                        }
                                        else if (towerEvent.key.code == sf::Keyboard::S)
                                        {
                                            int refundAmount = static_cast<int>(towerIt->getBuildCost() * 0.3);
                                            gold += refundAmount;
                                            std::cout << "타워가 판매되었습니다. 반환된 골드: " << refundAmount << "\n";

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
                            std::cout << "현재 위치에 타워가 없습니다.\n";
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter && selectedTowerIndex >= 0)
                    {
                        attemptPlaceTower();
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

void GameManager::attackUnits(std::vector<Unit>& activeUnits, int currentTick, bool currentWaveType) {
    // 모든 타워 처리
    for (auto& tower : placedTowers) {
        if (tower.getIsNoDamage() == 1) continue; // 버프 타워는 공격하지 않음

        if (currentTick % tower.getTimePerAttack() != 0) continue; // 공격 틱이 아니면 넘어감

        int range = tower.getAttackRange();
        int damage = tower.getDamage();
        int targetAmount = tower.getTargetAmount();
        int targetsAttacked = 0;

        // **기본 공격 처리**
        std::vector<Unit*> aoeTargets; // 범위 공격 대상 저장
        for (auto it = activeUnits.begin(); it != activeUnits.end() && targetsAttacked < targetAmount;) {
            int unitX = it->getX();
            int unitY = it->getY();
            int towerX = tower.getX();
            int towerY = tower.getY();

            int distanceSquared = (towerX - unitX) * (towerX - unitX) + (towerY - unitY) * (towerY - unitY);

            if (distanceSquared <= range * range) {
                // 기본 데미지 처리
                int newHp = calculateDamage(tower.getIsMagic(), damage, *it);
                it->reduceHp(newHp);

                // 유닛 제거 처리
                if (it->getHp() <= 0) {
                    if (currentWaveType) {
                        gold += it->getKillReward();
                    }
                    it = activeUnits.erase(it);
                    continue;
                }

                aoeTargets.push_back(&(*it)); // 범위 공격 대상을 저장
                targetsAttacked++;
            }
            ++it;
        }

        // **범위 공격 처리**
        if (tower.getIsNoDamage() == 2) {
            for (Unit* target : aoeTargets) { // 기본 공격 대상으로부터 범위 공격 수행
                for (auto aoeIt = activeUnits.begin(); aoeIt != activeUnits.end(); ) {
                    if (&(*aoeIt) == target) {
                        ++aoeIt;
                        continue; // 기본 공격 대상은 제외
                    }

                    int aoeDistanceSquared = (target->getX() - aoeIt->getX()) * (target->getX() - aoeIt->getX()) +
                        (target->getY() - aoeIt->getY()) * (target->getY() - aoeIt->getY());

                    if (aoeDistanceSquared <= 1 * 1) { // 범위 공격 거리 1칸
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

    // 화면 좌표로 변환 (이소메트릭 변환 적용)
    float startX = (towerX - towerY) * (ui.tileWidth / 2.0f) + ui.getWindow().getSize().x / 2.0f;
    float startY = (towerX + towerY) * (ui.tileHeight / 2.0f);

    float targetX = (unitX - unitY) * (ui.tileWidth / 2.0f) + ui.getWindow().getSize().x / 2.0f;
    float targetY = (unitX + unitY) * (ui.tileHeight / 2.0f);

    startX += projectileOffsetX;
    targetX += projectileOffsetX;
    startY += projectileOffsetY;
    targetY += projectileOffsetY;

    // 투사체 생성
    float speed = 400.0f; // 투사체 속도 (크게 설정하여 빠르게 이동)
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
            it = projectiles.erase(it); // 투사체 제거
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


    if (damagetype == 0) //물리 데미지
    {
        actualDamage = damage - unit.getArmor();
    }
    else if (damagetype == 1) //마법 데미지
    {
        actualDamage = damage - unit.getResist();
    }

    actualDamage = actualDamage > 0 ? actualDamage : 1;  // 최소 데미지 보장

    // 유닛의 줄어든 체력 반환
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

        // 타워가 유닛을 공격
        attackUnits(activeUnits, currentTick, currentwaveType);

        // UI 업데이트 및 콘솔 출력
        updateGameState(activeUnits);
        updateAndPrintMap(activeUnits); // 공격 웨이브에서도 맵 상태를 출력

        waveOver = isAttackWaveOver(activeUnits);
        if (previousPlayerLife > playerLife) waveOver = true;// 공격 성공 시 공격 웨이브 바로 종료

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        currentTick++;
    }

    std::cout << "공격 웨이브 종료!\n";
    if (previousPlayerLife > playerLife)
    {
        gold = static_cast<int>(gold * 1.2);
        std::cout << "공격 성공! 수비 재화가 증가했습니다. 현재 골드: " << gold << "\n";
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
            exit(0); // 프로그램 종료
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
                        std::cout << unitType.getUnitName() << " 유닛이 생산 대기열에 추가되었습니다.\n";
                    }
                    else
                    {
                        std::cout << "재화가 부족하여 유닛을 생산할 수 없습니다.\n";
                    }
                }
            }
        }
    }
}

void GameManager::updateAttackUnits(std::vector<Unit>& activeUnits)
{
    // 스폰 지점이 비어 있는지 확인
    bool sOccupied = false;
    for (const auto& unit : activeUnits)
    {
        if (unit.getX() == path[0].first && unit.getY() == path[0].second)
        {
            sOccupied = true;
            break;
        }
    }

    // 스폰 지점이 비어 있고, 대기열에 유닛이 있을 경우에만 유닛을 생성
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

    // 유닛 업데이트
    for (auto it = activeUnits.begin(); it != activeUnits.end();)
    {
        bool arrived = it->update();
        if (arrived)
        {
            std::cout << it->getName() << " 유닛이 목적지에 도달했습니다!\n";

            // 플레이어 라이프 감소 추가**
            playerLife -= 1;
            std::cout << "플레이어 라이프가 1 감소했습니다. 현재 라이프: " << playerLife << "\n";

            // 라이프가 0 이하이면 게임 종료**
            if (playerLife <= 0)
            {
                std::cout << "Game Over!\n";
                ui.getWindow().close();
                exit(0); // 프로그램 종료
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
        std::cout << "타워 설치 모드가 아닙니다.\n";
    }
}

bool GameManager::isTileSelectable(int x, int y)
{
    // 맵 범위 내인지 확인
    if (x < 0 || x >= static_cast<int>(map[0].size()) || y < 0 || y >= static_cast<int>(map.size()))
        return false;

    // 해당 타일의 값 가져오기
    std::string tileValue = map[y][x];

    // 타워가 설치 가능한 타일인지 또는 이미 타워가 설치된 타일인지 확인
    if (tileValue == "O")
    {
        // 설치 가능한 타일
        return true;
    }
    else
    {
        // 이미 설치된 타워가 있는지 확인
        auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
            [x, y](const PlacedTower& tower) {
                return tower.getX() == x && tower.getY() == y;
            });
        if (towerIt != placedTowers.end())
        {
            // 타워가 설치된 타일
            return true;
        }
    }

    // 타워 설치 불가능한 타일
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
                std::cout << newTower.getTowerName() << " 타워가 설치되었습니다!\n";
            }
            else
            {
                std::cout << "타워를 설치할 골드가 부족합니다.\n";
            }
        }
        else
        {
            std::cout << "해당 위치에 타워를 설치할 수 없습니다.\n";
        }
    }
    else
    {
        std::cout << "타워가 선택되지 않았습니다.\n";
    }
}