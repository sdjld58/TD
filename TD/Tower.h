#ifndef TOWER_H
#define TOWER_H

#include <string>
#include <vector>

class Tower {
public:
    Tower(int id, std::string towerName, int nextTowerID, int buildCost, int attackRange, int damage, bool isMagic, int timePerAttack, int targetAmount, int isNoDamage, const std::string& tool, const std::string& tool2);

    // Getter �Լ���
    int getId() const;
    std::string getTowerName() const;
    int getNextTowerID() const;
    int getBuildCost() const;
    int getAttackRange() const;
    int getDamage() const;
    bool getIsMagic() const;
    int getTimePerAttack() const;
    int getTargetAmount() const;
    int getIsNoDamage() const;
    std::string getTool() const;
    std::string getTool2() const;

    // ���׷��̵� �Լ� - Ÿ�� ����Ʈ�� ���ڷ� ����
    void upgrade(const std::vector<Tower>& towerList);
    void printInfo() const;
    void displayInfo() const;

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
    int isNoDamage; // ���� Ÿ�� ����
    std::string tool; // ���� ����
    std::string tool2; //�ο� ����
};

#endif // TOWER_H
