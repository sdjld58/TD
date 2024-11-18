#include "UI.h"
#include <iostream>

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
    if (!archerTowerTexture.loadFromFile("resources/images/towers/archer_level_1.png"))
        std::cerr << "archer_level_1.png�� �ε��� �� �����ϴ�.\n";
    archerTowerSprite.setTexture(archerTowerTexture);

    // **���� �ؽ�ó �ε�**
    if (!knightUnitTexture.loadFromFile("resources/images/units/knight_level_1.png"))
        std::cerr << "knight_level_1.png�� �ε��� �� �����ϴ�.\n";
    knightUnitSprite.setTexture(knightUnitTexture);

    // ���� ��������Ʈ ���� (�ʿ信 ���� ������ ũ�� ����)
    knightUnitSprite.setOrigin(knightUnitTexture.getSize().x / 2.0f, knightUnitTexture.getSize().y * 1.3);
    float unitScale = tileHeight / knightUnitTexture.getSize().y;
    knightUnitSprite.setScale(unitScale/1.5, unitScale/1.5);

    //��ư �׸���
    gui.setWindow(window);

    tgui::Button::Ptr towerButton = tgui::Button::create("tower");
    towerButton->setSize(100, 50); // ��ư ũ�� ����
    towerButton->setPosition(0, windowHeight - 50); // ��ư ��ġ ����
   
    // ��ư�� �ݹ� �Լ� ����
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

    // **���� �׸��� (��������Ʈ ���)**
    for (const auto& unit : units)
    {
        int tileX = unit.getX();
        int tileY = unit.getY();

        // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        screenX += offsetX;
        screenY += offsetY;

        // ���� ��������Ʈ ��ġ ����
        knightUnitSprite.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight);

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

        screenX += offsetX;
        screenY += offsetY;

        // Ÿ�� ��������Ʈ ��ġ ����
        float towerOffsetX = 30.0;
        float towerOffsetY = -20.0;
        archerTowerSprite.setPosition(screenX + towerOffsetX, screenY + towerOffsetY);

        window.draw(archerTowerSprite);
    }

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
