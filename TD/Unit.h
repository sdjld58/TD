#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <cmath>

class Unit {
private:
    int id;                   // 유닛의 id (UnitType의 id)
    std::string name;         // 유닛 이름
    int hp;                   // 현재 체력
    int armor;                // 방어력
    int resist;               // 저항력
    int killReward;           // 수비 라운드 처치시 보상 
    int LifeDamage;
    size_t pathIndex;         // 경로에서의 현재 위치 인덱스
    std::vector<std::pair<int, int>> path; // 이동 경로

    float posX, posY;         // 현재 위치 (부동소수점)
    int targetX, targetY;     // 목표 타일 좌표
    float speed;              // 이동 속도 (tiles per second)
    bool isArrived;           // 목적지 도달 여부

public:
    Unit(int id, const std::string& name, int hp, int timePerMove, int armor, int resist, int killReward, const std::vector<std::pair<int, int>>& path, int LifeDamage);
   static std::string getUIName(int id);
    // 행동 함수
    bool update();                        // 목적지에 도달 여부 반환 함수
    void updatePosition(float deltaTime); // 위치 업데이트 함수

    // 위치 정보
    int getId() const { return id; }
    float getPosX() const;
    float getPosY() const;
    int getTileX() const;
    int getTileY() const;
    int getArmor() const;
    int getResist() const;
    int getKillReward() const;
    int getLifeDamage() const;

    //HP관련 정보
    int getHp() const;
    void reduceHp(int damage);
    int maxHp;

    // 이름 가져오기
    std::string getName() const;

    // 출력
    void printStatus() const;

    // 도착 여부 확인
    bool hasArrived() const;
};

#endif // UNIT_H
