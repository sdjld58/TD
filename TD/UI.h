#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <utility> // std::pair 사용을 위해 필요
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

    // 유닛 상태 표시 텍스트 추가
    sf::Text unitStatusText;

    // 타일 텍스처 및 스프라이트
    sf::Texture roadTexture;
    sf::Sprite roadSprite;

    sf::Texture grassTexture;
    sf::Sprite grassSprite;

    sf::Texture buildingPlaceGrassTexture;
    sf::Sprite buildingPlaceGrassSprite;

    // 타워 텍스처 및 스프라이트
    sf::Texture archerTowerTexture;
    sf::Sprite archerTowerSprite;

    // 유닛 텍스처 및 스프라이트
    sf::Texture knightUnitTexture;
    sf::Sprite knightUnitSprite;

public:
    void initialize(const std::vector<std::vector<std::string>>& gameMap);
    void update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
        int playerLife, int gold, int selectedX = -1, int selectedY = -1);
    void render(); // 새로 추가한 함수
    sf::RenderWindow& getWindow();
    void setMap(const std::vector<std::vector<std::string>>& gameMap);
    void setPath(const std::vector<std::pair<int, int>>& gamePath);
    void setTowers(const std::vector<Tower>& gameTowers);
    void setUnitTypes(const std::vector<UnitType>& gameUnitTypes);
    // 타일 크기
    const float tileWidth = 128.0f;  // 이소메트릭 타일의 폭
    const float tileHeight = 76.0f;  // 이소메트릭 타일의 높이
};

#endif // UI_H
