#ifndef PLACEDTOWER_H
#define PLACEDTOWER_H

#include "Tower.h"
#include <utility>

class PlacedTower {
private:
    Tower baseTower;          // 기본 타워 속성
    int x, y;                 // 배치된 위치

public:
    // 생성자
    PlacedTower(const Tower& tower, int posX, int posY);
       

    // 위치 정보 접근자
    int getX() const;
    int getY() const;

    // 타워의 속성 접근자 (기본 타워의 속성에 접근)
    int getId() const;
    std::string getTowerName() const;
    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    bool getIsMagic() const;
    int getTimePerAttack() const;
    int getTargetAmount() const;

    // 타워 업그레이드
    void upgrade();
};

#endif // PLACEDTOWER_H
