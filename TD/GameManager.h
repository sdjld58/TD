#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <queue>
#include "UnitType.h"
#include "Wave.h"
#include "Unit.h"
#include "PlacedTower.h"
#include "Tower.h"
#include "UI.h"
#include "Projectile.h"

class GameManager
{
private:
    std::vector<std::vector<std::string>> map;
    std::vector<UnitType> unitTypes;
    std::vector<Wave> waves;
    std::vector<std::pair<int, int>> path;
    std::vector<Tower> towers;
    std::vector<PlacedTower> placedTowers;
    std::queue<int> unitProductionQueue;
    std::vector<Unit*> aoeTargets;
    std::vector<Unit> activeUnits;

    int playerLife;
    int gold;
    bool isPreparation;
    int attackGold;
    bool showUnitInfo;
    void handleTowerButtonClicked();
    int lastSpawnTick;
    int currentTick;

    void attemptPlaceTower();
    void selectTower(const std::vector<Tower>& towers, int towerId, int& selectedTowerIndex, UI& ui);
    UI ui;  // UI 클래스 인스턴스 추가

    int selectedX;
    int selectedY;
    int selectedTowerIndex;
    int selectedOption;
    bool isTowerPlacementMode;
    bool currentwaveType;

    std::vector<Projectile> projectiles;

    bool isTileSelectable(int x, int y);

    std::vector<std::wstring> prologueText = {
    L"끝없이 이어지는 두 왕국의 전쟁. 하나의 왕국은 광활한 영토와 자원을 자랑하는 라스테아(Lastea),",
    L"",
    L"또 다른 왕국은 치밀한 전략과 마법으로 유명한 벨다리온(Veldarion)...",
    L"",
    L"전쟁은 라스테아의 확장 정책과 벨다리온의 독립 수호 의지로 촉발되었고, 양측의 사상자는 날이 갈수록 늘어만 갔다.",
    L"",
    L"이 치열한 대립 속에서, 라스테아 왕국은 비밀 임무를 수행할 자를 뽑는다.",
    L"",
    L"카이론(Kairon), 그는 능숙한 전략가이자 뛰어난 첩보 요원으로,",
    L"",
    L"왕국의 미래를 짊어진 채 수비와 첩보 임무를 번갈아가며 수행해야 한다.",
    L"",
    L"적의 방어선을 뚫고 기밀을 훔치는 임무부터, 라스테아로 몰려오는 적군을 막아내는 수비전까지.",
    L"",
    L"그의 성패는 전쟁의 향방을 결정지을 것이다."
    };
    std::vector<std::wstring> Map1Text = {
        L"[숲의 전투]",
    L"",
    L"카이론은 라스테아의 동부 국경에 위치한 울창한 **에르델 숲(Erdel Forest)**으로 파견된다.",
    L"벨다리온의 정찰 부대가 숲을 이용해 침공을 시도 중이라는 첩보가 들어왔기 때문이다.",
    L"",
    L"카이론은 이곳에서 타워를 건설해 적의 물결을 막아내는 동시에,",
    L"벨다리온의 숲 기지로 잠입해 그들의 전진 계획과 병력 배치를 알아내야 한다.",
    L"",
    L"하지만 적은 단순히 방어에 머무르지 않고, 강력한 숲의 정령들을 소환해 카이론을 막아선다."
    };
    std::vector<std::wstring> Map2Text = {
         L"[사막의 전투]",
    L"",
    L"숲에서의 임무를 성공적으로 수행한 카이론은 이제 뜨거운 **사르하 사막(Sarha Desert)**으로 향한다.",
    L"벨다리온은 이 지역에서 거대한 마법 결계를 세우고,",
    L"전쟁의 판도를 뒤집을 강력한 무기를 준비 중이다.",
    L"",
    L"사막의 적군은 험난한 지형을 이용해 카이론의 방어를 압박하고,",
    L"그의 침투를 방해하려 한다.",
    L"",
    L"카이론은 사막의 독사, 마법으로 제어된 골렘,",
    L"그리고 강력한 바람을 견뎌내며 임무를 수행해야 한다."
    };
    std::vector<std::wstring> Map3Text = {
        L"[겨울의 전투]",
    L"",
    L"마지막 임무는 얼어붙은 **노르헤임 평원(Norheim Plains)**.",
    L"이곳은 라스테아와 벨다리온의 국경 지역으로,",
    L"벨다리온이 모든 병력을 집중해 최후의 결전을 준비 중이다.",
    L"",
    L"카이론은 적의 본거지로 잠입해 마지막 첩보 임무를 수행하는 동시에,",
    L"최종 수비전에서 북방의 용이라는 전설적인 보스 몬스터를 막아내야 한다.",
    L"",
    L"모든 것이 그의 손에 달려 있다."
    };
public:
    GameManager();
    void run(const std::string& stageFile);
    void loadMap(const std::string& filename);
    void parsePath();
    void loadUnitTypes(const std::string& filename);
    void loadWaves(const std::string& filename);
    void updateAndPrintMap(const std::vector<Unit>& activeUnits);
    void updateUnitPositions(sf::Time deltaTime);

    void startPreparationPhase();
    void loadTowerData(const std::string& filename); //타워 로드
    void attackUnits(std::vector<Unit>& activeUnits ,int currentTick, bool currentwaveType); //공격로직
    int calculateDamage(bool damageType, int baseDamage, const Unit& unit); //데미지 계산 로직
    void spawnUnits(std::vector<Unit>& activeUnits, std::queue<int>& unitQueue, int currentTick);
    void updateUnits(std::vector<Unit>& activeUnits);

    void createProjectile(const PlacedTower& tower, const Unit& targetUnit);
    void updateProjectiles(sf::Time deltaTime);

    void startAttackWave(const Wave& wave,int& currentTick);
    bool handleAttackInput();
    void updateAttackUnits(std::vector<Unit>& activeUnits, int currentTick);
    bool isAttackWaveOver(const std::vector<Unit>& activeUnits);

    // 추가된 메서드
    void updateGameState(std::vector<Unit>& activeUnits);
    void mapSelected();
    void showGameOverPopup();
    void showGameClearPopup();
    void gameStart();
    void gamePrologue(const std::vector<std::wstring>& prologueLines);
    void showLoadingScreen(const std::string& message);
    
  
};

#endif // GAMEMANAGER_H
