#include "UI.h"
#include <iostream>

void UI::initialize(const std::vector<std::vector<std::string>>& gameMap)
{
    map = gameMap;
    window.create(sf::VideoMode(1600, 900), "Tower Defense Game");

    // 폰트 로드
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cout << "폰트를 로드할 수 없습니다.\n";
    }

    // 텍스트 설정
    lifeText.setFont(font);
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition(10, 10);

    goldText.setFont(font);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(10, 40);

    // 타일 크기 (이소메트릭을 위해 수정)
    // tileSize는 유지하지만, 이소메트릭 타일의 폭과 높이를 따로 정의
    // 이소메트릭 타일의 폭과 높이 설정
    tileWidth = tileSize;         // 128
    tileHeight = tileSize / 2;    // 64

    // 유닛 및 타워의 색상 설정 (유닛/타워 이름별로 구분)
    unitColors["U1"] = sf::Color::Red;
    unitColors["U2"] = sf::Color::Green;
    unitColors["U3"] = sf::Color::Yellow;

    towerColors["T1"] = sf::Color::Blue;
    towerColors["T2"] = sf::Color::Cyan;
    towerColors["T3"] = sf::Color::Magenta;
}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
    int playerLife, int gold, int selectedX, int selectedY)
{
    window.clear();

    // 타일 크기 변수 설정 (이소메트릭을 위해 추가)
    const float tileWidth = tileSize;          // 128
    const float tileHeight = tileSize / 2.0f;  // 64

    // 맵 그리기
    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x)
        {
            std::string tileType = map[y][x];

            // 이소메트릭 좌표 변환
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            // 이소메트릭 타일 그리기 (마름모 형태)
            sf::ConvexShape tileShape;
            tileShape.setPointCount(4);
            tileShape.setPoint(0, sf::Vector2f(tileWidth / 2.0f, 0));
            tileShape.setPoint(1, sf::Vector2f(tileWidth, tileHeight / 2.0f));
            tileShape.setPoint(2, sf::Vector2f(tileWidth / 2.0f, tileHeight));
            tileShape.setPoint(3, sf::Vector2f(0, tileHeight / 2.0f));
            tileShape.setPosition(screenX, screenY);

            // 타일 색상 설정
            if (tileType == "P")
            {
                tileShape.setFillColor(sf::Color(150, 150, 150)); // 경로 타일
            }
            else if (tileType == "O")
            {
                tileShape.setFillColor(sf::Color(100, 100, 100)); // 설치 가능 타일
            }
            else if (tileType == "S")
            {
                tileShape.setFillColor(sf::Color::Green); // 시작 지점
            }
            else if (tileType == "D")
            {
                tileShape.setFillColor(sf::Color::Red); // 목적 지점
            }
            else
            {
                tileShape.setFillColor(sf::Color(50, 50, 50)); // 기본 타일
            }

            window.draw(tileShape);

            // 선택된 타일이면 반투명한 오버레이를 그려서 강조 표시
            if (x == selectedX && y == selectedY)
            {
                sf::ConvexShape overlayShape = tileShape;
                overlayShape.setFillColor(sf::Color(255, 255, 0, 100)); // 노란색, 투명도 100
                window.draw(overlayShape);
            }
        }
    }

    // 유닛 그리기 (크기를 tileSize보다 작게 설정)
    float unitScale = 0.4f; // 유닛 크기를 조절
    for (const auto& unit : units)
    {
        int tileX = unit.getX();
        int tileY = unit.getY();

        // 이소메트릭 좌표 변환
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        // 유닛의 위치를 타일의 중심으로 설정하고, 크기를 조절
        sf::CircleShape unitShape((tileWidth * unitScale) / 2.0f);
        unitShape.setOrigin((tileWidth * unitScale) / 2.0f, (tileWidth * unitScale) / 2.0f);
        unitShape.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight / 2.0f);

        // 유닛의 이름에 따른 색상 설정
        std::string unitName = unit.getName();
        if (unitColors.find(unitName) != unitColors.end())
        {
            unitShape.setFillColor(unitColors[unitName]);
        }
        else
        {
            unitShape.setFillColor(sf::Color::White);
        }

        window.draw(unitShape);
    }

    // 타워 그리기 (크기를 조절)
    float towerScale = 0.5f; // 타워 크기를 조절
    for (const auto& tower : placedTowers)
    {
        int tileX = tower.getX();
        int tileY = tower.getY();

        // 이소메트릭 좌표 변환
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        // 타워의 위치를 타일의 중심으로 설정하고, 크기를 조절
        sf::RectangleShape towerShape(sf::Vector2f(tileWidth * towerScale, (tileHeight * towerScale) * 2));
        towerShape.setOrigin((tileWidth * towerScale) / 2.0f, (tileHeight * towerScale));
        towerShape.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight / 2.0f);

        // 타워의 이름에 따른 색상 설정
        std::string towerName = tower.getTowerName();
        if (towerColors.find(towerName) != towerColors.end())
        {
            towerShape.setFillColor(towerColors[towerName]);
        }
        else
        {
            towerShape.setFillColor(sf::Color::White);
        }

        window.draw(towerShape);
    }

    // 플레이어 라이프 및 골드 표시
    lifeText.setString("Life: " + std::to_string(playerLife));
    goldText.setString("Gold: " + std::to_string(gold));

    window.draw(lifeText);
    window.draw(goldText);

    window.display();
}


sf::RenderWindow& UI::getWindow()
{
    return window;
}

void UI::setMap(const std::vector<std::vector<std::string>>& gameMap)
{
    map = gameMap;
}

void UI::setPath(const std::vector<std::pair<int, int>>& gamePath)
{
    path = gamePath;
}

void UI::setTowers(const std::vector<Tower>& gameTowers)
{
    towers = gameTowers;
}

void UI::setUnitTypes(const std::vector<UnitType>& gameUnitTypes)
{
    unitTypes = gameUnitTypes;
}
