#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <cmath>

class Unit {
private:
    int id;                   // ������ id (UnitType�� id)
    std::string name;         // ���� �̸�
    int hp;                   // ���� ü��
    int armor;                // ����
    int resist;               // ���׷�
    int killReward;           // ���� ���� óġ�� ���� 
    int LifeDamage;
    size_t pathIndex;         // ��ο����� ���� ��ġ �ε���
    std::vector<std::pair<int, int>> path; // �̵� ���

    float posX, posY;         // ���� ��ġ (�ε��Ҽ���)
    int targetX, targetY;     // ��ǥ Ÿ�� ��ǥ
    float speed;              // �̵� �ӵ� (tiles per second)
    bool isArrived;           // ������ ���� ����

public:
    Unit(int id, const std::string& name, int hp, int timePerMove, int armor, int resist, int killReward, const std::vector<std::pair<int, int>>& path, int LifeDamage);
   static std::string getUIName(int id);
    // �ൿ �Լ�
    bool update();                        // �������� ���� ���� ��ȯ �Լ�
    void updatePosition(float deltaTime); // ��ġ ������Ʈ �Լ�

    // ��ġ ����
    int getId() const { return id; }
    float getPosX() const;
    float getPosY() const;
    int getTileX() const;
    int getTileY() const;
    int getArmor() const;
    int getResist() const;
    int getKillReward() const;
    int getLifeDamage() const;

    //HP���� ����
    int getHp() const;
    void reduceHp(int damage);
    int maxHp;

    // �̸� ��������
    std::string getName() const;

    // ���
    void printStatus() const;

    // ���� ���� Ȯ��
    bool hasArrived() const;
};

#endif // UNIT_H
