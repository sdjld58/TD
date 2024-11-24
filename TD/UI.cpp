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
    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            std::string tileType = map[y][x];

            // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            screenX += offsetX;
            screenY += offsetY;

            // Ÿ�� ��������Ʈ ����
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

            // ��������Ʈ ��ġ ����
            tileSprite->setPosition(screenX, screenY);

            window.draw(*tileSprite);

            // ���õ� Ÿ���̸� �������� �������̸� �׷��� ���� ǥ��
            if (x == selectedX && y == selectedY) {
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

    // **Ÿ���� ���� �� ���� ���� Ȯ��**
    for (const auto& tower : placedTowers) {
        for (const auto& unit : units) {
            // Ÿ���� ���� �Ÿ� Ȯ��
            if (tower.isUnitInRange(unit.getX(), unit.getY(), tower.getX(), tower.getY())) {
                // ���� ���� ���� ������ drawBullet ȣ��
                drawBullet(window, tower, unit, placedTowers, units);
            }
        }
    }

    // **Ÿ�� �׸��� (drawTower �Լ� ȣ��)**
    for (const auto& tower : placedTowers) {
        drawTower(window, tower, offsetX, offsetY);
    }

    // **���� �׸��� (drawUnit �Լ� ȣ��)**
    for (const auto& unit : units) {
        drawUnit(window, unit, offsetX, offsetY);
    }

    // �÷��̾� ������ �� ��� ǥ��
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

    // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
    float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f;
    float screenY = (tileX + tileY) * (tileHeight / 2.0f);

    screenX += offsetX;
    screenY += offsetY;

    // ���� ��������Ʈ ��ġ ����
    knightUnitSprite.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight);

    // ü�� �� �׸���
    unitHpBar(window, screenX + tileWidth / 2.0f, screenY, unit.getHp(), 10); // ü�� �ִ밪 10���� ����

    // ���� ��������Ʈ �׸���
    window.draw(knightUnitSprite);
}
void UI::drawTower(sf::RenderWindow& window, const PlacedTower& tower, float offsetX, float offsetY) {
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

    // Ÿ�� ��������Ʈ �׸���
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

void UI::drawBullet(sf::RenderWindow& window, const PlacedTower& tower, const Unit& unit,
    const std::vector<PlacedTower>& placedTowers, const std::vector<Unit>& activeUnits) {
    sf::CircleShape bullet(5.f); // �Ѿ� ũ��
    bullet.setFillColor(sf::Color(255, 255, 0)); // ���� ���

    float offsetX = -100.0f;
    float offsetY = 200.0f;

    // Ÿ���� �̼Ҹ�Ʈ�� ȭ�� ��ǥ ���
    sf::Vector2f towerScreenPos(
        (tower.getX() - tower.getY()) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f + offsetX,
        (tower.getX() + tower.getY()) * (tileHeight / 2.0f) + offsetY
    );

    // ������ �̼Ҹ�Ʈ�� ȭ�� ��ǥ ���
    sf::Vector2f unitScreenPos(
        (unit.getX() - unit.getY()) * (tileWidth / 2.0f) + window.getSize().x / 2.0f - tileWidth / 2.0f + offsetX,
        (unit.getX() + unit.getY()) * (tileHeight / 2.0f) + offsetY
    );

    bullet.setPosition(towerScreenPos); // �Ѿ� ���� ��ġ

    // ���� ���� ���
    sf::Vector2f direction = unitScreenPos - towerScreenPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // ���� ���� ����ȭ
    direction /= distance;

    // �Ѿ� �ӵ� ����
    float bulletSpeed = 500.f; // �ȼ�/��

    sf::Vector2f velocity = direction * bulletSpeed;

    sf::Vector2f currentPosition = towerScreenPos;
    sf::Clock clock; // ��� �ð� ������

    // �ִϸ��̼� ����
    while (true) {
        float elapsedTime = clock.restart().asSeconds(); // ��� �ð� ����

        // �Ѿ��� ���� ��ġ ������Ʈ
        currentPosition += velocity * elapsedTime;
        bullet.setPosition(currentPosition);

        // �浹 üũ: �Ѿ��� ���ֿ� ������ ����
        if (std::abs(currentPosition.x - unitScreenPos.x) < 45.f &&
            std::abs(currentPosition.y - unitScreenPos.y) < 45.f) {
            std::cout << "Bullet hit the target at (" << unit.getX() << ", " << unit.getY() << ").\n";
            break;
        }

        // ȭ�� ������
        window.clear();

        // �� �� Ÿ��, ���� �ٽ� �׸���
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

        // Ÿ�� �׸���
        for (const auto& placedTower : placedTowers) {
            drawTower(window, placedTower, offsetX, offsetY);
        }

        // ���� �׸���
        for (const auto& activeUnit : activeUnits) {
            drawUnit(window, activeUnit, offsetX, offsetY);
        }

        // �Ѿ� �׸���
        window.draw(bullet);

        // ȭ�� ����
        window.display();
    }
    
}
