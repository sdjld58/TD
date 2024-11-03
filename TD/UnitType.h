#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <string>

class UnitType {
private:
    int id;                 // ������ id (���� ��ȣ)
    std::string unitName;   // ���� �̸�
    int hp;                 // ���� ü��
    int timePerMove;        // ������ �� ƽ�� �� Ÿ�Ͼ� �����̴���
    int armor;              // ���� ��ġ
    int resist;             // ���׷� ��ġ
    int productionCost;     // ���� ���̺꿡�� ������ �� �Ҹ�Ǵ� ���� ��ȭ
    int killReward;         // ���� ���̺꿡�� �׿��� �� ���޵Ǵ� ���� ��ȭ

public:
    // ������
    UnitType(int id, const std::string& unitName, int hp, int timePerMove, int armor, int resist, int productionCost, int killReward);

    // ������ �Լ���
    int getId() const;
    std::string getUnitName() const;
    int getHp() const;
    int getTimePerMove() const;
    int getArmor() const;
    int getResist() const;
    int getProductionCost() const;
    int getKillReward() const;

    // ������̳� �׽�Ʈ�� ���� �Լ�
    void printInfo() const;
};

#endif // UNITTYPE_H
