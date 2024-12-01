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
            ui.setInfoText({ "Ÿ���� ��ġ�ϼ���!!", "�� �Ǽ� �����Դϴ�..", "1�� : �˻� Ÿ�� \n2�� : �ü� Ÿ�� \n3�� : ������ Ÿ��" });
            startPreparationPhase(); // ���� ���̺� �غ�
            ui.setInfoText({ "������ �������� �ֽ��ϴ�!!", "�� ������ ���� ���ϸ� ���Դϴ�!", "..." });
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
                        unitType.getKillReward(), path, unitType.getLifeDamage());
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
                    spawnUnits(activeUnits, unitQueue,currentTick);

                    // ���� �̵�
                    updateUnits(activeUnits);

                    // ���� ó��
                    attackUnits(activeUnits, currentTick, currentwaveType);

                    // �ܼ� ���
                    updateAndPrintMap(activeUnits);

                    // ���� ���� ���� üũ
                    if (playerLife <= 0)
                    {
                        // ���� �˾� ȣ��
                        showGameOverPopup();
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
            ui.setInfoText({ "ħ���� �غ��ϼ���!!", "������� ������ �����ϴ�!", "..." });

            // ���� ���̺� ó��
            attackGold = wave.getGold();
            ui.updateattackGold(attackGold); //ui�� ������ȭ ǥ���ϱ� ����
            startAttackWave(wave, currentTick);
        }
    }


    mapSelected(); //�ʼ������� ���ƿ�
}

void GameManager::spawnUnits(std::vector<Unit>& activeUnits, std::queue<Unit>& unitQueue,int currentTick)
{
    // **���� ���� ����**
    static int lastSpawnTick = 0;       // ���������� ������ ������ ƽ
    const int spawnInterval = 2;      // ���� ���� ���� (ƽ ����)

    // **ƽ ���ݿ� ���� ���� ����**
    if (currentTick - lastSpawnTick >= spawnInterval && !unitQueue.empty())
    {
        Unit unit = unitQueue.front();
        unitQueue.pop();

        // ������ ���� ��ġ�� �߰�
        activeUnits.push_back(unit);
        lastSpawnTick = currentTick;   // ������ ���� ƽ ������Ʈ
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
    ui.update(activeUnits, placedTowers, playerLife, gold, selectedX,selectedY ,projectiles);
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
            int nextTowerID2 = std::stoi(tokens[3]);
            int attackRange = std::stoi(tokens[4]);
            int damage = std::stoi(tokens[5]);
            bool isMagic = std::stoi(tokens[6]) == 1;
            int timePerAttack = std::stoi(tokens[7]);
            int targetAmount = std::stoi(tokens[8]);
            int isNodamage = std::stoi(tokens[9]); // �߰��� isNodamage ó��
            std::string tool = tokens[10];        // �߰��� tool ó��
            std::string tool2 = tokens[11];

            Tower tower(id, towerName, nextTowerID, nextTowerID2, attackRange, damage,
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
    isTowerPlacementMode = true;  // Ÿ�� ��ġ ��� ���� ����
    bool isTowerSelected = false;
    selectedOption = -1;

    selectedX = mapWithUnits[0].size() / 2;
    selectedY = mapWithUnits.size() / 2;
    selectedTowerIndex = -1;  // �ʱⰪ -1�� ���� (���õ��� ����)

    while (isPreparation)
    {
        ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY);


        sf::Event event;
        while (ui.getWindow().pollEvent(event))
        {
            
            // **TGUI �̺�Ʈ ó�� �߰�**
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

                // WASD�� Ÿ�� �̵�
                if (event.key.code == sf::Keyboard::W && selectedY > 0)
                {
                    selectedY--;
                    selectedOption = -1;
                }
                
                else if (event.key.code == sf::Keyboard::S && selectedY < mapWithUnits.size() - 1)
                {
                    selectedY++;
                    selectedOption = -1;
                }
                else if (event.key.code == sf::Keyboard::A && selectedX > 0)
                {
                    selectedX--;
                    selectedOption = -1;
                }
                else if (event.key.code == sf::Keyboard::D && selectedX < mapWithUnits[0].size() - 1)
                {
                    selectedX++;
                    selectedOption = -1;
                }

                else if (event.key.code == sf::Keyboard::Escape)
                {
                    selectedTowerIndex = -1; // ���õ� Ÿ�� �ʱ�ȭ
                    std::cout << "Ÿ�� ������ �ʱ�ȭ�Ǿ����ϴ�.\n";

                    ui.update({}, placedTowers, playerLife, gold, selectedX, selectedY); // UI ����
                }

                // 1~9 Ű�� Ÿ�� ����
                else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9 && isTowerPlacementMode == true)
                {
                    int towerID = event.key.code - sf::Keyboard::Num0;
                    auto it = std::find_if(towers.begin(), towers.end(),
                        [towerID](const Tower& tower) { return tower.getId() == towerID; });

                    if (it != towers.end())
                    {
                        Tower& selectedTower = *it;
                        selectedTowerIndex = std::distance(towers.begin(), it);
                        std::cout << "Ÿ�� " << selectedTower.getTowerName() << " �� ���õǾ����ϴ�.\n";
                        ui.setInfoText({ selectedTower.getTool() + " Ÿ���� ���õǾ����ϴ�"," "," "});
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

                // Space Ű�� Ÿ�� ��ġ
                else if (event.key.code == sf::Keyboard::Space && selectedTowerIndex >= 0 && isTowerPlacementMode == true)
                {
                    attemptPlaceTower();
                    selectedTowerIndex = -1;
                }

                

                // ���� ���õ� Ÿ�Ͽ� �ִ� Ÿ���� ��ȣ�ۿ�
                auto towerIt = std::find_if(placedTowers.begin(), placedTowers.end(),
                    [this](const PlacedTower& tower)
                    {
                        return tower.getX() == selectedX && tower.getY() == selectedY;
                    });
               

                // Ÿ���� �ִ��� Ȯ���ϰ� ��� ����
                if (towerIt != placedTowers.end())
                {
                    isTowerPlacementMode = false; // Ÿ���� ������ ���׷��̵�/�Ǹ� ���� ��ȯ
                }
                else
                {
                    isTowerPlacementMode = true; // Ÿ���� ������ ��ġ ���� ��ȯ
                }

                

                if (towerIt != placedTowers.end())
                {
                    
                    // ���׷��̵�/�Ǹ� ����
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        selectedOption = 1; // �ɼ� 1 ����
                        selectedTowerIndex = -1;

                        std::cout << "�ɼ� 1 ���õ�.\n";
                    }
                    else if (event.key.code == sf::Keyboard::Num2 && towerIt->getTowerName() == "2")
                    {
                        selectedOption = 2; // �ɼ� 2 ����
                        selectedTowerIndex = -1;

                        std::cout << "�ɼ� 2 ���õ�.\n";
                    }
                    else if (event.key.code == sf::Keyboard::Num3)
                    {
                        selectedOption = 3; // �ɼ� 3 ����
                        selectedTowerIndex = -1;

                        std::cout << "�ɼ� 3 (�Ǹ�) ���õ�.\n";
                    }
                    else if (event.key.code == sf::Keyboard::Space && selectedOption > 0) // Space�� ����
                    {
                      
                        if (selectedOption == 1) // ���׷��̵� (1��)
                        {
                            towerIt->upgrade(gold, map, towers, 1);
                            
                        }
                        else if (selectedOption == 2) // ���׷��̵� (2��)
                        {
                            towerIt->upgrade(gold, map, towers, 2);
                            
                        }
                        else if (selectedOption == 3) // Ÿ�� �Ǹ� (3��)
                        {
                            int refundAmount = 30;
                            if (towerIt->getTowerName() == "1")                //�⺻Ÿ�� �Ǹ� ���
                            {
                                refundAmount = 30;
                            }
                            else if (towerIt->getTowerName() == "2")           //�߰��ܰ�Ÿ�� �Ǹ� ���
                            {
                                refundAmount = 40;
                            }
                            else if (towerIt->getTowerName() == "3")           //�����ܰ�Ÿ�� �Ǹ� ���
                            {
                                refundAmount = 50;
                            }
                            gold += refundAmount;
                            std::cout << "Ÿ���� �ǸŵǾ����ϴ�. ��ȯ�� ���: " << refundAmount << "\n";

                            map[towerIt->getY()][towerIt->getX()] = "O";
                            placedTowers.erase(towerIt);
                        }

                        selectedOption = -1; // ���� �ʱ�ȭ
                    }
                    else 
                    {
                        selectedOption = -1; // ���� ���
                    }
                    
                }

            }

            // ���콺 Ŭ������ Ÿ�� ����
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
                    if (towerIt != placedTowers.end())
                    {
                        isTowerPlacementMode = false; // Ÿ���� ������ ���׷��̵�/�Ǹ� ���� ��ȯ
                        
                    }
                    else
                    {
                       
                        isTowerPlacementMode = true; // Ÿ���� ������ ��ġ ���� ��ȯ
                    }

                    // Ÿ�� ��ġ �õ�
                    if (map[selectedY][selectedX] == "O" && selectedTowerIndex >= 0 && isTowerPlacementMode == true)
                    {
                        attemptPlaceTower();
                        selectedTowerIndex = -1;
                    }
                    
                }
            

            }
        }
    }
}

void GameManager::attackUnits(std::vector<Unit>& activeUnits, int currentTick, bool currentWaveType)
{
    // ��� Ÿ�� ó��
    for (auto& tower : placedTowers) 
    {
        if (tower.getIsNoDamage() == 1) continue; // ���� Ÿ���� �������� ����

        if (currentTick % tower.getTimePerAttack() != 0) continue; // ���� ƽ�� �ƴϸ� �Ѿ

        int range = tower.getAttackRange();
        int damage = tower.getDamage();
        int targetAmount = tower.getTargetAmount();
        int targetsAttacked = 0;

        // **�⺻ ���� ó��**
        std::vector<Unit*> aoeTargets; // ���� ���� ��� ����

        for (auto it = activeUnits.begin(); it != activeUnits.end() && targetsAttacked < targetAmount;)
        {
            int unitX = it->getX();
            int unitY = it->getY();
            int towerX = tower.getX();
            int towerY = tower.getY();

            int distanceSquared = (towerX - unitX) * (towerX - unitX) + (towerY - unitY) * (towerY - unitY);

            if (distanceSquared <= range * range) 
            {
                //����ü ����
                createProjectile(tower, *it);

                // �⺻ ������ ó��
                int newHp = calculateDamage(tower.getIsMagic(), damage, *it);
                it->reduceHp(newHp);

                // ���� ���� ó��
                if (it->getHp() <= 0) 
                {
                    if (currentWaveType)
                    
                    {
                        gold += it->getKillReward();
                    }
                    it = activeUnits.erase(it);
                    break;
                }

                aoeTargets.push_back(&(*it)); // ���� ���� ����� ����
                targetsAttacked++;
            }
            ++it;
        }

        // **���� ���� ó��**
        if (tower.getIsNoDamage() == 2)
        {
            for (Unit* target : aoeTargets)
            { // �⺻ ���� ������κ��� ���� ���� ����
                for (auto aoeIt = activeUnits.begin(); aoeIt != activeUnits.end(); )
                {
                    if (&(*aoeIt) == target) 
                    {
                        ++aoeIt;
                        continue; // �⺻ ���� ����� ����
                    }

                    int aoeDistanceSquared = (target->getX() - aoeIt->getX()) * (target->getX() - aoeIt->getX()) +
                        (target->getY() - aoeIt->getY()) * (target->getY() - aoeIt->getY());

                    if (aoeDistanceSquared <= 1 * 1) { // ���� ���� �Ÿ� 1ĭ
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

    sf::Clock clock;
    sf::Time lastLogicUpdateTime = sf::Time::Zero;
    sf::Time logicUpdateInterval = sf::milliseconds(500); // �� ������Ʈ ���� (500ms)
    
    while (!unitProductionQueue.empty()) { unitProductionQueue.pop(); }// ���� ���� ��⿭ �ʱ�ȭ

    while (!waveOver)
    {
        sf::Time elapsedTime = clock.restart();

        handleAttackInput();

        // **�� ������Ʈ �ð� üũ**
        lastLogicUpdateTime += elapsedTime;
        if (lastLogicUpdateTime >= logicUpdateInterval)
        {
            lastLogicUpdateTime -= logicUpdateInterval;
            currentTick++;

            updateAttackUnits(activeUnits);

            // Ÿ���� ������ ����
            attackUnits(activeUnits, currentTick, currentwaveType);
            ui.updateattackGold(attackGold);
            updateAndPrintMap(activeUnits); // ���� ���̺꿡���� �� ���¸� ���
        }

        // **����ü ������Ʈ (�� ������)**
        updateProjectiles(elapsedTime);

        // UI ������Ʈ �� �ܼ� ���
        updateGameState(activeUnits);

        waveOver = isAttackWaveOver(activeUnits);
        if (previousPlayerLife > playerLife) waveOver = true;// ���� ���� �� ���� ���̺� �ٷ� ����

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (!unitProductionQueue.empty()) { unitProductionQueue.pop(); }// ���� ���� ��⿭ �ʱ�ȭ,���ݿ��̺갡 ������ �ٷ� �ʱ�ȭ

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
                unitType.getKillReward(), path, unitType.getLifeDamage());
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
            playerLife -= it->getLifeDamage();
            

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
        ui.setInfoText({ "Ÿ�� ��ġ ��尡 �ƴմϴ�.\n","...","..."});
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
void GameManager::selectTower(const std::vector<Tower>& towers, int towerId, int& selectedTowerIndex, UI& ui)
{
    const auto it = std::find_if(towers.begin(), towers.end(),
        [towerId](const Tower& tower) { return tower.getId() == towerId; });

    if (it != towers.end()) {
        const Tower& selectedTower = *it; // const Tower& ���
        selectedTowerIndex = std::distance(towers.begin(), it);
        std::cout << "Ÿ�� " << selectedTower.getTowerName() << " �� ���õǾ����ϴ�.\n";
        ui.setInfoText({ selectedTower.getTool() + "Ÿ���� ���õǾ����ϴ�.\n", selectedTower.getTool2(), "..." });
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else {
        // Ÿ���� �������� ���� ��
        std::cout << "�ش� Ÿ���� �������� �ʽ��ϴ�.\n";

        // 1�� ���
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

            if (gold - 30 >= 0)                                      //Ÿ�� �Ǽ���� 
            {
                PlacedTower newTower(selectedTower, selectedX, selectedY);
                gold -= 30;

                map[selectedY][selectedX] = newTower.getTowerName();
                placedTowers.push_back(newTower);
                std::cout << newTower.getTowerName() << " Ÿ���� ��ġ�Ǿ����ϴ�!\n";
                
              
                ui.setInfoText({ newTower.getTool() + " Ÿ���� ��ġ�Ǿ����ϴ�!\n",
                    newTower.getTool2(),
                    "���ݷ�: " + std::to_string(newTower.getDamage()) +
                    "\n���� �ӵ�: " + std::to_string(newTower.getTimePerAttack()) +
                    "\n��Ÿ�: " + std::to_string(newTower.getAttackRange()) +
                    "\n���� ����: " + newTower.attackType() });
            }
            else
            {
                std::cout << "Ÿ���� ��ġ�� ��尡 �����մϴ�.\n";
                ui.setInfoText({"Ÿ���� ��ġ�� ��尡 �����մϴ�.\n", "...", "..."});
            }
        }
        else
        {
            std::cout << "�ش� ��ġ�� Ÿ���� ��ġ�� �� �����ϴ�.\n";
            ui.setInfoText({ "�ش� ��ġ�� Ÿ���� ��ġ�� �� �����ϴ�.\n","...","..." });
        }
    }
    else
    {
        std::cout << "Ÿ���� ���õ��� �ʾҽ��ϴ�.\n";
        ui.setInfoText({ "Ÿ���� ���õ��� �ʾҽ��ϴ�.\n","...","..." });
    }



}

void GameManager::mapSelected()
{
    // TGUI GUI ����
    tgui::Gui gui(ui.getWindow());

    // ������ ������� ����
    sf::RectangleShape background(sf::Vector2f(ui.getWindow().getSize().x, ui.getWindow().getSize().y));
    background.setFillColor(sf::Color::White);

    // �ؽ�Ʈ ���� �� ����
    auto titleLabel = tgui::Label::create("Select   A   Track");
    titleLabel->setPosition("50%", "7%");
    titleLabel->setOrigin(0.5f, 0.5f);
    titleLabel->setTextSize(140);
    titleLabel->getRenderer()->setTextColor(sf::Color::Black);
    tgui::Font font("resources/fonts/Bangers.ttf");
    titleLabel->getRenderer()->setFont(font);
    gui.add(titleLabel);

    // �� ���� ��ư ���� �� ũ�� ����
    auto button1 = tgui::Button::create();
    button1->setPosition("5%", "17%");
    button1->setSize("28%", "70%");
    button1->getRenderer()->setTexture("resources/images/stageImgs/spring.png"); // ��ư �̹���
    button1->getRenderer()->setOpacity(0.5f); // �⺻ ���� ����
    button1->onMouseEnter([button1]() { button1->getRenderer()->setOpacity(1.0f); }); // ���콺 ���� �� ���� ����
    button1->onMouseLeave([button1]() { button1->getRenderer()->setOpacity(0.5f); }); // ���콺�� ������ �ٽ� ������
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

    // Exit ��ư ���� �� �̹��� ����
    auto exitButton = tgui::Button::create(); // �ؽ�Ʈ ���� ��ư
    exitButton->setPosition("90%", "4%");          // ������ �� ��
    exitButton->setSize("7%", "10%");              // Exit ��ư ũ��
    exitButton->getRenderer()->setTexture("resources/images/stageImgs/exit.png"); // X �̹��� ����
    // �׵θ� ����
    exitButton->getRenderer()->setBorders(0); // �׵θ� �β��� 0���� ����
    exitButton->getRenderer()->setBorderColor(sf::Color::Transparent); // �׵θ� ������ �������� ����

    exitButton->getRenderer()->setOpacity(0.5f);   // �⺻ ���� ����
    exitButton->onMouseEnter([exitButton]() {
        exitButton->getRenderer()->setOpacity(1.0f); // ���콺 ���� �� ������
        });
    exitButton->onMouseLeave([exitButton]() {
        exitButton->getRenderer()->setOpacity(0.5f); // ���콺 ������ �ٽ� ������
        });
    gui.add(exitButton);

    // ��ư �Ʒ� �ؽ�Ʈ(Label) �߰�
    auto label1 = tgui::Label::create("B e g i n n e r");
    label1->setTextSize(60);
    label1->getRenderer()->setTextColor(sf::Color::Black);
    label1->getRenderer()->setFont(font);
    label1->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center); // �ؽ�Ʈ �߾� ����
    label1->setPosition(
        tgui::bindLeft(button1) + tgui::bindWidth(button1) / 2, // ��ư�� ���� �߾�
        tgui::bindBottom(button1) + 10                         // ��ư �Ʒ� �ణ ���
    );
    label1->setOrigin(0.5f, 0.0f); // �� �߾� ���� ����
    gui.add(label1);

    auto label2 = tgui::Label::create("I n t e r m e d i a t e");
    label2->setTextSize(60);
    label2->getRenderer()->setTextColor(sf::Color::Black);
    label2->getRenderer()->setFont(font);
    label2->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center); // �ؽ�Ʈ �߾� ����
    label2->setPosition(
        tgui::bindLeft(button2) + tgui::bindWidth(button2) / 2, // ��ư�� ���� �߾�
        tgui::bindBottom(button2) + 10                         // ��ư �Ʒ� �ణ ���
    );
    label2->setOrigin(0.5f, 0.0f); // �� �߾� ���� ����
    gui.add(label2);

    auto label3 = tgui::Label::create("A d v a n c e d");
    label3->setTextSize(60);
    label3->getRenderer()->setTextColor(sf::Color::Black);
    label3->getRenderer()->setFont(font);
    label3->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center); // �ؽ�Ʈ �߾� ����
    label3->setPosition(
        tgui::bindLeft(button3) + tgui::bindWidth(button3) / 2, // ��ư�� ���� �߾�
        tgui::bindBottom(button3) + 10                         // ��ư �Ʒ� �ణ ���
    );
    label3->setOrigin(0.5f, 0.0f); // �� �߾� ���� ����
    gui.add(label3);

    bool mapChosen = false;
    std::string selectedMap;
    std::string stageFile;

    button1->onClick([&]()
        {
            selectedMap = "Map1.csv";
            stageFile = "Stage1.csv";
            mapChosen = true;
        });

    button2->onClick([&]()
        {
            selectedMap = "Map2.csv";
            stageFile = "Stage2.csv";
            mapChosen = true;
        });

    button3->onClick([&]()
        {
            selectedMap = "Map3.csv";
            stageFile = "Stage3.csv";
            mapChosen = true;
        });
    exitButton->onClick([&]()
        {
            gameStart();
        });
    // GUI ����
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

        // ȭ�� �׸���
        ui.getWindow().clear();
        ui.getWindow().draw(background);
        gui.draw();
        ui.getWindow().display();
    }

    if (mapChosen)
    {
        std::cout << selectedMap << " ���� ���õǾ����ϴ�.\n";
        loadMap(selectedMap);
        parsePath();
        run(stageFile);  // ���õ� ������ ���� ����
    }
}

void GameManager::showGameOverPopup()
{
    // �˾� GUI ����
    tgui::Gui gui(ui.getWindow());

    // ��� ���� (������)
    sf::RectangleShape background(sf::Vector2f(ui.getWindow().getSize().x, ui.getWindow().getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150)); // ������ ������

    // �˾� â �����̳�
    auto popupPanel = tgui::Panel::create({ "50%", "30%" });
    popupPanel->setPosition("25%", "35%");
    popupPanel->getRenderer()->setBackgroundColor(sf::Color(255, 255, 255));
    popupPanel->getRenderer()->setBorders(2);
    popupPanel->getRenderer()->setBorderColor(sf::Color(0, 0, 0));
    gui.add(popupPanel);

    // �ؽ�Ʈ ��
    auto label = tgui::Label::create("Game Over!\nWhat would you like to do?");
    label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    label->setTextSize(24);
    label->setPosition("10%", "15%");
    popupPanel->add(label);

    // �� ���� ��ư
    auto mapSelectButton = tgui::Button::create("Return to Map Selection");
    mapSelectButton->setSize("80%", "20%");
    mapSelectButton->setPosition("10%", "50%");
    popupPanel->add(mapSelectButton);

    // ���� ���� ��ư
    auto exitButton = tgui::Button::create("Exit Game");
    exitButton->setSize("80%", "20%");
    exitButton->setPosition("10%", "75%");
    popupPanel->add(exitButton);

    bool actionSelected = false; // ����ڰ� ��ư�� �������� Ȯ��

    mapSelectButton->onClick([&]()
        {
            actionSelected = true;
            gui.remove(popupPanel);
            mapSelected(); // �� ���� ȭ������ ���ư���
        });

    exitButton->onClick([&]()
        {
            actionSelected = true;
            ui.getWindow().close(); // ���� â �ݱ�
        });

    while (ui.getWindow().isOpen() && !actionSelected)
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

        ui.getWindow().clear();
        ui.getWindow().draw(background);
        gui.draw();
        ui.getWindow().display();
    }
}

void GameManager::gameStart() {
    // TGUI GUI ����
    tgui::Gui gui(ui.getWindow());

    // ������ ����
    sf::RectangleShape background(sf::Vector2f(ui.getWindow().getSize().x, ui.getWindow().getSize().y));
    background.setFillColor(sf::Color::Blue);

    // ��Ʈ �ε�
    sf::Font bmFont;
    if (!bmFont.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        std::cerr << "Failed to load BMDOHYEON_ttf.ttf" << std::endl;
        return;
    }

    // ���� Ÿ��Ʋ ����
    sf::Text mainTitle;
    mainTitle.setFont(bmFont);
    mainTitle.setString(L"�� �� ��   �� �� ��");
    mainTitle.setCharacterSize(140); // ũ�� ����
    mainTitle.setFillColor(sf::Color::White);
    mainTitle.setOrigin(mainTitle.getLocalBounds().width / 2.f, mainTitle.getLocalBounds().height / 2.f);
    mainTitle.setPosition(ui.getWindow().getSize().x / 2.f, ui.getWindow().getSize().y * 0.2f); // ��ġ ����

    // "Spy of the Battlefield" �ؽ�Ʈ �߰� (���� Ÿ��Ʋ)
    auto subTitle = tgui::Label::create("Spy of the Battlefield");
    subTitle->setPosition("50%", "35%");          // ���� Ÿ��Ʋ �Ʒ��� ��ġ
    subTitle->setOrigin(0.5f, 0.5f);              // �߾� ����
    subTitle->setTextSize(70);                    // �ؽ�Ʈ ũ��
    subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, 0)); // ���� 0���� ����

    // Bangers ��Ʈ ����
    tgui::Font bangersFont("resources/fonts/Bangers.ttf");    // Bangers ��Ʈ ���
    subTitle->getRenderer()->setFont(bangersFont);           // ��Ʈ ����
    gui.add(subTitle);

    // "Start Game" ��ư �߰�
    auto startButton = tgui::Button::create("Start Game");
    startButton->setPosition("40%", "60%");
    startButton->setSize("20%", "10%");

    // ��ư ���� �ؽ�Ʈ ���� ����
    startButton->getRenderer()->setBackgroundColor(sf::Color::Green);
    startButton->getRenderer()->setTextColor(sf::Color::White);
    startButton->getRenderer()->setFont(bangersFont);      // ��ư �ؽ�Ʈ�� ��Ʈ ����

    // �ؽ�Ʈ ũ�� ����
    startButton->setTextSize(40); // ���ϴ� �۾� ũ�� (����: �ȼ�)

    // ��ư�� GUI�� �߰�
    gui.add(startButton);

    // ��ư Ŭ�� �̺�Ʈ ����
    bool gameStarted = false;
    startButton->onClick([&]() {
        gameStarted = true;
        });

    // �ִϸ��̼� ����
    sf::Clock clock; // �ð� ������ ���� SFML �ð�
    float fadeInTime = 3.0f; // ���� Ÿ��Ʋ ���̵��� ���� �ð�

    // GUI ����
    while (ui.getWindow().isOpen() && !gameStarted) {
        sf::Event event;
        while (ui.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                ui.getWindow().close();
                return;
            }
            gui.handleEvent(event);
        }

        // ���� Ÿ��Ʋ ���̵��� �ִϸ��̼�
        float elapsed = clock.getElapsedTime().asSeconds();
        if (elapsed < fadeInTime) {
            int alpha = static_cast<int>((elapsed / fadeInTime) * 255); // 0 ~ 255�� ��ȯ
            subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, alpha));
        }
        else {
            subTitle->getRenderer()->setTextColor(sf::Color(255, 255, 255, 255)); // ������ ǥ��
        }

        // ȭ�� �׸���
        ui.getWindow().clear();
        ui.getWindow().draw(background); // ��� �׸���
        ui.getWindow().draw(mainTitle);  // ���� Ÿ��Ʋ �׸���
        gui.draw();                      // TGUI GUI �׸���
        ui.getWindow().display();
    }

    // "Map Selected" ȭ�� ȣ��
    if (gameStarted) {
        mapSelected(); // ���� ���� �� mapSelected ȣ��
    }
}
