#include "PlacedTower.h"
#include <iostream>
#include <algorithm>

// 생성자
PlacedTower::PlacedTower(const Tower& tower, int posX, int posY)
    : baseTower(tower), x(posX), y(posY),
    currentDamage(tower.getDamage()),
    currentTimePerAttack(tower.getTimePerAttack()) {}

// 위치 관련 메서드
int PlacedTower::getX() const {
    return x;
}

int PlacedTower::getY() const {
    return y;
}

// 현재 속성 접근 메서드
int PlacedTower::getDamage() const {
    return currentDamage;
}

int PlacedTower::getTimePerAttack() const {
    return currentTimePerAttack;
}

// 기본 타워 속성 접근 메서드
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
    if (baseTower.getIsMagic()) return "마법";
    else return "물리";
}
int PlacedTower::getTargetAmount() const {
    return baseTower.getTargetAmount();
}

int PlacedTower::getIsNoDamage() const {
    return baseTower.getIsNoDamage();
}

// 업그레이드 메서드
bool PlacedTower::upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList,int num) {
    int nextTowerID = baseTower.getNextTowerID();
    nextTowerID = nextTowerID + num;
    if (nextTowerID == 0) {
        std::cout << baseTower.getTowerName() << "은(는) 이미 최고 단계입니다.\n";
        return false;
    }

    // 타워 리스트에서 nextTowerID에 해당하는 타워를 찾음
    auto it = std::find_if(towerList.begin(), towerList.end(), [nextTowerID](const Tower& t) {
        return t.getId() == nextTowerID;
        });

    if (it == towerList.end()) {
        std::cout << "업그레이드할 타워 정보를 찾을 수 없습니다.\n";
        return false;
    }

    int upgradeCost = it->getBuildCost();
    if (gold < upgradeCost) {
        std::cout << "골드가 부족합니다. 업그레이드 비용: " << upgradeCost << ", 현재 골드: " << gold << "\n";
        return false;
    }

    // 골드가 충분한 경우 업그레이드 진행
    gold -= upgradeCost;
    baseTower = *it; // 타워를 업그레이드된 타워로 대체
    currentDamage = baseTower.getDamage(); // 새 타워의 데미지 값 반영
    currentTimePerAttack = baseTower.getTimePerAttack(); // 새 타워의 공격 속도 반영
    map[y][x] = baseTower.getTowerName(); // 맵에 새로운 타워 이름으로 업데이트
    std::cout << baseTower.getTowerName() << "이(가) 업그레이드되었습니다.\n";
    return true;
}

// 버프 관련 메서드
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

// 버프 상태 확인 메서드
bool PlacedTower::hasReceivedBuff() const {
    return receivedBuff;
}
