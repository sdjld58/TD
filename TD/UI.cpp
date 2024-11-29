#include "UI.h"
#include <iostream>
#include <Windows.h>

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
    if (!archer1TowerTexture.loadFromFile("resources/images/towers/archer_level_1.png"))
        std::cerr << "archer_level_1.png를 로드할 수 없습니다.\n";
    archer1TowerSprite.setTexture(archer1TowerTexture);

    if (!archer2TowerTexture.loadFromFile("resources/images/towers/archer_level_2.png"))
        std::cerr << "archer_level_2.png를 로드할 수 없습니다.\n";
    archer2TowerSprite.setTexture(archer2TowerTexture);

    if (!archer3TowerTexture.loadFromFile("resources/images/towers/archer_level_3.png"))
        std::cerr << "archer_level_3.png를 로드할 수 없습니다.\n";
    archer3TowerSprite.setTexture(archer3TowerTexture);

    if (!wizard1TowerTexture.loadFromFile("resources/images/towers/wizard_level_1.png"))
        std::cerr << "wizard_level_1.png를 로드할 수 없습니다.\n";
    wizard1TowerSprite.setTexture(wizard1TowerTexture);

    if (!wizard2TowerTexture.loadFromFile("resources/images/towers/wizard_level_2.png"))
        std::cerr << "wizard_level_2.png를 로드할 수 없습니다.\n";
    wizard2TowerSprite.setTexture(wizard2TowerTexture);

    if (!wizard3TowerTexture.loadFromFile("resources/images/towers/wizard_level_3.png"))
        std::cerr << "wizard_level_3.png를 로드할 수 없습니다.\n";
    wizard3TowerSprite.setTexture(wizard3TowerTexture);

    if (!barrack1TowerTexture.loadFromFile("resources/images/towers/barrack_level_1.png"))
        std::cerr << "barrack_level_1.png를 로드할 수 없습니다.\n";
    barrack1TowerSprite.setTexture(barrack1TowerTexture);

    if (!barrack2TowerTexture.loadFromFile("resources/images/towers/barrack_level_2.png"))
        std::cerr << "barrack_level_2.png를 로드할 수 없습니다.\n";
    barrack2TowerSprite.setTexture(barrack2TowerTexture);

    if (!barrack3TowerTexture.loadFromFile("resources/images/towers/barrack_level_3.png"))
        std::cerr << "barrack_level_3.png를 로드할 수 없습니다.\n";
    barrack3TowerSprite.setTexture(barrack3TowerTexture);

    // 투사체 텍스처 로드
    if (!projectileTexture.loadFromFile("resources/images/towers/arrow.png"))
    {
        std::cerr << "arrow.png를 로드할 수 없습니다.\n";
    }

    // **유닛 텍스처 로드**
    if (!knightUnitTexture.loadFromFile("resources/images/units/knight_level_1.png"))
        std::cerr << "knight_level_1.png를 로드할 수 없습니다.\n";
    knightUnitSprite.setTexture(knightUnitTexture);

    // 유닛 스프라이트 설정 (필요에 따라 원점과 크기 조절)
    knightUnitSprite.setOrigin(knightUnitTexture.getSize().x / 2.0f, knightUnitTexture.getSize().y * 1.3);
    float unitScale = tileHeight / knightUnitTexture.getSize().y;
    knightUnitSprite.setScale(unitScale/1.5, unitScale/1.5);

    drawButtons();
}

void UI::drawButtons()
{
    if (curWaveIsDefense)
    {
        //버튼 그리기
        gui.setWindow(window);
        tgui::Font font("resources/fonts/BMDOHYEON_ttf.ttf");
        auto towerButton = tgui::Button::create("Tower");
        towerButton->setSize(120, 70);
        towerButton->setPosition(25, windowHeight - 380);
        towerButton->getRenderer()->setRoundedBorderRadius(30); // 둥근 모서리
        towerButton->getRenderer()->setBackgroundColor(sf::Color::White); // 흰색 배경
        towerButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // 파란색 텍스트
        towerButton->setTextSize(26); //글씨 크기 설정
        towerButton->getRenderer()->setFont(font); // 폰트 설정




        // 버튼에 콜백 함수 연결
        towerButton->onClick([this]() {
            if (onTowerButtonClicked)
            {
                onTowerButtonClicked();
            }
            });
       

        gui.add(towerButton, "towerButton");
    
    }
    else
    {
        gui.setWindow(window);
        tgui::Font font("resources/fonts/BMDOHYEON_ttf.ttf");

        auto firstUnitButton = tgui::Button::create("A");
        firstUnitButton->setSize(80, 70);
        firstUnitButton->setPosition(windowWidth - 200, 380);
        firstUnitButton->getRenderer()->setRoundedBorderRadius(30); // 둥근 모서리
        firstUnitButton->getRenderer()->setBackgroundColor(sf::Color::White); // 흰색 배경
        firstUnitButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // 파란색 텍스트
        firstUnitButton->setTextSize(26); //글씨 크기 설정
        firstUnitButton->getRenderer()->setFont(font); // 폰트 설정

        auto secondUnitButton = tgui::Button::create("B");
        secondUnitButton->setSize(80, 70);
        secondUnitButton->setPosition(windowWidth - 300, 380);
        secondUnitButton->getRenderer()->setRoundedBorderRadius(30); // 둥근 모서리
        secondUnitButton->getRenderer()->setBackgroundColor(sf::Color::White); // 흰색 배경
        secondUnitButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // 파란색 텍스트
        secondUnitButton->setTextSize(26); //글씨 크기 설정
        secondUnitButton->getRenderer()->setFont(font); // 폰트 설정

        auto thirdUnitButton = tgui::Button::create("C");
        thirdUnitButton->setSize(80, 70);
        thirdUnitButton->setPosition(windowWidth - 400, 380);
        thirdUnitButton->getRenderer()->setRoundedBorderRadius(30); // 둥근 모서리
        thirdUnitButton->getRenderer()->setBackgroundColor(sf::Color::White); // 흰색 배경
        thirdUnitButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // 파란색 텍스트
        thirdUnitButton->setTextSize(26); //글씨 크기 설정
        thirdUnitButton->getRenderer()->setFont(font); // 폰트 설정

        firstUnitButton->onClick([this]() {
            if (onFirstUnitButtonClicked)
            {
                onFirstUnitButtonClicked();
            }
            });
        secondUnitButton->onClick([this]() {
            if (onSecondUnitButtonClicked)
            {
                onSecondUnitButtonClicked();
            }
            });
        thirdUnitButton->onClick([this]() {
            if (onThirdUnitButtonClicked)
            {
                onThirdUnitButtonClicked();
            }
            });

        gui.add(firstUnitButton, "firstUnitButton");
        gui.add(secondUnitButton, "secondUnitButton");
        gui.add(thirdUnitButton, "thirdUnitButton");
    }
}

void UI::clearButtons()
{
    gui.removeAllWidgets();
}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
    int playerLife, int gold, int selectedX, int selectedY, const std::vector<Projectile>& projectiles)
{
    window.clear();

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
 
    // 투사체 그리기
    drawProjectiles(projectiles);

    // **유닛 그리기 (스프라이트 사용)**
    for (auto& unit : units) {
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
        unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit.getHp(), 20); // 체력 최대값 20으로 설정

        // 유닛 스프라이트 그리기
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

        sf::Sprite* towerSprite = nullptr;
        int towerID = tower.getId();

        float towerOffsetX = 0;
        float towerOffsetY = 0;

        switch (towerID)
        {
            case 1:
                towerSprite = &barrack1TowerSprite; towerSprite->setScale(1.1, 1.1);
                towerOffsetX = 25.0;
                towerOffsetY = 20.0;
                break;
            case 101:
                towerSprite = &barrack2TowerSprite; towerSprite->setScale(1.1, 1.1);
                towerOffsetX = 25.0;
                towerOffsetY = 20.0;
                break;
            case 102:
                towerSprite = &barrack3TowerSprite; towerSprite->setScale(1.1, 1.1);
                towerOffsetX = 25.0;
                towerOffsetY = 20.0;
                break;
            case 2:
                towerSprite = &archer1TowerSprite;
                towerOffsetX = 30.0;
                towerOffsetY = 20.0;
                break;
            case 201:
                towerSprite = &archer2TowerSprite;
                towerOffsetX = 30.0;
                towerOffsetY = 20.0;
                break;
            case 202:
                towerSprite = &archer3TowerSprite;
                towerOffsetX = 30.0;
                towerOffsetY = 20.0;
                break;
            case 3:
                towerSprite = &wizard1TowerSprite;
                towerSprite->setScale(1.3, 1.3);
                towerOffsetX = 15.0;
                towerOffsetY = 25.0;
                break;
            case 301:
                towerSprite = &wizard2TowerSprite;
                towerSprite->setScale(1.3, 1.3);
                towerOffsetX = 15.0;
                towerOffsetY = 25.0;
                break;
            case 302:
                towerSprite = &wizard3TowerSprite;
                towerSprite->setScale(1.3, 1.3);
                towerOffsetX = 15.0;
                towerOffsetY = 25.0;
                break;
            default:
                towerSprite = &archer1TowerSprite;
                break;
        }

        screenX += offsetX + towerOffsetX;
        screenY += offsetY + towerOffsetY;
            
        towerSprite->setPosition(screenX, screenY - tileHeight / 2.0f);
        window.draw(*towerSprite);

    }
    TextBox textBox(*this, curWaveIsDefense, { getInfoText(0),getInfoText(1), getInfoText(2)});
        textBox.draw(window);
    
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
void UI::setIsDefence(bool isDefence)
{
    curWaveIsDefense = isDefence;
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

const sf::Texture& UI::getProjectileTexture() const
{
    return projectileTexture;
}

void UI::drawProjectiles(const std::vector<Projectile>& projectiles)
{
    for (const auto& projectile : projectiles)
    {
        sf::Sprite sprite = projectile.getSprite();

        // 현재 스프라이트의 위치를 가져옵니다.
        sf::Vector2f position = sprite.getPosition();

        // offsetX와 offsetY를 더해줍니다.
        position.x += offsetX;
        position.y += offsetY;

        // 위치를 업데이트합니다.
        sprite.setPosition(position);

        // 스프라이트를 그립니다.
        window.draw(sprite);
    }
}

void UI::setInfoText(std::initializer_list<std::string> infoSentences) {
    // 기존 infoTexts 벡터 초기화
    infoTexts.clear();

    // 로케일 설정 (필요한 경우)
    // std::locale::global(std::locale(""));

    for (const auto& sentence : infoSentences) {
        // 만약 입력 문자열이 UTF-8이 아니라면 CP_ACP 사용
        int size_needed = MultiByteToWideChar(
            CP_ACP,
            0,
            sentence.c_str(),
            -1,
            NULL,
            0
        );

        if (size_needed > 0) {
            std::wstring wstr(size_needed - 1, 0);
            MultiByteToWideChar(
                CP_ACP,
                0,
                sentence.c_str(),
                -1,
                &wstr[0],
                size_needed
            );

            infoTexts.push_back(wstr);
        }
    }
}

std::wstring UI::getInfoText(int index) const {
    if (index < 0 || index >= static_cast<int>(infoTexts.size())) {
        throw std::out_of_range("Index out of range");
    }
    return infoTexts[index];
}