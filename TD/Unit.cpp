#include "Unit.h"
#include <iostream>

Unit::Unit(int id, const std::string& name, int hp, int timePerMove, int armor, int resist, int killReward, const std::vector<std::pair<int, int>>& path, int LifeDamage)
    : id(id), name(name), hp(hp), armor(armor), resist(resist), killReward(killReward), LifeDamage(LifeDamage), path(path), pathIndex(0), isArrived(false)
{
    if (!path.empty())
    {
        posX = static_cast<float>(path[0].first);
        posY = static_cast<float>(path[0].second);
        pathIndex = 1;

        if (path.size() > 1)
        {
            targetX = path[1].first;
            targetY = path[1].second;
        }
        else
        {
            targetX = path[0].first;
            targetY = path[0].second;
            isArrived = true;
        }
    }
    else //��ΰ� ���� ���
    {
        posX = posY = -1.0f;
        targetX = targetY = -1;
    }

    // speed ��� (timePerMove�� �̿�)
    speed = 1.0f / (timePerMove * 0.5f); // timePerMove�� Ŭ���� �ӵ��� ������
}

void Unit::updatePosition(float deltaTime)
{
    if (isArrived) return;

    float dirX = targetX - posX;
    float dirY = targetY - posY;
    float distance = std::sqrt(dirX * dirX + dirY * dirY);

    if (distance > 0)
    {
        dirX /= distance;
        dirY /= distance;

        float moveDist = speed * deltaTime;

        if (moveDist >= distance)
        {
            posX = static_cast<float>(targetX);
            posY = static_cast<float>(targetY);

            // ���� ��ǥ�� �̵�
            pathIndex++;
            if (pathIndex < path.size())
            {
                targetX = path[pathIndex].first;
                targetY = path[pathIndex].second;
            }
            else
            {
                isArrived = true;
            }
        }
        else
        {
            posX += dirX * moveDist;
            posY += dirY * moveDist;
        }
    }
}

bool Unit::update() 
{
    return isArrived;
}

// ��ġ
float Unit::getPosX() const { return posX; }
float Unit::getPosY() const { return posY; }
int Unit::getTileX() const { return static_cast<int>(std::floor(posX + 0.5f)); }
int Unit::getTileY() const { return static_cast<int>(std::floor(posY + 0.5f)); }

int Unit::getHp() const { return hp; }
int Unit::getArmor() const { return armor; }
int Unit::getResist() const { return resist; }
int Unit::getKillReward() const { return killReward; }
int Unit::getLifeDamage() const { return LifeDamage; }

std::string Unit::getName() const { return name; }

void Unit::printStatus() const 
{
    std::cout << "Unit " << name << " at (" << posX << ", " << posY << "), HP: " << hp << std::endl;
}

void Unit::reduceHp(int currentHp)
{
    hp = currentHp;
    if (hp < 0) hp = 0;
}

bool Unit::hasArrived() const
{
    return isArrived;
}