#include "PlacedTower.h"
#include <iostream>
#include <algorithm>

// ������
PlacedTower::PlacedTower(const Tower& tower, int posX, int posY)
    : baseTower(tower), x(posX), y(posY),
    currentDamage(tower.getDamage()),
    currentTimePerAttack(tower.getTimePerAttack()) {}

// ��ġ ���� �޼���
int PlacedTower::getX() const {
    return x;
}

int PlacedTower::getY() const {
    return y;
}

// ���� �Ӽ� ���� �޼���
int PlacedTower::getDamage() const {
    return currentDamage;
}

int PlacedTower::getTimePerAttack() const {
    return currentTimePerAttack;
}

// �⺻ Ÿ�� �Ӽ� ���� �޼���
int PlacedTower::getId() const {
    return baseTower.getId();
}

std::string PlacedTower::getTowerName() const {
    return baseTower.getTowerName();
}

int PlacedTower::getBuildCost() const {
    return baseTower.getBuildCost();
}

int PlacedTower::getAttackRange() const {
    return baseTower.getAttackRange();
}

bool PlacedTower::getIsMagic() const {
    return baseTower.getIsMagic();
}
std::string PlacedTower::getTool() const {
    return baseTower.getTool();
}
std::string PlacedTower::getTool2() const {
    return baseTower.getTool2();
}
std::string PlacedTower::attackType() const {
    if (baseTower.getIsMagic()) return "����";
    else return "����";
}
int PlacedTower::getTargetAmount() const {
    return baseTower.getTargetAmount();
}

int PlacedTower::getIsNoDamage() const {
    return baseTower.getIsNoDamage();
}

// ���׷��̵� �޼���
bool PlacedTower::upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList,int num) {
    int nextTowerID = baseTower.getNextTowerID();
    nextTowerID = nextTowerID + num;
    if (nextTowerID == 0) {
        std::cout << baseTower.getTowerName() << "��(��) �̹� �ְ� �ܰ��Դϴ�.\n";
        return false;
    }

    // Ÿ�� ����Ʈ���� nextTowerID�� �ش��ϴ� Ÿ���� ã��
    auto it = std::find_if(towerList.begin(), towerList.end(), [nextTowerID](const Tower& t) {
        return t.getId() == nextTowerID;
        });

    if (it == towerList.end()) {
        std::cout << "���׷��̵��� Ÿ�� ������ ã�� �� �����ϴ�.\n";
        return false;
    }

    int upgradeCost = it->getBuildCost();
    if (gold < upgradeCost) {
        std::cout << "��尡 �����մϴ�. ���׷��̵� ���: " << upgradeCost << ", ���� ���: " << gold << "\n";
        return false;
    }

    // ��尡 ����� ��� ���׷��̵� ����
    gold -= upgradeCost;
    baseTower = *it; // Ÿ���� ���׷��̵�� Ÿ���� ��ü
    currentDamage = baseTower.getDamage(); // �� Ÿ���� ������ �� �ݿ�
    currentTimePerAttack = baseTower.getTimePerAttack(); // �� Ÿ���� ���� �ӵ� �ݿ�
    map[y][x] = baseTower.getTowerName(); // �ʿ� ���ο� Ÿ�� �̸����� ������Ʈ
    std::cout << baseTower.getTowerName() << "��(��) ���׷��̵�Ǿ����ϴ�.\n";
    return true;
}

// ���� ���� �޼���
void PlacedTower::applyBuff(int damageBoost, int attackSpeedReduction) {
    currentDamage += damageBoost;
    currentTimePerAttack -= attackSpeedReduction;
    receivedBuff = true;
}

void PlacedTower::clearBuff() {
    currentDamage = baseTower.getDamage();
    currentTimePerAttack = baseTower.getTimePerAttack();
    receivedBuff = false;
}

// ���� ���� Ȯ�� �޼���
bool PlacedTower::hasReceivedBuff() const {
    return receivedBuff;
}
