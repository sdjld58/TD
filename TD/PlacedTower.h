#ifndef PLACED_TOWER_H
#define PLACED_TOWER_H

#include "Tower.h"
#include <vector>

class PlacedTower {
public:
    PlacedTower(const Tower& tower, int posX, int posY);

    // 위치 정보 접근자
    int getX() const;
    int getY() const;

    // 타워 속성 접근자 (기본 타워의 속성에 접근)
    int getId() const;
    std::string getTowerName() const;
    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    bool getIsMagic() const;
    int getTimePerAttack() const;
    int getTargetAmount() const;

    // 타워 업그레이드 함수
    bool upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList); // 타워 리스트를 매개변수로 받음

private:
    Tower baseTower;
    int x;
    int y;
};

#endif // PLACED_TOWER_H
