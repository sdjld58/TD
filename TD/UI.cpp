#include "UI.h"
#include <iostream>
#include <Windows.h>

void UI::initialize(const std::vector<std::vector<std::string>>& gameMap)
{
    map = gameMap;
    gotoMapSelected = false;
    // SFML 창 생성
    window.create(sf::VideoMode(windowWidth, windowHeight), "Tower Defense Game");

   

    // 텍스트 설정
    lifeText.setFont(font);
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition(10, 10);

    goldText.setFont(font);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(10, 40);

   
    // 잔디 타일 텍스처 로드
    if (!grassRoadTexture1.loadFromFile("resources/images/tiles/road_spring(1).png"))
        std::cerr << "road_spring(1).png를 로드할 수 없습니다.\n";
    grassRoadSprite1.setTexture(grassRoadTexture1);

    if (!grassRoadTexture2.loadFromFile("resources/images/tiles/road_spring(2).png"))
        std::cerr << "road_spring(2).png를 로드할 수 없습니다.\n";
    grassRoadSprite2.setTexture(grassRoadTexture2);

    if (!grassRoadTexture3.loadFromFile("resources/images/tiles/road_spring(3).png"))
        std::cerr << "road_spring(3).png를 로드할 수 없습니다.\n";
    grassRoadSprite3.setTexture(grassRoadTexture3);

    if (!grassRoadTexture4.loadFromFile("resources/images/tiles/road_spring(4).png"))
        std::cerr << "road_spring(4).png를 로드할 수 없습니다.\n";
    grassRoadSprite4.setTexture(grassRoadTexture4);

    if (!grassRoadTexture5.loadFromFile("resources/images/tiles/road_spring(5).png"))
        std::cerr << "road_spring(5).png를 로드할 수 없습니다.\n";
    grassRoadSprite5.setTexture(grassRoadTexture5);

    if (!grassRoadTexture6.loadFromFile("resources/images/tiles/road_spring(6).png"))
        std::cerr << "road_spring(6).png를 로드할 수 없습니다.\n";
    grassRoadSprite6.setTexture(grassRoadTexture6);

    // 사막 타일 텍스처 로드
    if (!desertRoadTexture1.loadFromFile("resources/images/tiles/road_desert(1).png"))
        std::cerr << "road_desert(1).png를 로드할 수 없습니다.\n";
    desertRoadSprite1.setTexture(desertRoadTexture1);

    if (!desertRoadTexture2.loadFromFile("resources/images/tiles/road_desert(2).png"))
        std::cerr << "road_desert(2).png를 로드할 수 없습니다.\n";
    desertRoadSprite2.setTexture(desertRoadTexture2);

    if (!desertRoadTexture3.loadFromFile("resources/images/tiles/road_desert(3).png"))
        std::cerr << "road_desert(3).png를 로드할 수 없습니다.\n";
    desertRoadSprite3.setTexture(desertRoadTexture3);

    if (!desertRoadTexture4.loadFromFile("resources/images/tiles/road_desert(4).png"))
        std::cerr << "road_desert(4).png를 로드할 수 없습니다.\n";
    desertRoadSprite4.setTexture(desertRoadTexture4);

    if (!desertRoadTexture5.loadFromFile("resources/images/tiles/road_desert(5).png"))
        std::cerr << "road_desert(5).png를 로드할 수 없습니다.\n";
    desertRoadSprite5.setTexture(desertRoadTexture5);

    if (!desertRoadTexture6.loadFromFile("resources/images/tiles/road_desert(6).png"))
        std::cerr << "road_desert(6).png를 로드할 수 없습니다.\n";
    desertRoadSprite6.setTexture(desertRoadTexture6);

    // 겨울 타일 텍스처 로드
    if (!winterRoadTexture1.loadFromFile("resources/images/tiles/road_winter(1).png"))
        std::cerr << "road_winter(1).png를 로드할 수 없습니다.\n";
    winterRoadSprite1.setTexture(winterRoadTexture1);

    if (!winterRoadTexture2.loadFromFile("resources/images/tiles/road_winter(2).png"))
        std::cerr << "road_winter(2).png를 로드할 수 없습니다.\n";
    winterRoadSprite2.setTexture(winterRoadTexture2);

    if (!winterRoadTexture3.loadFromFile("resources/images/tiles/road_winter(3).png"))
        std::cerr << "road_winter(3).png를 로드할 수 없습니다.\n";
    winterRoadSprite3.setTexture(winterRoadTexture3);

    if (!winterRoadTexture4.loadFromFile("resources/images/tiles/road_winter(4).png"))
        std::cerr << "road_winter(4).png를 로드할 수 없습니다.\n";
    winterRoadSprite4.setTexture(winterRoadTexture4);

    if (!winterRoadTexture5.loadFromFile("resources/images/tiles/road_winter(5).png"))
        std::cerr << "road_winter(5).png를 로드할 수 없습니다.\n";
    winterRoadSprite5.setTexture(winterRoadTexture5);

    if (!winterRoadTexture6.loadFromFile("resources/images/tiles/road_winter(6).png"))
        std::cerr << "road_winter(6).png를 로드할 수 없습니다.\n";
    winterRoadSprite6.setTexture(winterRoadTexture6);


    if (!grassTexture.loadFromFile("resources/images/tiles/grass.png"))
        std::cerr << "grass.png를 로드할 수 없습니다.\n";
    grassSprite.setTexture(grassTexture);

    if (!sandTexture.loadFromFile("resources/images/tiles/sand.png"))
        std::cerr << "sand.png를 로드할 수 없습니다.\n";
    sandSprite.setTexture(sandTexture);

    if (!snowTexture.loadFromFile("resources/images/tiles/snow.png"))
        std::cerr << "snow.png를 로드할 수 없습니다.\n";
    snowSprite.setTexture(snowTexture);

    if (!buildingPlaceGrassTexture.loadFromFile("resources/images/tiles/buildingPlaceGrass.png"))
        std::cerr << "buildingPlaceGrass.png를 로드할 수 없습니다.\n";
    buildingPlaceGrassSprite.setTexture(buildingPlaceGrassTexture);

    if (!buildingPlaceSandTexture.loadFromFile("resources/images/tiles/buildingPlaceSand.png"))
        std::cerr << "buildingPlaceSand.png를 로드할 수 없습니다.\n";
    buildingPlaceSandSprite.setTexture(buildingPlaceSandTexture);

    if (!buildingPlaceSnowTexture.loadFromFile("resources/images/tiles/buildingPlaceSnow.png"))
        std::cerr << "buildingPlaceSnow.png를 로드할 수 없습니다.\n";
    buildingPlaceSnowSprite.setTexture(buildingPlaceSnowTexture);

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
    if (!projectileTexture1.loadFromFile("resources/images/towers/sword.png"))
    {
        std::cerr << "sword.png를 로드할 수 없습니다.\n";
    }

    if (!projectileTexture2.loadFromFile("resources/images/towers/arrow.png"))
    {
        std::cerr << "arrow.png를 로드할 수 없습니다.\n";
    }

    if (!projectileTexture3.loadFromFile("resources/images/towers/wizard_bullet.png"))
    {
        std::cerr << "wizard_bullet.png를 로드할 수 없습니다.\n";
    }

    // Wolf
    if (!WolfTexture[0].loadFromFile("resources/images/units/Wolf1.png"))
        std::cerr << "Wolf1.png를 로드할 수 없습니다.\n";
    if (!WolfTexture[1].loadFromFile("resources/images/units/Wolf2.png"))
        std::cerr << "Wolf2.png를 로드할 수 없습니다.\n";
    if (!WolfTexture[2].loadFromFile("resources/images/units/Wolf3.png"))
        std::cerr << "Wolf3.png를 로드할 수 없습니다.\n";

    WolfSprite.setTexture(WolfTexture[0]);
    WolfSprite.setOrigin(WolfTexture[0].getSize().x / 2.0f, WolfTexture[0].getSize().y * 1.3f - 120);

    // Bat
    if (!BatTexture[0].loadFromFile("resources/images/units/Bat1.png"))
        std::cerr << "Bat1.png를 로드할 수 없습니다.\n";
    if (!BatTexture[1].loadFromFile("resources/images/units/Bat2.png"))
        std::cerr << "Bat2.png를 로드할 수 없습니다.\n";
    if (!BatTexture[2].loadFromFile("resources/images/units/Bat3.png"))
        std::cerr << "Bat3.png를 로드할 수 없습니다.\n";

    BatSprite.setTexture(BatTexture[0]);
    BatSprite.setOrigin(BatTexture[0].getSize().x / 2.0f, BatTexture[0].getSize().y * 1.3f - 240);

    // Bear
    if (!BearTexture[0].loadFromFile("resources/images/units/Bear1.png"))
        std::cerr << "Bear1.png를 로드할 수 없습니다.\n";
    if (!BearTexture[1].loadFromFile("resources/images/units/Bear2.png"))
        std::cerr << "Bear2.png를 로드할 수 없습니다.\n";
    if (!BearTexture[2].loadFromFile("resources/images/units/Bear3.png"))
        std::cerr << "Bear3.png를 로드할 수 없습니다.\n";

    BearSprite.setTexture(BearTexture[0]);
    BearSprite.setOrigin(BearTexture[0].getSize().x / 2.0f, BearTexture[0].getSize().y * 1.3f - 120);

    // BlueScorpion
    if (!BlueScorpionTexture[0].loadFromFile("resources/images/units/BlueScorpion1.png"))
        std::cerr << "BlueScorpion1.png를 로드할 수 없습니다.\n";
    if (!BlueScorpionTexture[1].loadFromFile("resources/images/units/BlueScorpion2.png"))
        std::cerr << "BlueScorpion2.png를 로드할 수 없습니다.\n";
    if (!BlueScorpionTexture[2].loadFromFile("resources/images/units/BlueScorpion3.png"))
        std::cerr << "BlueScorpion3.png를 로드할 수 없습니다.\n";

    BlueScorpionSprite.setTexture(BlueScorpionTexture[0]);
    BlueScorpionSprite.setOrigin(BlueScorpionTexture[0].getSize().x / 2.0f, BlueScorpionTexture[0].getSize().y * 1.3f - 120);

    // Juggernaut
    if (!JuggernautTexture[0].loadFromFile("resources/images/units/Juggernaut1.png"))
        std::cerr << "Juggernaut1.png를 로드할 수 없습니다.\n";
    if (!JuggernautTexture[1].loadFromFile("resources/images/units/Juggernaut2.png"))
        std::cerr << "Juggernaut2.png를 로드할 수 없습니다.\n";
    if (!JuggernautTexture[2].loadFromFile("resources/images/units/Juggernaut3.png"))
        std::cerr << "Juggernaut3.png를 로드할 수 없습니다.\n";

    JuggernautSprite.setTexture(JuggernautTexture[0]);
    JuggernautSprite.setOrigin(JuggernautTexture[0].getSize().x / 2.0f, JuggernautTexture[0].getSize().y * 1.3f - 160);

    // MohawkBat
    if (!MohawkBatTexture[0].loadFromFile("resources/images/units/MohawkBat1.png"))
        std::cerr << "MohawkBat1.png를 로드할 수 없습니다.\n";
    if (!MohawkBatTexture[1].loadFromFile("resources/images/units/MohawkBat2.png"))
        std::cerr << "MohawkBat2.png를 로드할 수 없습니다.\n";
    if (!MohawkBatTexture[2].loadFromFile("resources/images/units/MohawkBat3.png"))
        std::cerr << "MohawkBat3.png를 로드할 수 없습니다.\n";

    MohawkBatSprite.setTexture(MohawkBatTexture[0]);
    MohawkBatSprite.setOrigin(MohawkBatTexture[0].getSize().x / 2.0f, MohawkBatTexture[0].getSize().y * 1.3f - 240);

    // PossessedBear
    if (!PossessedBearTexture[0].loadFromFile("resources/images/units/PossessedBear1.png"))
        std::cerr << "PossesedBear1.png를 로드할 수 없습니다.\n";
    if (!PossessedBearTexture[1].loadFromFile("resources/images/units/PossessedBear2.png"))
        std::cerr << "PossesedBear2.png를 로드할 수 없습니다.\n";
    if (!PossessedBearTexture[2].loadFromFile("resources/images/units/PossessedBear3.png"))
        std::cerr << "PossesedBear3.png를 로드할 수 없습니다.\n";

    PossessedBearSprite.setTexture(PossessedBearTexture[0]);
    PossessedBearSprite.setOrigin(PossessedBearTexture[0].getSize().x / 2.0f, PossessedBearTexture[0].getSize().y * 1.3f - 120);

    // PossessedWolf
    if (!PossessedWolfTexture[0].loadFromFile("resources/images/units/PossessedWolf1.png"))
        std::cerr << "PossesedWolf1.png를 로드할 수 없습니다.\n";
    if (!PossessedWolfTexture[1].loadFromFile("resources/images/units/PossessedWolf2.png"))
        std::cerr << "PossesedWolf2.png를 로드할 수 없습니다.\n";
    if (!PossessedWolfTexture[2].loadFromFile("resources/images/units/PossessedWolf3.png"))
        std::cerr << "PossesedWolf3.png를 로드할 수 없습니다.\n";

    PossessedWolfSprite.setTexture(PossessedWolfTexture[0]);
    PossessedWolfSprite.setOrigin(PossessedWolfTexture[0].getSize().x / 2.0f, PossessedWolfTexture[0].getSize().y * 1.3f - 120);

    // Troll
    if (!TrollTexture[0].loadFromFile("resources/images/units/Troll1.png"))
        std::cerr << "Troll1.png를 로드할 수 없습니다.\n";
    if (!TrollTexture[1].loadFromFile("resources/images/units/Troll2.png"))
        std::cerr << "Troll2.png를 로드할 수 없습니다.\n";
    if (!TrollTexture[2].loadFromFile("resources/images/units/Troll3.png"))
        std::cerr << "Troll3.png를 로드할 수 없습니다.\n";

    TrollSprite.setTexture(TrollTexture[0]);
    TrollSprite.setOrigin(TrollTexture[0].getSize().x / 2.0f, TrollTexture[0].getSize().y * 1.3f - 160);

    // WinterBear
    if (!WinterBearTexture[0].loadFromFile("resources/images/units/WinterBear1.png"))
        std::cerr << "WinterBear1.png를 로드할 수 없습니다.\n";
    if (!WinterBearTexture[1].loadFromFile("resources/images/units/WinterBear2.png"))
        std::cerr << "WinterBear2.png를 로드할 수 없습니다.\n";
    if (!WinterBearTexture[2].loadFromFile("resources/images/units/WinterBear3.png"))
        std::cerr << "WinterBear3.png를 로드할 수 없습니다.\n";

    WinterBearSprite.setTexture(WinterBearTexture[0]);
    WinterBearSprite.setOrigin(WinterBearTexture[0].getSize().x / 2.0f, WinterBearTexture[0].getSize().y * 1.3f - 120);

    // WinterOgre
    if (!WinterOgreTexture[0].loadFromFile("resources/images/units/WinterOgre1.png"))
        std::cerr << "WinterOgre1.png를 로드할 수 없습니다.\n";
    if (!WinterOgreTexture[1].loadFromFile("resources/images/units/WinterOgre2.png"))
        std::cerr << "WinterOgre2.png를 로드할 수 없습니다.\n";
    if (!WinterOgreTexture[2].loadFromFile("resources/images/units/WinterOgre3.png"))
        std::cerr << "WinterOgre3.png를 로드할 수 없습니다.\n";

    WinterOgreSprite.setTexture(WinterOgreTexture[0]);
    WinterOgreSprite.setOrigin(WinterOgreTexture[0].getSize().x / 2.0f, WinterOgreTexture[0].getSize().y * 1.3f - 160);

    // WinterWolf
    if (!WinterWolfTexture[0].loadFromFile("resources/images/units/WinterWolf1.png"))
        std::cerr << "WinterWolf1.png를 로드할 수 없습니다.\n";
    if (!WinterWolfTexture[1].loadFromFile("resources/images/units/WinterWolf2.png"))
        std::cerr << "WinterWolf2.png를 로드할 수 없습니다.\n";
    if (!WinterWolfTexture[2].loadFromFile("resources/images/units/WinterWolf3.png"))
        std::cerr << "WinterWolf3.png를 로드할 수 없습니다.\n";

    WinterWolfSprite.setTexture(WinterWolfTexture[0]);
    WinterWolfSprite.setOrigin(WinterWolfTexture[0].getSize().x / 2.0f, WinterWolfTexture[0].getSize().y * 1.3f - 120);

    unitScale = tileHeight / static_cast<float>(WolfTexture[0].getSize().y) * 1.5;
}

void UI::drawButtons()
{
    // 버튼 생성 및 설정
    gui.setWindow(window);
    // 조작 및 설명 버튼 생성
    auto infoButton = tgui::Button::create();
    infoButton->setSize(70, 70); // 정사각형 크기
    infoButton->setPosition(windowWidth-80, windowHeight - 80); // 오른쪽하단에 조작키
    infoButton->getRenderer()->setBackgroundColor(sf::Color::White); // 배경색 흰색
    infoButton->getRenderer()->setBorders({ 0 }); // 테두리 설정

    // 조작 및 설명 버튼 이미지 설정
    infoButton->getRenderer()->setTexture("resources/images/icons/info_icon.png");

    // 조작 및 설명 버튼 마우스 오버 효과
    infoButton->onMouseEnter([infoButton]() {
        infoButton->getRenderer()->setOpacity(0.7f); // 마우스가 올라왔을 때 70% 불투명
        });
    infoButton->onMouseLeave([infoButton]() {
        infoButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때 100% 불투명
        });

    // 조작 및 설명 버튼 클릭 콜백
    infoButton->onClick([this]() {
        // 조작 및 설명 클릭 시 동작
        gameInfoStage();
        std::cout << "Info button clicked!" << std::endl;
        // 여기서 필요한 행동 추가
        });

    // GUI에 조작 및 설명 버튼 추가
    gui.add(infoButton, "infoButton");

    if (curWaveIsDefense)
    {
        // 버튼 생성 및 설정
        gui.setWindow(window);

        // 타워 버튼 생성
        auto towerButton = tgui::Button::create();
        towerButton->setSize(110, 110); // 정사각형 크기
        towerButton->setPosition(400, windowHeight - 100);
        towerButton->getRenderer()->setBackgroundColor(sf::Color::White); // 배경색 흰색
        towerButton->getRenderer()->setBorders({ 0 }); // 테두리 설정

        // 타워 버튼 이미지 설정
        towerButton->getRenderer()->setTexture("resources/images/icons/yellowcheck.png");

        // 타워 버튼 마우스 오버 효과
        towerButton->onMouseEnter([towerButton]() {
            towerButton->getRenderer()->setOpacity(0.7f); // 마우스가 올라왔을 때 70% 불투명
            });
        towerButton->onMouseLeave([towerButton]() {
            towerButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때 100% 불투명
            });

        // 타워 버튼 클릭 콜백
        towerButton->onClick([this]() {
            if (onTowerButtonClicked) {
                onTowerButtonClicked();
            }
            });

        // GUI에 타워 버튼 추가
        gui.add(towerButton, "towerButton");

       

    }
    else
    {
        gui.setWindow(window);
        tgui::Font font("resources/fonts/BMDOHYEON_ttf.ttf");

       
    }
}

void UI::clearButtons()
{
    gui.removeAllWidgets();
}

void UI::initializeBackground(const std::string& backgroundFilePath) {
    // 텍스처 한 번만 로드
    if (!backgroundTexture.loadFromFile(backgroundFilePath)) {
        std::cerr << "Failed to load background image: " << backgroundFilePath << std::endl;
        return;
    }

    // 텍스처 설정
    backgroundTexture.setSmooth(true); // 텍스처 부드럽게

    // 스프라이트 초기화
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(windowWidth) / backgroundTexture.getSize().x,
        static_cast<float>(windowHeight) / backgroundTexture.getSize().y
    );
}

void UI::drawBackground() {
    // 단순히 이미 로드된 배경 스프라이트를 그리기만 함
    window.draw(backgroundSprite);
}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
    int playerLife, int gold,  int selectedX, int selectedY, const std::vector<Projectile>& projectiles, int currentTick)
{
    window.clear();
    drawBackground();
    int mapType = 0;
    // 맵 그리기
    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x)
        {
            std::string tileType = map[y][x];
            if (x == 0 && y == 0)
            {
                if (tileType == "1")
                    mapType = 1;
                else if (tileType == "2")
                    mapType = 2;
                else if (tileType == "3")
                    mapType = 3;
            }

            // 이소메트릭 좌표 변환
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            screenX += offsetX;
            screenY += offsetY;

            // 타일 스프라이트 선택

            sf::Sprite* tileSprite = nullptr;

            if (mapType == 1)
            {
                if (tileType == "P1" || tileType == "S" || tileType == "D")
                    tileSprite = &grassRoadSprite1;

                else if (tileType == "P2")
                    tileSprite = &grassRoadSprite2;

                else if (tileType == "P3")
                    tileSprite = &grassRoadSprite3;

                else if (tileType == "P4")
                    tileSprite = &grassRoadSprite4;

                else if (tileType == "P5")
                    tileSprite = &grassRoadSprite5;

                else if (tileType == "P6")
                    tileSprite = &grassRoadSprite6;

                else if (tileType == "O")
                    tileSprite = &buildingPlaceGrassSprite;

                else
                    tileSprite = &grassSprite;
            }

            else if (mapType == 2)
            {
                if (tileType == "P1" || tileType == "S" || tileType == "D")
                    tileSprite = &desertRoadSprite1;

                else if (tileType == "P2")
                    tileSprite = &desertRoadSprite2;

                else if (tileType == "P3")
                    tileSprite = &desertRoadSprite3;

                else if (tileType == "P4")
                    tileSprite = &desertRoadSprite4;

                else if (tileType == "P5")
                    tileSprite = &desertRoadSprite5;

                else if (tileType == "P6")
                    tileSprite = &desertRoadSprite6;

                else if (tileType == "O")
                    tileSprite = &buildingPlaceSandSprite;

                else
                    tileSprite = &sandSprite;
            }

            else if (mapType == 3)
            {
                if (tileType == "P1" || tileType == "S" || tileType == "D")
                    tileSprite = &winterRoadSprite1;

                else if (tileType == "P2")
                    tileSprite = &winterRoadSprite2;

                else if (tileType == "P3")
                    tileSprite = &winterRoadSprite3;

                else if (tileType == "P4")
                    tileSprite = &winterRoadSprite4;

                else if (tileType == "P5")
                    tileSprite = &winterRoadSprite5;

                else if (tileType == "P6")
                    tileSprite = &winterRoadSprite6;

                else if (tileType == "O")
                    tileSprite = &buildingPlaceSnowSprite;

                else
                    tileSprite = &snowSprite;
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
        case 100:
            towerSprite = &barrack2TowerSprite; towerSprite->setScale(1.1, 1.1);
            towerOffsetX = 25.0;
            towerOffsetY = 20.0;
            break;
        case 101:
            towerSprite = &barrack3TowerSprite; towerSprite->setScale(1.1, 1.1);
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
        case 200:
            towerSprite = &archer2TowerSprite;
            towerOffsetX = 30.0;
            towerOffsetY = 20.0;
            break;
        case 201:
            towerSprite = &archer3TowerSprite;
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
        case 300:
            towerSprite = &wizard2TowerSprite;
            towerSprite->setScale(1.3, 1.3);
            towerOffsetX = 15.0;
            towerOffsetY = 25.0;
            break;
        case 301:
            towerSprite = &wizard3TowerSprite;
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

    // 투사체 그리기
    drawProjectiles(projectiles);

    // **유닛 그리기 (스프라이트 사용)**
    for (auto& unit : units) {
        float posX = unit.getPosX();
        float posY = unit.getPosY();

        float screenX = (posX - posY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
        float screenY = (posX + posY) * (tileHeight / 2.0f);

        screenX += offsetX;
        screenY += offsetY;

        const Unit* uPtr = &unit; // 이 유닛의 주소를 식별자로 사용
        float oldX;

        auto it = unitOldScreenX.find(uPtr);
        if (it == unitOldScreenX.end()) {
            // 첫 등장인 경우
            // 첫 틱에는 방향 판별 없이 현재 screenX를 oldScreenX로 설정
            unitOldScreenX[uPtr] = screenX;
            oldX = screenX;
        }
        else {
            oldX = it->second;
        }

        // 스크린 좌표 변화를 통한 방향 판단 (epsilon으로 미세변화 무시)
        float deltaX = screenX - oldX;
        float epsilon = 0.1f;
        float scaleX = unitScale;
        float scaleY = unitScale;

        if (std::abs(deltaX) > epsilon) {
            if (deltaX > 0) {
                // 오른쪽 이동
                scaleX = -unitScale;
            }
            else {
                // 왼쪽 이동
                scaleX = unitScale;
            }
        }
        else {
            // 변화량이 미미하다면 이전 방향 유지
            // 이전 틱 scaleX 값을 저장하거나,
            // 첫 등장 시 기본값(왼쪽 바라보기) 유지하는 로직 필요
            // 여기서는 기본이 왼쪽이라면 scaleX 음수를 유지하거나 하는 로직 추가 가능
        }

        // 애니메이션 프레임
        int id = unit.getId();
        int animationFrame = (currentTick) % 2; // 3프레임 애니메이션 사용 (1,2,3번 프레임)

        // 각 유닛에 따른 Sprite, Texture 포인터
        sf::Sprite* currentSprite = nullptr;
        sf::Texture* currentTextures = nullptr;

        // id에 따른 Sprite, Texture 배열 선택
        switch (id) {
        case 1: // Wolf
            currentSprite = &WolfSprite;
            currentTextures = WolfTexture;
            break;
        case 2: // Bat
            currentSprite = &BatSprite;
            currentTextures = BatTexture;
            scaleX *= 2; scaleY *= 2;
            break;
        case 3: // Bear
            currentSprite = &BearSprite;
            currentTextures = BearTexture;
            break;
        case 4: // PossessedWolf
            currentSprite = &PossessedWolfSprite;
            currentTextures = PossessedWolfTexture;
            break;
        case 5: // MohawkBat
            currentSprite = &MohawkBatSprite;
            currentTextures = MohawkBatTexture;
            scaleX *= 2; scaleY *= 2;
            break;
        case 6: // PossessedBear
            currentSprite = &PossessedBearSprite;
            currentTextures = PossessedBearTexture;
            break;
        case 7: // BlueScorpion
            currentSprite = &BlueScorpionSprite;
            currentTextures = BlueScorpionTexture;
            break;
        case 8: // WinterWolf
            currentSprite = &WinterWolfSprite;
            currentTextures = WinterWolfTexture;
            break;
        case 9: // WinterBear
            currentSprite = &WinterBearSprite;
            currentTextures = WinterBearTexture;
            break;
        case 10: // Juggernaut
            currentSprite = &JuggernautSprite;
            currentTextures = JuggernautTexture;
            scaleX *= 2; scaleY *= 2;
            break;
        case 11: // Troll
            currentSprite = &TrollSprite;
            currentTextures = TrollTexture;
            scaleX *= 2; scaleY *= 2;
            break;
        case 12: // WinterOgre
            currentSprite = &WinterOgreSprite;
            currentTextures = WinterOgreTexture;
            scaleX *= 2; scaleY *= 2;
            break;
        default:
            // 정의되지 않은 id일 경우
            currentSprite = &WolfSprite;      // 기본값으로 Wolf로
            currentTextures = WolfTexture;
            break;
        }

        // 현재 유닛의 스프라이트와 텍스처 설정
        currentSprite->setTexture(currentTextures[animationFrame]);
        currentSprite->setScale(scaleX, scaleY);
        currentSprite->setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight);

        // 스프라이트 그리기
        window.draw(*currentSprite);

        // 체력바 그리기
        unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit);

        // 이번 틱 screenX 저장
        unitOldScreenX[uPtr] = screenX;
    }
    TextBox textBox(*this, curWaveIsDefense, { getInfoText(0),getInfoText(1), getInfoText(2)});
        
    textBox.updateState(playerLife, gold, attackGold);
    textBox.draw(window);
    gui.draw();

    window.display();
}


void UI::updateattackGold(int attackgold) {
    attackGold = attackgold;
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
void UI::unitHpBar(sf::RenderWindow& window, float screenX, float screenY, const Unit& unit) {
    // 기본 체력 바 크기
    float healthBarWidth = 40.0f;         // 체력 바 너비
    float healthBarHeight = 5.0f;        // 체력 바 높이
    float yOffset = 10.0f;               // 기본 y축 오프셋

    int currentHp = unit.getHp();
    int maxHp = unit.maxHp;
    float healthRatio = static_cast<float>(currentHp) / maxHp; // 체력 비율

    // ID가 100 초과인 경우 체력바 크기와 위치 조정
    if (unit.getId() >=10) {
        healthBarWidth =65.0f;  // 체력 바 너비 증가
        yOffset = 75.0f;         // 체력 바 위치를 더 높게 설정
    }

    // 체력 바 배경
    sf::RectangleShape healthBarBackground(sf::Vector2f(healthBarWidth, healthBarHeight));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50)); // 회색 배경
    healthBarBackground.setPosition(screenX - healthBarWidth / 2.0f, screenY - yOffset);

    // 체력 바 (현재 체력)
    sf::RectangleShape healthBar(sf::Vector2f(healthBarWidth * healthRatio, healthBarHeight));

    // 체력 비율에 따라 색상 변경
    if (healthRatio > 0.66f) {
        healthBar.setFillColor(sf::Color(0, 200, 0)); // 초록색
    }
    else if (healthRatio > 0.33f) {
        healthBar.setFillColor(sf::Color(255, 255, 0)); // 노란색
    }
    else {
        healthBar.setFillColor(sf::Color(200, 0, 0)); // 빨간색
    }

    healthBar.setPosition(screenX - healthBarWidth / 2.0f, screenY - yOffset);

    // 체력 바 그리기
    window.draw(healthBarBackground);
    window.draw(healthBar);
}


const sf::Texture& UI::getProjectileTexture1() const
{
    return projectileTexture1;
}

const sf::Texture& UI::getProjectileTexture2() const
{
    return projectileTexture2;
}

const sf::Texture& UI::getProjectileTexture3() const
{
    return projectileTexture3;
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

void UI::gameInfoStage() {
    // 창 크기 계산
    sf::Vector2u windowSize = window.getSize();
    float boxWidth = windowSize.x * 2 / 3.0f;
    float boxHeight = windowSize.y * 2 / 3.0f;

    // 패널 생성
    auto panel = tgui::Panel::create({ boxWidth, boxHeight });
    panel->setPosition((windowSize.x - boxWidth) / 2.0f, (windowSize.y - boxHeight) / 2.0f);
    panel->getRenderer()->setBorders({ 10 });                 // 테두리 추가
    panel->getRenderer()->setBorderColor({ 255, 255, 255 });  // 테두리 흰색
    panel->getRenderer()->setRoundedBorderRadius(20);         // 모서리를 둥글게

    // 배경 이미지 설정
    auto backgroundImage = tgui::Picture::create("resources/images/stageimgs/stageInfo.png");
    backgroundImage->setSize("100%", "100%"); // 패널 크기에 맞게 배경 이미지 설정
    panel->add(backgroundImage);

    // 종료 버튼 추가
    auto closeButton = tgui::Button::create("종료");
    closeButton->setSize(70, 70); // 버튼 크기
    closeButton->setPosition((boxWidth - 100), 17); // 패널 오른쪽 상단 배치
    closeButton->getRenderer()->setBorders(0); // 버튼 테두리 제거
    closeButton->getRenderer()->setTexture("resources/images/icons/close.png"); // 종료 버튼 이미지

    // 마우스 오버 효과
    closeButton->onMouseEnter([closeButton]() {
        closeButton->getRenderer()->setOpacity(0.7f); // 마우스가 올라왔을 때 70% 불투명
        });
    closeButton->onMouseLeave([closeButton]() {
        closeButton->getRenderer()->setOpacity(1.0f); // 마우스가 떠났을 때 100% 불투명
        });

    // 버튼 클릭 시 패널 제거
    closeButton->onClick([this, panel]() {
        gui.remove(panel); // 패널 삭제
        });

    panel->add(closeButton);

    // GUI에 패널 추가
    gui.add(panel, "gameInfoPanel");

}