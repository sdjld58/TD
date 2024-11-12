#ifndef PLACED_TOWER_H
#define PLACED_TOWER_H

#include "Tower.h"
#include <vector>

class PlacedTower {
public:
    PlacedTower(const Tower& tower, int posX, int posY);

    // ��ġ ���� ������
    int getX() const;
    int getY() const;

    // Ÿ�� �Ӽ� ������ (�⺻ Ÿ���� �Ӽ��� ����)
    int getId() const;
    std::string getTowerName() const;
    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    bool getIsMagic() const;
    int getTimePerAttack() const;
    int getTargetAmount() const;

    // Ÿ�� ���׷��̵� �Լ�
    bool upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList); // Ÿ�� ����Ʈ�� �Ű������� ����

private:
    Tower baseTower;
    int x;
    int y;
};

#endif // PLACED_TOWER_H
