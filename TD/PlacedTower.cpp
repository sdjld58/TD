#include "PlacedTower.h"
#include <iostream>

// 생성자 정의
PlacedTower::PlacedTower(const Tower& tower, int posX, int posY)
    : baseTower(tower), x(posX), y(posY) {}

// 위치 정보 접근자
int PlacedTower::getX() const 
{
    return x;
}

int PlacedTower::getY() const 
{
    return y;
}

// 타워 속성 접근자 (기본 타워의 속성에 접근)
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

// 타워 업그레이드 함수
void PlacedTower::upgrade()
{
    baseTower.upgrade();
}
