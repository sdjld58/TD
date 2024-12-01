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

    ui.onTowerButtonClicked = [this]()
        {
        this->handleTowerButtonClicked();
        };

}

void GameManager::run()
{
   
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
        ui.setIsDefence(currentwaveType);
        ui.clearButtons();
        ui.drawButtons();
      
        if (wave.getIsDefence())
        {
            gold += wave.getGold();
            ui.setInfoText({ "타워를 설치하세요!!", "빈 건설 부지입니다..", "1번 : 검사 타워 \n2번 : 궁수 타워 \n3번 : 마법사 타워" });
            startPreparationPhase(); // 수비 웨이브 준비
            ui.setInfoText({ "적들이 몰려오고 있습니다!!", "이 공격을 막지 못하면 끝입니다!", "..." });
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
                        unitType.getKillReward(), path, unitType.getLifeDamage());
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
                    spawnUnits(activeUnits, unitQueue,currentTick);

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
            ui.setInfoText({ "침투를 준비하세요!!", "대기중인 유닛이 없습니다!", "..." });

            // 공격 웨이브 처리
            attackGold = wave.getGold();
            ui.updateattackGold(attackGold); //ui로 공격재화 표시하기 위해
            startAttackWave(wave, currentTick);
        }
    }


    std::cout << "프로그램을 종료합니다.\n";
}

void GameManager::spawnUnits(std::vector<Unit>& activeUnits, std::queue<Unit>& unitQueue,int currentTick)
{
    // **스폰 간격 변수**
    static int lastSpawnTick = 0;       // 마지막으로 유닛이 생성된 틱
    const int spawnInterval = 2;      // 유닛 생성 간격 (틱 단위)

    // **틱 간격에 따라 유닛 생성**
    if (currentTick - lastSpawnTick >= spawnInterval && !unitQueue.empty())
    {
        Unit unit = unitQueue.front();
        unitQueue.pop();

        // 유닛을 스폰 위치에 추가
        activeUnits.push_back(unit);
        lastSpawnTick = currentTick;   // 마지막 생성 틱 업데이트
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
            playerLife -= it->getLifeDamage();
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
    ui.update(activeUnits, placedTowers, playerLife, gold, selectedX,selectedY ,projectiles);
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

        if (tokens.size() == 9)
        {
            int id = std::stoi(tokens[0]);
            std::string unitName = tokens[1];
            int hp = std::stoi(tokens[2]);
            int timePerMove = std::stoi(tokens[3]);
            int armor = std::stoi(tokens[4]);
            int resist = std::stoi(tokens[5]);
            int productionCost = std::stoi(tokens[6]);
            int killReward = std::stoi(tokens[7]);
            int LifeDamage = std::stoi(tokens[8]);


            UnitType unitType(id, unitName, hp, timePerMove, armor, resist, productionCost, killReward, LifeDamage);
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
            int nextTowerID2 = std::stoi(tokens[3]);
            int attackRange = std::stoi(tokens[4]);
            int damage = std::stoi(tokens[5]);
            bool isMagic = std::stoi(tokens[6]) == 1;
            int timePerAttack = std::stoi(tokens[7]);
            int targetAmount = std::stoi(tokens[8]);
            int isNodamage = std::stoi(tokens[9]); // 추가된 isNodamage 처리
            std::string tool = tokens[10];        // 추가된 tool 처리
            std::string tool2 = tokens[11];

            Tower tower(id, towerName, nextTowerID, nextTowerID2, attackRange, damage,
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
    isTowerPlacementMode = true;  // 타워 설치 모드 상태 변수
    bool isTowerSelected = false;
    int selectedOption = -1;

    selectedX = mapWithUnits[0].size() / 2;
    selectedY = mapWithUnits.size() / 2;
    selectedTowerIndex = -1;  // 초기값 -1로 설정 (선택되지 않음)

    while (isPreparation)
    {
        ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY);
       

        sf::Event event;
        while (ui.getWindow().pollEvent(event))
        {
            

            // **TGUI 이벤트 처리 추가**
            ui.gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
            {
                ui.getWindow().close();
                return;
            }

            if (event.type == sf::Event::KeyPressed)
            {
             
                if (event.key.code == sf::Keyboard::F)
                {
                    isPreparation = false;
                    break;
                }

                // WASD로 타일 이동
                if (event.key.code == sf::Keyboard::W && selectedY > 0)
                    selectedY--;
                else if (event.key.code == sf::Keyboard::S && selectedY < mapWithUnits.size() - 1)
                    selectedY++;
                else if (event.key.code == sf::Keyboard::A && selectedX > 0)
                    selectedX--;
                else if (event.key.code == sf::Keyboard::D && selectedX < mapWithUnits[0].size() - 1)
                    selectedX++;

                else if (event.key.code == sf::Keyboard::Escape)
                {
                    selectedTowerIndex = -1; // 선택된 타워 초기화
                    std::cout << "타워 선택이 초기화되었습니다.\n";

                    ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI 갱신
                }

                // 1~9 키로 타워 선택
                else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9 && isTowerPlacementMode == true)
                {
                    int towerID = event.key.code - sf::Keyboard::Num0;
                    auto it = std::find_if(towers.begin(), towers.end(),
                        [towerID](const Tower& tower) { return tower.getId() == towerID; });

                    if (it != towers.end())
                    {
                        Tower& selectedTower = *it;
                        selectedTowerIndex = std::distance(towers.begin(), it);
                        std::cout << "타워 " << selectedTower.getTowerName() << " 이 선택되었습니다.\n";
                        ui.setInfoText({ selectedTower.getTool() + " 타워가 선택되었습니다"," "," "});
                    }
                    else
                    {
                        auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
                            [this](const PlacedTower& tower) 
                            {
                                return tower.getX() == selectedX && tower.getY() == selectedY;
                            });
                    }
                }

                // Space 키로 타워 배치
                else if (event.key.code == sf::Keyboard::Space && selectedTowerIndex >= 0 && isTowerPlacementMode == true)
                {
                    attemptPlaceTower();
                }

                // 현재 선택된 타일에 있는 타워와 상호작용
                auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
                    [this](const PlacedTower& tower)
                    {
                        return tower.getX() == selectedX && tower.getY() == selectedY;
                    });

                // 타워가 있는지 확인하고 모드 설정
                if (towerIt != placedTowers.end())
                {
                    isTowerPlacementMode = false; // 타워가 있으면 업그레이드/판매 모드로 전환
                }
                else
                {
                    isTowerPlacementMode = true; // 타워가 없으면 설치 모드로 전환
                }

                static int selectedOption = -1; // -1: 아무것도 선택하지 않음

                if (towerIt != placedTowers.end())
                {
                    // 업그레이드/판매 선택
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        selectedOption = 1; // 옵션 1 선택
                        std::cout << "옵션 1 선택됨.\n";
                    }
                    else if (event.key.code == sf::Keyboard::Num2 && towerIt->getTowerName() == "2")
                    {
                        selectedOption = 2; // 옵션 2 선택
                        std::cout << "옵션 2 선택됨.\n";
                    }
                    else if (event.key.code == sf::Keyboard::Num3)
                    {
                        selectedOption = 3; // 옵션 3 선택
                        std::cout << "옵션 3 (판매) 선택됨.\n";
                    }
                    else if (event.key.code == sf::Keyboard::Space && selectedOption > 0) // Space로 실행
                    {
                        if (selectedOption == 1) // 업그레이드 (1번)
                        {
                            towerIt->upgrade(gold, map, towers, 1);
                            
                        }
                        else if (selectedOption == 2) // 업그레이드 (2번)
                        {
                            towerIt->upgrade(gold, map, towers, 2);
                            
                        }
                        else if (selectedOption == 3) // 타워 판매 (3번)
                        {
                            int refundAmount = 30;
                            if (towerIt->getTowerName() == "1")                //기본타워 판매 비용
                            {
                                refundAmount = 30;
                            }
                            else if (towerIt->getTowerName() == "2")           //중간단계타워 판매 비용
                            {
                                refundAmount = 40;
                            }
                            else if (towerIt->getTowerName() == "3")           //최종단계타워 판매 비용
                            {
                                refundAmount = 50;
                            }
                            gold += refundAmount;
                            std::cout << "타워가 판매되었습니다. 반환된 골드: " << refundAmount << "\n";

                            map[towerIt->getY()][towerIt->getX()] = "O";
                            placedTowers.erase(towerIt);
                        }

                        selectedOption = -1; // 선택 초기화
                    }
                    else 
                    {
                        selectedOption = -1; // 선택 취소
                    }
                }

            }

            // 마우스 클릭으로 타일 선택
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(ui.getWindow());
                float mouseX = static_cast<float>(mousePosition.x);
                float mouseY = static_cast<float>(mousePosition.y);

                float mapWidth = static_cast<float>(mapWithUnits[0].size());
                float mapHeight = static_cast<float>(mapWithUnits.size());

                float centerX = ui.getWindow().getSize().x / 2.0f;
                float centerY = 0.0f;

                float tempX = (mouseX - centerX) / (ui.tileWidth / 2.0f);
                float tempY = mouseY / (ui.tileHeight / 2.0f);

                float tileX = (tempX + tempY) / 2.0f - 2;
                float tileY = (tempY - tempX) / 2.0f - 3.5;

                int clickedTileX = static_cast<int>(std::floor(tileX));
                int clickedTileY = static_cast<int>(std::floor(tileY));

                if (clickedTileX >= 0 && clickedTileX < static_cast<int>(mapWidth) &&
                    clickedTileY >= 0 && clickedTileY < static_cast<int>(mapHeight))
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
                        ui.setInfoText({ "침투에 대비하세요!\n",
                            "",
                            "1번 : 검사 타워 \n2번 : 궁수 타워 \n3번 : 마법사 타워" });
                    }
                }
            }
        }
    }
}

void GameManager::attackUnits(std::vector<Unit>& activeUnits, int currentTick, bool currentWaveType)
{
    // 모든 타워 처리
    for (auto& tower : placedTowers) 
    {
        if (tower.getIsNoDamage() == 1) continue; // 버프 타워는 공격하지 않음

        if (currentTick % tower.getTimePerAttack() != 0) continue; // 공격 틱이 아니면 넘어감

        int range = tower.getAttackRange();
        int damage = tower.getDamage();
        int targetAmount = tower.getTargetAmount();
        int targetsAttacked = 0;

        // **기본 공격 처리**
        std::vector<Unit*> aoeTargets; // 범위 공격 대상 저장

        for (auto it = activeUnits.begin(); it != activeUnits.end() && targetsAttacked < targetAmount;)
        {
            int unitX = it->getX();
            int unitY = it->getY();
            int towerX = tower.getX();
            int towerY = tower.getY();

            int distanceSquared = (towerX - unitX) * (towerX - unitX) + (towerY - unitY) * (towerY - unitY);

            if (distanceSquared <= range * range) 
            {
                //투사체 생성
                createProjectile(tower, *it);

                // 기본 데미지 처리
                int newHp = calculateDamage(tower.getIsMagic(), damage, *it);
                it->reduceHp(newHp);

                // 유닛 제거 처리
                if (it->getHp() <= 0) 
                {
                    if (currentWaveType)
                    
                    {
                        gold += it->getKillReward();
                    }
                    it = activeUnits.erase(it);
                    break;
                }

                aoeTargets.push_back(&(*it)); // 범위 공격 대상을 저장
                targetsAttacked++;
            }
            ++it;
        }

        // **범위 공격 처리**
        if (tower.getIsNoDamage() == 2)
        {
            for (Unit* target : aoeTargets)
            { // 기본 공격 대상으로부터 범위 공격 수행
                for (auto aoeIt = activeUnits.begin(); aoeIt != activeUnits.end(); )
                {
                    if (&(*aoeIt) == target) 
                    {
                        ++aoeIt;
                        continue; // 기본 공격 대상은 제외
                    }

                    int aoeDistanceSquared = (target->getX() - aoeIt->getX()) * (target->getX() - aoeIt->getX()) +
                        (target->getY() - aoeIt->getY()) * (target->getY() - aoeIt->getY());

                    if (aoeDistanceSquared <= 1 * 1) { // 범위 공격 거리 1칸
                        const int reducedDamage = damage - 1;
                        int finalAoeDamage = calculateDamage(tower.getIsMagic(), reducedDamage, *aoeIt);
                        aoeIt->reduceHp(finalAoeDamage);

                        if (aoeIt->getHp() <= 0) 
                        {
                            if (currentWaveType)
                            {
                                gold += aoeIt->getKillReward();
                            }
                            aoeIt = activeUnits.erase(aoeIt);
                        }
                        else
                        {
                            ++aoeIt;
                        }
                    }
                    else
                    {
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

    sf::Clock clock;
    sf::Time lastLogicUpdateTime = sf::Time::Zero;
    sf::Time logicUpdateInterval = sf::milliseconds(500); // 논리 업데이트 간격 (500ms)
    
    while (!unitProductionQueue.empty()) { unitProductionQueue.pop(); }// 유닛 생산 대기열 초기화

    while (!waveOver)
    {
        sf::Time elapsedTime = clock.restart();

        handleAttackInput();

        // **논리 업데이트 시간 체크**
        lastLogicUpdateTime += elapsedTime;
        if (lastLogicUpdateTime >= logicUpdateInterval)
        {
            lastLogicUpdateTime -= logicUpdateInterval;
            currentTick++;

            updateAttackUnits(activeUnits);

            // 타워가 유닛을 공격
            attackUnits(activeUnits, currentTick, currentwaveType);
            ui.updateattackGold(attackGold);
            updateAndPrintMap(activeUnits); // 공격 웨이브에서도 맵 상태를 출력
        }

        // **투사체 업데이트 (매 프레임)**
        updateProjectiles(elapsedTime);

        // UI 업데이트 및 콘솔 출력
        updateGameState(activeUnits);

        waveOver = isAttackWaveOver(activeUnits);
        if (previousPlayerLife > playerLife) waveOver = true;// 공격 성공 시 공격 웨이브 바로 종료

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (!unitProductionQueue.empty()) { unitProductionQueue.pop(); }// 유닛 생산 대기열 초기화,공격웨이브가 끝나고 바로 초기화

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
                unitType.getKillReward(), path, unitType.getLifeDamage());
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
            playerLife -= it->getLifeDamage();
            

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
        ui.setInfoText({ "타워 설치 모드가 아닙니다.\n","...","..."});
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
void GameManager::selectTower(const std::vector<Tower>& towers, int towerId, int& selectedTowerIndex, UI& ui)
{
    const auto it = std::find_if(towers.begin(), towers.end(),
        [towerId](const Tower& tower) { return tower.getId() == towerId; });

    if (it != towers.end()) {
        const Tower& selectedTower = *it; // const Tower& 사용
        selectedTowerIndex = std::distance(towers.begin(), it);
        std::cout << "타워 " << selectedTower.getTowerName() << " 이 선택되었습니다.\n";
        ui.setInfoText({ selectedTower.getTool() + "타워가 선택되었습니다.\n", selectedTower.getTool2(), "..." });
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else {
        // 타워가 존재하지 않을 때
        std::cout << "해당 타워가 존재하지 않습니다.\n";

        // 1초 대기
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void GameManager::attemptPlaceTower()
{
    if (selectedTowerIndex >= 0)
    {
        if (map[selectedY][selectedX] == "O")
        {
            Tower selectedTower = towers[selectedTowerIndex];

            if (gold - 30 >= 0)                                      //타워 건설비용 
            {
                PlacedTower newTower(selectedTower, selectedX, selectedY);
                gold -= 30;

                map[selectedY][selectedX] = newTower.getTowerName();
                placedTowers.push_back(newTower);
                std::cout << newTower.getTowerName() << " 타워가 설치되었습니다!\n";
                
              
                ui.setInfoText({ newTower.getTool() + " 타워가 설치되었습니다!\n",
                    newTower.getTool2(),
                    "공격력: " + std::to_string(newTower.getDamage()) +
                    "\n공격 속도: " + std::to_string(newTower.getTimePerAttack()) +
                    "\n사거리: " + std::to_string(newTower.getAttackRange()) +
                    "\n공격 유형: " + newTower.attackType() });
            }
            else
            {
                std::cout << "타워를 설치할 골드가 부족합니다.\n";
                ui.setInfoText({"타워를 설치할 골드가 부족합니다.\n", "...", "..."});
            }
        }
        else
        {
            std::cout << "해당 위치에 타워를 설치할 수 없습니다.\n";
            ui.setInfoText({ "해당 위치에 타워를 설치할 수 없습니다.\n","...","..." });
        }
    }
    else
    {
        std::cout << "타워가 선택되지 않았습니다.\n";
        ui.setInfoText({ "타워가 선택되지 않았습니다.\n","...","..." });
    }
}

void GameManager::mapSelected()
{
    // TGUI GUI 생성
    tgui::Gui gui(ui.getWindow());

    // 배경색을 흰색으로 설정
    sf::RectangleShape background(sf::Vector2f(ui.getWindow().getSize().x, ui.getWindow().getSize().y));
    background.setFillColor(sf::Color::White);

    // 텍스트 생성 및 설정
    auto titleLabel = tgui::Label::create("Select   A   Track");
    titleLabel->setPosition("50%", "7%");
    titleLabel->setOrigin(0.5f, 0.5f);
    titleLabel->setTextSize(140);
    titleLabel->getRenderer()->setTextColor(sf::Color::Black);
    tgui::Font font("resources/fonts/Bangers.ttf");
    titleLabel->getRenderer()->setFont(font);
    gui.add(titleLabel);

    // 맵 선택 버튼 생성 및 크기 설정
    auto button1 = tgui::Button::create();
    button1->setPosition("5%", "17%");
    button1->setSize("28%", "70%");
    button1->getRenderer()->setTexture("resources/images/stageImgs/spring.png"); // 버튼 이미지
    button1->getRenderer()->setOpacity(0.5f); // 기본 투명도 설정
    button1->onMouseEnter([button1]() { button1->getRenderer()->setOpacity(1.0f); }); // 마우스 오버 시 투명도 제거
    button1->onMouseLeave([button1]() { button1->getRenderer()->setOpacity(0.5f); }); // 마우스가 떠나면 다시 반투명
    gui.add(button1);

    auto button2 = tgui::Button::create();
    button2->setPosition("36.5%", "17%");
    button2->setSize("28%", "70%");
    button2->getRenderer()->setTexture("resources/images/stageImgs/dessert.png");
    button2->getRenderer()->setOpacity(0.5f);
    button2->onMouseEnter([button2]() { button2->getRenderer()->setOpacity(1.0f); });
    button2->onMouseLeave([button2]() { button2->getRenderer()->setOpacity(0.5f); });
    gui.add(button2);

    auto button3 = tgui::Button::create();
    button3->setPosition("68%", "17%");
    button3->setSize("28%", "70%");
    button3->getRenderer()->setTexture("resources/images/stageImgs/winter.png");
    button3->getRenderer()->setOpacity(0.5f);
    button3->onMouseEnter([button3]() { button3->getRenderer()->setOpacity(1.0f); });
    button3->onMouseLeave([button3]() { button3->getRenderer()->setOpacity(0.5f); });
    gui.add(button3);

    // Exit 버튼 생성 및 이미지 적용
    auto exitButton = tgui::Button::create(); // 텍스트 없는 버튼
    exitButton->setPosition("90%", "4%");          // 오른쪽 위 끝
    exitButton->setSize("7%", "10%");              // Exit 버튼 크기
    exitButton->getRenderer()->setTexture("resources/images/stageImgs/exit.png"); // X 이미지 적용
    // 테두리 제거
    exitButton->getRenderer()->setBorders(0); // 테두리 두께를 0으로 설정
    exitButton->getRenderer()->setBorderColor(sf::Color::Transparent); // 테두리 색상을 투명으로 설정

    exitButton->getRenderer()->setOpacity(0.5f);   // 기본 투명도 설정
    exitButton->onMouseEnter([exitButton]() {
        exitButton->getRenderer()->setOpacity(1.0f); // 마우스 오버 시 불투명
        });
    exitButton->onMouseLeave([exitButton]() {
        exitButton->getRenderer()->setOpacity(0.5f); // 마우스 떠나면 다시 반투명
        });
    gui.add(exitButton);

    // 버튼 아래 텍스트(Label) 추가
    auto label1 = tgui::Label::create("B e g i n n e r");
    label1->setTextSize(60);
    label1->getRenderer()->setTextColor(sf::Color::Black);
    label1->getRenderer()->setFont(font);
    label1->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center); // 텍스트 중앙 정렬
    label1->setPosition(
        tgui::bindLeft(button1) + tgui::bindWidth(button1) / 2, // 버튼의 가로 중앙
        tgui::bindBottom(button1) + 10                         // 버튼 아래 약간 띄움
    );
    label1->setOrigin(0.5f, 0.0f); // 라벨 중앙 기준 정렬
    gui.add(label1);

    auto label2 = tgui::Label::create("I n t e r m e d i a t e");
    label2->setTextSize(60);
    label2->getRenderer()->setTextColor(sf::Color::Black);
    label2->getRenderer()->setFont(font);
    label2->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center); // 텍스트 중앙 정렬
    label2->setPosition(
        tgui::bindLeft(button2) + tgui::bindWidth(button2) / 2, // 버튼의 가로 중앙
        tgui::bindBottom(button2) + 10                         // 버튼 아래 약간 띄움
    );
    label2->setOrigin(0.5f, 0.0f); // 라벨 중앙 기준 정렬
    gui.add(label2);

    auto label3 = tgui::Label::create("A d v a n c e d");
    label3->setTextSize(60);
    label3->getRenderer()->setTextColor(sf::Color::Black);
    label3->getRenderer()->setFont(font);
    label3->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center); // 텍스트 중앙 정렬
    label3->setPosition(
        tgui::bindLeft(button3) + tgui::bindWidth(button3) / 2, // 버튼의 가로 중앙
        tgui::bindBottom(button3) + 10                         // 버튼 아래 약간 띄움
    );
    label3->setOrigin(0.5f, 0.0f); // 라벨 중앙 기준 정렬
    gui.add(label3);

    bool mapChosen = false;
    std::string selectedMap;
    // 버튼 클릭 이벤트 설정
    button1->onClick([&]()
        {
            selectedMap = "Map1.csv";
            mapChosen = true;
        });

    button2->onClick([&]()
        {
            selectedMap = "Map2.csv";
            mapChosen = true;
        });

    button3->onClick([&]()
        {
            selectedMap = "Map3.csv";
            mapChosen = true;
        });

    exitButton->onClick([&]()
        {
            gameStart();
        });

    // GUI 루프
    while (ui.getWindow().isOpen() && !mapChosen)
    {
        sf::Event event;
        while (ui.getWindow().pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                ui.getWindow().close();
                return;
            }

            gui.handleEvent(event);
        }

        // 화면 그리기
        ui.getWindow().clear();
        ui.getWindow().draw(background);
        gui.draw();
        ui.getWindow().display();
    }

    if (mapChosen)
    {
        std::cout << selectedMap << " 맵이 선택되었습니다.\n";
        loadMap(selectedMap);
        parsePath();
        run(); // 선택된 맵으로 게임 실행
    }
}

void GameManager::gameStart() {
    // TGUI GUI 생성
    tgui::Gui gui(ui.getWindow());

    // 첫 번째 반은 다크 블루
    sf::RectangleShape leftBackground(sf::Vector2f(ui.getWindow().getSize().x / 2, ui.getWindow().getSize().y));
    leftBackground.setFillColor(sf::Color(0, 0, 139)); // 다크 블루

    // 두 번째 반은 레드
    sf::RectangleShape rightBackground(sf::Vector2f(ui.getWindow().getSize().x / 2, ui.getWindow().getSize().y));
    rightBackground.setFillColor(sf::Color::Red);
    rightBackground.setPosition(ui.getWindow().getSize().x / 2, 0);

    // 폰트 로드
    sf::Font bmFont;
    if (!bmFont.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        std::cerr << "Failed to load BMDOHYEON_ttf.ttf" << std::endl;
        return;
    }

    // 메인 타이틀 설정
    sf::Text mainTitle;
    mainTitle.setFont(bmFont);
    mainTitle.setString(L"전 장 의   스 파 이");
    mainTitle.setCharacterSize(140); // 크기 조정
    mainTitle.setFillColor(sf::Color::White);
    mainTitle.setOrigin(mainTitle.getLocalBounds().width / 2.f, mainTitle.getLocalBounds().height / 2.f);
    mainTitle.setPosition(ui.getWindow().getSize().x / 2.f, ui.getWindow().getSize().y * 0.2f); // 위치 설정

    // "Spy of the Battlefield" 텍스트 추가 (서브 타이틀)
    auto subTitle = tgui::Label::create("Spy of the Battlefield");
    subTitle->setPosition("50%", "35%");          // 메인 타이틀 아래에 배치
    subTitle->setOrigin(0.5f, 0.5f);              // 중앙 정렬
    subTitle->setTextSize(70);                    // 텍스트 크기
    subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, 0)); // 투명도 0으로 시작

    // Bangers 폰트 적용
    tgui::Font bangersFont("resources/fonts/Bangers.ttf");    // Bangers 폰트 경로
    subTitle->getRenderer()->setFont(bangersFont);           // 폰트 설정
    gui.add(subTitle);

    // "Start Game" 버튼 추가
    auto startButton = tgui::Button::create("Start Game");
    startButton->setPosition("40%", "60%");
    startButton->setSize("20%", "10%");

    // 버튼 배경과 텍스트 색상 설정
    startButton->getRenderer()->setBackgroundColor(sf::Color::Green);
    startButton->getRenderer()->setTextColor(sf::Color::White);
    startButton->getRenderer()->setFont(bangersFont);      // 버튼 텍스트의 폰트 설정

    // 텍스트 크기 조정
    startButton->setTextSize(40); // 원하는 글씨 크기 (단위: 픽셀)

    // 버튼을 GUI에 추가
    gui.add(startButton);

    // 버튼 클릭 이벤트 설정
    bool gameStarted = false;
    startButton->onClick([&]() {
        gameStarted = true;
        });

    // 애니메이션 변수
    sf::Clock clock; // 시간 측정을 위한 SFML 시계
    float fadeInTime = 3.0f; // 서브 타이틀 페이드인 지속 시간

    // GUI 루프
    while (ui.getWindow().isOpen() && !gameStarted) {
        sf::Event event;
        while (ui.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                ui.getWindow().close();
                return;
            }
            gui.handleEvent(event);
        }

        // 서브 타이틀 페이드인 애니메이션
        float elapsed = clock.getElapsedTime().asSeconds();
        if (elapsed < fadeInTime) {
            int alpha = static_cast<int>((elapsed / fadeInTime) * 255); // 0 ~ 255로 변환
            subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, alpha));
        }
        else {
            subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, 255)); // 완전히 표시
        }

        // 화면 그리기
        ui.getWindow().clear();
        ui.getWindow().draw(leftBackground);
        ui.getWindow().draw(rightBackground);

        ui.getWindow().draw(mainTitle);  // 메인 타이틀 그리기
        gui.draw();                      // TGUI GUI 그리기
        ui.getWindow().display();
    }

    // "Map Selected" 화면 호출
    if (gameStarted) {
        mapSelected(); // 게임 시작 후 mapSelected 호출
    }
}
