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


std::string PlacedTower::uiOptionSelect( const std::vector<Tower>& towerList, int num) const {
    int nextTowerID = this->getNextTowerID();
    int nextTowerID2 = this->getNextTowerID2();

    // 타워 리스트에서 nextTowerID에 해당하는 타워를 찾음
    auto it = std::find_if(towerList.begin(), towerList.end(), [nextTowerID](const Tower& t) {
        return t.getId() == nextTowerID;
        });

    auto it2 = std::find_if(towerList.begin(), towerList.end(), [nextTowerID2](const Tower& t) {
        return t.getId() == nextTowerID2;
        });

    int upgradeCost = (nextTowerID % 100 == 0) ? 50 : 100;
    int upgradeCost2 = (nextTowerID2 % 100 == 0) ? 50 : 100;
    int refundAmount = 30; // 초기값 설정

    // 옵션 선택시 띄우는 창
    if (num == 0) {
        if (this->getTowerName() == "1") {
            refundAmount = 30;

            if (it != towerList.end()) {
                return
                    "    Basic Tower 입니다.\n\n  [1]   '" + it->getTool() +
                    "' 타워로 upgrade\n              (소요 골드 : " + std::to_string(upgradeCost) + ")\n  [3]   "
                    + "판매 (기대 골드 : " + std::to_string(refundAmount) + ")";
            }

            else {
                return "uiOptionSelect 오류입니다";
            }
        }
        // Advanced 타워 선택
        else if (this->getTowerName() == "2") {
            refundAmount = 40;

            if (it != towerList.end() && it2 != towerList.end()) {
                return
                    "    Advanced Tower 입니다.\n\n  [1]   '"
                    + it->getTool() + "' 타워로 upgrade\n             (소요 골드 : " + std::to_string(upgradeCost) + ")\n  [2]   '"
                    + it2->getTool() + "' 타워로 upgrade\n             (소요 골드 : " + std::to_string(upgradeCost2) + ")\n  [3]   "
                    + "판매 ( +" + std::to_string(refundAmount) + " 골드 )";

            }
            else {
                return
                    "upgrade 가능한 타워를 찾을 수 없습니다.\n",
                    "판매시 기대 골드 : " + std::to_string(refundAmount);
            }
        }
        // Ultimate 타워 선택
        else {
            refundAmount = 50;

            return
                "    Ultimate Tower 입니다.\n\n  [3]   판매 (기대 골드 : " + std::to_string(refundAmount) + ")";

        }
    }
    else if (num == 3) { // 판매 시 띄우는 화면
        return  "(환급골드 : " + std::to_string(refundAmount) + ")";
    }
  
    else if (num == 1) { // 1로 업그레이드시 띄우는 화면
        return (it->getTool() + " 타워로 업그레이드하습니까?\n             (소요 골드 : " + std::to_string(upgradeCost) + ")\n\n    " + it->getTool2());
    }
    else if (num == 2) { // 2로 업그레이드시 띄우는 화면
        return (it2->getTool() + " 타워로 업그레이드하습니까?\n             (소요 골드 : " + std::to_string(upgradeCost2) + ")\n\n    " + it2->getTool2());
    }
    else {
        return "잘못된 키 입니다";
    }
    
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

int PlacedTower::getTargetAmount() const {
    return baseTower.getTargetAmount();
}

int PlacedTower::getIsNoDamage() const {
    return baseTower.getIsNoDamage();
}

// 업그레이드 메서드
bool PlacedTower::upgrade(int& gold, std::vector<std::vector<std::string>>& map, const std::vector<Tower>& towerList,int num) 
{
    int nextTowerID = 0;

    if (num == 1)
    {
        nextTowerID = baseTower.getNextTowerID();

     
    }
    else if (num == 2)
    {
        nextTowerID = baseTower.getNextTowerID2();

    }
    

   
    if (nextTowerID == 0) 
    {
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
    int upgradeCost = 0;

    if (nextTowerID % 100 == 0)
    {
        upgradeCost = 50;
    }
    else
    {
        upgradeCost = 100;
    }

   
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



    if (currentTimePerAttack - attackSpeedReduction >= 1)
    {
        currentTimePerAttack -= attackSpeedReduction;
    }
    else if (currentTimePerAttack - attackSpeedReduction < 1)
    {
        currentTimePerAttack = 1;
    }
   
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
int PlacedTower::getNextTowerID() const {
    return baseTower.getNextTowerID();
}
int PlacedTower::getNextTowerID2() const {
    return baseTower.getNextTowerID2();
}
std::string PlacedTower::attackType() const {
    if (getIsMagic()) return "마법";
    else return "물리";
}