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
    size_t pathIndex;         // ��ο����� ���� ��ġ �ε���
    std::vector<std::pair<int, int>> path; // �̵� ���

public:
    Unit(int id, const std::string& name, int hp, int timePerMove, const std::vector<std::pair<int, int>>& path);

    // �ൿ �Լ�
    bool update(); // �̵� �� ���� ������Ʈ, �������� �����ϸ� true ��ȯ

    // ��ġ ����
    int getX() const;
    int getY() const;

    //HP���� ����
    int getHp() const;
    void reduceHp(int damage);

    // �̸� ��������
    std::string getName() const;

    // ���
    void printStatus() const;
};

#endif // UNIT_H
