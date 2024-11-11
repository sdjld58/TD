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

    // Ÿ�� �ؽ�ó �ε� (���÷� �ܻ� �簢�� ���)
    tileTexture.create(tileSize, tileSize);
    sf::Image tileImage;
    tileImage.create(tileSize, tileSize, sf::Color(100, 100, 100));
    tileTexture.update(tileImage);
    tileSprite.setTexture(tileTexture);

    // ���� �� Ÿ�� �ؽ�ó �ε� (���÷� �ܻ� �簢�� ���)
    unitTexture.create(tileSize, tileSize);
    sf::Image unitImage;
    unitImage.create(tileSize, tileSize, sf::Color::Red);
    unitTexture.update(unitImage);
    unitSprite.setTexture(unitTexture);

    towerTexture.create(tileSize, tileSize);
    sf::Image towerImage;
    towerImage.create(tileSize, tileSize, sf::Color::Blue);
    towerTexture.update(towerImage);
    towerSprite.setTexture(towerTexture);

    // ���� �� Ÿ���� ���� ���� (����/Ÿ�� �̸����� ����)
    unitColors["U1"] = sf::Color::Red;
    unitColors["U2"] = sf::Color::Green;
    unitColors["U3"] = sf::Color::Yellow;

    towerColors["T1"] = sf::Color::Blue;
    towerColors["T2"] = sf::Color::Cyan;
    towerColors["T3"] = sf::Color::Magenta;
}

void UI::update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers, int playerLife, int gold, int selectedX, int selectedY)
{
    window.clear();

    // �� �׸���
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            std::string tileType = map[y][x];
            sf::RectangleShape tileShape(sf::Vector2f(tileSize, tileSize));
            tileShape.setPosition(x * tileSize, y * tileSize);

            // ���õ� Ÿ���̸� �ٸ� �������� ǥ��
            if ((int)x == selectedX && (int)y == selectedY)
            {
                tileShape.setFillColor(sf::Color::Yellow); // ���õ� Ÿ�� ����
            }
            else
            {
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
            }

            window.draw(tileShape);
        }
    }

    // ���� �׸��� (ũ�⸦ tileSize���� �۰� ����)
    float unitScale = 0.8f; // ���� ũ�⸦ tileSize�� 80%�� ����
    for (const auto& unit : units)
    {
        sf::RectangleShape unitShape(sf::Vector2f(tileSize * unitScale, tileSize * unitScale));
        unitShape.setOrigin((tileSize * unitScale) / 2, (tileSize * unitScale) / 2); // �߽����� ����� ����
        unitShape.setPosition((unit.getX() + 0.5f) * tileSize, (unit.getY() + 0.5f) * tileSize);

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

    // Ÿ�� �׸��� (�ʿ��ϴٸ� Ÿ���� ũ�⸦ ����)
    float towerScale = 0.9f; // Ÿ�� ũ�⸦ tileSize�� 90%�� ����
    for (const auto& tower : placedTowers)
    {
        sf::RectangleShape towerShape(sf::Vector2f(tileSize * towerScale, tileSize * towerScale));
        towerShape.setOrigin((tileSize * towerScale) / 2, (tileSize * towerScale) / 2); // �߽����� ����� ����
        towerShape.setPosition((tower.getX() + 0.5f) * tileSize, (tower.getY() + 0.5f) * tileSize);

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
