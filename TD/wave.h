#ifndef WAVE_H
#define WAVE_H

#include <vector>

class Wave {
private:
    int waveID;               // 웨이브의 번호
    int life;                 // 플레이어의 초기 life 값 (1번 웨이브만 의미 있음)
    bool isDefence;           // 수비 웨이브 여부 (true: 수비, false: 공격)
    int gold;                 // 웨이브 시작 시 지급하는 재화의 양
    std::vector<int> units;   // 수비 웨이브에서 등장하는 적 유닛 ID 목록

public:
    Wave(int waveID, int life, bool isDefence, int gold, const std::vector<int>& units);

    // 접근자 함수들
    int getWaveID() const;
    int getLife() const;
    bool getIsDefence() const;
    int getGold() const;
    const std::vector<int>& getUnits() const;

    // 디버깅이나 테스트를 위한 함수
    void printInfo() const;
};

#endif // WAVE_H
