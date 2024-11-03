#include "UnitType.h"
#include <iostream>

UnitType::UnitType(int id, const std::string& unitName, int hp, int timePerMove, int armor, int resist, int productionCost, int killReward)
    : id(id), unitName(unitName), hp(hp), timePerMove(timePerMove), armor(armor), resist(resist), productionCost(productionCost), killReward(killReward) {}

// ������ �Լ���
int UnitType::getId() const { return id; }
std::string UnitType::getUnitName() const { return unitName; }
int UnitType::getHp() const { return hp; }
int UnitType::getTimePerMove() const { return timePerMove; }
int UnitType::getArmor() const { return armor; }
int UnitType::getResist() const { return resist; }
int UnitType::getProductionCost() const { return productionCost; }
int UnitType::getKillReward() const { return killReward; }

// ������̳� �׽�Ʈ�� ���� �Լ�
void UnitType::printInfo() const {
    std::cout << "ID: " << id << ", Name: " << unitName << ", HP: " << hp
        << ", TimePerMove: " << timePerMove << ", Armor: " << armor
        << ", Resist: " << resist << ", ProductionCost: " << productionCost
        << ", KillReward: " << killReward << std::endl;
}
