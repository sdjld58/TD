#include "UI.h"
#include <iostream>

void UI::initialize(const std::vector<std::vector<std::string>>& gameMap)
{
    map = gameMap;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Tower Defense Game");

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

    // 타일 텍스처 로드
    if (!roadTexture.loadFromFile("resources/images/tiles/road_spring(1).png"))
        std::cerr << "road_spring(1).png를 로드할 수 없습니다.\n";
    roadSprite.setTexture(roadTexture);

    if (!grassTexture.loadFromFile("resources/images/tiles/grass.png"))
        std::cerr << "grass.png를 로드할 수 없습니다.\n";
    grassSprite.setTexture(grassTexture);

    if (!buildingPlaceGrassTexture.loadFromFile("resources/images/tiles/buildingPlaceGrass.png"))
        std::cerr << "buildingPlaceGrass.png를 로드할 수 없습니다.\n";
    buildingPlaceGrassSprite.setTexture(buildingPlaceGrassTexture);

    // 타워 텍스처 로드
    if (!archerTowerTexture.loadFromFile("resources/images/towers/archer_level_1.png"))
        std::cerr << "archer_level_1.png를 로드할 수 없습니다.\n";
    archerTowerSprite.setTexture(archerTowerTexture);

    // **유닛 텍스처 로드**
    if (!knightUnitTexture.loadFromFile("resources/images/units/knight_level_1.png"))
        std::cerr << "knight_level_1.png를 로드할 수 없습니다.\n";
    knightUnitSprite.setTexture(knightUnitTexture);

    // 유닛 스프라이트 설정 (필요에 따라 원점과 크기 조절)
    knightUnitSprite.setOrigin(knightUnitTexture.getSize().x / 2.0f, knightUnitTexture.getSize().y * 1.3);
    float unitScale = tileHeight / knightUnitTexture.getSize().y;
    knightUnitSprite.setScale(unitScale/1.5, unitScale/1.5);

    //버튼 그리기
    gui.setWindow(window);

    tgui::Button::Ptr towerButton = tgui::Button::create("tower");
    towerButton->setSize(100, 50); // 버튼 크기 설정
    towerButton->setPosition(0, windowHeight - 50); // 버튼 위치 설정
   
    // 버튼에 콜백 함수 연결
    towerButton->onClick([this]() {
        if (onTowerButtonClicked)
        {
            onTowerButtonClicked();
        }
        });

    gui.add(towerButton, "towerButton");


}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
    int playerLife, int gold, int selectedX, int selectedY)
{
    window.clear();
    
    float offsetX = -100.0f;
    float offsetY = 200.0f;

    // 맵 그리기
    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x)
        {
            std::string tileType = map[y][x];

            // 이소메트릭 좌표 변환
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            screenX += offsetX;
            screenY += offsetY;

            // 타일 스프라이트 선택
            sf::Sprite* tileSprite = nullptr;
            if (tileType == "P" || tileType == "S" || tileType == "D")
            {
                tileSprite = &roadSprite;
            }
            else if (tileType == "O")
            {
                tileSprite = &buildingPlaceGrassSprite;
            }
            else
            {
                tileSprite = &grassSprite;
            }

            // 스프라이트 위치 설정
            tileSprite->setPosition(screenX, screenY);

            window.draw(*tileSprite);

            // 선택된 타일이면 반투명한 오버레이를 그려서 강조 표시
            if (x == selectedX && y == selectedY)
            {
                sf::ConvexShape overlay;
                overlay.setPointCount(4);
                overlay.setPoint(0, sf::Vector2f(tileWidth / 2.0f, 0));
                overlay.setPoint(1, sf::Vector2f(tileWidth, tileHeight / 2.0f));
                overlay.setPoint(2, sf::Vector2f(tileWidth / 2.0f, tileHeight));
                overlay.setPoint(3, sf::Vector2f(0, tileHeight / 2.0f));
                overlay.setPosition(screenX, screenY);
                overlay.setFillColor(sf::Color(255, 255, 0, 100)); // 노란색, 투명도 100
                window.draw(overlay);
            }
        }
    }

    // **유닛 그리기 (스프라이트 사용)**
    for (const auto& unit : units)
    {
        int tileX = unit.getX();
        int tileY = unit.getY();

        // 이소메트릭 좌표 변환
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        screenX += offsetX;
        screenY += offsetY;

        // 유닛 스프라이트 위치 설정
        knightUnitSprite.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight);

        window.draw(knightUnitSprite);
    }

    // 타워 그리기
    for (const auto& tower : placedTowers)
    {
        int tileX = tower.getX();
        int tileY = tower.getY();

        // 이소메트릭 좌표 변환
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        screenX += offsetX;
        screenY += offsetY;

        // 타워 스프라이트 위치 설정
        float towerOffsetX = 30.0;
        float towerOffsetY = -20.0;
        archerTowerSprite.setPosition(screenX + towerOffsetX, screenY + towerOffsetY);

        window.draw(archerTowerSprite);
    }

    // 플레이어 라이프 및 골드 표시
    lifeText.setString("Life: " + std::to_string(playerLife));
    goldText.setString("Gold: " + std::to_string(gold));

    window.draw(lifeText);
    window.draw(goldText);

    gui.draw();

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
