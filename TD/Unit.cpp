#include "Unit.h"
#include <iostream>

Unit::Unit(int id, const std::string& name, int hp, int timePerMove, const std::vector<std::pair<int, int>>& path)
    : id(id), name(name), hp(hp), timePerMove(timePerMove), moveCounter(0), path(path), pathIndex(0) {
    if (!path.empty()) {
        x = path[0].first;
        y = path[0].second;
        pathIndex = 1;
    }
    else {
        x = y = -1; // ��ΰ� ���� ���
    }
}

bool Unit::update() {
    moveCounter++;
    if (moveCounter >= timePerMove) {
        moveCounter = 0;
        // �̵�
        if (pathIndex < path.size()) {
            x = path[pathIndex].first;
            y = path[pathIndex].second;
            pathIndex++;
            if (pathIndex == path.size()) {
                return true; // �������� ����
            }
        }
    }
    return false; // ���� �̵� ��
}

int Unit::getX() const { return x; }
int Unit::getY() const { return y; }

std::string Unit::getName() const { return name; }

void Unit::printStatus() const {
    std::cout << "Unit " << name << " at (" << x << ", " << y << "), HP: " << hp << std::endl;
}
