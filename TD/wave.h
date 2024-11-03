#ifndef WAVE_H
#define WAVE_H

#include <vector>

class Wave {
private:
    int waveID;               // ���̺��� ��ȣ
    int life;                 // �÷��̾��� �ʱ� life �� (1�� ���̺길 �ǹ� ����)
    bool isDefence;           // ���� ���̺� ���� (true: ����, false: ����)
    int gold;                 // ���̺� ���� �� �����ϴ� ��ȭ�� ��
    std::vector<int> units;   // ���� ���̺꿡�� �����ϴ� �� ���� ID ���

public:
    Wave(int waveID, int life, bool isDefence, int gold, const std::vector<int>& units);

    // ������ �Լ���
    int getWaveID() const;
    int getLife() const;
    bool getIsDefence() const;
    int getGold() const;
    const std::vector<int>& getUnits() const;

    // ������̳� �׽�Ʈ�� ���� �Լ�
    void printInfo() const;
};

#endif // WAVE_H
