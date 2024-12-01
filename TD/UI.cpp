#include "UI.h"
#include <iostream>
#include <Windows.h>

void UI::initialize(const std::vector<std::vector<std::string>>& gameMap)
{
    map = gameMap;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Tower Defense Game");

    // ��Ʈ �ε�
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cout << "��Ʈ�� �ε��� �� �����ϴ�.\n";
    }

    // �ؽ�Ʈ ����
    lifeText.setFont(font);
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition(10, 10);

    goldText.setFont(font);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(10, 40);

    

    // Ÿ�� �ؽ�ó �ε�
    if (!roadTexture.loadFromFile("resources/images/tiles/road_spring(1).png"))
        std::cerr << "road_spring(1).png�� �ε��� �� �����ϴ�.\n";
    roadSprite.setTexture(roadTexture);

    if (!grassTexture.loadFromFile("resources/images/tiles/grass.png"))
        std::cerr << "grass.png�� �ε��� �� �����ϴ�.\n";
    grassSprite.setTexture(grassTexture);

    if (!buildingPlaceGrassTexture.loadFromFile("resources/images/tiles/buildingPlaceGrass.png"))
        std::cerr << "buildingPlaceGrass.png�� �ε��� �� �����ϴ�.\n";
    buildingPlaceGrassSprite.setTexture(buildingPlaceGrassTexture);

    // Ÿ�� �ؽ�ó �ε�
    if (!archer1TowerTexture.loadFromFile("resources/images/towers/archer_level_1.png"))
        std::cerr << "archer_level_1.png�� �ε��� �� �����ϴ�.\n";
    archer1TowerSprite.setTexture(archer1TowerTexture);

    if (!archer2TowerTexture.loadFromFile("resources/images/towers/archer_level_2.png"))
        std::cerr << "archer_level_2.png�� �ε��� �� �����ϴ�.\n";
    archer2TowerSprite.setTexture(archer2TowerTexture);

    if (!archer3TowerTexture.loadFromFile("resources/images/towers/archer_level_3.png"))
        std::cerr << "archer_level_3.png�� �ε��� �� �����ϴ�.\n";
    archer3TowerSprite.setTexture(archer3TowerTexture);

    if (!wizard1TowerTexture.loadFromFile("resources/images/towers/wizard_level_1.png"))
        std::cerr << "wizard_level_1.png�� �ε��� �� �����ϴ�.\n";
    wizard1TowerSprite.setTexture(wizard1TowerTexture);

    if (!wizard2TowerTexture.loadFromFile("resources/images/towers/wizard_level_2.png"))
        std::cerr << "wizard_level_2.png�� �ε��� �� �����ϴ�.\n";
    wizard2TowerSprite.setTexture(wizard2TowerTexture);

    if (!wizard3TowerTexture.loadFromFile("resources/images/towers/wizard_level_3.png"))
        std::cerr << "wizard_level_3.png�� �ε��� �� �����ϴ�.\n";
    wizard3TowerSprite.setTexture(wizard3TowerTexture);

    if (!barrack1TowerTexture.loadFromFile("resources/images/towers/barrack_level_1.png"))
        std::cerr << "barrack_level_1.png�� �ε��� �� �����ϴ�.\n";
    barrack1TowerSprite.setTexture(barrack1TowerTexture);

    if (!barrack2TowerTexture.loadFromFile("resources/images/towers/barrack_level_2.png"))
        std::cerr << "barrack_level_2.png�� �ε��� �� �����ϴ�.\n";
    barrack2TowerSprite.setTexture(barrack2TowerTexture);

    if (!barrack3TowerTexture.loadFromFile("resources/images/towers/barrack_level_3.png"))
        std::cerr << "barrack_level_3.png�� �ε��� �� �����ϴ�.\n";
    barrack3TowerSprite.setTexture(barrack3TowerTexture);

    // ����ü �ؽ�ó �ε�
    if (!projectileTexture.loadFromFile("resources/images/towers/arrow.png"))
    {
        std::cerr << "arrow.png�� �ε��� �� �����ϴ�.\n";
    }

    // **���� �ؽ�ó �ε�**
    if (!knightUnitTexture.loadFromFile("resources/images/units/knight_level_1.png"))
        std::cerr << "knight_level_1.png�� �ε��� �� �����ϴ�.\n";
    knightUnitSprite.setTexture(knightUnitTexture);

    // ���� ��������Ʈ ���� (�ʿ信 ���� ������ ũ�� ����)
    knightUnitSprite.setOrigin(knightUnitTexture.getSize().x / 2.0f, knightUnitTexture.getSize().y * 1.3);
    float unitScale = tileHeight / knightUnitTexture.getSize().y;
    knightUnitSprite.setScale(unitScale/1.5, unitScale/1.5);

    drawButtons();
}

void UI::drawButtons()
{
    // ��ư ���� �� ����
    gui.setWindow(window);
    // ���� �� ���� ��ư ����
    auto infoButton = tgui::Button::create();
    infoButton->setSize(70, 70); // ���簢�� ũ��
    infoButton->setPosition(windowWidth-80, windowHeight - 80); // �������ϴܿ� ����Ű
    infoButton->getRenderer()->setBackgroundColor(sf::Color::White); // ���� ���
    infoButton->getRenderer()->setBorders({ 0 }); // �׵θ� ����

    // ���� �� ���� ��ư �̹��� ����
    infoButton->getRenderer()->setTexture("resources/images/icons/info_icon.png");

    // ���� �� ���� ��ư ���콺 ���� ȿ��
    infoButton->onMouseEnter([infoButton]() {
        infoButton->getRenderer()->setOpacity(0.7f); // ���콺�� �ö���� �� 70% ������
        });
    infoButton->onMouseLeave([infoButton]() {
        infoButton->getRenderer()->setOpacity(1.0f); // ���콺�� ������ �� 100% ������
        });

    // ���� �� ���� ��ư Ŭ�� �ݹ�
    infoButton->onClick([this]() {
        // ���� �� ���� Ŭ�� �� ����
        gameInfoStage();
        std::cout << "Info button clicked!" << std::endl;
        // ���⼭ �ʿ��� �ൿ �߰�
        });

    // GUI�� ���� �� ���� ��ư �߰�
    gui.add(infoButton, "infoButton");

    if (curWaveIsDefense)
    {
        // ��ư ���� �� ����
        gui.setWindow(window);

        // Ÿ�� ��ư ����
        auto towerButton = tgui::Button::create();
        towerButton->setSize(110, 110); // ���簢�� ũ��
        towerButton->setPosition(400, windowHeight - 100);
        towerButton->getRenderer()->setBackgroundColor(sf::Color::White); // ���� ���
        towerButton->getRenderer()->setBorders({ 0 }); // �׵θ� ����

        // Ÿ�� ��ư �̹��� ����
        towerButton->getRenderer()->setTexture("resources/images/icons/yellowcheck.png");

        // Ÿ�� ��ư ���콺 ���� ȿ��
        towerButton->onMouseEnter([towerButton]() {
            towerButton->getRenderer()->setOpacity(0.7f); // ���콺�� �ö���� �� 70% ������
            });
        towerButton->onMouseLeave([towerButton]() {
            towerButton->getRenderer()->setOpacity(1.0f); // ���콺�� ������ �� 100% ������
            });

        // Ÿ�� ��ư Ŭ�� �ݹ�
        towerButton->onClick([this]() {
            if (onTowerButtonClicked) {
                onTowerButtonClicked();
            }
            });

        // GUI�� Ÿ�� ��ư �߰�
        gui.add(towerButton, "towerButton");

       

    }
    else
    {
        gui.setWindow(window);
        tgui::Font font("resources/fonts/BMDOHYEON_ttf.ttf");

        auto firstUnitButton = tgui::Button::create("A");
        firstUnitButton->setSize(80, 70);
        firstUnitButton->setPosition(windowWidth - 200, 380);
        firstUnitButton->getRenderer()->setRoundedBorderRadius(30); // �ձ� �𼭸�
        firstUnitButton->getRenderer()->setBackgroundColor(sf::Color::White); // ��� ���
        firstUnitButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // �Ķ��� �ؽ�Ʈ
        firstUnitButton->setTextSize(26); //�۾� ũ�� ����
        firstUnitButton->getRenderer()->setFont(font); // ��Ʈ ����

        auto secondUnitButton = tgui::Button::create("B");
        secondUnitButton->setSize(80, 70);
        secondUnitButton->setPosition(windowWidth - 300, 380);
        secondUnitButton->getRenderer()->setRoundedBorderRadius(30); // �ձ� �𼭸�
        secondUnitButton->getRenderer()->setBackgroundColor(sf::Color::White); // ��� ���
        secondUnitButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // �Ķ��� �ؽ�Ʈ
        secondUnitButton->setTextSize(26); //�۾� ũ�� ����
        secondUnitButton->getRenderer()->setFont(font); // ��Ʈ ����

        auto thirdUnitButton = tgui::Button::create("C");
        thirdUnitButton->setSize(80, 70);
        thirdUnitButton->setPosition(windowWidth - 400, 380);
        thirdUnitButton->getRenderer()->setRoundedBorderRadius(30); // �ձ� �𼭸�
        thirdUnitButton->getRenderer()->setBackgroundColor(sf::Color::White); // ��� ���
        thirdUnitButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // �Ķ��� �ؽ�Ʈ
        thirdUnitButton->setTextSize(26); //�۾� ũ�� ����
        thirdUnitButton->getRenderer()->setFont(font); // ��Ʈ ����

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

    // �� �׸���
    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x)
        {
            std::string tileType = map[y][x];

            // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            screenX += offsetX;
            screenY += offsetY;

            // Ÿ�� ��������Ʈ ����
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

            // ��������Ʈ ��ġ ����
            tileSprite->setPosition(screenX, screenY);

            window.draw(*tileSprite);

            // ���õ� Ÿ���̸� �������� �������̸� �׷��� ���� ǥ��
            if (x == selectedX && y == selectedY)
            {
                sf::ConvexShape overlay;
                overlay.setPointCount(4);
                overlay.setPoint(0, sf::Vector2f(tileWidth / 2.0f, 0));
                overlay.setPoint(1, sf::Vector2f(tileWidth, tileHeight / 2.0f));
                overlay.setPoint(2, sf::Vector2f(tileWidth / 2.0f, tileHeight));
                overlay.setPoint(3, sf::Vector2f(0, tileHeight / 2.0f));
                overlay.setPosition(screenX, screenY);
                overlay.setFillColor(sf::Color(255, 255, 0, 100)); // �����, ���� 100
                window.draw(overlay);
            }
        }
    }
 
    // ����ü �׸���
    drawProjectiles(projectiles);

    // **���� �׸��� (��������Ʈ ���)**
    for (auto& unit : units) {
        int tileX = unit.getX();
        int tileY = unit.getY();

        // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        screenX += offsetX;
        screenY += offsetY;

        // ���� ��������Ʈ ��ġ ����
        knightUnitSprite.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight);

        // ü�� �� �׸���
        unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit.getHp(), 20); // ü�� �ִ밪 10���� ����

        // ���� ��������Ʈ �׸���
        window.draw(knightUnitSprite);
    }

    // Ÿ�� �׸���
    for (const auto& tower : placedTowers)
    {
        int tileX = tower.getX();
        int tileY = tower.getY();

        // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
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


// ü�¹ٸ� �����ϴ� �Լ�
void UI::unitHpBar(sf::RenderWindow& window, float screenX, float screenY, int currentHp, int maxHp) {
    float healthBarWidth = 40.0f;         // ü�� �� �ʺ�
    float healthBarHeight = 5.0f;        // ü�� �� ����
    float healthRatio = static_cast<float>(currentHp) / maxHp; // ü�� ����

    // ü�� �� ���
    sf::RectangleShape healthBarBackground(sf::Vector2f(healthBarWidth, healthBarHeight));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50)); // ȸ�� ���
    healthBarBackground.setPosition(screenX - healthBarWidth / 2.0f, screenY - 10);

    // ü�� �� (���� ü��)
    sf::RectangleShape healthBar(sf::Vector2f(healthBarWidth * healthRatio, healthBarHeight));
    healthBar.setFillColor(sf::Color(200, 0, 0)); // ������
    healthBar.setPosition(screenX - healthBarWidth / 2.0f, screenY - 10);

    // ü�� �� �׸���
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

        // ���� ��������Ʈ�� ��ġ�� �����ɴϴ�.
        sf::Vector2f position = sprite.getPosition();

        // offsetX�� offsetY�� �����ݴϴ�.
        position.x += offsetX;
        position.y += offsetY;

        // ��ġ�� ������Ʈ�մϴ�.
        sprite.setPosition(position);

        // ��������Ʈ�� �׸��ϴ�.
        window.draw(sprite);
    }
}

void UI::setInfoText(std::initializer_list<std::string> infoSentences) {
    // ���� infoTexts ���� �ʱ�ȭ
    infoTexts.clear();

    // ������ ���� (�ʿ��� ���)
    // std::locale::global(std::locale(""));

    for (const auto& sentence : infoSentences) {
        // ���� �Է� ���ڿ��� UTF-8�� �ƴ϶�� CP_ACP ���
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
    // ��Ʈ ���� ��� ����
    std::string fontPath = "resources/fonts/BMDOHYEON_ttf.ttf";

    // ��Ʈ �ε� Ȯ��
    if (!sf::Font().loadFromFile(fontPath)) {
        std::cerr << "��Ʈ ������ �ε��� �� �����ϴ�: " << fontPath << std::endl;
        return;
    }

    // â ũ�� ���
    sf::Vector2u windowSize = window.getSize();
    float boxWidth = windowSize.x * 2 / 3.0f;
    float boxHeight = windowSize.y * 2 / 3.0f;

    // ������ �г� ����
    auto panel = tgui::Panel::create({ boxWidth, boxHeight });
    panel->setPosition((windowSize.x - boxWidth) / 2.0f, (windowSize.y - boxHeight) / 2.0f);
    panel->getRenderer()->setBackgroundColor({ 0, 0, 0, 150 }); // ������ ���, 150 ����
    panel->getRenderer()->setBorders({ 10 });                  // �׵θ� �߰�
    panel->getRenderer()->setBorderColor({ 255, 255, 255 });   // �׵θ� ���
    panel->getRenderer()->setRoundedBorderRadius(20);          // �𼭸��� �ձ۰�

    // ���� ���̺� ���� �ؽ�Ʈ �߰� (����, 1/4 ����)
    std::wstring defenseWaveText =
        L"[���� ���̺�]\n\n";
    auto defenseText = tgui::Label::create();
    defenseText->setText(defenseWaveText); // �ؽ�Ʈ ����
    defenseText->setPosition(boxWidth / 4 - 100, 50); // �г��� 1/4 ������ ��ġ
    defenseText->setTextSize(25);
    defenseText->getRenderer()->setTextColor({ 135, 206, 250 }); // ���� �Ķ��� (LightSkyBlue)
    defenseText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(defenseText);

    // ���� ���̺� ���� �ؽ�Ʈ �߰� (������, 3/4 ����)
    std::wstring attackWaveText =
        L"[���� ���̺�]\n\n";
    auto attackText = tgui::Label::create();
    attackText->setText(attackWaveText);  // �ؽ�Ʈ ����
    attackText->setPosition(boxWidth * 3 / 4 - 100, 50); // �г��� 3/4 ������ ��ġ
    attackText->setTextSize(25);
    attackText->getRenderer()->setTextColor({ 255, 105, 180 }); // ��ũ�� ��Ʈ
    attackText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(attackText);


    // �׸� �߰� (�߾�)
  

    auto wasdkey = tgui::Picture::create("resources/images/icons/wasdkey.png"); // �̹��� ���� ���
    wasdkey->setSize({ 240, 160 });  // �׸� ũ��
    wasdkey->setPosition((boxWidth - 200) / 2-40, 20); // �г� �߾ӿ� ��ġ
    panel->add(wasdkey);

    auto spacekey = tgui::Picture::create("resources/images/icons/spacekey.png"); // �̹��� ���� ���
    spacekey->setSize({ 150, 150 });  // �׸� ũ��
    spacekey->setPosition((boxWidth - 200) / 2 +90, 150); // �г� �߾ӿ� ��ġ
    panel->add(spacekey);

    auto fkey = tgui::Picture::create("resources/images/icons/fkey.png"); // �̹��� ���� ���
    fkey->setSize({ 120, 120 });  // �׸� ũ��
    fkey->setPosition((boxWidth - 200) / 2, 270); // �г� �߾ӿ� ��ġ
    panel->add(fkey);
    
    auto onekey = tgui::Picture::create("resources/images/icons/1key.png"); // �̹��� ���� ���
    onekey->setSize({ 120, 120 });  // �׸� ũ��
    onekey->setPosition((boxWidth - 200) / 2 -200, 370); // �г� �߾ӿ� ��ġ
    panel->add(onekey);
  
    auto twokey = tgui::Picture::create("resources/images/icons/2key.png"); // �̹��� ���� ���
    twokey->setSize({ 120, 120 });  // �׸� ũ��
    twokey->setPosition((boxWidth - 200) / 2 - 120, 370); // �г� �߾ӿ� ��ġ
    panel->add(twokey);

    auto threekey = tgui::Picture::create("resources/images/icons/three.png"); // �̹��� ���� ���
    threekey->setSize({ 600, 120 });  // �׸� ũ��
    threekey->setPosition((boxWidth - 200) / 2 -300, 470); // �г� �߾ӿ� ��ġ
    panel->add(threekey);

    auto esckey = tgui::Picture::create("resources/images/icons/esckey.png"); // �̹��� ���� ���
    esckey->setSize({ 120, 120 });  // �׸� ũ��
    esckey->setPosition((boxWidth - 200) / 2+20 ,570 ); // �г� �߾ӿ� ��ġ
    panel->add(esckey);

    // WASD ���� "�� �̵�Ű" �߰�
    auto wasdText = tgui::Label::create();
    wasdText->setText(L"�� �̵�Ű");
    wasdText->setTextSize(25);
    wasdText->getRenderer()->setTextColor({ 255, 255, 255 }); // ��� �ؽ�Ʈ
    wasdText->setPosition((boxWidth - 200) / 2 + 200, 100); // WASD �Ʒ��� ��ġ
    wasdText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(wasdText);

    // Space Ű ���� "Ÿ�� ��ġ Ű" �߰�
    auto spaceText = tgui::Label::create();
    spaceText->setText(L"Ÿ�� ��ġ Ű");
    spaceText->setTextSize(25);
    spaceText->getRenderer()->setTextColor({ 135, 206, 250 }); // ���� �Ķ��� �ؽ�Ʈ
    spaceText->setPosition((boxWidth - 200) / 2 - 70, 200); // SpaceKey ����
    spaceText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(spaceText);

    // F Ű ���� "���� ���̺� ���� Ű" �߰�
    auto fText = tgui::Label::create();
    fText->setText(L"���� ���̺� ���� Ű");
    fText->setTextSize(25);
    fText->getRenderer()->setTextColor({ 135, 206, 250 }); // ���� �Ķ��� �ؽ�Ʈ
    fText->setPosition((boxWidth - 200) / 2 - 240, 300); // FKey ����
    fText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(fText);

    // 1�� Ű ���� "Ÿ�� ���� Ű" �߰�
    auto oneText = tgui::Label::create();
    oneText->setText(L"     <1,2,3��> Ÿ�� ���� Ű\n\n  [Ÿ�� ��ġ ��]\n <1,2��> ���׷��̵� Ű \n <3��> Ÿ�� �Ǹ�Ű");
    oneText->setTextSize(25);
    oneText->getRenderer()->setTextColor({ 135, 206, 250 }); // ���� �Ķ��� �ؽ�Ʈ
    oneText->setPosition((boxWidth - 200) / 2 - 540, 400); // 1�� Ű ����
    oneText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(oneText);

    // ���� ���̺� ���� ���� Ű (Three Key ������)
    auto threeText = tgui::Label::create();
    threeText->setText(L"                <1~9��>          \n���� ���̺� �� ���� ���� �� ���� Ű");
    threeText->setTextSize(25);
    threeText->getRenderer()->setTextColor({ 255, 105, 180 }); // ���� ������ �ؽ�Ʈ
    threeText->setPosition((boxWidth - 200) / 2 + 290, 470); // ThreeKey ������
    threeText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(threeText);

    // ESC Ű ���� "���� ��� Ű" �߰�
    auto escText = tgui::Label::create();
    escText->setText(L"���� ��� Ű");
    escText->setTextSize(25);
    escText->getRenderer()->setTextColor({ 255, 255, 255 }); // ��� �ؽ�Ʈ
    escText->setPosition((boxWidth - 200) / 2 + 150, 600); // ESC ������
    escText->getRenderer()->setFont(tgui::Font(fontPath));
    panel->add(escText);

    // ���� ��ư �߰� (�Ʒ� �߾�)
    auto closeButton = tgui::Button::create("����");
    closeButton->setSize(70, 70); // ��ư ũ��
    closeButton->setPosition((boxWidth-100), 17); // �г� ������ ��� ��ġ
    closeButton->getRenderer()->setBorders(0);
    // close ��ư �̹��� ����
    closeButton->getRenderer()->setTexture("resources/images/icons/close.png");

    //close ��ư ���콺 ���� ȿ��
    closeButton->onMouseEnter([closeButton]() {
        closeButton->getRenderer()->setOpacity(0.7f); // ���콺�� �ö���� �� 70% ������
        });
    closeButton->onMouseLeave([closeButton]() {
        closeButton->getRenderer()->setOpacity(1.0f); // ���콺�� ������ �� 100% ������
        });

    // ��ư Ŭ�� �� �г� ����
    closeButton->onClick([this, panel]() {
        gui.remove(panel); // �г� ����
        });

    panel->add(closeButton);

    // GUI�� �г� �߰�
    gui.add(panel, "gameInfoPanel");
}
