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

    // 타일 텍스처 로드 (예시로 단색 사각형 사용)
    tileTexture.create(tileSize, tileSize);
    sf::Image tileImage;
    tileImage.create(tileSize, tileSize, sf::Color(100, 100, 100));
    tileTexture.update(tileImage);
    tileSprite.setTexture(tileTexture);

    // 유닛 및 타워 텍스처 로드 (예시로 단색 사각형 사용)
    unitTexture.create(tileSize, tileSize);
    sf::Image unitImage;
    unitImage.create(tileSize, tileSize, sf::Color::Red);
    unitTexture.update(unitImage);
    unitSprite.setTexture(unitTexture);

    towerTexture.create(tileSize, tileSize);
    sf::Image towerImage;
    towerImage.create(tileSize, tileSize, sf::Color::Blue);
    towerTexture.update(towerImage);
    towerSprite.setTexture(towerTexture);

    // 유닛 및 타워의 색상 설정 (유닛/타워 이름별로 구분)
    unitColors["U1"] = sf::Color::Red;
    unitColors["U2"] = sf::Color::Green;
    unitColors["U3"] = sf::Color::Yellow;

    towerColors["T1"] = sf::Color::Blue;
    towerColors["T2"] = sf::Color::Cyan;
    towerColors["T3"] = sf::Color::Magenta;
}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers, int playerLife, int gold, int selectedX, int selectedY)
{
    window.clear();

    // 맵 그리기
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            std::string tileType = map[y][x];
            sf::RectangleShape tileShape(sf::Vector2f(tileSize, tileSize));
            tileShape.setPosition(x * tileSize, y * tileSize);

            // 선택된 타일이면 다른 색상으로 표시
            if ((int)x == selectedX && (int)y == selectedY)
            {
                tileShape.setFillColor(sf::Color::Yellow); // 선택된 타일 색상
            }
            else
            {
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
            }

            window.draw(tileShape);
        }
    }

    // 유닛 그리기 (크기를 tileSize보다 작게 설정)
    float unitScale = 0.8f; // 유닛 크기를 tileSize의 80%로 설정
    for (const auto& unit : units)
    {
        sf::RectangleShape unitShape(sf::Vector2f(tileSize * unitScale, tileSize * unitScale));
        unitShape.setOrigin((tileSize * unitScale) / 2, (tileSize * unitScale) / 2); // 중심점을 가운데로 설정
        unitShape.setPosition((unit.getX() + 0.5f) * tileSize, (unit.getY() + 0.5f) * tileSize);

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

    // 타워 그리기 (필요하다면 타워도 크기를 조절)
    float towerScale = 0.9f; // 타워 크기를 tileSize의 90%로 설정
    for (const auto& tower : placedTowers)
    {
        sf::RectangleShape towerShape(sf::Vector2f(tileSize * towerScale, tileSize * towerScale));
        towerShape.setOrigin((tileSize * towerScale) / 2, (tileSize * towerScale) / 2); // 중심점을 가운데로 설정
        towerShape.setPosition((tower.getX() + 0.5f) * tileSize, (tower.getY() + 0.5f) * tileSize);

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
