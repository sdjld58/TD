#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Unit.h"
#include "PlacedTower.h"
#include "Tower.h"
#include "UnitType.h"

class UI
{
private:
    sf::RenderWindow window;
    std::vector<std::vector<std::string>> map;
    std::vector<std::pair<int, int>> path;
    std::vector<Tower> towers;
    std::vector<UnitType> unitTypes;

    // 리소스 로딩을 위한 멤버 변수
    sf::Font font;
    sf::Text lifeText;
    sf::Text goldText;

    // 타일 크기
    const int tileSize = 128;

    // 맵 타일에 대한 텍스처 및 스프라이트
    sf::Texture tileTexture;
    sf::Sprite tileSprite;

    // 유닛 및 타워에 대한 텍스처 및 스프라이트
    sf::Texture unitTexture;
    sf::Texture towerTexture;
    sf::Sprite unitSprite;
    sf::Sprite towerSprite;

    // 유닛 및 타워의 색상 구분을 위한 맵
    std::map<std::string, sf::Color> unitColors;
    std::map<std::string, sf::Color> towerColors;

public:
    void initialize(const std::vector<std::vector<std::string>>& gameMap);
    void update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers, int playerLife, int gold, int selectedX = -1, int selectedY = -1);
    sf::RenderWindow& getWindow();
    void setMap(const std::vector<std::vector<std::string>>& gameMap);
    void setPath(const std::vector<std::pair<int, int>>& gamePath);
    void setTowers(const std::vector<Tower>& gameTowers);
    void setUnitTypes(const std::vector<UnitType>& gameUnitTypes);

};

#endif // UI_H
