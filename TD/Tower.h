#ifndef TOWER_H
#define TOWER_H

#include <string>
#include <vector>

class Tower {
public:
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

    // 업그레이드 함수 - 타워 리스트를 인자로 받음
    void upgrade(const std::vector<Tower>& towerList);
    void printInfo() const;

private:
    int id;
    std::string towerName;
    int nextTowerID;
    int buildCost;
    int attackRange;
    int damage;
    bool isMagic;
    int timePerAttack;
    int targetAmount;
};

#endif // TOWER_H
