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
#include "Projectile.h"
#include "TextBox.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include<string>



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
    sf::Texture archer1TowerTexture;
    sf::Sprite archer1TowerSprite;
    sf::Texture archer2TowerTexture;
    sf::Sprite archer2TowerSprite;
    sf::Texture archer3TowerTexture;
    sf::Sprite archer3TowerSprite;

    sf::Texture wizard1TowerTexture;
    sf::Sprite wizard1TowerSprite;
    sf::Texture wizard2TowerTexture;
    sf::Sprite wizard2TowerSprite;
    sf::Texture wizard3TowerTexture;
    sf::Sprite wizard3TowerSprite;

    sf::Texture barrack1TowerTexture;
    sf::Sprite barrack1TowerSprite;
    sf::Texture barrack2TowerTexture;
    sf::Sprite barrack2TowerSprite;
    sf::Texture barrack3TowerTexture;
    sf::Sprite barrack3TowerSprite;

    // ����ü �ؽ�ó
    sf::Texture projectileTexture;

    // ���� �ؽ�ó �� ��������Ʈ
    sf::Texture knightUnitTexture;
    sf::Sprite knightUnitSprite;

    float offsetX = -100.0f;
    float offsetY = 200.0f;
   
    int attackGold;
    
    //���� ���̺� ����
    bool curWaveIsDefense = true;
    
    //TextBox�� ����ϱ����� ��� ������
    std::vector<std::wstring> infoTexts; // ���� infoText�� �����ϴ� ����

public:
    void initialize(const std::vector<std::vector<std::string>>& gameMap);
    void update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
        int playerLife, int gold, int selectedX = -1, int selectedY = -1,
        const std::vector<Projectile>& projectiles = {});

    void updateattackGold(int attackgold);
    
    void drawButtons();
    void clearButtons();
    void  gameInfoStage(); //����Ű ����ȭ��
    
    sf::RenderWindow& getWindow();
    void setMap(const std::vector<std::vector<std::string>>& gameMap);
    void setPath(const std::vector<std::pair<int, int>>& gamePath);
    void setTowers(const std::vector<Tower>& gameTowers);
    void setUnitTypes(const std::vector<UnitType>& gameUnitTypes);
    void setIsDefence(bool isDefence); 

    //�ؽ�Ʈ �ڽ� ���� ����
    void setInfoText(std::initializer_list<std::string> infoSentences);
    std::wstring getInfoText(int index) const;

    UI() : infoTexts() {}

    //����ü
    const sf::Texture& getProjectileTexture() const;
    void drawProjectiles(const std::vector<Projectile>& projectiles);

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
    std::function<void()> onOneButtonClicked;
    std::function<void()> onTwoButtonClicked;
    std::function<void()> onThreeButtonClicked;

    std::function<void()> onFirstUnitButtonClicked;
    std::function<void()> onSecondUnitButtonClicked;
    std::function<void()> onThirdUnitButtonClicked;
};


//TGUI ��ư ������ ���� Ŭ����
class RoundedRectangleShape : public sf::Shape {
public:
    RoundedRectangleShape(const sf::Vector2f& size, float radius, size_t pointCount = 30)
        : m_size(size), m_radius(radius), m_pointCount(pointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) {
        m_size = size;
        update();
    }

    void setRadius(float radius) {
        m_radius = radius;
        update();
    }

    virtual std::size_t getPointCount() const override {
        return m_pointCount * 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const override {
        float deltaAngle = 90.f / m_pointCount;
        sf::Vector2f center;
        size_t corner = index / m_pointCount;

        switch (corner) {
        case 0: center = { m_size.x - m_radius, m_radius }; break;
        case 1: center = { m_radius, m_radius }; break;
        case 2: center = { m_radius, m_size.y - m_radius }; break;
        case 3: center = { m_size.x - m_radius, m_size.y - m_radius }; break;
        }

        float angle = deltaAngle * (index % m_pointCount);
        angle = corner * 90.f + angle;
        return {
            center.x + m_radius * std::cos(angle * 3.14159265f / 180.f),
            center.y - m_radius * std::sin(angle * 3.14159265f / 180.f)
        };
    }

private:
    sf::Vector2f m_size;
    float m_radius;
    size_t m_pointCount;
};





#endif // UI_H
