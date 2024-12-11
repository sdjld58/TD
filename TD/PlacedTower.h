#ifndef PLACEDTOWER_H
#define PLACEDTOWER_H

#include "Tower.h"
#include <vector>

class PlacedTower {
public:
    // ������
    PlacedTower(const Tower& tower, int posX, int posY,int currentTIme);

    // ��ġ ���� �޼���
    int getX() const;
    int getY() const;

    // ���� �Ӽ� ���� �޼���
    int getDamage() const;
    int getTimePerAttack() const;

    // �⺻ Ÿ�� �Ӽ� ���� �޼���
    int getId() const;
    std::string getTowerName() const;
    int getAttackRange() const;
    bool getIsMagic() const;
    int getTargetAmount() const;
    int getIsNoDamage() const;
    std::string getTool() const;
    std::string getTool2() const;
    std::string attackType() const; //���ݼӼ� ���ڿ��� ���

    std::string uiOptionSelect(const std::vector<Tower>& towerList, int num)const;
    int getNextTowerID() const;
    int getNextTowerID2() const;

    // ���׷��̵� �޼���
    bool upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList,int num);

    // ���� ���� �޼���
    void applyBuff(int damageBoost, int attackSpeedReduction);
    void clearBuff();

    // ���� ���� ����
    bool hasReceivedBuff() const;

    bool isAttack;
    int currentTime;



private:
    Tower baseTower;          // �⺻ Ÿ�� ����
    int x, y;                 // Ÿ�� ��ġ
    int currentDamage;        // ���� ���ݷ�
    int currentTimePerAttack; // ���� ���� �ӵ�
    bool receivedBuff = false; // ������ �޾Ҵ��� ����
};

#endif // PLACEDTOWER_H
