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
    if (curWaveIsDefense)
    {
        //��ư �׸���
        gui.setWindow(window);
        tgui::Font font("resources/fonts/BMDOHYEON_ttf.ttf");
        auto towerButton = tgui::Button::create("Tower");
        towerButton->setSize(120, 70);
        towerButton->setPosition(25, windowHeight - 380);
        towerButton->getRenderer()->setRoundedBorderRadius(30); // �ձ� �𼭸�
        towerButton->getRenderer()->setBackgroundColor(sf::Color::White); // ��� ���
        towerButton->getRenderer()->setTextColor(sf::Color(0, 120, 215)); // �Ķ��� �ؽ�Ʈ
        towerButton->setTextSize(26); //�۾� ũ�� ����
        towerButton->getRenderer()->setFont(font); // ��Ʈ ����




        // ��ư�� �ݹ� �Լ� ����
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
    int playerLife, int gold, int selectedX, int selectedY, const std::vector<Projectile>& projectiles)
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
        unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit.getHp(), 20); // ü�� �ִ밪 20���� ����

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
        textBox.draw(window);
    
    // �÷��̾� ������ �� ��� ǥ��
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