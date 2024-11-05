#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <string>
#include <queue>
#include "UnitType.h"
#include "Wave.h"
#include "Unit.h"
#include "PlacedTower.h"
#include "Tower.h"

class GameManager
{
private:
    std::vector<std::vector<std::string>> map;
    std::vector<UnitType> unitTypes; // 유닛 타입 리스트
    std::vector<Wave> waves;         // 웨이브 리스트
    std::vector<std::pair<int, int>> path; // 유닛 이동 경로
    std::vector<Tower> towers;            // 기본 타워 속성을 저장하는 벡터
    std::vector<PlacedTower> placedTowers; // 배치된 타워를 저장하는 벡터

    int playerLife;
    int gold;
    bool isPreparation;

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
    void startPreparationPhase();
    void constructTower();
    void erasecursol();
    void loadTowerData(const std::string& filename); //타워 로드
   
};

#endif // GAMEMANAGER_H
