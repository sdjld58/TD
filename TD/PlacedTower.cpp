#include "PlacedTower.h"
#include <iostream>

// ������ ����
PlacedTower::PlacedTower(const Tower& tower, int posX, int posY)
    : baseTower(tower), x(posX), y(posY) {}

// ��ġ ���� ������
int PlacedTower::getX() const 
{
    return x;
}

int PlacedTower::getY() const 
{
    return y;
}

// Ÿ�� �Ӽ� ������ (�⺻ Ÿ���� �Ӽ��� ����)
int PlacedTower::getId() const
{
    return baseTower.getId();
}

std::string PlacedTower::getTowerName() const 
{
    return baseTower.getTowerName();
}

int PlacedTower::getBuildCost() const
{
    return baseTower.getBuildCost();
}

int PlacedTower::getAttackRange() const 
{
    return baseTower.getAttackRange();
}

int PlacedTower::getDamage() const 
{
    return baseTower.getDamage();
}

bool PlacedTower::getIsMagic() const 
{
    return baseTower.getIsMagic();
}

int PlacedTower::getTimePerAttack() const 
{
    return baseTower.getTimePerAttack();
}

int PlacedTower::getTargetAmount() const 
{
    return baseTower.getTargetAmount();
}

// Ÿ�� ���׷��̵� �Լ�
void PlacedTower::upgrade()
{
    baseTower.upgrade();
}
