#ifndef TOWER_H
#define TOWER_H

#include <string>

class Tower {
private:
    int id;                // 타워의 고유 ID
    std::string towerName; // 타워의 이름
    int nextTowerID;       // 다음 업그레이드 타워의 ID (0이면 최종 단계)
    int buildCost;         // 타워를 설치할 때 드는 비용
    int attackRange;       // 타워의 공격 사거리
    int damage;            // 타워의 공격 대미지
    bool isMagic;          // 마법 대미지 여부 (true면 마법 대미지, false면 물리 대미지)
    int timePerAttack;     // 한 번 공격하는데 걸리는 틱 수
    int targetAmount;      // 한 번 공격할 때 타겟 수

public:
    // 생성자
    Tower(int id, std::string towerName, int nextTowerID, int buildCost, int attackRange, int damage, bool isMagic, int timePerAttack, int targetAmount);

    // Getter 함수들
    int getId() const;
    std::string getTowerName() const;
    int getNextTowerID() const;
    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    bool getIsMagic() const;
    int getTimePerAttack() const;
    int getTargetAmount() const;

    // 업그레이드 함수
    void upgrade();

    // 타워 정보 출력 함수
    void printInfo() const;
};

#endif // TOWER_H
