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

void GameManager::run(const std::string& stageFile)
{
    loadUnitTypes("UnitTypes.csv");
    loadTowerData("TowerData.csv");
    loadWaves(stageFile);

    ui.setMap(map);
    ui.setPath(path);
    ui.setTowers(towers);
    ui.setUnitTypes(unitTypes);
    ui.judgeStage = this->judgeStage;
    currentTick = 0;

    for (const auto& wave : waves)
    {
       
        int waveID = wave.getWaveID();
        currentwaveType = wave.getIsDefence();
        ui.setIsDefence(currentwaveType);
        ui.clearButtons();
        ui.drawButtons();
        projectiles.clear();

        ui.unitCounts.clear();
  
        for (int unitID : wave.getUnits())
        {
            ui.unitCounts[unitID]++;
        }
        
       
        if (wave.getIsDefence())
        {
            if (isAttackSuccess == 1)
                gold += wave.getGold() * 2;
            else
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

            std::queue<int> unitQueue;

            // 웨이브에 포함된 유닛 생성
            for (int unitID : wave.getUnits())
            {
                unitQueue.push(unitID);
            }


            // **게임 루프 수정 부분 시작**
            sf::Clock clock;
            sf::Clock frameClock; // 프레임 시간 측정용 시계
            sf::Time lastLogicUpdateTime = sf::Time::Zero;
            sf::Time logicUpdateInterval = sf::milliseconds(500); // 논리 업데이트 간격 (500ms)
            bool waveOver = false;

            while (!waveOver && ui.getWindow().isOpen())
            {
                sf::Time deltaTime = frameClock.restart();

                // 이벤트 처리
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

                // 논리 업데이트 시간 체크
                lastLogicUpdateTime += deltaTime;
                if (lastLogicUpdateTime >= logicUpdateInterval)
                {
                    lastLogicUpdateTime -= logicUpdateInterval;
                    currentTick++;

                    // 논리 업데이트 수행
                    spawnUnits(activeUnits, unitQueue, currentTick);
                    attackUnits(activeUnits, currentTick, currentwaveType);
                    ui.updateattackGold(attackGold);
                    updateAndPrintMap(activeUnits);
                }

                // 유닛 위치 업데이트
                updateUnitPositions(deltaTime);

                // 유닛 도착 여부 확인 및 처리
                updateUnits(activeUnits);

                // 투사체 업데이트
                updateProjectiles(deltaTime);

                // 화면 그리기
                updateGameState(activeUnits);

                // 게임 종료 조건 체크
                if (playerLife <= 0)
                {
                    showGameOverPopup();
                }

                if (activeUnits.empty() && unitQueue.empty())
                {
                    std::cout << "웨이브 " << waveID << " 클리어!\n";
                    waveOver = true;
                }
            }
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
   

    showGameClearPopup();
}

void GameManager::spawnUnits(std::vector<Unit>& activeUnits, std::queue<int>& unitQueue, int currentTick)
{
    // **스폰 간격 변수**
    lastSpawnTick = 0;       // 마지막으로 유닛이 생성된 틱
    const int spawnInterval = 2;      // 유닛 생성 간격 (틱 단위)

    // **틱 간격에 따라 유닛 생성**
    if (currentTick - lastSpawnTick >= spawnInterval && !unitQueue.empty())
    {
        int unitId = unitQueue.front();
        unitQueue.pop();

        // 유닛 ID로 유닛 생성
        auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
            [unitId](const UnitType& ut) { return ut.getId() == unitId; });

        if (it != unitTypes.end())
        {
            const UnitType& unitType = *it;
            Unit unit(unitType.getId(), unitType.getUnitName(), unitType.getHp(),
                unitType.getTimePerMove(), unitType.getArmor(), unitType.getResist(),
                unitType.getKillReward(), path, unitType.getLifeDamage());
            activeUnits.push_back(unit);
            lastSpawnTick = currentTick; // 마지막 생성 틱 업데이트
        }
    }
}


void GameManager::updateUnits(std::vector<Unit>& activeUnits)
{
    for (auto it = activeUnits.begin(); it != activeUnits.end();)
    {
        if (it->hasArrived())
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
        int x = unit.getPosX();
        int y = unit.getPosY();
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
            << ", 위치: (" << unit.getPosX() << ", " << unit.getPosY() << ")"
            << ", 체력: " << unit.getHp() << "\n";
    }
    // 유닛 생산 대기열 출력 추가
    if (!unitProductionQueue.empty())
    {
        std::cout << "\n=== 유닛 생산 대기열 ===\n";
        std::queue<int> tempQueue = unitProductionQueue; // 대기열 복사본 생성
        std::string queueText = "";
        while (!tempQueue.empty())
        {
            int unitId = tempQueue.front();
            tempQueue.pop();

            // 유닛 ID로 유닛 이름 찾기
            auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                [unitId](const UnitType& ut) { return ut.getId() == unitId; });
            std::string name;
            if (it != unitTypes.end())
            {
                std::cout << "유닛: " << it->getUnitName() << "\n";
                std::string name=Unit::getUIName(it->getId());
                queueText += name ;
            }
            else
            {
                std::cout << "유닛 ID: " << unitId << "\n";
            }
        }
       
        ui.setInfoText({ "유닛을 침투시키세요","[유닛 대기열]\n",queueText});
        
    }
    else ui.setInfoText({ "유닛을 침투시키세요","대기중인 유닛이 없습니다!\n"," " });
}






void GameManager::updateGameState(std::vector<Unit>& activeUnits)
{
    // UI를 통해 게임 상태를 업데이트하고 화면을 그립니다.
    ui.update(activeUnits, placedTowers, playerLife, gold, selectedX, selectedY, projectiles, currentTick);
}

void GameManager::loadMap(const std::string& filename)
{
    map.clear();
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
                // 경로 타일 확인 조건 수정
                if (!visited[ny][nx] && (map[ny][nx] == "D" || (map[ny][nx].size() >= 1 && map[ny][nx][0] == 'P'))) {
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
            int isNodamage = std::stoi(tokens[9]); 
            std::string tool = tokens[10];        
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
    selectedOption = -1;

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
                {
                    selectedY--;
                    selectedOption = -1;
                    selectedTowerIndex = -1;
                }

                else if (event.key.code == sf::Keyboard::S && selectedY < mapWithUnits.size() - 1)
                {
                    selectedY++;
                    selectedOption = -1;
                    selectedTowerIndex = -1;
                }
                else if (event.key.code == sf::Keyboard::A && selectedX > 0)
                {
                    selectedX--;
                    selectedOption = -1;
                    selectedTowerIndex = -1;
                }
                else if (event.key.code == sf::Keyboard::D && selectedX < mapWithUnits[0].size() - 1)
                {
                    selectedX++;
                    selectedOption = -1;
                    selectedTowerIndex = -1;
                }

                else if (event.key.code == sf::Keyboard::Escape)
                {
                    selectedTowerIndex = -1; // 선택된 타워 초기화
                    std::cout << "타워 선택이 초기화되었습니다.\n";
                    ui.setInfoText({ "타워 선택이 초기화되었습니다."," "," " });
                    ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI 갱신
                }

                if (map[selectedY][selectedX] == "O")
                {
                    ui.setInfoText({ " ","빈 건설 부지입니다.", "1번 : 검사 타워 \n2번 : 궁수 타워 \n3번 : 마법사 타워" });
                    ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI 갱신
                }
                if ((map[selectedY][selectedX] != "O" && isTowerPlacementMode == true))
                {
                    ui.setInfoText({ " ","건설 가능 지점이 아닙니다.", "웨이브 시작 : F" });
                    ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI 갱신
                    selectedTowerIndex = -1;
                }

                else if (map[selectedY][selectedX] == "O")
                {
                    

                    // 1~9 키로 타워 선택
                     if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9 && isTowerPlacementMode == true)
                     {
                            int towerID = event.key.code - sf::Keyboard::Num0;
                            auto it = std::find_if(towers.begin(), towers.end(),
                                [towerID](const Tower& tower) { return tower.getId() == towerID; });

                            if (it != towers.end())
                            {
                                Tower& selectedTower = *it;
                                selectedTowerIndex = std::distance(towers.begin(), it);
                                std::cout << "타워 " << selectedTower.getTowerName() << " 이 선택되었습니다.\n";
                                ui.setInfoText({ selectedTower.getTool() + " 타워가 선택되었습니다!\n",
                                                 selectedTower.getTool2(),
                            "공격력: " + std::to_string(selectedTower.getDamage()) +
                            "\n공격 속도: " + std::to_string(selectedTower.getTimePerAttack()) +
                            "\n사거리: " + std::to_string(selectedTower.getAttackRange()) +
                            "\n공격 유형: " + selectedTower.attackType() + "\n\n[space] : 설치" });
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
                         selectedTowerIndex = -1;
                     }
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
                    ui.setInfoText({ towerIt->getTool() + " 타워", towerIt->uiOptionSelect(towers,0)," " });
                }
                else
                {
                    isTowerPlacementMode = true; // 타워가 없으면 설치 모드로 전환
                }



                if (towerIt != placedTowers.end())
                {
                    ui.setInfoText({ towerIt->getTool() + " 타워", towerIt->uiOptionSelect(towers,0)," " });
                   
                    selectedTowerIndex = -1;
                    // 업그레이드/판매 선택
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        if (towerIt->getTowerName() == "2" || towerIt->getTowerName() == "1")
                        {
                            selectedOption = 1; // 옵션 1 선택
                            selectedTowerIndex = -1;

                            std::cout << "옵션 1 선택됨.\n";
                            ui.setInfoText({ towerIt->getTool() + " 타워",towerIt->uiOptionSelect(towers,1)," " });
                        }
                        
                    }
                    else if (event.key.code == sf::Keyboard::Num2 && towerIt->getTowerName() == "2")
                    {
                        selectedOption = 2; // 옵션 2 선택
                        selectedTowerIndex = -1;

                        std::cout << "옵션 2 선택됨.\n";
                        ui.setInfoText({ towerIt->getTool() + " 타워",towerIt->uiOptionSelect(towers,2)," " });
                    }
                    else if (event.key.code == sf::Keyboard::Num3)
                    {
                        selectedOption = 3; // 옵션 3 선택
                        selectedTowerIndex = -1;

                        std::cout << "옵션 3 (판매) 선택됨.\n";
                        ui.setInfoText({ "타워를 판매하시겠습니까?",towerIt->uiOptionSelect(towers,3),"판매를 하려면 [space]를 누르세요" });
                    }
                    else if (event.key.code == sf::Keyboard::Space && selectedOption > 0) // Space로 실행
                    {

                        if (selectedOption == 1) // 업그레이드 (1번)
                        {
                            if (towerIt->upgrade(gold, map, towers, 1)) {
                                ui.setInfoText({ towerIt->getTool() + "로 업그레이드 되었습니다",
                                    towerIt->getTool2(),
                                    "공격력: " + std::to_string(towerIt->getDamage()) +
                                "\n공격 속도: " + std::to_string(towerIt->getTimePerAttack()) +
                                "\n사거리: " + std::to_string(towerIt->getAttackRange()) +
                                "\n공격 유형: " + towerIt->attackType() + "\n\n[space] : 옵션창" });
                            }
                            else  ui.setInfoText({ "업그레이드에 실패하였습니다","골드가 부족합니다.","타워를 판매하거나 [F]키를 통해\n수비웨이브를 시작하세요!" });

                        }
                        else if (selectedOption == 2) // 업그레이드 (2번)
                        {
                            if (towerIt->upgrade(gold, map, towers, 2)) {
                                ui.setInfoText({ towerIt->getTool() + "로 업그레이드 되었습니다",
                                   towerIt->getTool2(),
                                   "공격력: " + std::to_string(towerIt->getDamage()) +
                               "\n공격 속도: " + std::to_string(towerIt->getTimePerAttack()) +
                               "\n사거리: " + std::to_string(towerIt->getAttackRange()) +
                               "\n공격 유형: " + towerIt->attackType() + "\n\n[space] : 옵션창" });
                            }
                            else  ui.setInfoText({ "업그레이드에 실패하였습니다","골드가 부족합니다.","타워를 판매하거나 [F]키를 통해\n수비웨이브를 시작하세요!" });

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
                            ui.setInfoText({ "타워가 판매되었습니다.","반환된 골드: " + std::to_string(refundAmount)," " });
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
                selectedOption = -1;
                sf::Vector2i mousePosition = sf::Mouse::getPosition(ui.getWindow());
                float mouseX = static_cast<float>(mousePosition.x);
                float mouseY = static_cast<float>(mousePosition.y);

                float mapWidth = static_cast<float>(map[0].size());
                float mapHeight = static_cast<float>(map.size());

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
                    selectedX = clickedTileX;
                    selectedY = clickedTileY;
                    auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
                        [this](const PlacedTower& tower)
                        {
                            return tower.getX() == selectedX && tower.getY() == selectedY;
                        });

                    if (map[selectedY][selectedX] == "O")
                    {
                        ui.setInfoText({ " ","빈 건설 부지입니다.", "1번 : 검사 타워 \n2번 : 궁수 타워 \n3번 : 마법사 타워" });
                        ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI 갱신
                    }


                    if (towerIt != placedTowers.end())
                    {
                        isTowerPlacementMode = false; // 타워가 있으면 업그레이드/판매 모드로 전환
                        ui.setInfoText({ towerIt->getTool() + " 타워", towerIt->uiOptionSelect(towers,0)," " });

                    }
                    else
                    {

                        isTowerPlacementMode = true; // 타워가 없으면 설치 모드로 전환
                    }

                    if ((map[selectedY][selectedX] != "O" && isTowerPlacementMode == true))
                    {
                        ui.setInfoText({ " ","건설 가능 지점이 아닙니다.", "웨이브 시작 : F" });
                        ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI 갱신
                        selectedTowerIndex = -1;
                    }

                    // 타워 설치 시도
                    if (map[selectedY][selectedX] == "O" && selectedTowerIndex >= 0 && isTowerPlacementMode == true)
                    {
                        attemptPlaceTower();
                        selectedTowerIndex = -1;
                       
                    }
                   
                    
                }

                auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
                    [this](const PlacedTower& tower)
                    {
                        return tower.getX() == selectedX && tower.getY() == selectedY;
                    });
                if (towerIt != placedTowers.end())
                {
                    isTowerPlacementMode = false; // 타워가 있으면 업그레이드/판매 모드로 전환
                    ui.setInfoText({ towerIt->getTool() + " 타워", towerIt->uiOptionSelect(towers,0)," " });

                }


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

void GameManager::attackUnits(std::vector<Unit>& activeUnits, int currentTick, bool currentWaveType)
{
    // 모든 타워 초기화 (버프 해제)
    for (auto& tower : placedTowers)
    {
        tower.clearBuff();
    }

    // **버프 타워 처리**
    for (const auto& buffTower : placedTowers)
    {
        if (buffTower.getIsNoDamage() != 1) continue; // 버프 타워가 아니면 넘어감

        for (auto& targetTower : placedTowers)
        {
            if (&buffTower == &targetTower || targetTower.getIsNoDamage() == 1) continue; // 자기 자신 또는 다른 버프 타워 제외

            // 거리 계산
            int dx = buffTower.getX() - targetTower.getX();
            int dy = buffTower.getY() - targetTower.getY();
            int distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= buffTower.getAttackRange() * buffTower.getAttackRange())
            {
                // 타워에 버프 적용
                targetTower.applyBuff(buffTower.getDamage(), buffTower.getTimePerAttack());
            }
        }
    }

    // **공격 타워 처리**
    for (auto& tower : placedTowers)
    {
        if (tower.getIsNoDamage() == 1) continue; // 버프 타워는 공격하지 않음

        if (currentTick % tower.getTimePerAttack() != 0) continue; // 공격 틱이 아니면 넘어감

        

        int range = tower.getAttackRange();
        int damage = tower.getDamage();
        int targetAmount = tower.getTargetAmount();
        int targetsAttacked = 0;

        // 기본 공격 처리
        std::vector<Unit*> aoeTargets;

        for (auto it = activeUnits.begin(); it != activeUnits.end() && targetsAttacked < targetAmount;)
        {
            float unitX = it->getPosX();
            float unitY = it->getPosY();
            int towerX = tower.getX();
            int towerY = tower.getY();

            float distanceSquared = (towerX - unitX) * (towerX - unitX) + (towerY - unitY) * (towerY - unitY);

            if (distanceSquared <= range * range)
            {
                Unit* currentUnit = &(*it);

                // 투사체 생성
                createProjectile(tower, *it);

                // 기본 데미지 처리
                int newHp = calculateDamage(tower.getIsMagic(), damage, *it);
                it->reduceHp(newHp);

                bool unitErased = false;

                // 유닛 제거 처리
                if (it->getHp() <= 0)
                {
                    if (currentWaveType)
                    {
                        gold += it->getKillReward();
                    }
                    it = activeUnits.erase(it);
                    unitErased = true;
                }
                else
                {
                    ++it;
                }

                if (!unitErased)
                {
                    aoeTargets.push_back(currentUnit);
                }
                targetsAttacked++;
            }
            else
            {
                ++it;
            }
        }

        // 범위 공격 처리
        if (tower.getIsNoDamage() == 2)
        {
            for (Unit* target : aoeTargets)
            {
                for (auto aoeIt = activeUnits.begin(); aoeIt != activeUnits.end();)
                {
                    if (&(*aoeIt) == target)
                    {
                        ++aoeIt;
                        continue; // 기본 공격 대상은 제외
                    }

                    int aoeDistanceSquared = (target->getPosX() - aoeIt->getPosX()) * (target->getPosX() - aoeIt->getPosX()) +
                        (target->getPosY() - aoeIt->getPosY()) * (target->getPosY() - aoeIt->getPosY());

                    if (aoeDistanceSquared <= 1 * 1)
                    { // 범위 공격 거리 1칸
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
    float unitX = targetUnit.getPosX();
    float unitY = targetUnit.getPosY();

    // 화면 좌표로 변환 (이소메트릭 변환 적용)
    float startX = (towerX - towerY) * (ui.tileWidth / 2.0f) + ui.getWindow().getSize().x / 2.0f;
    float startY = (towerX + towerY) * (ui.tileHeight / 2.0f);

    float targetX = (unitX - unitY) * (ui.tileWidth / 2.0f) + ui.getWindow().getSize().x / 2.0f;
    float targetY = (unitX + unitY) * (ui.tileHeight / 2.0f);

    startX += projectileOffsetX;
    targetX += projectileOffsetX;
    startY += projectileOffsetY;
    targetY += projectileOffsetY;

    int towerID = tower.getId();
    if (towerID == 1 || towerID == 100 || towerID == 101 || towerID == 102)
    {
        float speed = 400.0f; 
        Projectile projectile(startX, startY, targetX, targetY, speed, ui.getProjectileTexture1());
        projectiles.push_back(projectile);
    }

    else if (towerID == 2 || towerID == 200 || towerID == 201 || towerID == 202)
    {
        float speed = 800.0f;
        Projectile projectile(startX, startY, targetX, targetY, speed, ui.getProjectileTexture2());
        projectiles.push_back(projectile);
    }

    else if (towerID == 3 || towerID == 300 || towerID == 301 || towerID == 302)
    {
        float speed = 600.0f;
        Projectile projectile(startX, startY, targetX, targetY, speed, ui.getProjectileTexture3());
        projectiles.push_back(projectile);
    }
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

void GameManager::updateUnitPositions(sf::Time deltaTime)
{
    for (auto& unit : activeUnits)
    {
        unit.updatePosition(deltaTime.asSeconds());
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
    bool waveOver = false;
    int previousPlayerLife = playerLife;

    sf::Clock clock;
    sf::Clock frameClock; // 프레임 시간 측정용 시계
    sf::Time lastLogicUpdateTime = sf::Time::Zero;
    sf::Time logicUpdateInterval = sf::milliseconds(500); // 논리 업데이트 간격 (500ms)

    activeUnits.clear(); // 이전 유닛 목록 초기화
    while (!unitProductionQueue.empty()) { unitProductionQueue.pop(); } // 유닛 생산 대기열 초기화

    while (!waveOver && ui.getWindow().isOpen())
    {
        sf::Time deltaTime = frameClock.restart();

        // 키 값 입력 및 공격 웨이브 종료 요청 확인
        bool endWaveRequested = handleAttackInput();

        // **논리 업데이트 시간 체크**
        lastLogicUpdateTime += deltaTime;
        if (lastLogicUpdateTime >= logicUpdateInterval)
        {
            lastLogicUpdateTime -= logicUpdateInterval;
            currentTick++;

            // 유닛 스폰 및 이동
            updateAttackUnits(activeUnits, currentTick);
            // 공격 처리
            attackUnits(activeUnits, currentTick, currentwaveType);

            // 대기열 정보 업데이트
            std::queue<int> tempQueue = unitProductionQueue; // 대기열 복사
            std::vector<std::string> queueInfo;
            while (!tempQueue.empty())
            {
                int unitId = tempQueue.front();
                tempQueue.pop();

                auto it = std::find_if(unitTypes.begin(), unitTypes.end(),
                    [unitId](const UnitType& ut) { return ut.getId() == unitId; });

                if (it != unitTypes.end())
                {
                    queueInfo.push_back(it->getUnitName());
                }
                else
                {
                    queueInfo.push_back("Unknown Unit");
                }
            }
            updateAndPrintMap(activeUnits);
            // 대기열 정보를 UI에 표시
          
            
        }
        // 공격 웨이브 종료 조건 확인
        if (endWaveRequested)
        {
            if (activeUnits.empty() && unitProductionQueue.empty())
            {
                waveOver = true;
                std::cout << "공격 웨이브가 종료되었습니다.\n";
            }
            else
            {
                std::cout << "필드 또는 대기열에 유닛이 남아 있어 웨이브를 종료할 수 없습니다.\n";
            }
        }
        else
        {
            waveOver = isAttackWaveOver(activeUnits);
        }


        // 유닛 위치 업데이트 (매 프레임마다 호출)
        updateUnitPositions(deltaTime);

        // 유닛 도착 여부 확인 및 처리
        updateUnits(activeUnits);

        // 투사체 업데이트
        updateProjectiles(deltaTime);

        // UI 업데이트
        updateGameState(activeUnits);

        // 게임 종료 조건 체크
        if (playerLife <= 0)
        {
            showGameOverPopup();
           
        }
    }

    while (!unitProductionQueue.empty()) { unitProductionQueue.pop(); } // 유닛 생산 대기열 초기화

    std::cout << "공격 웨이브 종료!\n";
    if (previousPlayerLife > playerLife)
    {
        isAttackSuccess = 1;
        std::cout << "공격 성공!\n";
    }
    else
        isAttackSuccess = 0;
}


bool GameManager::handleAttackInput()
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
            // 'q' 키 입력 시 처리
            if (event.key.code == sf::Keyboard::Q)
            {
                return true; // 공격 웨이브 종료 요청, 해당 지점 참고해서 종료 버튼 추가하면 좋을듯?
            }

            int unitId = 0;
            switch (event.key.code)
            {
                case sf::Keyboard::Num1:
                    unitId = 1;
                    break;
                case sf::Keyboard::Num2:
                    unitId = 2;
                    break;
                case sf::Keyboard::Num3:
                    unitId = 3;
                    break;
                case sf::Keyboard::Num4:
                    unitId = 4;
                    break;
                case sf::Keyboard::Num5:
                    unitId = 5;
                    break;
                case sf::Keyboard::Num6:
                    unitId = 6;
                    break;
            }

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
            ui.updateattackGold(attackGold);
        }
    }
    return false; // 공격 웨이브 종료 요청 없음
}

void GameManager::updateAttackUnits(std::vector<Unit>& activeUnits, int currentTick)
{
    // 유닛 스폰
    spawnUnits(activeUnits, unitProductionQueue, currentTick);

    // 유닛 업데이트
    for (auto it = activeUnits.begin(); it != activeUnits.end();)
    {
        bool arrived = it->update();
        if (arrived)
        {
            std::cout << it->getName() << " 유닛이 목적지에 도달했습니다!\n";

            // 플레이어 라이프 감소
            playerLife -= it->getLifeDamage();

            // 라이프가 0 이하이면 게임 종료
            if (playerLife <= 0)
            {
                showGameOverPopup();
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
                
            }
            else
            {
                std::cout << "타워를 설치할 골드가 부족합니다.\n";
                ui.setInfoText({"타워를 설치할 골드가 부족합니다.\n", " ", "타워를 판매하거나 [F]키를 통해\n수비웨이브를 시작하세요!"});
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
    selectedX = 0;
    selectedY = 0;
    selectedTowerIndex = -1;
    selectedOption = -1;
    isTowerPlacementMode = true;
    int image = 0;

    // 게임 데이터 초기화
    map.clear();
    waves.clear();
    activeUnits.clear();
    placedTowers.clear();
    projectiles.clear();
    unitProductionQueue = std::queue<int>();

    gold = 0; // 초기 골드 값 설정
    playerLife = 10; // 초기 라이프 설정
    attackGold = 0;

    // TGUI GUI 생성
    tgui::Gui gui(ui.getWindow());

    // 배경색을 흰색으로 설정
    sf::RectangleShape background(sf::Vector2f(ui.getWindow().getSize().x, ui.getWindow().getSize().y));
    background.setFillColor(sf::Color(75, 0, 130));

    // 텍스트 생성 및 설정
    auto titleLabel = tgui::Label::create("Select   A   Track");
    titleLabel->setPosition("50%", "7%");
    titleLabel->setOrigin(0.5f, 0.5f);
    titleLabel->setTextSize(140);
    titleLabel->getRenderer()->setTextColor(sf::Color::White);
    tgui::Font font("resources/fonts/Bangers.ttf");
    titleLabel->getRenderer()->setFont(font);
    gui.add(titleLabel);

    // 맵 선택 버튼 생성 및 크기 설정
    auto button1 = tgui::Button::create();
    button1->setPosition("5%", "17%");
    button1->setSize("28%", "70%");
    button1->getRenderer()->setTexture("resources/images/stageImgs/spring.png"); // 버튼 이미지
    button1->getRenderer()->setBorders(7);
    button1->getRenderer()->setOpacity(0.5f); // 기본 투명도 설정
    button1->onMouseEnter([button1]() { button1->getRenderer()->setOpacity(1.0f); }); // 마우스 오버 시 투명도 제거
    button1->onMouseLeave([button1]() { button1->getRenderer()->setOpacity(0.5f); }); // 마우스가 떠나면 다시 반투명
    gui.add(button1);

    auto button2 = tgui::Button::create();
    button2->setPosition("36.5%", "17%");
    button2->setSize("28%", "70%");
    button2->getRenderer()->setTexture("resources/images/stageImgs/dessert.png");
    button2->getRenderer()->setBorders(7);
    button2->getRenderer()->setOpacity(0.5f);
    button2->onMouseEnter([button2]() { button2->getRenderer()->setOpacity(1.0f); });
    button2->onMouseLeave([button2]() { button2->getRenderer()->setOpacity(0.5f); });
    gui.add(button2);

    auto button3 = tgui::Button::create();
    button3->setPosition("68%", "17%");
    button3->setSize("28%", "70%");
    button3->getRenderer()->setTexture("resources/images/stageImgs/winter.png");
    button3->getRenderer()->setBorders(7);
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
    label1->getRenderer()->setTextColor(sf::Color::White);
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
    label2->getRenderer()->setTextColor(sf::Color::White);
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
    label3->getRenderer()->setTextColor(sf::Color::White);
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
    std::vector<std::wstring> selectMapText;
    // 버튼 클릭 이벤트 설정
    std::string stageFile;



    button1->onClick([&]()
        {

            selectedMap = "Map1.csv";
            stageFile = "Stage1.csv";
            mapChosen = true;
            selectMapText = Map1Text;
            image = 1;
            judgeStage = 1;
           
        });

    button2->onClick([&]()
        {

            selectedMap = "Map2.csv";
            stageFile = "Stage2.csv";
            mapChosen = true;
            selectMapText = Map2Text;
            image = 2;
            judgeStage = 2;
        });

    button3->onClick([&]()
        {
            selectedMap = "Map3.csv";
            stageFile = "Stage3.csv";
            mapChosen = true;
            selectMapText = Map3Text;
            image = 3;
            judgeStage = 3;
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
        ui.gui.removeAllWidgets(); // 이전 GUI 요소 제거
        ui.getWindow().clear();    // 화면 초기화
        ui.getWindow().display();  // 초기화 화면 강제 갱신
        showLoadingScreen("MAP LOADING...");

        if (image == 1)
        {
            ui.initializeBackground("resources/images/maps/springmap.png");
        }
        else if (image == 2)
        {
            ui.initializeBackground("resources/images/maps/dessertmap.png");
        }
        else if (image == 3)
        {
            ui.initializeBackground("resources/images/maps/wintermap.png");
        }

        std::cout << selectedMap << " 맵이 선택되었습니다.\n";
        gamePrologue(selectMapText);
        loadMap(selectedMap);
        parsePath();
        run(stageFile);  // 선택된 맵으로 게임 실행
    }
}

void GameManager::showGameOverPopup()
{
    // 창 크기 계산
    sf::Vector2u windowSize = ui.getWindow().getSize();
    float boxWidth = windowSize.x * 2 / 3.0f;
    float boxHeight = windowSize.y * 2 / 3.0f;

    // 패널 생성
    auto panel = tgui::Panel::create({ boxWidth, boxHeight });
    panel->setPosition((windowSize.x - boxWidth) / 2.0f, (windowSize.y - boxHeight) / 2.0f);
    panel->getRenderer()->setBackgroundColor({ 139, 0, 0, 200 }); // 빨간색 반투명 배경
    panel->getRenderer()->setBorders({ 10 });                    // 테두리 추가
    panel->getRenderer()->setBorderColor({ 0, 0, 0 });     // 테두리 블랙
    panel->getRenderer()->setRoundedBorderRadius(20);            // 모서리를 둥글게

 
    // **Game Over 라벨 추가**
    auto gameOverLabel = tgui::Label::create("Game Over");
    gameOverLabel->setTextSize(250); // 텍스트 크기
    gameOverLabel->getRenderer()->setTextColor({ 0, 0, 0 }); // 블랙 텍스트
    gameOverLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    gameOverLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    gameOverLabel->setPosition("10%", "15%"); // 패널의 중앙 상단에 배치
    
    tgui::Font font("resources/fonts/Bangers.ttf");
    gameOverLabel->getRenderer()->setFont(font);
    panel->add(gameOverLabel);

    // 종료 버튼 추가
    auto closeButton = tgui::Button::create();
    closeButton->setSize("15%", "22%"); // 버튼 크기
    closeButton->setPosition("23%", "70%"); // 패널의 왼쪽 하단에 배치
    closeButton->getRenderer()->setBorders(0); // 버튼 테두리 제거
    closeButton->getRenderer()->setTexture("resources/images/icons/cancel.png"); // 종료 버튼 이미지

    closeButton->onMouseEnter([closeButton]() {
        closeButton->getRenderer()->setOpacity(0.5f); // 마우스 오버 효과
        });
    closeButton->onMouseLeave([closeButton]() {
        closeButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때
        });

    closeButton->onClick([this, panel]() {
        ui.getWindow().close();
        });

    panel->add(closeButton);

    // 맵 선택 버튼 추가
    auto mapSelectButton = tgui::Button::create();
    mapSelectButton->setSize("15%", "22%"); // 버튼 크기
    mapSelectButton->setPosition("62%", "70%"); // 패널의 오른쪽 하단에 배치
    mapSelectButton->getRenderer()->setBorders(0); // 버튼 테두리 제거
    mapSelectButton->getRenderer()->setTexture("resources/images/icons/back.png"); // 맵 선택 버튼 이미지

    mapSelectButton->onMouseEnter([mapSelectButton]() {
        mapSelectButton->getRenderer()->setOpacity(0.5f); // 마우스 오버 효과
        });
    mapSelectButton->onMouseLeave([mapSelectButton]() {
        mapSelectButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때
        });

    mapSelectButton->onClick([this, panel]() {
        mapSelected();
        });

    panel->add(mapSelectButton);

    // GUI에 패널 추가
    ui.gui.add(panel, "gameOverPanel");

    // **UI 이벤트 루프**
    while (ui.getWindow().isOpen()) {
        sf::Event event;
        while (ui.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                ui.getWindow().close();
                return;
            }

            ui.gui.handleEvent(event); // TGUI 이벤트 처리
        }

     
        ui.gui.draw(); // TGUI 다시 그리기
        ui.getWindow().display();
    }
}


void GameManager::showGameClearPopup()
{
    // 창 크기 계산
    sf::Vector2u windowSize = ui.getWindow().getSize();
    float boxWidth = windowSize.x * 2 / 3.0f;
    float boxHeight = windowSize.y * 2 / 3.0f;

    // 패널 생성
    auto panel = tgui::Panel::create({ boxWidth, boxHeight });
    panel->setPosition((windowSize.x - boxWidth) / 2.0f, (windowSize.y - boxHeight) / 2.0f);
    panel->getRenderer()->setBackgroundColor({ 135, 206, 235, 200 }); // 파란색 반투명 배경
    panel->getRenderer()->setBorders({ 10 });                    // 테두리 추가
    panel->getRenderer()->setBorderColor({ 255,255,153 });     // 테두리 화이트
    panel->getRenderer()->setRoundedBorderRadius(20);            // 모서리를 둥글게


    // **Game Over 라벨 추가**
    auto gameOverLabel = tgui::Label::create("Game Clear");
    gameOverLabel->setTextSize(250); // 텍스트 크기
    gameOverLabel->getRenderer()->setTextColor({ 255,255,153}); // 노란 텍스트
    gameOverLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    gameOverLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    gameOverLabel->setPosition("7%", "15%"); // 패널의 중앙 상단에 배치
    
    tgui::Font font("resources/fonts/Bangers.ttf");
    gameOverLabel->getRenderer()->setFont(font);
    panel->add(gameOverLabel);

    // 종료 버튼 추가
    auto closeButton = tgui::Button::create();
    closeButton->setSize("15%", "22%"); // 버튼 크기
    closeButton->setPosition("23%", "70%"); // 패널의 왼쪽 하단에 배치
    closeButton->getRenderer()->setBorders(0); // 버튼 테두리 제거
    closeButton->getRenderer()->setTexture("resources/images/icons/cancel.png"); // 종료 버튼 이미지

    closeButton->onMouseEnter([closeButton]() {
        closeButton->getRenderer()->setOpacity(0.5f); // 마우스 오버 효과
        });
    closeButton->onMouseLeave([closeButton]() {
        closeButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때
        });

    closeButton->onClick([this, panel]() {
        ui.getWindow().close();
        });

    panel->add(closeButton);

    // 맵 선택 버튼 추가
    auto mapSelectButton = tgui::Button::create();
    mapSelectButton->setSize("15%", "22%"); // 버튼 크기
    mapSelectButton->setPosition("62%", "70%"); // 패널의 오른쪽 하단에 배치
    mapSelectButton->getRenderer()->setBorders(0); // 버튼 테두리 제거
    mapSelectButton->getRenderer()->setTexture("resources/images/icons/back.png"); // 맵 선택 버튼 이미지

    mapSelectButton->onMouseEnter([mapSelectButton]() {
        mapSelectButton->getRenderer()->setOpacity(0.5f); // 마우스 오버 효과
        });
    mapSelectButton->onMouseLeave([mapSelectButton]() {
        mapSelectButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때
        });

    mapSelectButton->onClick([this, panel]() {
        mapSelected();
        });

    panel->add(mapSelectButton);

    // GUI에 패널 추가
    ui.gui.add(panel, "gameClearPanel");

    // **UI 이벤트 루프**
    while (ui.getWindow().isOpen()) {
        sf::Event event;
        while (ui.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                ui.getWindow().close();
                return;
            }

            ui.gui.handleEvent(event); // TGUI 이벤트 처리
        }


        ui.gui.draw(); // TGUI 다시 그리기
        ui.getWindow().display();
    }
}



void GameManager::gameStart() {
    // TGUI GUI 생성
    tgui::Gui gui(ui.getWindow());

    // 첫 번째 반은 다크 블루
    sf::RectangleShape leftBackground(sf::Vector2f(ui.getWindow().getSize().x / 2, ui.getWindow().getSize().y));
    leftBackground.setFillColor(sf::Color(0, 0, 80)); // 다크 블루

    // 두 번째 반은 레드
    sf::RectangleShape rightBackground(sf::Vector2f(ui.getWindow().getSize().x / 2, ui.getWindow().getSize().y));
    rightBackground.setFillColor(sf::Color(139, 0, 0));
    rightBackground.setPosition(ui.getWindow().getSize().x / 2, 0);

    // 폰트 로드
    sf::Font bangerFont;
    if (!bangerFont.loadFromFile("resources/fonts/Bangers.ttf")) {
        std::cerr << "Failed to load Bangers.ttf" << std::endl;
        return;
    }

    // 메인 타이틀 설정
    sf::Text mainTitle;
    mainTitle.setFont(bangerFont);
    mainTitle.setString("Spy of the Battlefield");
    mainTitle.setCharacterSize(140); // 크기 조정
    mainTitle.setFillColor(sf::Color::White);
    mainTitle.setOrigin(mainTitle.getLocalBounds().width / 2.f, mainTitle.getLocalBounds().height / 2.f);
    mainTitle.setPosition(ui.getWindow().getSize().x / 2.f, ui.getWindow().getSize().y * 0.2f); // 위치 설정

    // "Spy of the Battlefield" 텍스트 추가 (서브 타이틀)
    auto subTitle = tgui::Label::create(L"전 장 의   스 파 이");
    subTitle->setPosition("50%", "35%");          // 메인 타이틀 아래에 배치
    subTitle->setOrigin(0.5f, 0.5f);              // 중앙 정렬
    subTitle->setTextSize(70);                    // 텍스트 크기
    subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, 0)); // 투명도 0으로 시작

    // Bangers 폰트 적용
    tgui::Font bmFont("resources/fonts/BMDOHYEON_ttf.ttf");    // bm폰트 경로
    subTitle->getRenderer()->setFont(bmFont);           // 폰트 설정
    gui.add(subTitle);

    // "Start Game" 버튼 추가
    auto startButton = tgui::Button::create();
    startButton->setPosition("43.5%", "65%");
    startButton->setSize("13%", "21%");
    startButton->getRenderer()->setBorders(0);

    // 버튼 기본 아이콘 설정
    startButton->getRenderer()->setTexture("resources/images/icons/start.png"); // 기본 아이콘 경로

    // 마우스 오버 효과
    startButton->onMouseEnter([&leftBackground, &rightBackground, startButton]() {
        // 배경색 좌우 반전: 왼쪽 빨간색, 오른쪽 파란색
        leftBackground.setFillColor(sf::Color(139,0,0));
        rightBackground.setFillColor(sf::Color(0, 0, 80)); // 다크 블루

        // 버튼 아이콘을 마우스 오버 상태로 변경
        startButton->getRenderer()->setTexture("resources/images/icons/start2.png"); // 오버 상태 아이콘 경로
        });

    startButton->onMouseLeave([&leftBackground, &rightBackground, startButton]() {
        // 배경색 복구: 왼쪽 파란색, 오른쪽 빨간색
        leftBackground.setFillColor(sf::Color(0, 0, 80)); // 다크 블루
        rightBackground.setFillColor(sf::Color(139, 0, 0));

        // 버튼 아이콘을 기본 상태로 복구
        startButton->getRenderer()->setTexture("resources/images/icons/start.png"); // 기본 아이콘 경로
        });

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

        gamePrologue(prologueText);
        mapSelected(); // 게임 시작 후 mapSelected 호출
    }
}

void GameManager::gamePrologue(const std::vector<std::wstring>& prologueLines) {
    // 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    sf::Text prologueText;
    prologueText.setFont(font);
    prologueText.setCharacterSize(30); // 글씨 크기 조정
    prologueText.setFillColor(sf::Color::White);
    prologueText.setStyle(sf::Text::Bold);

    sf::Vector2u windowSize = ui.getWindow().getSize();
    prologueText.setPosition(150, 150); // 텍스트 시작 위치

    // 다음 버튼 생성
    auto nextButton = tgui::Button::create();
    nextButton->setSize(150, 160); // 버튼 크기 키움
    nextButton->setPosition(windowSize.x - 200, windowSize.y - 200); // 화면 오른쪽 하단
    nextButton->getRenderer()->setBackgroundColor(sf::Color::Transparent);
    nextButton->getRenderer()->setTexture("resources/images/icons/next.png"); // 버튼 이미지
    nextButton->getRenderer()->setBorders(0); // 테두리 제거

    // 마우스 오버 효과
    nextButton->onMouseEnter([nextButton]() {
        nextButton->getRenderer()->setOpacity(0.7f); // 불투명도 낮춤
        });
    nextButton->onMouseLeave([nextButton]() {
        nextButton->getRenderer()->setOpacity(1.0f); // 원래 상태로 복구
        });

    // GUI에 버튼 추가
    tgui::Gui gui(ui.getWindow());
    gui.add(nextButton);

    // 다음 버튼 클릭 여부
    bool nextClicked = false;
    nextButton->onClick([&nextClicked]() {
        nextClicked = true;
        });

    // 프롤로그 애니메이션 변수
    sf::Clock clock;
    float lineDelay = 0.3f; // 각 줄 표시 간격 (초)
    size_t currentLineIndex = 0; // 현재 표시 중인 줄의 인덱스

    // 프롤로그 화면 루프
    while (ui.getWindow().isOpen() && !nextClicked) {
        sf::Event event;
        while (ui.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                ui.getWindow().close();
                return;
            }
            gui.handleEvent(event);
        }

        // 애니메이션: 일정 시간 간격으로 줄 추가
        if (currentLineIndex < prologueLines.size() &&
            clock.getElapsedTime().asSeconds() > currentLineIndex * lineDelay) {
            std::wstring displayedText;
            for (size_t i = 0; i <= currentLineIndex; ++i) {
                displayedText += prologueLines[i] + L"\n";
            }
            prologueText.setString(displayedText);
            currentLineIndex++;
        }


        ui.getWindow().clear(sf::Color::Black); // 배경 검은색
        ui.getWindow().draw(prologueText);      // 프롤로그 텍스트
        gui.draw();                             // 버튼
        ui.getWindow().display();
    }
}

void GameManager::showLoadingScreen(const std::string& message) {
    sf::RenderWindow& window = ui.getWindow();

    // 배경 설정
    window.clear(sf::Color::Black);

    // 텍스트 설정
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        std::cerr << "폰트를 로드할 수 없습니다.\n";
        return;
    }

    sf::Text loadingText;
    loadingText.setFont(font);
    loadingText.setString(message);
    loadingText.setCharacterSize(50);
    loadingText.setFillColor(sf::Color::White);

    // 텍스트 중앙 정렬
    sf::FloatRect textBounds = loadingText.getLocalBounds();
    loadingText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    loadingText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    // 텍스트 화면에 표시
    window.draw(loadingText);
    window.display(); // 화면 강제 업데이트
}

