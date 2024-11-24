#include "Tower.h"
#include <iostream>
#include <algorithm>

// ������ ����
Tower::Tower(int id, std::string towerName, int nextTowerID, int buildCost, int attackRange, int damage, bool isMagic, int timePerAttack, int targetAmount, int isNoDamage, const std::string& tool)
    : id(id), towerName(towerName), nextTowerID(nextTowerID), buildCost(buildCost), attackRange(attackRange), damage(damage), isMagic(isMagic), timePerAttack(timePerAttack), targetAmount(targetAmount), isNoDamage(isNoDamage), tool(tool) {}

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
int Tower::getIsNoDamage() const { return isNoDamage; }
std::string Tower::getTool() const { return tool; }

// ���׷��̵� �Լ� ���� - Ÿ�� ����Ʈ���� nextTowerID�� �ش��ϴ� Ÿ���� ã��
void Tower::upgrade(const std::vector<Tower>& towerList)
{
    if (nextTowerID == 0)
    {
        std::cout << towerName << "��(��) �̹� �ְ� �ܰ��Դϴ�.\n";
        return;
    }

    auto it = std::find_if(towerList.begin(), towerList.end(), [this](const Tower& t) {
        return t.getId() == this->nextTowerID;
        });

    if (it != towerList.end())
    {
        *this = *it; // ���� Ÿ�� ��ü�� ���׷��̵�� Ÿ���� ��ü
        std::cout << towerName << "��(��) ���׷��̵�Ǿ����ϴ�.\n";
    }
    else
    {
        std::cout << "���׷��̵��� Ÿ�� ������ ã�� �� �����ϴ�.\n";
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
        << "��ǥ ��: " << targetAmount << "��\n"
        << "���� Ÿ�� ����: " << (isNoDamage ? "��" : "�ƴϿ�") << "\n"
        << "����: " << tool << "\n";
}

void Tower::displayInfo() const 
{
    std::cout << "Ÿ�� �̸�: " << towerName << "\n";
    std::cout << "����: " << tool << "\n";
    std::cout << "���ݷ�: " << damage << "\n";
    std::cout << "��Ÿ�: " << attackRange << "\n";
    std::cout << "��ġ ���: " << buildCost << "\n";
    std::cout << "--------------------------------\n";
}