#include "Wave.h"
#include <iostream>

Wave::Wave(int waveID, int life, bool isDefence, int gold, const std::vector<int>& units)
    : waveID(waveID), life(life), isDefence(isDefence), gold(gold), units(units) {}

int Wave::getWaveID() const { return waveID; }
int Wave::getLife() const { return life; }
bool Wave::getIsDefence() const { return isDefence; }
int Wave::getGold() const { return gold; }
const std::vector<int>& Wave::getUnits() const { return units; }

void Wave::printInfo() const {
    std::cout << "WaveID: " << waveID << ", Life: " << life
        << ", IsDefence: " << (isDefence ? "Yes" : "No")
        << ", Gold: " << gold << ", Units: ";
    for (int unitID : units) {
        std::cout << unitID << ' ';
    }
    std::cout << std::endl;
}
