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
    UI ui;  // UI Ŭ���� �ν��Ͻ� �߰�

    int selectedX;
    int selectedY;
    int selectedTowerIndex;
    int selectedOption;
    bool isTowerPlacementMode;
    bool currentwaveType;

    std::vector<Projectile> projectiles;

    bool isTileSelectable(int x, int y);

    std::vector<std::wstring> prologueText = {
    L"������ �̾����� �� �ձ��� ����. �ϳ��� �ձ��� ��Ȱ�� ����� �ڿ��� �ڶ��ϴ� ���׾�(Lastea),",
    L"",
    L"�� �ٸ� �ձ��� ġ���� ������ �������� ������ ���ٸ���(Veldarion)...",
    L"",
    L"������ ���׾��� Ȯ�� ��å�� ���ٸ����� ���� ��ȣ ������ �˹ߵǾ���, ������ ����ڴ� ���� ������ �þ ����.",
    L"",
    L"�� ġ���� �븳 �ӿ���, ���׾� �ձ��� ��� �ӹ��� ������ �ڸ� �̴´�.",
    L"",
    L"ī�̷�(Kairon), �״� �ɼ��� ���������� �پ ø�� �������,",
    L"",
    L"�ձ��� �̷��� ������ ä ����� ø�� �ӹ��� �����ư��� �����ؾ� �Ѵ�.",
    L"",
    L"���� ���� �հ� ����� ��ġ�� �ӹ�����, ���׾Ʒ� �������� ������ ���Ƴ��� ����������.",
    L"",
    L"���� ���д� ������ ����� �������� ���̴�."
    };
    std::vector<std::wstring> Map1Text = {
        L"[���� ����]",
    L"",
    L"ī�̷��� ���׾��� ���� ���濡 ��ġ�� ��â�� **������ ��(Erdel Forest)**���� �İߵȴ�.",
    L"���ٸ����� ���� �δ밡 ���� �̿��� ħ���� �õ� ���̶�� ø���� ���Ա� �����̴�.",
    L"",
    L"ī�̷��� �̰����� Ÿ���� �Ǽ��� ���� ������ ���Ƴ��� ���ÿ�,",
    L"���ٸ����� �� ������ ������ �׵��� ���� ��ȹ�� ���� ��ġ�� �˾Ƴ��� �Ѵ�.",
    L"",
    L"������ ���� �ܼ��� �� �ӹ����� �ʰ�, ������ ���� ���ɵ��� ��ȯ�� ī�̷��� ���Ƽ���."
    };
    std::vector<std::wstring> Map2Text = {
         L"[�縷�� ����]",
    L"",
    L"�������� �ӹ��� ���������� ������ ī�̷��� ���� �߰ſ� **�縣�� �縷(Sarha Desert)**���� ���Ѵ�.",
    L"���ٸ����� �� �������� �Ŵ��� ���� ��踦 �����,",
    L"������ �ǵ��� ������ ������ ���⸦ �غ� ���̴�.",
    L"",
    L"�縷�� ������ �賭�� ������ �̿��� ī�̷��� �� �й��ϰ�,",
    L"���� ħ���� �����Ϸ� �Ѵ�.",
    L"",
    L"ī�̷��� �縷�� ����, �������� ����� ��,",
    L"�׸��� ������ �ٶ��� �ߵ����� �ӹ��� �����ؾ� �Ѵ�."
    };
    std::vector<std::wstring> Map3Text = {
        L"[�ܿ��� ����]",
    L"",
    L"������ �ӹ��� ������ **�븣���� ���(Norheim Plains)**.",
    L"�̰��� ���׾ƿ� ���ٸ����� ���� ��������,",
    L"���ٸ����� ��� ������ ������ ������ ������ �غ� ���̴�.",
    L"",
    L"ī�̷��� ���� �������� ������ ������ ø�� �ӹ��� �����ϴ� ���ÿ�,",
    L"���� ���������� �Ϲ��� ���̶�� �������� ���� ���͸� ���Ƴ��� �Ѵ�.",
    L"",
    L"��� ���� ���� �տ� �޷� �ִ�."
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
    void loadTowerData(const std::string& filename); //Ÿ�� �ε�
    void attackUnits(std::vector<Unit>& activeUnits ,int currentTick, bool currentwaveType); //���ݷ���
    int calculateDamage(bool damageType, int baseDamage, const Unit& unit); //������ ��� ����
    void spawnUnits(std::vector<Unit>& activeUnits, std::queue<int>& unitQueue, int currentTick);
    void updateUnits(std::vector<Unit>& activeUnits);

    void createProjectile(const PlacedTower& tower, const Unit& targetUnit);
    void updateProjectiles(sf::Time deltaTime);

    void startAttackWave(const Wave& wave,int& currentTick);
    bool handleAttackInput();
    void updateAttackUnits(std::vector<Unit>& activeUnits, int currentTick);
    bool isAttackWaveOver(const std::vector<Unit>& activeUnits);

    // �߰��� �޼���
    void updateGameState(std::vector<Unit>& activeUnits);
    void mapSelected();
    void showGameOverPopup();
    void showGameClearPopup();
    void gameStart();
    void gamePrologue(const std::vector<std::wstring>& prologueLines);
    void showLoadingScreen(const std::string& message);
    
  
};

#endif // GAMEMANAGER_H
