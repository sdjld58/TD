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
    else //경로가 없을 경우
    {
        posX = posY = -1.0f;
        targetX = targetY = -1;
    }

    // speed 계산 (timePerMove를 이용)
    speed = 1.0f / (timePerMove * 0.5f); // timePerMove가 클수록 속도가 느려짐
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

            // 다음 목표로 이동
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

// 위치
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