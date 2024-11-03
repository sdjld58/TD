#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <string>

class UnitType {
private:
    int id;                 // 유닛의 id (고유 번호)
    std::string unitName;   // 유닛 이름
    int hp;                 // 유닛 체력
    int timePerMove;        // 유닛이 몇 틱에 한 타일씩 움직이는지
    int armor;              // 방어력 수치
    int resist;             // 저항력 수치
    int productionCost;     // 공격 웨이브에서 생산할 때 소모되는 공격 재화
    int killReward;         // 수비 웨이브에서 죽였을 때 지급되는 수비 재화

public:
    // 생성자
    UnitType(int id, const std::string& unitName, int hp, int timePerMove, int armor, int resist, int productionCost, int killReward);

    // 접근자 함수들
    int getId() const;
    std::string getUnitName() const;
    int getHp() const;
    int getTimePerMove() const;
    int getArmor() const;
    int getResist() const;
    int getProductionCost() const;
    int getKillReward() const;

    // 디버깅이나 테스트를 위한 함수
    void printInfo() const;
};

#endif // UNITTYPE_H
