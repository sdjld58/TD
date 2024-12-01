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
    int playerLife, int gold,  int selectedX, int selectedY, const std::vector<Projectile>& projectiles)
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
        unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit.getHp(), 20); // 체력 최대값 10으로 설정

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

void UI::gameInfoStage() {
    // 폰트 파일 경로 설정
    std::string fontPath = "resources/fonts/BMDOHYEON_ttf.ttf";

    // 폰트 로드 확인
    if (!sf::Font().loadFromFile(fontPath)) {
        std::cerr << "폰트 파일을 로드할 수 없습니다: " << fontPath << std::endl;
        return;
    }

    // 창 크기 계산
    sf::Vector2u windowSize = window.getSize();
    float boxWidth = windowSize.x * 2 / 3.0f;
    float boxHeight = windowSize.y * 2 / 3.0f;

    // 반투명 패널 생성
    auto panel = tgui::Panel::create({ boxWidth, boxHeight });
    panel->setPosition((windowSize.x - boxWidth) / 2.0f, (windowSize.y - boxHeight) / 2.0f);
    panel->getRenderer()->setBackgroundColor({ 0, 0, 0, 150 }); // 검은색 배경, 150 투명도
    panel->getRenderer()->setBorders({ 10 });                  // 테두리 추가
    panel->getRenderer()->setBorderColor({ 255, 255, 255 });   // 테두리 흰색
    panel->getRenderer()->setRoundedBorderRadius(20);          // 모서리를 둥글게

    // 수비 웨이브 설명 텍스트 추가 (왼쪽, 1/4 지점)
    std::wstring defenseWaveText =
        L"[수비 웨이브]\n\n";
    auto defenseText = tgui::Label::create();
    defenseText->setText(defenseWaveText); // 텍스트 설정
    defenseText->setPosition(boxWidth / 4 - 100, 50); // 패널의 1/4 지점에 배치
    defenseText->setTextSize(25);
    defenseText->getRenderer()->setTextColor({ 135, 206, 250 }); // 밝은 파란색 (LightSkyBlue)
    defenseText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(defenseText);

    // 공격 웨이브 설명 텍스트 추가 (오른쪽, 3/4 지점)
    std::wstring attackWaveText =
        L"[공격 웨이브]\n\n";
    auto attackText = tgui::Label::create();
    attackText->setText(attackWaveText);  // 텍스트 설정
    attackText->setPosition(boxWidth * 3 / 4 - 100, 50); // 패널의 3/4 지점에 배치
    attackText->setTextSize(25);
    attackText->getRenderer()->setTextColor({ 255, 105, 180 }); // 핑크색 폰트
    attackText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(attackText);


    // 그림 추가 (중앙)
  

    auto wasdkey = tgui::Picture::create("resources/images/icons/wasdkey.png"); // 이미지 파일 경로
    wasdkey->setSize({ 240, 160 });  // 그림 크기
    wasdkey->setPosition((boxWidth - 200) / 2-40, 20); // 패널 중앙에 배치
    panel->add(wasdkey);

    auto spacekey = tgui::Picture::create("resources/images/icons/spacekey.png"); // 이미지 파일 경로
    spacekey->setSize({ 150, 150 });  // 그림 크기
    spacekey->setPosition((boxWidth - 200) / 2 +90, 150); // 패널 중앙에 배치
    panel->add(spacekey);

    auto fkey = tgui::Picture::create("resources/images/icons/fkey.png"); // 이미지 파일 경로
    fkey->setSize({ 120, 120 });  // 그림 크기
    fkey->setPosition((boxWidth - 200) / 2, 270); // 패널 중앙에 배치
    panel->add(fkey);
    
    auto onekey = tgui::Picture::create("resources/images/icons/1key.png"); // 이미지 파일 경로
    onekey->setSize({ 120, 120 });  // 그림 크기
    onekey->setPosition((boxWidth - 200) / 2 -200, 370); // 패널 중앙에 배치
    panel->add(onekey);
  
    auto twokey = tgui::Picture::create("resources/images/icons/2key.png"); // 이미지 파일 경로
    twokey->setSize({ 120, 120 });  // 그림 크기
    twokey->setPosition((boxWidth - 200) / 2 - 120, 370); // 패널 중앙에 배치
    panel->add(twokey);

    auto threekey = tgui::Picture::create("resources/images/icons/three.png"); // 이미지 파일 경로
    threekey->setSize({ 600, 120 });  // 그림 크기
    threekey->setPosition((boxWidth - 200) / 2 -300, 470); // 패널 중앙에 배치
    panel->add(threekey);

    auto esckey = tgui::Picture::create("resources/images/icons/esckey.png"); // 이미지 파일 경로
    esckey->setSize({ 120, 120 });  // 그림 크기
    esckey->setPosition((boxWidth - 200) / 2+20 ,570 ); // 패널 중앙에 배치
    panel->add(esckey);

    // WASD 옆에 "맵 이동키" 추가
    auto wasdText = tgui::Label::create();
    wasdText->setText(L"맵 이동키");
    wasdText->setTextSize(25);
    wasdText->getRenderer()->setTextColor({ 255, 255, 255 }); // 흰색 텍스트
    wasdText->setPosition((boxWidth - 200) / 2 + 200, 100); // WASD 아래에 배치
    wasdText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(wasdText);

    // Space 키 옆에 "타워 설치 키" 추가
    auto spaceText = tgui::Label::create();
    spaceText->setText(L"타워 설치 키");
    spaceText->setTextSize(25);
    spaceText->getRenderer()->setTextColor({ 135, 206, 250 }); // 형광 파란색 텍스트
    spaceText->setPosition((boxWidth - 200) / 2 - 70, 200); // SpaceKey 왼쪽
    spaceText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(spaceText);

    // F 키 옆에 "수비 웨이브 시작 키" 추가
    auto fText = tgui::Label::create();
    fText->setText(L"수비 웨이브 시작 키");
    fText->setTextSize(25);
    fText->getRenderer()->setTextColor({ 135, 206, 250 }); // 형광 파란색 텍스트
    fText->setPosition((boxWidth - 200) / 2 - 240, 300); // FKey 왼쪽
    fText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(fText);

    // 1번 키 옆에 "타워 선택 키" 추가
    auto oneText = tgui::Label::create();
    oneText->setText(L"     <1,2,3번> 타워 선택 키\n\n  [타워 설치 시]\n <1,2번> 업그레이드 키 \n <3번> 타워 판매키");
    oneText->setTextSize(25);
    oneText->getRenderer()->setTextColor({ 135, 206, 250 }); // 형광 파란색 텍스트
    oneText->setPosition((boxWidth - 200) / 2 - 540, 400); // 1번 키 왼쪽
    oneText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(oneText);

    // 공격 웨이브 유닛 선택 키 (Three Key 오른쪽)
    auto threeText = tgui::Label::create();
    threeText->setText(L"                <1~9번>          \n공격 웨이브 시 유닛 선택 및 공격 키");
    threeText->setTextSize(25);
    threeText->getRenderer()->setTextColor({ 255, 105, 180 }); // 형광 빨간색 텍스트
    threeText->setPosition((boxWidth - 200) / 2 + 290, 470); // ThreeKey 오른쪽
    threeText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(threeText);

    // ESC 키 옆에 "실행 취소 키" 추가
    auto escText = tgui::Label::create();
    escText->setText(L"실행 취소 키");
    escText->setTextSize(25);
    escText->getRenderer()->setTextColor({ 255, 255, 255 }); // 흰색 텍스트
    escText->setPosition((boxWidth - 200) / 2 + 150, 600); // ESC 오른쪽
    escText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(escText);

    // 종료 버튼 추가 (아래 중앙)
    auto closeButton = tgui::Button::create("종료");
    closeButton->setSize(70, 70); // 버튼 크기
    closeButton->setPosition((boxWidth-100), 17); // 패널 오른쪽 상단 배치
    closeButton->getRenderer()->setBorders(0);
    // close 버튼 이미지 설정
    closeButton->getRenderer()->setTexture("resources/images/icons/close.png");

    //close 버튼 마우스 오버 효과
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
