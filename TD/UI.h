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
    sf::Texture backgroundTexture; //��� ȭ��
    sf::Sprite backgroundSprite;

    std::unordered_map<const Unit*, float> unitOldScreenX;
    float unitScale;

    // ���ҽ� �ε��� ���� ��� ����
    sf::Font font;
    sf::Text lifeText;
    sf::Text goldText;

    // ���� ���� ǥ�� �ؽ�Ʈ �߰�
    sf::Text unitStatusText;

    // Ÿ�� �ؽ�ó �� ��������Ʈ
    sf::Texture grassRoadTexture1;
    sf::Sprite grassRoadSprite1;
    sf::Texture grassRoadTexture2;
    sf::Sprite grassRoadSprite2;
    sf::Texture grassRoadTexture3;
    sf::Sprite grassRoadSprite3;
    sf::Texture grassRoadTexture4;
    sf::Sprite grassRoadSprite4;
    sf::Texture grassRoadTexture5;
    sf::Sprite grassRoadSprite5;
    sf::Texture grassRoadTexture6;
    sf::Sprite grassRoadSprite6;

    sf::Texture desertRoadTexture1;
    sf::Sprite desertRoadSprite1;
    sf::Texture desertRoadTexture2;
    sf::Sprite desertRoadSprite2;
    sf::Texture desertRoadTexture3;
    sf::Sprite desertRoadSprite3;
    sf::Texture desertRoadTexture4;
    sf::Sprite desertRoadSprite4;
    sf::Texture desertRoadTexture5;
    sf::Sprite desertRoadSprite5;
    sf::Texture desertRoadTexture6;
    sf::Sprite desertRoadSprite6;

    sf::Texture winterRoadTexture1;
    sf::Sprite winterRoadSprite1;
    sf::Texture winterRoadTexture2;
    sf::Sprite winterRoadSprite2;
    sf::Texture winterRoadTexture3;
    sf::Sprite winterRoadSprite3;
    sf::Texture winterRoadTexture4;
    sf::Sprite winterRoadSprite4;
    sf::Texture winterRoadTexture5;
    sf::Sprite winterRoadSprite5;
    sf::Texture winterRoadTexture6;
    sf::Sprite winterRoadSprite6;


    sf::Texture grassTexture;
    sf::Sprite grassSprite;
    
    sf::Texture sandTexture;
    sf::Sprite sandSprite;

    sf::Texture snowTexture;
    sf::Sprite snowSprite;

    sf::Texture buildingPlaceGrassTexture;
    sf::Sprite buildingPlaceGrassSprite;

    sf::Texture buildingPlaceSandTexture;
    sf::Sprite buildingPlaceSandSprite;

    sf::Texture buildingPlaceSnowTexture;
    sf::Sprite buildingPlaceSnowSprite;

    // Ÿ�� �ؽ�ó �� ��������Ʈ
    sf::Texture archer1TowerTexture;
    sf::Sprite archer1TowerSprite;
    sf::Texture archer2TowerTexture;
    sf::Sprite archer2TowerSprite;
    sf::Texture archer3TowerTexture;
    sf::Sprite archer3TowerSprite;
    sf::Texture archer4TowerTexture;
    sf::Sprite archer4TowerSprite;

    sf::Texture wizard1TowerTexture;
    sf::Sprite wizard1TowerSprite;
    sf::Texture wizard2TowerTexture;
    sf::Sprite wizard2TowerSprite;
    sf::Texture wizard3TowerTexture;
    sf::Sprite wizard3TowerSprite;
    sf::Texture wizard4TowerTexture;
    sf::Sprite wizard4TowerSprite;

    sf::Texture barrack1TowerTexture;
    sf::Sprite barrack1TowerSprite;
    sf::Texture barrack2TowerTexture;
    sf::Sprite barrack2TowerSprite;
    sf::Texture barrack3TowerTexture;
    sf::Sprite barrack3TowerSprite;
    sf::Texture barrack4TowerTexture;
    sf::Sprite barrack4TowerSprite;

    // ����ü �ؽ�ó
    sf::Texture projectileTexture1;
    sf::Texture projectileTexture2;
    sf::Texture projectileTexture3;
    sf::Texture projectileTexture4;

    // ���� �ؽ�ó �� ��������Ʈ
    sf::Texture WolfTexture[3];
    sf::Sprite WolfSprite;
    sf::Texture BatTexture[3];
    sf::Sprite BatSprite;
    sf::Texture BearTexture[3];
    sf::Sprite BearSprite;
    sf::Texture BlueScorpionTexture[3];
    sf::Sprite BlueScorpionSprite;
    sf::Texture JuggernautTexture[3];
    sf::Sprite JuggernautSprite;
    sf::Texture MohawkBatTexture[3];
    sf::Sprite MohawkBatSprite;
    sf::Texture PossessedBearTexture[3];
    sf::Sprite PossessedBearSprite;
    sf::Texture PossessedWolfTexture[3];
    sf::Sprite PossessedWolfSprite;
    sf::Texture TrollTexture[3];
    sf::Sprite TrollSprite;
    sf::Texture WinterBearTexture[3];
    sf::Sprite WinterBearSprite;
    sf::Texture WinterWolfTexture[3];
    sf::Sprite WinterWolfSprite;
    sf::Texture WinterOgreTexture[3];
    sf::Sprite WinterOgreSprite;

    float offsetX = -100.0f;
    float offsetY = 100.0f;
   
    int attackGold;
    
    //���� ���̺� ����
    bool curWaveIsDefense = true;
    
    //TextBox�� ����ϱ����� ��� ������
    std::vector<std::wstring> infoTexts; // ���� infoText�� �����ϴ� ����




    class TextBox {

    protected:
        std::vector<sf::Text> sideTexts;       // ���� �̸� �ؽ�Ʈ�� ������ ����
        std::vector<sf::Sprite> sideSprites;  // ���� ��������Ʈ�� ������ ����

        const UI* uip; // UI ��ü ����
   
        int life;
        int gold;
        int attackGold;

        sf::Vector2f position;
        sf::Vector2f sidePosition = sf::Vector2f(0.f, 0.f);

        sf::Vector2f size = sf::Vector2f(500.f, 350.f); // ������ �ڽ� ũ��
        sf::Vector2f sideSize; // ������ ���̵� �ڽ� ũ��


        float cornerRadius = 40.f;                      // ������ �ձ� �𼭸� �ݰ�

        sf::Color color;
        sf::Color sideColor;
        sf::Font font;
        std::vector<sf::Text> texts;

        const int windowWidth;
        const int windowHeight;

    public:
        TextBox(const UI& ui, bool isDefense, const std::vector<std::wstring>& strings);
        void draw(sf::RenderWindow& window);
        void sideDraw(sf::RenderWindow& window);
        void updateState(int playerlife, int gold, int attackgold);
 
    };

public:
    std::unordered_map<int, int> unitCounts; // <���� ID, ����>
    int judgeStage=0;

    void initialize(const std::vector<std::vector<std::string>>& gameMap);
    void update(const std::vector<Unit>& units, const std::vector<PlacedTower>& placedTowers,
        int playerLife, int gold, int selectedX = -1, int selectedY = -1,
        const std::vector<Projectile>& projectiles = {}, int currentTick = 0);
    bool gotoMapSelected;
    void updateattackGold(int attackgold);
    
    void drawButtons();
    void clearButtons();
    void  gameInfoStage(); //����Ű ����ȭ��
    void initializeBackground(const std::string& backgroundFilePath);
    void drawBackground();
 

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
    const sf::Texture& getProjectileTexture1() const;
    const sf::Texture& getProjectileTexture2() const;
    const sf::Texture& getProjectileTexture3() const;
    const sf::Texture& getProjectileTexture4() const;

    void drawProjectiles(const std::vector<Projectile>& projectiles);

    void unitHpBar(sf::RenderWindow& window, float screenX, float screenY, const Unit &unit); //���� ü�¹�

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
