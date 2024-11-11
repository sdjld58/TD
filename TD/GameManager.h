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
    std::queue<int> unitProductionQueue; // ���� ���� ��⿭

    int playerLife;
    int gold;
    bool isPreparation;
    int attackGold; // ���� ���̺� ���� ��ȭ

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
    void erasecursol();
    void loadTowerData(const std::string& filename); //Ÿ�� �ε�
    void attackUnits(std::vector<Unit>& activeUnits ,int currentTick); //���ݷ���
    int calculateDamage(bool damageType, int baseDamage, const Unit& unit); //������ ��� ����
   
    

    void startAttackWave(const Wave& wave,int& currentTick); //���� ���̺� ó��
    void handleAttackInput();  // �÷��̾� ���� �Է� ó��
    void updateAttackUnits(std::vector<Unit>& activeUnits); //���� ���� ���� �� ������Ʈ
    bool isAttackWaveOver(const std::vector<Unit>& activeUnits); //���� ���̺� ���� ���� Ȯ��

};

#endif // GAMEMANAGER_H
