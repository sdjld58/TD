#include "GameManager.h"

int main() {
    GameManager gameManager;
    gameManager.run();
    return 0;
}

/*
2024/11/? ���� ������Ʈ
�켱 ���� ���̺�� �Ѿ�� �ؼ� defense �귻ġ �����

    GameManager.h
���� ���� �߰�
unitProductionQueue
attackGold

�Լ� ���� �߰�
startAttackWave(const Wave& wave)
handleAttackInput()
updateAttackUnits(std::vector<Unit>& activeUnits);
isAttackWaveOver(const std::vector<Unit>& activeUnits);

    GameManager.cpp
run�Լ� ���� ���̺� ó�� �߰�

�Լ� ���� �߰�
startAttackWave(const Wave& wave)
handleAttackInput()
updateAttackUnits(std::vector<Unit>& activeUnits);
isAttackWaveOver(const std::vector<Unit>& activeUnits);



*/