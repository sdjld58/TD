#include "Tower.h"
#include <iostream>
#include <algorithm>

// 생성자 정의
Tower::Tower(int id, std::string towerName, int nextTowerID, int buildCost, int attackRange, int damage, bool isMagic, int timePerAttack, int targetAmount, int isNoDamage, const std::string& tool)
    : id(id), towerName(towerName), nextTowerID(nextTowerID), buildCost(buildCost), attackRange(attackRange), damage(damage), isMagic(isMagic), timePerAttack(timePerAttack), targetAmount(targetAmount), isNoDamage(isNoDamage), tool(tool) {}

// Getter 함수들 정의
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

// 업그레이드 함수 정의 - 타워 리스트에서 nextTowerID에 해당하는 타워를 찾음
void Tower::upgrade(const std::vector<Tower>& towerList)
{
    if (nextTowerID == 0)
    {
        std::cout << towerName << "은(는) 이미 최고 단계입니다.\n";
        return;
    }

    auto it = std::find_if(towerList.begin(), towerList.end(), [this](const Tower& t) {
        return t.getId() == this->nextTowerID;
        });

    if (it != towerList.end())
    {
        *this = *it; // 현재 타워 객체를 업그레이드된 타워로 대체
        std::cout << towerName << "이(가) 업그레이드되었습니다.\n";
    }
    else
    {
        std::cout << "업그레이드할 타워 정보를 찾을 수 없습니다.\n";
    }
}

// 타워 정보 출력 함수 정의
void Tower::printInfo() const {
    std::cout << "타워 이름: " << towerName << "\n"
        << "ID: " << id << "\n"
        << "설치 비용: " << buildCost << "\n"
        << "공격 사거리: " << attackRange << "\n"
        << "대미지: " << damage << (isMagic ? " (마법 대미지)" : " (물리 대미지)") << "\n"
        << "공격 속도: " << timePerAttack << "틱마다 한 번 공격\n"
        << "목표 수: " << targetAmount << "개\n"
        << "버프 타워 여부: " << (isNoDamage ? "예" : "아니오") << "\n"
        << "도구: " << tool << "\n";
}

void Tower::displayInfo() const 
{
    std::cout << "타워 이름: " << towerName << "\n";
    std::cout << "설명: " << tool << "\n";
    std::cout << "공격력: " << damage << "\n";
    std::cout << "사거리: " << attackRange << "\n";
    std::cout << "설치 비용: " << buildCost << "\n";
    std::cout << "--------------------------------\n";
}