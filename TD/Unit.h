#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>

class Unit {
private:
    int id;                   // ������ id (UnitType�� id)
    std::string name;         // ���� �̸�
    int hp;                   // ���� ü��
    int timePerMove;          // �� ƽ�� �� ĭ �����̴���
    int moveCounter;          // �̵��� ���� ƽ ī����
    int x, y;                 // ���� ��ġ
    int armor;                // ����
    int resist;               // ���׷�
    int killReward;           // ���� ���� óġ�� ���� 
    int LifeDamage;
    size_t pathIndex;         // ��ο����� ���� ��ġ �ε���
    std::vector<std::pair<int, int>> path; // �̵� ���

public:
    Unit(int id, const std::string& name, int hp, int timePerMove,int armor, int resist, int killReward, const std::vector<std::pair<int, int>>& path, int LifeDamage);

    // �ൿ �Լ�
    bool update(); // �̵� �� ���� ������Ʈ, �������� �����ϸ� true ��ȯ

    // ��ġ ����
    int getX() const;
    int getY() const;
    int getArmor() const;
    int getResist() const;
    int getKillReward() const;
    int getLifeDamage() const;

    //HP���� ����
    int getHp() const;
    void reduceHp(int damage);

    // �̸� ��������
    std::string getName() const;

    // ���
    void printStatus() const;
};

#endif // UNIT_H
