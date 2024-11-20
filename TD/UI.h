#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <utility> // std::pair ����� ���� �ʿ�
#include "Unit.h"
#include "PlacedTower.h"
#include "Tower.h"
#include "UnitType.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>



class UI
{
private:
    sf::RenderWindow window;
    std::vector<std::vector<std::string>> map;
    std::vector<std::pair<int, int>> path;
    std::vector<Tower> towers;
    std::vector<UnitType> unitTypes;

    // ���ҽ� �ε��� ���� ��� ����
    sf::Font font;
    sf::Text lifeText;
    sf::Text goldText;

    // ���� ���� ǥ�� �ؽ�Ʈ �߰�
    sf::Text unitStatusText;

    // Ÿ�� �ؽ�ó �� ��������Ʈ
    sf::Texture roadTexture;
    sf::Sprite roadSprite;

    sf::Texture grassTexture;
    sf::Sprite grassSprite;

    sf::Texture buildingPlaceGrassTexture;
    sf::Sprite buildingPlaceGrassSprite;

    // Ÿ�� �ؽ�ó �� ��������Ʈ
    sf::Texture archerTowerTexture;
    sf::Sprite archerTowerSprite;

    // ���� �ؽ�ó �� ��������Ʈ
    sf::Texture knightUnitTexture;
    sf::Sprite knightUnitSprite;

public:
    void initialize(const std::vector<std::vector<std::string>>& gameMap);
    void update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
        int playerLife, int gold, int selectedX = -1, int selectedY = -1);
    void render(); // ���� �߰��� �Լ�
    sf::RenderWindow& getWindow();
    void setMap(const std::vector<std::vector<std::string>>& gameMap);
    void setPath(const std::vector<std::pair<int, int>>& gamePath);
    void setTowers(const std::vector<Tower>& gameTowers);
    void setUnitTypes(const std::vector<UnitType>& gameUnitTypes);

    void unitHpBar(sf::RenderWindow& window, float screenX, float screenY, int currentHp, int maxHp); //���� ü�¹�

    // Ÿ�� ũ��
    const float tileWidth = 128.0f;  // �̼Ҹ�Ʈ�� Ÿ���� ��
    const float tileHeight = 76.0f;  // �̼Ҹ�Ʈ�� Ÿ���� ����

    //â ũ��
    const int windowWidth = 1920;
    const int windowHeight = 1080;

    //��ư
    tgui::Gui gui;
    std::function<void()> onTowerButtonClicked;
};




#endif // UI_H
