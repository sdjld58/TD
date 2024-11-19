#ifndef PLACEDTOWER_H
#define PLACEDTOWER_H

#include "Tower.h"
#include <vector>

class PlacedTower {
public:
    // 생성자
    PlacedTower(const Tower& tower, int posX, int posY);

    // 위치 관련 메서드
    int getX() const;
    int getY() const;

    // 현재 속성 접근 메서드
    int getDamage() const;
    int getTimePerAttack() const;

    // 기본 타워 속성 접근 메서드
    int getId() const;
    std::string getTowerName() const;
    int getBuildCost() const;
    int getAttackRange() const;
    bool getIsMagic() const;
    int getTargetAmount() const;
    bool getIsNoDamage() const;

    // 업그레이드 메서드
    bool upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList);

    // 버프 관련 메서드
    void applyBuff(int damageBoost, int attackSpeedReduction);
    void clearBuff();

    // 버프 상태 관리
    bool hasReceivedBuff() const;

private:
    Tower baseTower;          // 기본 타워 정보
    int x, y;                 // 타워 위치
    int currentDamage;        // 현재 공격력
    int currentTimePerAttack; // 현재 공격 속도
    bool receivedBuff = false; // 버프를 받았는지 여부
};

#endif // PLACEDTOWER_H
