#include "UI.h"
#include <iostream>

void UI::initialize(const std::vector<std::vector<std::string>>& gameMap)
{
    map = gameMap;
    window.create(sf::VideoMode(1600, 900), "Tower Defense Game");

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

    // Ÿ�� ũ�� (�̼Ҹ�Ʈ���� ���� ����)
    // tileSize�� ����������, �̼Ҹ�Ʈ�� Ÿ���� ���� ���̸� ���� ����
    // �̼Ҹ�Ʈ�� Ÿ���� ���� ���� ����
    tileWidth = tileSize;         // 128
    tileHeight = tileSize / 2;    // 64

    // ���� �� Ÿ���� ���� ���� (����/Ÿ�� �̸����� ����)
    unitColors["U1"] = sf::Color::Red;
    unitColors["U2"] = sf::Color::Green;
    unitColors["U3"] = sf::Color::Yellow;

    towerColors["T1"] = sf::Color::Blue;
    towerColors["T2"] = sf::Color::Cyan;
    towerColors["T3"] = sf::Color::Magenta;
}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
    int playerLife, int gold, int selectedX, int selectedY)
{
    window.clear();

    // Ÿ�� ũ�� ���� ���� (�̼Ҹ�Ʈ���� ���� �߰�)
    const float tileWidth = tileSize;          // 128
    const float tileHeight = tileSize / 2.0f;  // 64

    // �� �׸���
    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x)
        {
            std::string tileType = map[y][x];

            // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
            float screenX = (x - y) * (tileWidth / 2.0f) + window.getSize().x / 2.0f;
            float screenY = (x + y) * (tileHeight / 2.0f);

            // �̼Ҹ�Ʈ�� Ÿ�� �׸��� (������ ����)
            sf::ConvexShape tileShape;
            tileShape.setPointCount(4);
            tileShape.setPoint(0, sf::Vector2f(tileWidth / 2.0f, 0));
            tileShape.setPoint(1, sf::Vector2f(tileWidth, tileHeight / 2.0f));
            tileShape.setPoint(2, sf::Vector2f(tileWidth / 2.0f, tileHeight));
            tileShape.setPoint(3, sf::Vector2f(0, tileHeight / 2.0f));
            tileShape.setPosition(screenX, screenY);

            // Ÿ�� ���� ����
            if (tileType == "P")
            {
                tileShape.setFillColor(sf::Color(150, 150, 150)); // ��� Ÿ��
            }
            else if (tileType == "O")
            {
                tileShape.setFillColor(sf::Color(100, 100, 100)); // ��ġ ���� Ÿ��
            }
            else if (tileType == "S")
            {
                tileShape.setFillColor(sf::Color::Green); // ���� ����
            }
            else if (tileType == "D")
            {
                tileShape.setFillColor(sf::Color::Red); // ���� ����
            }
            else
            {
                tileShape.setFillColor(sf::Color(50, 50, 50)); // �⺻ Ÿ��
            }

            window.draw(tileShape);

            // ���õ� Ÿ���̸� �������� �������̸� �׷��� ���� ǥ��
            if (x == selectedX && y == selectedY)
            {
                sf::ConvexShape overlayShape = tileShape;
                overlayShape.setFillColor(sf::Color(255, 255, 0, 100)); // �����, ���� 100
                window.draw(overlayShape);
            }
        }
    }

    // ���� �׸��� (ũ�⸦ tileSize���� �۰� ����)
    float unitScale = 0.4f; // ���� ũ�⸦ ����
    for (const auto& unit : units)
    {
        int tileX = unit.getX();
        int tileY = unit.getY();

        // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        // ������ ��ġ�� Ÿ���� �߽����� �����ϰ�, ũ�⸦ ����
        sf::CircleShape unitShape((tileWidth * unitScale) / 2.0f);
        unitShape.setOrigin((tileWidth * unitScale) / 2.0f, (tileWidth * unitScale) / 2.0f);
        unitShape.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight / 2.0f);

        // ������ �̸��� ���� ���� ����
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

    // Ÿ�� �׸��� (ũ�⸦ ����)
    float towerScale = 0.5f; // Ÿ�� ũ�⸦ ����
    for (const auto& tower : placedTowers)
    {
        int tileX = tower.getX();
        int tileY = tower.getY();

        // �̼Ҹ�Ʈ�� ��ǥ ��ȯ
        float screenX = (tileX - tileY) * (tileWidth / 2.0f) + window.getSize().x / 2.0f;
        float screenY = (tileX + tileY) * (tileHeight / 2.0f);

        // Ÿ���� ��ġ�� Ÿ���� �߽����� �����ϰ�, ũ�⸦ ����
        sf::RectangleShape towerShape(sf::Vector2f(tileWidth * towerScale, (tileHeight * towerScale) * 2));
        towerShape.setOrigin((tileWidth * towerScale) / 2.0f, (tileHeight * towerScale));
        towerShape.setPosition(screenX + tileWidth / 2.0f, screenY + tileHeight / 2.0f);

        // Ÿ���� �̸��� ���� ���� ����
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

    // �÷��̾� ������ �� ��� ǥ��
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
