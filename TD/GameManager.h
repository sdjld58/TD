#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <string>
#include <queue>
#include "UnitType.h"
#include "Wave.h"
#include "Unit.h"
#include "PlacedTower.h"
#include "Tower.h"
#include "UI.h"  // UI Ŭ���� �߰�

class GameManager
{
private:
    std::vector<std::vector<std::string>> map;
    std::vector<UnitType> unitTypes;
    std::vector<Wave> waves;
    std::vector<std::pair<int, int>> path;
    std::vector<Tower> towers;
    std::vector<PlacedTower> placedTowers;
    std::queue<int> unitProductionQueue;

    int playerLife;
    int gold;
    bool isPreparation;
    int attackGold;
    bool showUnitInfo;

    UI ui;  // UI Ŭ���� �ν��Ͻ� �߰�

public:
    GameManager();
    void run();
    void loadMap(const std::string& filename);
    void parsePath();
    void loadUnitTypes(const std::string& filename);
    void loadWaves(const std::string& filename);
    

    void startPreparationPhase();
    void loadTowerData(const std::string& filename); //Ÿ�� �ε�
    void attackUnits(std::vector<Unit>& activeUnits ,int currentTick); //���ݷ���
    int calculateDamage(bool damageType, int baseDamage, const Unit& unit); //������ ��� ����
   
    

    void startAttackWave(const Wave& wave,int& currentTick);
    void handleAttackInput();
    void updateAttackUnits(std::vector<Unit>& activeUnits);
    bool isAttackWaveOver(const std::vector<Unit>& activeUnits);

    // �߰��� �޼���
    void updateGameState(std::vector<Unit>& activeUnits);
    //������ �ִ� ���� ���� ǥ��
    void displayUnitInfo(const std::vector<Unit>& activeUnits);
};

#endif // GAMEMANAGER_H
