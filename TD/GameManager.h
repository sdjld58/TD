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

class GameManager
{
private:
    std::vector<std::vector<std::string>> map;
    std::vector<UnitType> unitTypes; // ���� Ÿ�� ����Ʈ
    std::vector<Wave> waves;         // ���̺� ����Ʈ
    std::vector<std::pair<int, int>> path; // ���� �̵� ���
    std::vector<Tower> towers;            // �⺻ Ÿ�� �Ӽ��� �����ϴ� ����
    std::vector<PlacedTower> placedTowers; // ��ġ�� Ÿ���� �����ϴ� ����

    int playerLife;
    int gold;
    bool isPreparation;

public:
    GameManager();
    void run();
    void loadMap(const std::string& filename);
    void printMap();
    void parsePath(); // �ʿ��� ��� �Ľ�
    void loadUnitTypes(const std::string& filename);
    void printUnitTypes();
    void loadWaves(const std::string& filename);
    void printWaves();
    void updateAndPrintMap(const std::vector<Unit>& activeUnits); // �� ������Ʈ �� ���
    void startPreparationPhase();
    void constructTower();
    void erasecursol();
    void loadTowerData(const std::string& filename); //Ÿ�� �ε�
   
};

#endif // GAMEMANAGER_H
