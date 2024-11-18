#include "PlacedTower.h"
#include <iostream>

// ������ ����
PlacedTower::PlacedTower(const Tower& tower, int posX, int posY)
    : baseTower(tower), x(posX), y(posY) {}

// ��ġ ���� ������
int PlacedTower::getX() const { return x; }
int PlacedTower::getY() const { return y; }

// Ÿ�� �Ӽ� ������ (�⺻ Ÿ���� �Ӽ��� ����)
int PlacedTower::getId() const { return baseTower.getId(); }
std::string PlacedTower::getTowerName() const { return baseTower.getTowerName(); }
int PlacedTower::getBuildCost() const { return baseTower.getBuildCost(); }
int PlacedTower::getAttackRange() const { return baseTower.getAttackRange(); }
int PlacedTower::getDamage() const { return baseTower.getDamage(); }
bool PlacedTower::getIsMagic() const { return baseTower.getIsMagic(); }
int PlacedTower::getTimePerAttack() const { return baseTower.getTimePerAttack(); }
int PlacedTower::getTargetAmount() const { return baseTower.getTargetAmount(); }

// Ÿ�� ���׷��̵� �Լ�
bool PlacedTower::upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList)
{
    // ���׷��̵��� Ÿ�� ID�� ����
    int nextTowerID = baseTower.getNextTowerID();
    if (nextTowerID == 0)
    {
        std::cout << baseTower.getTowerName() << "��(��) �̹� �ְ� �ܰ��Դϴ�.\n";
        return false;
    }

    // Ÿ�� ����Ʈ���� nextTowerID�� �ش��ϴ� Ÿ���� ã��
    auto it = std::find_if(towerList.begin(), towerList.end(), [nextTowerID](const Tower& t)
        {
        return t.getId() == nextTowerID;
        });

    if (it == towerList.end())
    {
        std::cout << "���׷��̵��� Ÿ�� ������ ã�� �� �����ϴ�.\n";
        return false;
    }

    int upgradeCost = it->getBuildCost();
    if (gold < upgradeCost)
    {
        std::cout << "��尡 �����մϴ�. ���׷��̵� ���: " << upgradeCost << ", ���� ���: " << gold << "\n";
        return false;
    }

    // ��尡 ����� ��� ���׷��̵� ����
    gold -= upgradeCost;
    baseTower = *it; // Ÿ���� ���׷��̵�� Ÿ���� ��ü
    map[y][x] = baseTower.getTowerName(); // �ʿ� ���ο� Ÿ�� �̸����� ������Ʈ
    std::cout << baseTower.getTowerName() << "��(��) ���׷��̵�Ǿ����ϴ�.\n";
    return true;
}