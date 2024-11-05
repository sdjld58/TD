#ifndef TOWER_H
#define TOWER_H

#include <string>

class Tower {
private:
    int id;                // Ÿ���� ���� ID
    std::string towerName; // Ÿ���� �̸�
    int nextTowerID;       // ���� ���׷��̵� Ÿ���� ID (0�̸� ���� �ܰ�)
    int buildCost;         // Ÿ���� ��ġ�� �� ��� ���
    int attackRange;       // Ÿ���� ���� ��Ÿ�
    int damage;            // Ÿ���� ���� �����
    bool isMagic;          // ���� ����� ���� (true�� ���� �����, false�� ���� �����)
    int timePerAttack;     // �� �� �����ϴµ� �ɸ��� ƽ ��
    int targetAmount;      // �� �� ������ �� Ÿ�� ��

public:
    // ������
    Tower(int id, std::string towerName, int nextTowerID, int buildCost, int attackRange, int damage, bool isMagic, int timePerAttack, int targetAmount);

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

    // ���׷��̵� �Լ�
    void upgrade();

    // Ÿ�� ���� ��� �Լ�
    void printInfo() const;
};

#endif // TOWER_H
