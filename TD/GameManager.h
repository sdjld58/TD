#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <string>
#include <queue>
#include "UnitType.h"
#include "Wave.h"
#include "Unit.h"

class GameManager {
private:
    std::vector<std::vector<std::string>> map;
    std::vector<UnitType> unitTypes; // ���� Ÿ�� ����Ʈ
    std::vector<Wave> waves;         // ���̺� ����Ʈ
    std::vector<std::pair<int, int>> path; // ���� �̵� ���

    int playerLife;
    int gold;

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
};

#endif // GAMEMANAGER_H
