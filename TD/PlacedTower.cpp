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


std::string PlacedTower::uiOptionSelect( const std::vector<Tower>& towerList, int num) const {
    int nextTowerID = this->getNextTowerID();
    int nextTowerID2 = this->getNextTowerID2();

    // Ÿ�� ����Ʈ���� nextTowerID�� �ش��ϴ� Ÿ���� ã��
    auto it = std::find_if(towerList.begin(), towerList.end(), [nextTowerID](const Tower& t) {
        return t.getId() == nextTowerID;
        });

    auto it2 = std::find_if(towerList.begin(), towerList.end(), [nextTowerID2](const Tower& t) {
        return t.getId() == nextTowerID2;
        });

    int upgradeCost = (nextTowerID % 100 == 0) ? 50 : 100;
    int upgradeCost2 = (nextTowerID2 % 100 == 0) ? 50 : 100;
    int refundAmount = 30; // �ʱⰪ ����

    // �ɼ� ���ý� ���� â
    if (num == 0) {
        if (this->getTowerName() == "1") {
            refundAmount = 30;

            if (it != towerList.end()) {
                return
                    "    Basic Tower �Դϴ�.\n\n  [1]   '" + it->getTool() +
                    "' Ÿ���� upgrade\n              (�ҿ� ��� : " + std::to_string(upgradeCost) + ")\n  [3]   "
                    + "�Ǹ� (��� ��� : " + std::to_string(refundAmount) + ")";
            }

            else {
                return "uiOptionSelect �����Դϴ�";
            }
        }
        // Advanced Ÿ�� ����
        else if (this->getTowerName() == "2") {
            refundAmount = 40;

            if (it != towerList.end() && it2 != towerList.end()) {
                return
                    "    Advanced Tower �Դϴ�.\n\n  [1]   '"
                    + it->getTool() + "' Ÿ���� upgrade\n             (�ҿ� ��� : " + std::to_string(upgradeCost) + ")\n  [2]   '"
                    + it2->getTool() + "' Ÿ���� upgrade\n             (�ҿ� ��� : " + std::to_string(upgradeCost2) + ")\n  [3]   "
                    + "�Ǹ� ( +" + std::to_string(refundAmount) + " ��� )";

            }
            else {
                return
                    "upgrade ������ Ÿ���� ã�� �� �����ϴ�.\n",
                    "�ǸŽ� ��� ��� : " + std::to_string(refundAmount);
            }
        }
        // Ultimate Ÿ�� ����
        else {
            refundAmount = 50;

            return
                "    Ultimate Tower �Դϴ�.\n\n  [3]   �Ǹ� (��� ��� : " + std::to_string(refundAmount) + ")";

        }
    }
    else if (num == 3) { // �Ǹ� �� ���� ȭ��
        return  "(ȯ�ް�� : " + std::to_string(refundAmount) + ")";
    }
  
    else if (num == 1) { // 1�� ���׷��̵�� ���� ȭ��
        return (it->getTool() + " Ÿ���� ���׷��̵��Ͻ��ϱ�?\n             (�ҿ� ��� : " + std::to_string(upgradeCost) + ")\n\n    " + it->getTool2());
    }
    else if (num == 2) { // 2�� ���׷��̵�� ���� ȭ��
        return (it2->getTool() + " Ÿ���� ���׷��̵��Ͻ��ϱ�?\n             (�ҿ� ��� : " + std::to_string(upgradeCost2) + ")\n\n    " + it2->getTool2());
    }
    else {
        return "�߸��� Ű �Դϴ�";
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

// ���׷��̵� �޼���
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

// ���� ���� Ȯ�� �޼���
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
    if (getIsMagic()) return "����";
    else return "����";
}