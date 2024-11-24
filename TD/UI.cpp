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
    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            std::string tileType = map[y][x];

            // 이소메트릭 좌표 변환
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            screenX += offsetX;
            screenY += offsetY;

            // 타일 스프라이트 선택
            sf::Sprite* tileSprite = nullptr;
            if (tileType == "P" || tileType == "S" || tileType == "D") {
                tileSprite = &roadSprite;
            }
            else if (tileType == "O") {
                tileSprite = &buildingPlaceGrassSprite;
            }
            else {
                tileSprite = &grassSprite;
            }

            // 스프라이트 위치 설정
            tileSprite->setPosition(screenX, screenY);

            window.draw(*tileSprite);

            // 선택된 타일이면 반투명한 오버레이를 그려서 강조 표시
            if (x == selectedX && y == selectedY) {
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

    // **타워와 유닛 간 공격 범위 확인**
    for (const auto& tower : placedTowers) {
        for (const auto& unit : units) {
            // 타워와 유닛 거리 확인
            if (tower.isUnitInRange(unit.getX(), unit.getY(), tower.getX(), tower.getY())) {
                // 공격 범위 내에 있으면 drawBullet 호출
                drawBullet(window, tower, unit, placedTowers, units);
            }
        }
    }

    // **타워 그리기 (drawTower 함수 호출)**
    for (const auto& tower : placedTowers) {
        drawTower(window, tower, offsetX, offsetY);
    }

    // **유닛 그리기 (drawUnit 함수 호출)**
    for (const auto& unit : units) {
        drawUnit(window, unit, offsetX, offsetY);
    }

    // 플레이어 라이프 및 골드 표시
    lifeText.setString("Life: " + std::to_string(playerLife));
    goldText.setString("Gold: " + std::to_string(gold));

    window.draw(lifeText);
    window.draw(goldText);

    gui.draw();

    window.display();
}


void UI::drawUnit(sf::RenderWindow& window, const Unit& unit, float offsetX, float offsetY) {
    int tileX = unit.getX();
    int tileY = unit.getY();

    // 이소메트릭 좌표 변환
    float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
    float screenY = (tileX + tileY) * (tileHeight / 2.0f);

    screenX += offsetX;
    screenY += offsetY;

    // 유닛 스프라이트 위치 설정
    knightUnitSprite.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight);

    // 체력 바 그리기
    unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit.getHp(), 10); // 체력 최대값 10으로 설정

    // 유닛 스프라이트 그리기
    window.draw(knightUnitSprite);
}
void UI::drawTower(sf::RenderWindow& window, const PlacedTower& tower, float offsetX, float offsetY) {
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

    // 타워 스프라이트 그리기
    window.draw(archerTowerSprite);
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

// 체력바를 생성하는 함수
void UI::unitHpBar(sf::RenderWindow& window, float screenX, float screenY, int currentHp, int maxHp) {
    float healthBarWidth = 40.0f;         // 체력 바 너비
    float healthBarHeight = 5.0f;        // 체력 바 높이
    float healthRatio = static_cast<float>(currentHp) / maxHp; // 체력 비율

    // 체력 바 배경
    sf::RectangleShape healthBarBackground(sf::Vector2f(healthBarWidth, healthBarHeight));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50)); // 회색 배경
    healthBarBackground.setPosition(screenX - healthBarWidth / 2.0f, screenY - 10);

    // 체력 바 (현재 체력)
    sf::RectangleShape healthBar(sf::Vector2f(healthBarWidth * healthRatio, healthBarHeight));
    healthBar.setFillColor(sf::Color(200, 0, 0)); // 빨간색
    healthBar.setPosition(screenX - healthBarWidth / 2.0f, screenY - 10);

    // 체력 바 그리기
    window.draw(healthBarBackground);
    window.draw(healthBar);
}

void UI::drawBullet(sf::RenderWindow& window, const PlacedTower& tower, const Unit& unit,
    const std::vector<PlacedTower>& placedTowers, const std::vector<Unit>& activeUnits) {
    sf::CircleShape bullet(5.f); // 총알 크기
    bullet.setFillColor(sf::Color(255, 255, 0)); // 형광 노랑

    float offsetX = -100.0f;
    float offsetY = 200.0f;

    // 타워의 이소메트릭 화면 좌표 계산
    sf::Vector2f towerScreenPos(
        (tower.getX() - tower.getY()) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f + offsetX,
        (tower.getX() + tower.getY()) * (tileHeight / 2.0f) + offsetY
    );

    // 유닛의 이소메트릭 화면 좌표 계산
    sf::Vector2f unitScreenPos(
        (unit.getX() - unit.getY()) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f + offsetX,
        (unit.getX() + unit.getY()) * (tileHeight / 2.0f) + offsetY
    );

    bullet.setPosition(towerScreenPos); // 총알 시작 위치

    // 방향 벡터 계산
    sf::Vector2f direction = unitScreenPos - towerScreenPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 방향 벡터 정규화
    direction /= distance;

    // 총알 속도 설정
    float bulletSpeed = 500.f; // 픽셀/초

    sf::Vector2f velocity = direction * bulletSpeed;

    sf::Vector2f currentPosition = towerScreenPos;
    sf::Clock clock; // 경과 시간 측정용

    // 애니메이션 루프
    while (true) {
        float elapsedTime = clock.restart().asSeconds(); // 경과 시간 측정

        // 총알의 현재 위치 업데이트
        currentPosition += velocity * elapsedTime;
        bullet.setPosition(currentPosition);

        // 충돌 체크: 총알이 유닛에 닿으면 종료
        if (std::abs(currentPosition.x - unitScreenPos.x) < 45.f &&
            std::abs(currentPosition.y - unitScreenPos.y) < 45.f) {
            std::cout << "Bullet hit the target at (" << unit.getX() << ", " << unit.getY() << ").\n";
            break;
        }

        // 화면 렌더링
        window.clear();

        // 맵 및 타워, 유닛 다시 그리기
        for (int y = 0; y < static_cast<int>(map.size()); ++y) {
            for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
                std::string tileType = map[y][x];

                sf::Vector2f tileScreenPos(
                    (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f + offsetX,
                    (x + y) * (tileHeight / 2.0f) + offsetY
                );

                sf::Sprite* tileSprite = nullptr;
                if (tileType == "P" || tileType == "S" || tileType == "D") {
                    tileSprite = &roadSprite;
                }
                else if (tileType == "O") {
                    tileSprite = &buildingPlaceGrassSprite;
                }
                else {
                    tileSprite = &grassSprite;
                }

                tileSprite->setPosition(tileScreenPos);
                window.draw(*tileSprite);
            }
        }

        // 타워 그리기
        for (const auto& placedTower : placedTowers) {
            drawTower(window, placedTower, offsetX, offsetY);
        }

        // 유닛 그리기
        for (const auto& activeUnit : activeUnits) {
            drawUnit(window, activeUnit, offsetX, offsetY);
        }

        // 총알 그리기
        window.draw(bullet);

        // 화면 갱신
        window.display();
    }
    
}
