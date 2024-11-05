#ifndef PLACEDTOWER_H
#define PLACEDTOWER_H

#include "Tower.h"
#include <utility>

class PlacedTower {
private:
    Tower baseTower;          // �⺻ Ÿ�� �Ӽ�
    int x, y;                 // ��ġ�� ��ġ

public:
    // ������
    PlacedTower(const Tower& tower, int posX, int posY);
       

    // ��ġ ���� ������
    int getX() const;
    int getY() const;

    // Ÿ���� �Ӽ� ������ (�⺻ Ÿ���� �Ӽ��� ����)
    int getId() const;
    std::string getTowerName() const;
    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    bool getIsMagic() const;
    int getTimePerAttack() const;
    int getTargetAmount() const;

    // Ÿ�� ���׷��̵�
    void upgrade();
};

#endif // PLACEDTOWER_H
