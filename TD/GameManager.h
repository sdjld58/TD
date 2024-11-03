#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <string>
#include <queue>
#include "UnitType.h"
#include "Wave.h"
#include "Unit.h"

class GameManager {
private:
    std::vector<std::vector<std::string>> map;
    std::vector<UnitType> unitTypes; // 유닛 타입 리스트
    std::vector<Wave> waves;         // 웨이브 리스트
    std::vector<std::pair<int, int>> path; // 유닛 이동 경로

    int playerLife;
    int gold;

public:
    GameManager();
    void run();
    void loadMap(const std::string& filename);
    void printMap();
    void parsePath(); // 맵에서 경로 파싱
    void loadUnitTypes(const std::string& filename);
    void printUnitTypes();
    void loadWaves(const std::string& filename);
    void printWaves();
    void updateAndPrintMap(const std::vector<Unit>& activeUnits); // 맵 업데이트 및 출력
};

#endif // GAMEMANAGER_H
