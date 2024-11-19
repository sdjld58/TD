#ifndef PLACEDTOWER_H
#define PLACEDTOWER_H

#include "Tower.h"
#include <vector>

class PlacedTower {
public:
    // ������
    PlacedTower(const Tower& tower, int posX, int posY);

    // ��ġ ���� �޼���
    int getX() const;
    int getY() const;

    // ���� �Ӽ� ���� �޼���
    int getDamage() const;
    int getTimePerAttack() const;

    // �⺻ Ÿ�� �Ӽ� ���� �޼���
    int getId() const;
    std::string getTowerName() const;
    int getBuildCost() const;
    int getAttackRange() const;
    bool getIsMagic() const;
    int getTargetAmount() const;
    bool getIsNoDamage() const;

    // ���׷��̵� �޼���
    bool upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList);

    // ���� ���� �޼���
    void applyBuff(int damageBoost, int attackSpeedReduction);
    void clearBuff();

    // ���� ���� ����
    bool hasReceivedBuff() const;

private:
    Tower baseTower;          // �⺻ Ÿ�� ����
    int x, y;                 // Ÿ�� ��ġ
    int currentDamage;        // ���� ���ݷ�
    int currentTimePerAttack; // ���� ���� �ӵ�
    bool receivedBuff = false; // ������ �޾Ҵ��� ����
};

#endif // PLACEDTOWER_H
