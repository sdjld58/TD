#include "GameManager.h"

int main() {
    GameManager gameManager;
    gameManager.run();
    return 0;
}

/*
2024/11/? 공격 업데이트
우선 공격 웨이브로 넘어가야 해서 defense 브렌치 사용함

    GameManager.h
변수 선언 추가
unitProductionQueue
attackGold

함수 선언 추가
startAttackWave(const Wave& wave)
handleAttackInput()
updateAttackUnits(std::vector<Unit>& activeUnits);
isAttackWaveOver(const std::vector<Unit>& activeUnits);

    GameManager.cpp
run함수 공격 웨이브 처리 추가

함수 구현 추가
startAttackWave(const Wave& wave)
handleAttackInput()
updateAttackUnits(std::vector<Unit>& activeUnits);
isAttackWaveOver(const std::vector<Unit>& activeUnits);



*/