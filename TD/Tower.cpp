#include "Tower.h"
#include <iostream>

// ������ ����
Tower::Tower(int id, std::string towerName, int nextTowerID, int buildCost, int attackRange, int damage, bool isMagic, int timePerAttack, int targetAmount)
    : id(id), towerName(towerName), nextTowerID(nextTowerID), buildCost(buildCost), attackRange(attackRange), damage(damage), isMagic(isMagic), timePerAttack(timePerAttack), targetAmount(targetAmount) {}

// Getter �Լ��� ����
int Tower::getId() const { return id; }
std::string Tower::getTowerName() const { return towerName; }
int Tower::getNextTowerID() const { return nextTowerID; }
int Tower::getBuildCost() const { return buildCost; }
int Tower::getAttackRange() const { return attackRange; }
int Tower::getDamage() const { return damage; }
bool Tower::getIsMagic() const { return isMagic; }
int Tower::getTimePerAttack() const { return timePerAttack; }
int Tower::getTargetAmount() const { return targetAmount; }

// ���׷��̵� �Լ� ����
void Tower::upgrade() {
    if (nextTowerID == 0) {
        std::cout << towerName << "��(��) �̹� �ְ� �ܰ��Դϴ�.\n";
    }
    else {
        id = nextTowerID; // ���� Ÿ�� ID�� ���׷��̵�
        std::cout << towerName << "��(��) ���׷��̵�Ǿ����ϴ�.\n";
    }
}

// Ÿ�� ���� ��� �Լ� ����
void Tower::printInfo() const {
    std::cout << "Ÿ�� �̸�: " << towerName << "\n"
        << "ID: " << id << "\n"
        << "��ġ ���: " << buildCost << "\n"
        << "���� ��Ÿ�: " << attackRange << "\n"
        << "�����: " << damage << (isMagic ? " (���� �����)" : " (���� �����)") << "\n"
        << "���� �ӵ�: " << timePerAttack << "ƽ���� �� �� ����\n"
        << "��ǥ ��: " << targetAmount << "��\n";
}
