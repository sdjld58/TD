#include "PlacedTower.h"
#include <iostream>

// 생성자 정의
PlacedTower::PlacedTower(const Tower& tower, int posX, int posY)
    : baseTower(tower), x(posX), y(posY) {}

// 위치 정보 접근자
int PlacedTower::getX() const { return x; }
int PlacedTower::getY() const { return y; }

// 타워 속성 접근자 (기본 타워의 속성에 접근)
int PlacedTower::getId() const { return baseTower.getId(); }
std::string PlacedTower::getTowerName() const { return baseTower.getTowerName(); }
int PlacedTower::getBuildCost() const { return baseTower.getBuildCost(); }
int PlacedTower::getAttackRange() const { return baseTower.getAttackRange(); }
int PlacedTower::getDamage() const { return baseTower.getDamage(); }
bool PlacedTower::getIsMagic() const { return baseTower.getIsMagic(); }
int PlacedTower::getTimePerAttack() const { return baseTower.getTimePerAttack(); }
int PlacedTower::getTargetAmount() const { return baseTower.getTargetAmount(); }

// 타워 업그레이드 함수
bool PlacedTower::upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList)
{
    // 업그레이드할 타워 ID를 얻음
    int nextTowerID = baseTower.getNextTowerID();
    if (nextTowerID == 0)
    {
        std::cout << baseTower.getTowerName() << "은(는) 이미 최고 단계입니다.\n";
        return false;
    }

    // 타워 리스트에서 nextTowerID에 해당하는 타워를 찾음
    auto it = std::find_if(towerList.begin(), towerList.end(), [nextTowerID](const Tower& t)
        {
        return t.getId() == nextTowerID;
        });

    if (it == towerList.end())
    {
        std::cout << "업그레이드할 타워 정보를 찾을 수 없습니다.\n";
        return false;
    }

    int upgradeCost = it->getBuildCost();
    if (gold < upgradeCost)
    {
        std::cout << "골드가 부족합니다. 업그레이드 비용: " << upgradeCost << ", 현재 골드: " << gold << "\n";
        return false;
    }

    // 골드가 충분한 경우 업그레이드 진행
    gold -= upgradeCost;
    baseTower = *it; // 타워를 업그레이드된 타워로 대체
    map[y][x] = baseTower.getTowerName(); // 맵에 새로운 타워 이름으로 업데이트
    std::cout << baseTower.getTowerName() << "이(가) 업그레이드되었습니다.\n";
    return true;
}